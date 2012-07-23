/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>
#include <schwa/msgpack.h>

#include <zmq.hpp>

#include <condition_variable>
#include <thread>

namespace cf = schwa::config;
namespace mp = schwa::msgpack;


static bool NOISY;
static std::mutex input_lock;
static std::mutex output_lock;


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
run_thread(zmq::context_t &context, const std::string &bind, std::istream &in, std::ostream &out) {
  // prepare the ØMQ source socket
  zmq::socket_t socket(context, ZMQ_REQ);
  if (NOISY) std::cerr << "[source] binding to " << bind << std::endl;
  socket.connect(bind.c_str());

  zmq::message_t msg;
  while (true) {
    // lazily read the next document off the input stream
    std::stringstream ss;
    input_lock.lock();
    const bool doc_found = read_doc(in, ss);
    input_lock.unlock();
    if (NOISY) std::cerr << "[source] doc_found=" << doc_found << std::endl;
    if (!doc_found)
      break;

    // if we have a document to send, send it through
    const size_t size = ss.tellp();
    ss.seekg(0);
    msg.rebuild(size);
    std::memcpy(msg.data(), ss.str().c_str(), size);
    socket.send(msg);

    // wait for the reply back
    socket.recv(&msg);
    if (NOISY) std::cerr << "[source] recv size=" << msg.size() << std::endl;

    // write the contents out to the ostream
    output_lock.lock();
    out.write(static_cast<const char *>(msg.data()), msg.size());
    out.flush();
    output_lock.unlock();
  }

  if (NOISY) std::cerr << "[source] shutting down" << std::endl;
}


int
main(int argc, char *argv[]) {
  cf::OpMain cfg("drdist", "A docrep parallelisation source and sink");
  cf::IStreamOp input(cfg, "input", "The input file");
  cf::OStreamOp output(cfg, "output", "The output file");
  cf::Op<std::string> server(cfg, "server", "The network address of the server", "tcp://localhost:7300");
  cf::Op<unsigned long> nthreads(cfg, "nthreads", "The number of threads to fire up", 8);
  cf::Op<bool> quiet(cfg, "quiet", "Quiet mode", false);
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
  std::istream &in = input.file();
  std::ostream &out = output.file();
  NOISY = !quiet();

  // prepare the ØMQ context and sockets
  zmq::context_t context(1);

  // run the threads
  std::vector<std::thread> threads;
  for (unsigned long i = 0; i != nthreads(); ++i)
    threads.push_back(std::thread(&run_thread, std::ref(context), server(), std::ref(in), std::ref(out)));
  for (auto &t : threads)
    t.join();

  return 0;
}
