/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cstring>
#include <cerrno>
#include <memory>
#include <string>
#include <sstream>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/dr/config.h>
#include <schwa/drdist/helpers.h>
#include <schwa/io/logging.h>

#include <zmq.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;

namespace schwa {
namespace drdist {

template <typename DOC>
static std::string
process_doc(const std::string &input_doc_bytes, typename DOC::Schema &schema, void (*callback)(DOC &)) {
  std::istringstream in(input_doc_bytes);
  std::ostringstream out;

  dr::Reader reader(in, schema);

  DOC doc;
  if (!(reader >> doc)) {
    LOG(ERROR) << "Failed to read received document" << std::endl;
    return "";
  }

  LOG(DEBUG) << "Processing document " << input_doc_bytes.size() << std::endl;
  callback(doc);

  dr::Writer writer(out, schema);
  writer << doc;

  return out.str();
}


template <typename DOC>
static bool
drworker_recv(void *const source, void *const sink, void *const control, typename DOC::Schema &schema, void (*callback)(DOC &)) {
  size_t buffer_written;
  size_t buffer_len = 2*1024*1024;  // 2MB
  std::unique_ptr<char[]> buffer(new char[buffer_len]);

  MessageType msg_type;
  uint64_t doc_num;
  std::string doc_bytes;

  std::string msg;

  // Setup ØMQ polling between the source and control sockets.
  zmq_pollitem_t poll_items[] = {
      {source, 0, ZMQ_POLLIN, 0},
      {control, 0, ZMQ_POLLIN, 0},
  };

  // Infinite loop for reading messages.
  bool terminate = false;
  while (!terminate) {
    if (zmq_poll(poll_items, sizeof(poll_items)/sizeof(zmq_pollitem_t), -1) == -1) {
      LOG(CRITICAL) << "Call to zmq_poll failed: " << zmq_strerror(zmq_errno()) << std::endl;
      return false;
    }

    // Receive the message on the socket that was polled.
    void *socket = nullptr;
    if (poll_items[0].revents & ZMQ_POLLIN)
      socket = source;
    else if (poll_items[1].revents & ZMQ_POLLIN)
      socket = control;
    else
      continue;
    if (!recv_multipart(socket, buffer, buffer_len, buffer_written))
      return false;

    // Decode the received message.
    unpack_message(buffer.get(), buffer_written, msg_type, doc_num, doc_bytes);

    // Act upon the received message.
    switch (msg_type) {
    case MessageType::DOCUMENT:
      doc_bytes = process_doc(doc_bytes, schema, callback);
      msg = build_message(MessageType::DOCUMENT, doc_num, doc_bytes);
      if (!safe_zmq_send(sink, msg.c_str(), msg.size(), 0))
        return false;
      break;
    case MessageType::TERMINATE:
      LOG(INFO) << "Received command to terminate. Shutting down." << std::endl;
      terminate = true;
      break;
    default:
      LOG(ERROR) << "Unknown message type received: " << static_cast<uint8_t>(msg_type) << std::endl;
      break;
    }
  }

  return true;
}


template <typename DOC>
static bool
drworker(const std::string &source_addr, const std::string &sink_addr, const std::string &control_addr, typename DOC::Schema &schema, void (*callback)(DOC &)) {
  // Prepare the ØMQ context and connect to the sockets.
  void *context, *source, *sink, *control;
  if (!safe_zmq_ctx_new(context))
    return false;
  if (!safe_zmq_socket_connect(context, source, ZMQ_PULL, source_addr))
    return false;
  if (!safe_zmq_socket_connect(context, sink, ZMQ_PUSH, sink_addr))
    return false;
  if (!safe_zmq_socket_connect(context, control, ZMQ_SUB, control_addr))
    return false;
  if (zmq_setsockopt(control, ZMQ_SUBSCRIBE, nullptr, 0) == -1) {
    LOG(CRITICAL) << "Failed to set ZMQ_SUBSCRIBE filter on socket: " << zmq_strerror(zmq_errno()) << std::endl;
    return false;
  }

  // Receive and process documents from the source, sending results back to the sink.
  bool success = drworker_recv(source, sink, control, schema, callback);

  // Close the sockets and destroy the ØMQ context.
  success &= safe_zmq_close(control);
  success &= safe_zmq_close(sink);
  success &= safe_zmq_close(source);
  success &= safe_zmq_ctx_destroy(context);
  return success;
}


template <typename DOC, typename LOGGER>
int
worker_main(const int argc, char **const argv, cf::Main &cfg, typename DOC::Schema &schema, void (*callback)(DOC &)) {
  // Build upon an option parser.
  cf::Op<std::string> host(cfg, "host", "The network host to connect to", "127.0.0.1");
  cf::Op<uint32_t> source_port(cfg, "source-port", "The network port to bind to on which to pull docrep documents", 7301);
  cf::Op<uint32_t> sink_port(cfg, "sink-port", "The network port to bind to on which to push docrep documents", 7302);
  cf::Op<uint32_t> control_port(cfg, "control-port", "The network port to bind to on which to subscribe to control messages", 7303);
  dr::DocrepGroup dr(cfg, schema);

  // Parse argv.
  cfg.main<LOGGER>(argc, argv);

  // Run the drworker main.
  const std::string source_addr = schwa::drdist::build_socket_addr(host(), source_port());
  const std::string sink_addr = schwa::drdist::build_socket_addr(host(), sink_port());
  const std::string control_addr = schwa::drdist::build_socket_addr(host(), control_port());

  const bool success = schwa::drdist::drworker(source_addr, sink_addr, control_addr, schema, callback);
  return success ? 0 : 1;
}


template <typename DOC, typename LOGGER>
int
worker_main(const int argc, char **const argv, cf::Main &cfg, void (*callback)(DOC &)) {
  typename DOC::Schema schema;
  return worker_main<DOC, LOGGER>(argc, argv, cfg, schema, callback);
}

}  // drdist
}  // schwa
