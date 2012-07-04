/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>
#include <schwa/msgpack.h>

#include <zmq.hpp>

namespace cf = schwa::config;
namespace mp = schwa::msgpack;


static void
loop(std::ostream &out, const bool noisy, const std::string &bind) {
  // prepare the ZMQ context and socket
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_PULL);
  if (noisy)
    std::cerr << "connecting to server " << bind << std::endl;
  socket.bind(bind.c_str());

  while (true) {
    // wait for a request
    zmq::message_t msg;
    if (noisy)
      std::cerr << "waiting for a message..." << std::endl;
    socket.recv(&msg);
    std::cerr << "  recieved! size=" << msg.size() << std::endl;

    // write the contents out to the ostream
    out.write(static_cast<const char *>(msg.data()), msg.size());
    out.flush();
  }
}


int
main(int argc, char *argv[]) {
  cf::OpGroup cfg("drsink", "A docrep parallelisation sink");
  cf::OStreamOp op_out(cfg, "output", "The output file");
  cf::Op<std::string> op_bind(cfg, "bind", "The network binding for the ZMQ server", "tcp://*:7301");
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

  // loop
  try {
    loop(op_out.file(), !op_quiet(), op_bind());
  }
  catch (zmq::error_t &e) {
    std::cerr << "zmp::error_t (" << e.num() << "): " << e.what() << std::endl;
    return 1;
  }

  return 0;
}
