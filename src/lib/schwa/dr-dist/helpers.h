/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRDIST_HELPERS_H_
#define SCHWA_DRDIST_HELPERS_H_

#include <schwa/_base.h>

#include <iosfwd>
#include <memory>
#include <string>


namespace schwa {
  namespace dr_dist {

    enum class MessageType : uint8_t {
      DOCUMENT = 0,
      DOCUMENT_COUNT = 1,
      TERMINATE = 2,
    };

    bool safe_zmq_close(void *socket);
    bool safe_zmq_ctx_destroy(void *context);
    bool safe_zmq_ctx_new(void *&context);
    bool safe_zmq_send(void *socket, const void *buf, size_t len, int flags);
    bool safe_zmq_socket_bind(void *zmq_context, void *&socket, int zmq_socket_type, const std::string &addr);
    bool safe_zmq_socket_connect(void* zmq_context, void *&socket, int zmq_socket_type, const std::string &addr);

    bool recv_multipart(void *socket, std::unique_ptr<char[]> &buffer, size_t &buffer_len, size_t &buffer_written);

    std::string build_message(MessageType type, uint64_t doc_num, const std::string &doc);
    std::string build_message(MessageType type, uint64_t doc_num, const char *doc, size_t doc_len);
    void        unpack_message(const char *buf, size_t buf_len, MessageType &msg_type, uint64_t &doc_num, std::string &doc);

    std::string build_socket_addr(const std::string &host, uint32_t port);

  }
}

#endif  // SCHWA_DRDIST_HELPERS_H_
