/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_HTTP_H_
#define SCHWA_FORMATS_HTTP_H_

#include <string>

#include <schwa/_base.h>


namespace schwa {
  namespace formats {

    class HTTPParser {
    protected:
      const uint8_t *_content_type_ptr;
      const uint8_t *_message_ptr;
      size_t _content_length_consumed;
      size_t _content_length_stated;
      bool _in_content_length;
      bool _in_content_type_charset;
      bool _seen_content_length;
      unsigned int _status_code;
      std::string _content_type;
      std::string _content_type_charset;

      bool _run(const uint8_t *input, size_t nbytes);

      void _content_type_type_start(const uint8_t *fpc);
      void _content_type_type_end(const uint8_t *fpc);
      void _content_type_subtype_start(const uint8_t *fpc);
      void _content_type_subtype_end(const uint8_t *fpc);
      void _content_type_param_key_start(const uint8_t *fpc);
      void _content_type_param_key_end(const uint8_t *fpc);
      void _content_type_param_val_start(const uint8_t *fpc);
      void _content_type_param_val_end(const uint8_t *fpc);

      void _message_body_start(const uint8_t *fpc);
      void _message_body_consume(uint8_t fc);
      bool _message_body_test(void);
      void _message_body_end(const uint8_t *fpc);

      void _content_length_start(const uint8_t *fpc);
      void _content_length_end(const uint8_t *fpc);
      void _content_length_consume(const uint8_t fc);

      void _status_code_consume(const uint8_t fc);

      virtual void _reset(void);

    public:
      explicit HTTPParser(void);
      virtual ~HTTPParser(void);

      inline const std::string &content_type(void) const { return _content_type; }
      inline const std::string &charset(void) const { return _content_type_charset; }
      inline bool has_charset(void) const { return !_content_type_charset.empty(); }
      inline const uint8_t *message(void) const { return _message_ptr; }
      inline size_t message_nbytes(void) const { return _content_length_consumed; }
      inline unsigned int status_code(void) const { return _status_code; }

      bool run(const uint8_t *input, size_t nbytes);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(HTTPParser);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_HTTP_H_
