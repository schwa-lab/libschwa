/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cerrno>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include <schwa/config.h>
#include <schwa/drdist.h>
#include <schwa/io/logging.h>

#include <zmq.h>

namespace cf = schwa::config;
namespace io = schwa::io;

static volatile bool sink_created = false;
static std::condition_variable sink_created_cv;
static std::mutex sink_created_lock;

namespace schwa {
namespace drdist {

static bool
drdist_source(const std::string &source_addr, const std::string &sink_addr, std::istream &input) {
  // Prepare the ØMQ context and construct the source socket.
  void *context, *source, *sink;
  if (!safe_zmq_ctx_new(context))
    return false;
  if (!safe_zmq_socket_bind(context, source, ZMQ_PUSH, source_addr))
    return false;

  // Construct and connect to the sink.
  LOG(DEBUG) << "Waiting for sink to be created..." << std::endl;
  {
    std::unique_lock<std::mutex> lock(sink_created_lock);
    while (!sink_created)
      sink_created_cv.wait(lock);
  }
  {
    // Work out how to connect to the sink.
    std::string sink_connect_addr = sink_addr;
    size_t pos = sink_connect_addr.find("//*:");
    if (pos != std::string::npos)
      sink_connect_addr.replace(pos + 2, 1, "127.0.0.1");

    if (!safe_zmq_socket_connect(context, sink, ZMQ_PUSH, sink_connect_addr))
      return false;
  }

  // Read documents from the input stream and broadcast them out.
  bool success = true;
  uint64_t doc_num;
  for (doc_num = 0; ; ++doc_num) {
    LOG(DEBUG) << "Attempting to read doc_num=" << doc_num << std::endl;
    std::stringstream doc;
    if (!read_doc(input, doc))
      break;
    doc.seekg(0);

    // Construct and send the transport message.
    const std::string msg = build_message(MessageType::DOCUMENT, doc_num, doc.str());
    if (!safe_zmq_send(source, msg.c_str(), msg.size(), 0)) {
      success = false;
      break;
    }
  }

  // Tell the sink how many documents to expect.
  {
    const std::string msg = build_message(MessageType::DOCUMENT_COUNT, doc_num, nullptr, 0);
    success &= safe_zmq_send(sink, msg.c_str(), msg.size(), 0);
  }

  // Construct the terminate transport message.
  const std::string msg = build_message(MessageType::TERMINATE, 0, nullptr, 0);

  // While the sink has not received all documents back, tell any connecting clients to terminate.
  while (1) {
    // Send the transport message.
    const int nbytes = zmq_send(source, msg.c_str(), msg.size(), 0);
    if (nbytes == -1) {
      LOG(CRITICAL) << "Failed to send document via the source socket: " << zmq_strerror(errno) << std::endl;
      success = false;
      break;
    }
    else if (static_cast<size_t>(nbytes) != msg.size()) {
      LOG(CRITICAL) << "Wrong number of bytes sent on source socket (" << nbytes << " instead of " << msg.size() << ")" << std::endl;
      success = false;
      break;
    }
  }

  // Close the source and sink sockets and destroy the ØMQ context.
  success &= safe_zmq_close(sink);
  success &= safe_zmq_close(sink);
  success &= safe_zmq_ctx_destroy(context);
  return success;
}


static bool
drdist_sink(const std::string &sink_addr, std::ostream &output, const bool preserve_order) {
  (void)output;
  (void)preserve_order;

  // Prepare the ØMQ context and connect to the sink socket.
  void *context, *sink;
  if (!safe_zmq_ctx_new(context))
    return false;
  if (!safe_zmq_socket_bind(context, sink, ZMQ_PULL, sink_addr))
    return false;

  // Tell the source that the sink socket has been created.
  {
    std::unique_lock<std::mutex> lock(sink_created_lock);
    sink_created = true;
    sink_created_cv.notify_all();
  }

  uint64_t ndocs = 0, doc_written = 0, ndocs_received = 0;
  (void)doc_written;
  bool ndocs_known = false;

  size_t buffer_written;
  size_t buffer_len = 2*1024*1024;  // 2MB
  std::unique_ptr<char[]> buffer(new char[buffer_len]);

  MessageType msg_type;
  uint64_t doc_num;
  std::string doc_bytes;

  // Listen for documents to come back in until we've received all of them.
  while (!ndocs_known || ndocs_received != ndocs) {
    if (!recv_multipart(sink, buffer, buffer_len, buffer_written))
      return false;

    // Decode the received message.
    unpack_message(buffer.get(), buffer_written, msg_type, doc_num, doc_bytes);

    // Act upon the received message.
    switch (msg_type) {
    case MessageType::DOCUMENT:
      LOG(DEBUG) << "received document " << doc_num << " of " << doc_bytes.size() << " bytes" << std::endl;
      break;
    case MessageType::DOCUMENT_COUNT:
      ndocs = doc_num;
      ndocs_known = true;
      LOG(DEBUG) << "count " << ndocs << std::endl;
      break;
    default:
      LOG(ERROR) << "Unknown message type received: " << static_cast<uint8_t>(msg_type) << std::endl;
      break;
    }
  }

  // Close the sink socket and destroy the ØMQ context.
  bool success = true;
  success &= safe_zmq_close(sink);
  success &= safe_zmq_ctx_destroy(context);
  return success;
}

}
}


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::OpMain cfg("drdist", "A docrep parallelisation source and sink");
  cf::IStreamOp input(cfg, "input", "The input file");
  cf::OStreamOp output(cfg, "output", "The output file");
  cf::Op<std::string> source(cfg, "source", "The network address to send docrep documents from", "tcp://*:7301");
  cf::Op<std::string> sink(cfg, "sink", "The network address to receive docrep documents from", "tcp://*:7302");
  cf::Op<bool> preserve_order(cfg, "preserve_order", "Whether or not the order of documents written out should be preserved", false);
  cf::OStreamOp log(cfg, "log", "The file to log to", cf::OStreamOp::STDERR_STRING);
  cf::LogLevelOp log_level(cfg, "loglevel", "The level to log at", "info");

  // Parse argv.
  cfg.main(argc, argv);

  // Configure logging.
  io::default_logger = new io::ThreadsafePrettyLogger(log.file());
  io::default_logger->threshold(log_level());

  // Run the source and sink threads.
  std::thread source_thread(&schwa::drdist::drdist_source, source(), sink(), std::ref(input.file()));
  std::thread sink_thread(&schwa::drdist::drdist_sink, sink(), std::ref(output.file()), preserve_order());
  source_thread.join();
  sink_thread.join();

  return 0;
}
