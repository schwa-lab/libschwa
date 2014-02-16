/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRDIST_H_
#define SCHWA_DRDIST_H_

#include <iosfwd>
#include <memory>
#include <string>

#include <schwa/_base.h>
#include <schwa/msgpack.h>

namespace schwa {
  namespace drdist {

    enum class MessageType : uint8_t {
      DOCUMENT = 0,
      TERMINATE = 1,
      DOCUMENT_COUNT = 2,
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

    template <typename IN>
    void
    unpack_message(IN &reader, MessageType &msg_type, uint64_t &doc_num, std::string &doc) {
      namespace mp = schwa::msgpack;
      msg_type = static_cast<MessageType>(mp::read_uint8(reader));
      doc_num = mp::read_uint64(reader);
      doc = mp::read_raw(reader);
    }

    bool read_doc(std::istream &in, std::ostream &out);

  }
}

#endif  // SCHWA_DRDIST_H_
