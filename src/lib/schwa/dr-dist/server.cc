/* -*- Mode: C++; indent-tabs-mode: nil -*- */
/**
 * The parallel processing topology used here is what is described in Figure 19 of
 * http://zguide.zeromq.org/page:all#Handling-Errors-and-ETERM.
 *
 * \image html fig19.png
 **/
#include <schwa/dr-dist/server.h>

#include <cerrno>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_map>

#include <schwa/config.h>
#include <schwa/dr/reader.h>
#include <schwa/dr-dist/helpers.h>
#include <schwa/io/logging.h>

#include <zmq.h>

namespace dr = schwa::dr;
namespace io = schwa::io;

static volatile bool sink_created = false;
static std::condition_variable sink_created_cv;
static std::mutex sink_created_lock;


namespace schwa {
namespace dr_dist {

bool
source(const std::string &source_addr, const std::string &direct_sink_addr, std::istream &input) {
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
    if (!dr::read_lazy_doc(input, doc))
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


bool
sink(const std::string &sink_addr, const std::string &control_addr, const bool preserve_order, const bool kill_clients, std::ostream &output) {
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

}  // namespace dr_dist
}  // namespace schwa
