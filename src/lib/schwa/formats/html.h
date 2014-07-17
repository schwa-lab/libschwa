/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_HTML_H_
#define SCHWA_FORMATS_HTML_H_

#include <list>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <schwa/_base.h>
#include <schwa/unicode.h>


namespace schwa {

  class EncodingResult;

  namespace formats {

    /**
     * http://www.w3.org/html/wg/drafts/html/master/syntax.html#parsing-main-inbody
     **/
    class HTMLLexer {
    public:
      static const std::unordered_map<std::string, unicode_t> NAMED_CHARACTER_REFERENCES;
      static const std::unordered_set<std::string> FORMATTING_TAGS;
      static const std::unordered_set<std::string> VOID_TAGS;

    protected:
      const EncodingResult *_encoding_result;
      bool _enable_debug;
      const uint8_t *_cdata_start_ptr;
      const uint8_t *_comment_start_ptr;
      const uint8_t *_cr_start_ptr;
      const uint8_t *_tag_start_ptr;
      const uint8_t *_tag_name_start_ptr;
      bool _tag_self_closing;
      bool _in_body;
      std::stringstream _text;
      std::string _tag_name;
      std::list<std::string> _open_tags;

      virtual void _debug(const std::string &rule, const uint8_t *ts, const uint8_t *te);
      bool _run(const uint8_t *input, size_t nbytes);

      void _set_tag_name(const uint8_t *tag_name_end_ptr);

      void _decimal_character_reference_start(const uint8_t *fpc);
      void _decimal_character_reference_end(const uint8_t *fpc);
      void _hex_character_reference_start(const uint8_t *fpc);
      void _hex_character_reference_end(const uint8_t *fpc);
      void _named_character_reference_start(const uint8_t *fpc);
      void _named_character_reference_end(const uint8_t *fpc);
      void _text_character_start(const uint8_t *fpc);
      void _text_character_end(const uint8_t *fpc);

      void _cdata_start(const uint8_t *fpc);
      void _cdata_end(const uint8_t *fpc);
      void _comment_start(const uint8_t *fpc);
      void _comment_end(const uint8_t *fpc);
      void _doctype_start(const uint8_t *fpc);
      void _doctype_end(const uint8_t *fpc);

      void _open_tag_start(const uint8_t *fpc);
      void _open_tag_end(const uint8_t *fpc);
      void _open_tag_name_start(const uint8_t *fpc);
      void _open_tag_name_end(const uint8_t *fpc);
      void _open_tag_self_closing(void);
      void _close_tag_start(const uint8_t *fpc);
      void _close_tag_end(const uint8_t *fpc);
      void _close_tag_name_start(const uint8_t *fpc);
      void _close_tag_name_end(const uint8_t *fpc);

      virtual void _seen_tag(const uint8_t *ts, const uint8_t *te);
      virtual void _seen_text(const uint8_t *ts, const uint8_t *te);

      virtual void _reset(const EncodingResult *er);
      virtual void _begin_document(void);
      virtual void _end_document(void);

    public:
      explicit HTMLLexer(bool enable_debug=false);
      virtual ~HTMLLexer(void);

      bool run(const uint8_t *input, size_t nbytes);
      bool run(const EncodingResult &er);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(HTMLLexer);
    };


    class HTMLCharsetSniffer {
    private:
      const uint8_t *_start;
      bool _is_charset;
      bool _is_content_type;
      std::string _current_key;
      std::string _content_type;
      std::string _charset;

      bool _run_charset_sniff(const uint8_t *input, size_t nbytes);
      bool _run_content_type(const uint8_t *input, size_t nbytes);

      void _attribute_name_start(const uint8_t *fpc);
      void _attribute_name_end(const uint8_t *fpc);
      void _attribute_value_start(const uint8_t *fpc);
      void _attribute_value_end(const uint8_t *fpc);

      void _content_type_param_key_start(const uint8_t *fpc);
      void _content_type_param_key_end(const uint8_t *fpc);
      void _content_type_param_val_start(const uint8_t *fpc);
      void _content_type_param_val_end(const uint8_t *fpc);

      void _meta_end(void);

      void _reset(void);

    public:
      HTMLCharsetSniffer(void);
      ~HTMLCharsetSniffer(void) { }

      inline const std::string &charset(void) const { return _charset; }
      inline bool has_charset(void) const { return !_charset.empty(); }

      bool run(const uint8_t *input, size_t nbytes);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(HTMLCharsetSniffer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_HTML_H_
