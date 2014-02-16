/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cstring>
#include <cerrno>
#include <memory>
#include <string>
#include <sstream>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/drdist.h>
#include <schwa/io/logging.h>

#include <zmq.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace schwa {
namespace drdist {

//static void
//process_doc(dr::Doc &doc) {
  //LOG(INFO) << "processing document " << &doc << std::endl;
//}


static bool
drworker_recv(void *const source, void *const sink, const size_t init_buffer_len=2*1024*1024) {
  size_t buffer_written;
  size_t buffer_len = init_buffer_len;
  std::unique_ptr<char[]> buffer(new char[buffer_len]);

  MessageType msg_type;
  uint64_t doc_num;
  std::string doc_bytes;

  // Infinite loop for reading messages.
  bool terminate = false;
  while (!terminate) {
    if (!recv_multipart(source, buffer, buffer_len, buffer_written))
      return false;

    // Decode the received message.
    unpack_message(buffer.get(), buffer_written, msg_type, doc_num, doc_bytes);

    // Act upon the received message.
    switch (msg_type) {
    case MessageType::DOCUMENT:
      LOG(INFO) << "sending document number " << doc_num << " of size " << doc_bytes.size() << " back to sink " << sink << std::endl;
      break;
    case MessageType::TERMINATE:
      terminate = true;
      break;
    default:
      LOG(ERROR) << "Unknown message type received: " << static_cast<uint8_t>(msg_type) << std::endl;
      break;
    }
  }

  return true;
}


static bool
drworker(const std::string &source_addr, const std::string &sink_addr) {
  // Prepare the ØMQ context and connect to the sockets.
  void *context, *source, *sink;
  if (!safe_zmq_ctx_new(context))
    return false;
  if (!safe_zmq_socket_connect(context, source, ZMQ_PULL, source_addr))
    return false;
  if (!safe_zmq_socket_connect(context, sink, ZMQ_PUSH, sink_addr))
    return false;

  // Receive and process documents from the source, sending results back to the sink.
  bool success = drworker_recv(source, sink);

  // Close the source and sink sockets and destroy the ØMQ context.
  success &= safe_zmq_close(sink);
  success &= safe_zmq_close(source);
  success &= safe_zmq_ctx_destroy(context);
  return success;
}

}
}


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::OpMain cfg("drworker", "A docrep parallelisation worker");
  cf::Op<std::string> source(cfg, "source", "The network address to receive docrep documents from", "tcp://localhost:7301");
  cf::Op<std::string> sink(cfg, "sink", "The network address to send docrep documents from", "tcp://localhost:7302");
  cf::OStreamOp log(cfg, "log", "The file to log to", cf::OStreamOp::STDERR_STRING);
  cf::LogLevelOp log_level(cfg, "loglevel", "The level to log at", "info");

  // Parse argv.
  cfg.main(argc, argv);

  // Configure logging.
  io::default_logger = new io::PrettyLogger(log.file());
  io::default_logger->threshold(log_level());

  // Run the drworker main.
  const bool success = schwa::drdist::drworker(source(), sink());
  return success ? 0 : 1;
}
