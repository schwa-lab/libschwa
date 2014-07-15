/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_HTML_H_
#define SCHWA_FORMATS_HTML_H_

#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <schwa/_base.h>
#include <schwa/unicode.h>


namespace schwa {

  class EncodingResult;

  namespace formats {

    class HTMLLexer {
    public:
      static const std::unordered_map<std::string, unicode_t> NAMED_CHARACTER_REFERENCES;
      static const std::unordered_set<std::string> VOID_TAGS;

    protected:
      const EncodingResult *_encoding_result;
      bool _enable_debug;
      const uint8_t *_comment_start_ptr;
      const uint8_t *_cr_start_ptr;
      const uint8_t *_tag_start_ptr;
      const uint8_t *_tag_name_start_ptr;
      bool _tag_self_closing;
      std::stringstream _text;

      virtual void _debug(const std::string &rule, const uint8_t *ts, const uint8_t *te);
      bool _run(const uint8_t *input, size_t nbytes);

      virtual void _decimal_character_reference_start(const uint8_t *fpc);
      virtual void _decimal_character_reference_end(const uint8_t *fpc);
      virtual void _hex_character_reference_start(const uint8_t *fpc);
      virtual void _hex_character_reference_end(const uint8_t *fpc);
      virtual void _named_character_reference_start(const uint8_t *fpc);
      virtual void _named_character_reference_end(const uint8_t *fpc);
      virtual void _text_character_start(const uint8_t *fpc);
      virtual void _text_character_end(const uint8_t *fpc);

      virtual void _cdata_start(const uint8_t *fpc);
      virtual void _cdata_end(const uint8_t *fpc);
      virtual void _comment_start(const uint8_t *fpc);
      virtual void _comment_end(const uint8_t *fpc);
      virtual void _doctype_start(const uint8_t *fpc);
      virtual void _doctype_end(const uint8_t *fpc);

      virtual void _open_tag_start(const uint8_t *fpc);
      virtual void _open_tag_end(const uint8_t *fpc);
      virtual void _open_tag_name_start(const uint8_t *fpc);
      virtual void _open_tag_name_end(const uint8_t *fpc);
      virtual void _open_tag_self_closing(void);
      virtual void _close_tag_start(const uint8_t *fpc);
      virtual void _close_tag_end(const uint8_t *fpc);
      virtual void _close_tag_name_start(const uint8_t *fpc);
      virtual void _close_tag_name_end(const uint8_t *fpc);

      virtual void _seen_tag(const uint8_t *ts, const uint8_t *te);
      virtual void _seen_text(const uint8_t *ts, const uint8_t *te);

    public:
      explicit HTMLLexer(bool enable_debug=false);
      virtual ~HTMLLexer(void);

      virtual bool run(const uint8_t *input, size_t nbytes);
      virtual bool run(const EncodingResult &er);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(HTMLLexer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_HTML_H_
