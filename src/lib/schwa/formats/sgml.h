/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_SGML_H_
#define SCHWA_FORMATS_SGML_H_

#include <cstring>
#include <iosfwd>

#include <schwa/_base.h>
#include <schwa/encoding.h>
#include <schwa/pool.h>
#include <schwa/utils/buffer.h>


namespace schwa {

  namespace formats {

    class SGMLishAttribute {
    private:
      const uint8_t *const _name;
      const uint8_t *const _value;
      SGMLishAttribute *_next;

    public:
      SGMLishAttribute(const uint8_t *name, const uint8_t *value, SGMLishAttribute *next=nullptr) : _name(name), _value(value), _next(next) { }

      inline const uint8_t *name(void) const { return _name; }
      inline SGMLishAttribute *next(void) const { return _next; }
      inline const uint8_t *value(void) const { return _value; }

      inline void set_next(SGMLishAttribute *next) { _next = next; }

      std::ostream &pprint(std::ostream &out) const;
    };


    enum class SGMLishNodeType : uint8_t {
      START_TAG, END_TAG,
      EMPTY_TAG,
      COMMENT, CDATA, XML_DECL,
      TEXT,
    };


    class SGMLishNode {
    private:
      const SGMLishNodeType _type;
      const uint8_t *const _data;
      SGMLishNode *_child;
      SGMLishNode *_sibling;
      SGMLishAttribute *_attribute;

      inline void _set_sibling(SGMLishNode &node) { _sibling = &node; }

    public:
      SGMLishNode(SGMLishNodeType type, const uint8_t *data, SGMLishAttribute *attribute=nullptr);

      inline SGMLishAttribute *attribute(void) const { return _attribute; }
      inline SGMLishNode *child(void) const { return _child; }
      inline const uint8_t *name(void) const { return _data; }
      inline SGMLishNode *sibling(void) const { return _sibling; }
      inline const uint8_t *text(void) const { return _data; }
      inline SGMLishNodeType type(void) const { return _type; }

      void add_child(SGMLishNode &node);

      std::ostream &pprint(std::ostream &out) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(SGMLishNode);
    };


    class SGMLishLexer {
    public:
      static constexpr const size_t DEFAULT_BUFFER_GROW_SIZE = 4 * 1024 * 1024;  //!< Default amount the internal buffer grows by.

    protected:
      class LexBuffer {
      private:
        const uint8_t *_start;
        Buffer _buffer;

      public:
        explicit LexBuffer(const size_t buffer_grow) : _start(nullptr), _buffer(buffer_grow) { }

        inline void
        clear(void) {
          _start = nullptr;
          _buffer.clear();
        }

        inline const Buffer &buffer(void) const { return _buffer; }
        inline bool empty(void) const { return _buffer.empty(); }
        inline const uint8_t *start(void) const { return _start; }

        inline std::ostream &
        output(std::ostream &out) const {
          out.write(reinterpret_cast<const char *>(_buffer.buffer()), _buffer.used());
          return out;
        }

        inline std::ostream &
        output(std::ostream &out, const uint8_t *const fpc) const {
          out.write(reinterpret_cast<const char *>(_start), fpc - _start);
          return out;
        }

        inline void set_end(const uint8_t *const fpc) { _buffer.write(_start, fpc); }
        inline void set_start(const uint8_t *const fpc) { _start = fpc; }

        inline void
        write(const uint8_t c) {
          _buffer.write(c);
          _start = nullptr;
        }

        inline void
        write(const uint8_t *data, const size_t nbytes) {
          _buffer.write(data, nbytes);
          _start = nullptr;
        }
      };

      const EncodingResult &_encoding_result;
      Pool *_pool;

      const uint8_t *p;
      const uint8_t *pe;
      const uint8_t *eof;
      const uint8_t *ts;
      const uint8_t *te;
      int act;
      int cs;

      LexBuffer _attr_name_buffer;
      LexBuffer _tag_name_buffer;
      LexBuffer _text_buffer;

      SGMLishAttribute *_attribute;
      SGMLishNode *_node;

      inline void _attr_name_end(const uint8_t *fpc) { _attr_name_buffer.set_end(fpc); }
      inline void _attr_name_start(const uint8_t *fpc) { _attr_name_buffer.set_start(fpc); }
      inline void _character_end(const uint8_t *fpc) { _text_buffer.set_end(fpc); }
      inline void _character_start(const uint8_t *fpc) { _text_buffer.set_start(fpc); }
      inline void _tag_name_end(const uint8_t *fpc) { _tag_name_buffer.set_end(fpc); }
      inline void _tag_name_start(const uint8_t *fpc) { _tag_name_buffer.set_start(fpc); }

      void _character_reference_decimal(const uint8_t *const fpc);
      void _character_reference_hex(const uint8_t *const fpc);
      void _character_reference_named(const uint8_t *const fpc);

      void _init(void);

      uint8_t *_create_poold_tag_name(void);

      void _create_attr(void);
      void _create_cdata_node(void);
      void _create_comment_node(void);
      void _create_empty_tag_node(void);
      void _create_end_tag_node(void);
      void _create_start_tag_node(void);
      void _create_text_node(void);
      void _create_xml_decl_node(void);

    public:
      const SGMLishLexer(const EncodingResult &er);

      inline bool at_eof(void) const { return p == pe; }
      inline const uint8_t *get_p(void) const { return p; }
      inline const uint8_t *get_pe(void) const { return pe; }

      SGMLishNode *lex(Pool &pool);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(SGMLishLexer);
    };


    /**
     * \code{.cpp}
       Pool pool(...);
       SGMLishParser parser(...);
       for (SGMLishNode *root = nullptr; (root = parser.parse(pool)) != nullptr; ) {
         ...
         pool.clear();  // NOTE: once the pool is cleared, all SGMLishNode objects and their data (recursively) become invalid.
       }
       if (!parser.eof()) {
         throw std::runtime_error("Parser stopped parsing but did not hit EOF");
       }
     * \endcode
     **/
    class SGMLishParser {
    private:
      SGMLishLexer _lexer;
      SGMLishNode *_root;

    public:
      explicit SGMLishParser(const EncodingResult &er);

      inline bool eof(void) const { return _lexer.at_eof(); }
      inline const uint8_t *get_p(void) const { return _lexer.get_p(); }
      inline const uint8_t *get_pe(void) const { return _lexer.get_pe(); }
      inline SGMLishNode *root(void) const { return _root; }

      SGMLishNode *parse(Pool &pool);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(SGMLishParser);
    };


  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_SGML_H_
