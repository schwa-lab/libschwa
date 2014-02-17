/* -*- Mode: C++; indent-tabs-mode: nil -*- */
/**
 * The parallel processing topology used here is what is described in Figure 19 of
 * http://zguide.zeromq.org/page:all#Handling-Errors-and-ETERM.
 *
 * \image html fig19.png
 **/
#include <cerrno>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

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
drdist_source(const std::string &source_addr, const std::string &direct_sink_addr, std::istream &input) {
  // Wait for the sink to be created before starting.
  LOG(DEBUG) << "Waiting for sink to be created..." << std::endl;
  {
    std::unique_lock<std::mutex> lock(sink_created_lock);
    sink_created_cv.wait(lock, [](void){ return sink_created; });
  }

  // Prepare the ØMQ context and construct the sockets;
  void *context, *source, *sink;
  if (!safe_zmq_ctx_new(context))
    return false;
  if (!safe_zmq_socket_bind(context, source, ZMQ_PUSH, source_addr))
    return false;
  if (!safe_zmq_socket_connect(context, sink, ZMQ_PUSH, direct_sink_addr))
    return false;

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
  const std::string msg = build_message(MessageType::DOCUMENT_COUNT, doc_num, nullptr, 0);
  success &= safe_zmq_send(sink, msg.c_str(), msg.size(), 0);

  // Close the source and sink sockets and destroy the ØMQ context.
  success &= safe_zmq_close(sink);
  success &= safe_zmq_close(source);
  success &= safe_zmq_ctx_destroy(context);
  return success;
}


static bool
drdist_sink(const std::string &sink_addr, const std::string &control_addr, const bool preserve_order, const bool kill_clients, std::ostream &output) {
  // Prepare the ØMQ context and create the sockets.
  void *context, *sink, *control;
  if (!safe_zmq_ctx_new(context))
    return false;
  if (!safe_zmq_socket_bind(context, sink, ZMQ_PULL, sink_addr))
    return false;
  if (!safe_zmq_socket_bind(context, control, ZMQ_PUB, control_addr))
    return false;

  // Tell the source that the sink socket has been created.
  {
    std::unique_lock<std::mutex> lock(sink_created_lock);
    sink_created = true;
    sink_created_cv.notify_all();
  }

  uint64_t ndocs = 0, ndocs_written = 0, ndocs_received = 0;
  bool ndocs_known = false;
  std::unordered_map<uint64_t, std::string> unwritten;

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
      ++ndocs_received;
      if (preserve_order) {
        unwritten.emplace(doc_num, doc_bytes);
        for (decltype(unwritten)::const_iterator it; (it = unwritten.find(ndocs_written)) != unwritten.end(); ) {
          output << it->second;
          ++ndocs_written;
          unwritten.erase(it);
        }
      }
      else {
        output << doc_bytes;
        ++ndocs_written;
      }
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

  // This *should* always be true. Assert just as a sanity check.
  if (ndocs_received != ndocs_written || !unwritten.empty()) {
    LOG(CRITICAL) << "ndocs_received=" << ndocs_received << " ndocs_written=" << ndocs_written << " |unwritten|=" << unwritten.size() << std::endl;
  }

  // Publish that the clients should now terminate.
  bool success = true;
  if (kill_clients) {
    const std::string msg = build_message(MessageType::TERMINATE, 0, nullptr, 0);
    if (!safe_zmq_send(control, msg.c_str(), msg.size(), 0))
      success = false;
  }

  // Close the sockets and destroy the ØMQ context.
  success &= safe_zmq_close(control);
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
  cf::Op<std::string> bind_host(cfg, "bind_host", "The network hostname to bind to", "*");
  cf::Op<uint32_t> source_port(cfg, "source_port", "The network port to bind to on which to push docrep documents", 7301);
  cf::Op<uint32_t> sink_port(cfg, "sink_port", "The network port to bind to on which to pull docrep documents", 7302);
  cf::Op<uint32_t> control_port(cfg, "control_port", "The network port to bind to on which to publish control messages", 7303);
  cf::Op<bool> preserve_order(cfg, "preserve_order", "Whether or not the order of documents written out should be preserved", true);
  cf::Op<bool> kill_clients(cfg, "kill_clients", "Whether or not to instruct the clients to terminate once all of the documents have been processed", true);
  cf::OStreamOp log(cfg, "log", "The file to log to", cf::OStreamOp::STDERR_STRING);
  cf::LogLevelOp log_level(cfg, "log_level", "The level to log at", "info");

  // Parse argv.
  cfg.main(argc, argv);

  // Configure logging.
  io::default_logger = new io::ThreadsafePrettyLogger(log.file());
  io::default_logger->threshold(log_level());

  // Construct the network address strings.
  const std::string source_addr = schwa::drdist::build_socket_addr(bind_host(), source_port());
  const std::string sink_addr = schwa::drdist::build_socket_addr(bind_host(), sink_port());
  const std::string control_addr = schwa::drdist::build_socket_addr(bind_host(), control_port());
  const std::string direct_sink_addr = schwa::drdist::build_socket_addr(bind_host() == "*" ? "127.0.0.1" : bind_host(), sink_port());

  // Run the source and sink threads.
  bool success_source, success_sink;
  auto wrap_source = [&](std::istream &input) {
    success_source = schwa::drdist::drdist_source(source_addr, direct_sink_addr, input);
  };
  auto wrap_sink = [&](std::ostream &output) {
    success_sink = schwa::drdist::drdist_sink(sink_addr, control_addr, preserve_order(), kill_clients(), output);
  };
  std::thread source_thread(wrap_source, std::ref(input.file()));
  std::thread sink_thread(wrap_sink, std::ref(output.file()));
  source_thread.join();
  sink_thread.join();

  return success_source && success_sink ? 0 : 1;
}
