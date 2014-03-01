/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr-dist/helpers.h>

#include <cstring>
#include <iostream>
#include <sstream>

#include <schwa/io/array_reader.h>
#include <schwa/io/logging.h>
#include <schwa/msgpack.h>
#include <schwa/utils/enums.h>

#include <zmq.h>

namespace io = schwa::io;
namespace mp = schwa::msgpack;


namespace schwa {
namespace dr_dist {

bool
safe_zmq_close(void *const socket) {
  if (zmq_close(socket) == -1) {
    LOG(CRITICAL) << "Failed to close the ØMQ socket: " << zmq_strerror(zmq_errno()) << std::endl;
    return false;
  }
  return true;
}


bool
safe_zmq_ctx_destroy(void *context) {
  if (zmq_ctx_destroy(context) == -1) {
    LOG(CRITICAL) << "Failed to destroy ØMQ context: " << zmq_strerror(zmq_errno()) << std::endl;
    return false;
  }
  return true;
}


bool
safe_zmq_ctx_new(void *&context) {
  context = zmq_ctx_new();
  if (context == nullptr) {
    LOG(CRITICAL) << "Failed to create ØMQ context: " << zmq_strerror(zmq_errno()) << std::endl;
    return false;
  }
  return true;
}


bool
safe_zmq_send(void *const socket, const void *const buf, const size_t len, const int flags) {
  const int nbytes = zmq_send(socket, buf, len, flags);
  if (nbytes == -1) {
    LOG(CRITICAL) << "Failed to send: " << zmq_strerror(zmq_errno()) << std::endl;
    return false;
  }
  else if (static_cast<size_t>(nbytes) != len) {
    LOG(CRITICAL) << "Wrong number of bytes sent (" << nbytes << " instead of " << len << ")" << std::endl;
    return false;
  }
  return true;
}


bool
safe_zmq_socket_bind(void *const zmq_context, void *&socket, const int zmq_socket_type, const std::string &addr) {
  socket = zmq_socket(zmq_context, zmq_socket_type);
  if (socket == nullptr) {
    LOG(CRITICAL) << "Failed to create ØMQ socket: " << zmq_strerror(zmq_errno()) << std::endl;
    return false;
  }
  if (zmq_bind(socket, addr.c_str()) == -1) {
    LOG(CRITICAL) << "Failed to bind the ØMQ socket: " << zmq_strerror(zmq_errno()) << std::endl;
    return false;
  }
  return true;
}


bool
safe_zmq_socket_connect(void *const zmq_context, void *&socket, const int zmq_socket_type, const std::string &addr) {
  socket = zmq_socket(zmq_context, zmq_socket_type);
  if (socket == nullptr) {
    LOG(CRITICAL) << "Failed to create ØMQ socket: " << zmq_strerror(zmq_errno()) << std::endl;
    return false;
  }
  if (zmq_connect(socket, addr.c_str()) == -1) {
    LOG(CRITICAL) << "Failed to connect the ØMQ socket: " << zmq_strerror(zmq_errno()) << std::endl;
    return false;
  }
  return true;
}


std::string
build_message(const MessageType type, const uint64_t doc_num, const std::string &doc) {
  return build_message(type, doc_num, doc.c_str(), doc.size());
}


std::string
build_message(const MessageType type, const uint64_t doc_num, const char *const doc, const size_t doc_len) {
  std::stringstream msg;
  mp::write_uint8(msg, to_underlying(type));
  mp::write_uint64(msg, doc_num);
  mp::write_raw(msg, doc, doc_len);
  msg.seekg(0);
  return msg.str();
}


void
unpack_message(const char *const buf, const size_t buf_len, MessageType &msg_type, uint64_t &doc_num, std::string &doc) {
  io::ArrayReader reader(buf, buf_len);
  msg_type = from_underlying<MessageType>(mp::read_uint8(reader));
  doc_num = mp::read_uint64(reader);
  doc = mp::read_raw(reader);
}


bool
recv_multipart(void *const socket, std::unique_ptr<char[]> &buffer, size_t &buffer_len, size_t &buffer_written) {
  int64_t more = 0;
  size_t more_size = sizeof(more);

  // How far we've populated the buffer.
  char *buffer_upto = buffer.get();
  size_t buffer_left = buffer_len;

  // Read all parts of a multi-part message.
  do {
    // Initialise the ØMQ message part to read into.
    zmq_msg_t msg;
    if (zmq_msg_init(&msg) == -1) {
      LOG(CRITICAL) << "Failed to init ØMQ message: " << zmq_strerror(zmq_errno()) << std::endl;
      return false;
    }

    // Block until a message part is available.
    const int nbytes = zmq_msg_recv(&msg, socket, 0);
    if (nbytes == -1) {
      LOG(CRITICAL) << "Failed to zmq_msg_recv: " << zmq_strerror(zmq_errno()) << std::endl;
      return false;
    }

    // Increase the size of the buffer if required.
    if (static_cast<size_t>(nbytes) > buffer_left) {
      LOG(DEBUG) << "Increasing buffer size from " << buffer_len << " and adding " << nbytes << std::endl;
      const size_t used = buffer_len - buffer_left;
      buffer_len += nbytes;
      buffer_left += nbytes;
      buffer.reset(new char[buffer_len]);
      buffer_upto = buffer.get() + used;
    }

    // Copy the message part into our buffer.
    std::memcpy(buffer_upto, zmq_msg_data(&msg), nbytes);
    buffer_upto += nbytes;
    buffer_left -= nbytes;

    // Determine if there are any more parts to this message.
    if (zmq_getsockopt(socket, ZMQ_RCVMORE, &more, &more_size) == -1) {
      LOG(CRITICAL) << "Failed to zmq_getsockopt ZMQ_RCVMORE: " << zmq_strerror(zmq_errno()) << std::endl;
      return false;
    }

    // Close the ØMQ message part.
    if (zmq_msg_close(&msg) == -1) {
      LOG(CRITICAL) << "Failed to close ØMQ message: " << zmq_strerror(zmq_errno()) << std::endl;
      return false;
    }
  } while (more);

  // Update how many bytes were written to the buffer.
  buffer_written = buffer_len - buffer_left;

  return true;
}


std::string
build_socket_addr(const std::string &host, const uint32_t port) {
  std::ostringstream ss;
  ss << "tcp://" << host << ":" << port;
  return ss.str();
}

}
}
