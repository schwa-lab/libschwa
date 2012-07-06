/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>
#include <schwa/msgpack.h>

#include <zmq.hpp>

#include <condition_variable>
#include <thread>

namespace cf = schwa::config;
namespace mp = schwa::msgpack;


template <typename T=unsigned int>
class Counter {
protected:
  volatile T _value;
  std::mutex _mutex;
  std::condition_variable _cv;

public:
  Counter(const T initial_value=T()) : _value(initial_value) { }
  Counter(const Counter &) = delete;
  Counter &operator =(const Counter &) = delete;

  inline void
  increment(void) {
    std::unique_lock<std::mutex> lock(_mutex);
    ++_value;
    _cv.notify_all();
  }

  inline void
  wait_until(const T value) {
    std::unique_lock<std::mutex> lock(_mutex);
    while (true) {
      if (_value == value)
        break;
      _cv.wait(lock);
    }
  }
};


static bool noisy;
static Counter<> inproc_ndocs;


static inline bool
is_array(const mp::WireType &t) {
  return t == mp::WireType::ARRAY_FIXED || t == mp::WireType::ARRAY_16 || t == mp::WireType::ARRAY_32;
}


static bool
read_doc(std::istream &in, std::ostream &out) {
  static char *buf = nullptr;
  static size_t size = 0;

  mp::WireType type;
  if (in.peek() == EOF)
    return false;

  // <klasses> header
  if (!mp::read_lazy(in, out, type))
    return false;
  if (!is_array(type))
    return false;

  // <stores> header
  if (!mp::read_lazy(in, out, type))
    return false;
  if (!is_array(type))
    return false;

  // instances
  while (true) {
    const int h = in.peek();
    if (h == EOF || is_array(mp::header_type(h)))
      break;

    const uint64_t nbytes = mp::read_uint(in);
    mp::write_uint(out, nbytes);

    if (nbytes > size) {
      delete [] buf;
      buf = new char[nbytes];
      size = nbytes;
    }
    in.read(buf, nbytes);
    out.write(buf, nbytes);
  }

  return true;
}


static void
run_source(zmq::context_t &context, std::istream &in, const std::string &source_bind) {
  // prepare the ØMQ source socket
  zmq::socket_t source(context, ZMQ_PUSH);
  if (noisy) std::cerr << "[source] binding to " << source_bind << std::endl;
  source.bind(source_bind.c_str());

  // prepare the ØMQ socket for notifying the sink how many documents to expect
  zmq::socket_t ndocs_socket(context, ZMQ_PAIR);
  inproc_ndocs.wait_until(1);
  ndocs_socket.connect("inproc://ndocs");
  if (noisy) std::cerr << "[source] bound to inproc://ndocs" << std::endl;

  // loop through each document on the input stream
  uint64_t ndocs = 0;
  zmq::message_t msg;
  while (true) {
    // lazily read the next document off the input stream
    std::stringstream ss;
    const bool doc_found = read_doc(in, ss);
    if (noisy)
      std::cerr << "[source] doc_found=" << doc_found << std::endl;
    if (!doc_found)
      break;
    ++ndocs;

    // if we have a document to send, send it through
    const size_t size = ss.tellp();
    ss.seekg(0);

    msg.rebuild(size);
    std::memcpy(msg.data(), ss.str().c_str(), size);
    source.send(msg);
  }

  // notify the sink how many documents to expect
  if (noisy)
    std::cerr << "[source] notifying sink to expect " << ndocs << " documents" << std::endl;
  msg.rebuild(sizeof(ndocs));
  std::memcpy(msg.data(), &ndocs, sizeof(ndocs));
  ndocs_socket.send(msg);

  if (noisy)
    std::cerr << "[source] terminating" << std::endl;
}


static void
run_sink(zmq::context_t &context, std::ostream &out, const std::string &sink_bind) {
  // prepare the ØMQ sink socket
  zmq::socket_t sink(context, ZMQ_PULL);
  if (noisy) std::cerr << "[sink] binding to " << sink_bind << std::endl;
  sink.bind(sink_bind.c_str());

  // prepare the ØMQ socket for receiving how many documents to expect from the source
  zmq::socket_t ndocs_socket(context, ZMQ_PAIR);
  ndocs_socket.bind("inproc://ndocs");
  inproc_ndocs.increment();

  // listen for messages from both sources
  zmq::pollitem_t items [] = {
    {sink,         0, ZMQ_POLLIN, 0},
    {ndocs_socket, 0, ZMQ_POLLIN, 0},
  };

  // wait for messages
  bool ndocs_expected_set = false;
  uint64_t ndocs_recv = 0, ndocs_expected = 0;
  while (true) {
    // wait for a request
    zmq::message_t msg;
    zmq::poll(&items[0], 2, -1);

    // work out where the request came from
    if (items[0].revents & ZMQ_POLLIN) {
      // sink message
      sink.recv(&msg);
      ++ndocs_recv;
      std::cerr << "[sink] sink message recieved! size=" << msg.size() << " doc#=" << ndocs_recv << " expected=" << (ndocs_expected_set ? ndocs_expected : 0) << std::endl;

      // write the contents out to the ostream
      out.write(static_cast<const char *>(msg.data()), msg.size());
      out.flush();
    }
    else if (items[1].revents & ZMQ_POLLIN) {
      // ndocs message
      ndocs_socket.recv(&msg);
      std::memcpy(&ndocs_expected, msg.data(), sizeof(ndocs_expected));
      ndocs_expected_set = true;
    }

    // if we've read in all of the documents we're expecting to see, stop looping
    if (ndocs_expected_set && ndocs_expected == ndocs_recv)
      break;
  }

  if (noisy) std::cerr << "[sink] terminating" << std::endl;
}


int
main(int argc, char *argv[]) {
  cf::OpMain cfg("drdist", "A docrep parallelisation source and sink");
  cf::IStreamOp op_in(cfg, "input", "The input file");
  cf::OStreamOp op_out(cfg, "output", "The output file");
  cf::Op<std::string> op_source(cfg, "source", "The network binding for the ØMQ source", "tcp://*:7300");
  cf::Op<std::string> op_sink(cfg, "sink", "The network binding for the ØMQ sink", "tcp://*:7301");
  cf::Op<bool> op_quiet(cfg, "quiet", "Quiet mode", false);
  try {
    if (!cfg.process(argc - 1, argv + 1))
      return 1;
  }
  catch (cf::ConfigException &e) {
    std::cerr << schwa::print_exception("ConfigException", e) << std::endl;
    cfg.help(std::cerr);
    return 1;
  }

  // extract values out of config options
  std::istream &in = op_in.file();
  std::ostream &out = op_out.file();
  noisy = !op_quiet();

  // prepare the ØMQ context and sockets
  zmq::context_t context(1);

  // run the threads
  std::thread t_source(run_source, std::ref(context), std::ref(in), op_source());
  std::thread t_sink(run_sink, std::ref(context), std::ref(out), op_sink());
  t_source.join();
  t_sink.join();

  return 0;
}
