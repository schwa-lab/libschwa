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
  zmq::socket_t socket(context, ZMQ_REQ);
  if (noisy)
    std::cerr << "connecting to server " << bind << std::endl;
  socket.connect(bind.c_str());

  while (true) {
    // send the server a notice that we are ready to accept documents
    zmq::message_t request;
    socket.send(request);

    // wait for a reply
    zmq::message_t reply;
    if (noisy)
      std::cerr << "waiting for reply ..." << std::endl;
    socket.recv(&reply);
    std::cerr << "  recieved! size=" << reply.size() << std::endl;

    // empty message signals no more work to do
    if (reply.size() == 0)
      break;

    out.write(static_cast<const char *>(reply.data()), reply.size());
  }
}


int
main(int argc, char *argv[]) {
  cf::OpGroup cfg("drreceiver", "A docrep parallelisation sink");
  cf::OStreamOp op_out(cfg, "output", "The output file");
  cf::Op<std::string> op_bind(cfg, "bind", "The network binding for the ZMQ server", "tcp://localhost:5555");
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
