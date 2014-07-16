/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_HTTP_H_
#define SCHWA_FORMATS_HTTP_H_

#include <string>

#include <schwa/_base.h>


namespace schwa {
  namespace formats {

    class HTTPLexer {
    protected:
      size_t _content_length_consumed;
      size_t _content_length_stated;
      std::string _current_key;
      std::string _current_val;
      bool _in_content_length;
      bool _seen_content_length;

      bool _run(const uint8_t *input, size_t nbytes);

      virtual void _field_name_start(const uint8_t *fpc);
      virtual void _field_name_end(const uint8_t *fpc);
      virtual void _field_name_consume(const uint8_t c);
      virtual void _field_value_start(const uint8_t *fpc);
      virtual void _field_value_end(const uint8_t *fpc);
      virtual void _field_value_consume(const uint8_t c);

      virtual void _message_body_start(const uint8_t *fpc);
      virtual void _message_body_consume(uint8_t c);
      virtual bool _message_body_test(void);
      virtual void _message_body_end(const uint8_t *fpc);

      virtual void _reset(void);

    public:
      explicit HTTPLexer(void);
      virtual ~HTTPLexer(void);

      bool run(const uint8_t *input, size_t nbytes);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(HTTPLexer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_HTTP_H_
