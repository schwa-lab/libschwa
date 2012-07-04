/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>
#include <schwa/msgpack.h>

#include <zmq.hpp>

namespace cf = schwa::config;
namespace mp = schwa::msgpack;


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


int
main(int argc, char *argv[]) {
  cf::OpGroup cfg("drsend", "A docrep parallelisation source");
  cf::IStreamOp op_in(cfg, "input", "The input file");
  cf::Op<std::string> op_bind(cfg, "bind", "The network binding for the ZMQ server", "tcp://*:5555");
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
  const bool noisy = !op_quiet();

  // prepare the ZMQ context and socket
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REP);
  if (noisy)
    std::cerr << "binding server to " << op_bind() << std::endl;
  socket.bind(op_bind().c_str());

  // listen for incomming connections
  while (true) {
    // accept incomming requests
    zmq::message_t request;
    if (noisy)
      std::cerr << "waiting for request..." << std::endl;
    socket.recv(&request);

    // lazily read the next document off the input stream
    std::stringstream ss;
    const bool doc_found = read_doc(in, ss);
    if (noisy)
      std::cerr << "  received! doc_found=" << doc_found << std::endl;

    if (doc_found) {
      // if we have a document to send, reply in the affirmative
      const size_t size = ss.tellp();
      ss.seekg(0);

      zmq::message_t reply(size);
      std::memcpy(reply.data(), ss.str().c_str(), size);
      socket.send(reply);
    }
    else {
      // no documents left, so reply in the negative
      zmq::message_t reply;
      socket.send(reply);
    }
  }

  return 0;
}
