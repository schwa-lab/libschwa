/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_SGML_H_
#define SCHWA_FORMATS_SGML_H_

#include <cstring>
#include <iosfwd>

#include <schwa/_base.h>
#include <schwa/encoding.h>
#include <schwa/pool.h>
#include <schwa/unicode.h>
#include <schwa/utils/buffer.h>
#include <schwa/utils/ragel.h>


namespace schwa {
  namespace formats {

    class SGMLishAttribute {
    public:
      using PooledOffsetBuffer = OffsetBuffer<PoolAllocator<uint8_t>>;

    private:
      const uint8_t *const _name;
      const uint8_t *const _value;
      SGMLishAttribute *_next;

    public:
      SGMLishAttribute(const uint8_t *name, const uint8_t *value, SGMLishAttribute *next=nullptr);

      inline const uint8_t *name(void) const { return _name; }
      inline SGMLishAttribute *next(void) const { return _next; }
      inline const uint8_t *value(void) const { return _value; }

      inline void set_next(SGMLishAttribute *next) { _next = next; }

      std::ostream &pprint(std::ostream &out) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(SGMLishAttribute);
    };


    enum class SGMLishNodeType : uint8_t {
      START_TAG, END_TAG,
      EMPTY_TAG,
      COMMENT, CDATA, XML_DECL,
      TEXT,
    };


    class SGMLishNode {
    public:
      using PooledOffsetBuffer = OffsetBuffer<PoolAllocator<uint8_t>>;

    private:
      const SGMLishNodeType _type;
      const PooledOffsetBuffer *_name;
      const PooledOffsetBuffer *_text;
      SGMLishNode *_child;
      SGMLishNode *_sibling;
      SGMLishAttribute *_attribute;

      std::ostream &_pprint(std::ostream &out, unsigned int indent) const;
      inline void _set_sibling(SGMLishNode &node) { _sibling = &node; }

    public:
      SGMLishNode(SGMLishNodeType type, const PooledOffsetBuffer *name, const PooledOffsetBuffer *text, SGMLishAttribute *attribute);

      inline SGMLishAttribute *attribute(void) const { return _attribute; }
      inline SGMLishNode *child(void) const { return _child; }
      inline const PooledOffsetBuffer *name(void) const { return _name; }
      inline SGMLishNode *sibling(void) const { return _sibling; }
      inline const PooledOffsetBuffer *text(void) const { return _text; }
      inline SGMLishNodeType type(void) const { return _type; }

      inline bool is_cdata(void) const { return _type == SGMLishNodeType::CDATA; }
      inline bool is_comment(void) const { return _type == SGMLishNodeType::COMMENT; }
      inline bool is_empty_tag(void) const { return _type == SGMLishNodeType::EMPTY_TAG; }
      inline bool is_end_tag(void) const { return _type == SGMLishNodeType::END_TAG; }
      inline bool is_start_tag(void) const { return _type == SGMLishNodeType::START_TAG; }
      inline bool is_text(void) const { return _type == SGMLishNodeType::TEXT; }
      inline bool is_xml_decl(void) const { return _type == SGMLishNodeType::XML_DECL; }

      inline bool
      has_name(const std::string &name) const {
        if (_name == nullptr)
          return false;
        return std::strncmp(name.c_str(), reinterpret_cast<const char *>(_name->bytes()), _name->nitems_used()) == 0;
      }

      void add_child(SGMLishNode &node);

      std::ostream &pprint(std::ostream &out) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(SGMLishNode);
    };


    class SGMLishLexer {
    public:
      using PooledOffsetBuffer = OffsetBuffer<PoolAllocator<uint8_t>>;

      static constexpr const size_t DEFAULT_BUFFER_GROW_SIZE = 4 * 1024 * 1024;  //!< Default amount the internal buffer grows by.

    private:
      class LexBuffer {
      private:
        const uint8_t *_start;
        PooledOffsetBuffer _buffer;

      public:
        LexBuffer(const size_t buffer_grow, Pool &pool) :
            _start(nullptr),
            _buffer(buffer_grow, 0, PoolAllocator<uint8_t>(pool))
          { }

        inline const PooledOffsetBuffer &buffer(void) const { return _buffer; }
        inline bool empty(void) const { return _buffer.empty(); }
        inline const uint8_t *start(void) const { return _start; }

        inline void
        clear(void) {
          _start = nullptr;
          _buffer.clear();
        }

        inline void
        set_end(const uint8_t *const fpc) {
          uint8_t utf8[4];
          for (const uint8_t *start = _start; start != fpc; ) {
            const size_t nbytes = read_utf8(&start, fpc, utf8);
            write(utf8, nbytes, nbytes);
          }
          _start = nullptr;
        }

        inline void
        set_start(const uint8_t *const begin, const uint8_t *const fpc) {
          _start = fpc;
          _buffer.set_initial_offset(fpc - begin);
        }

        inline void
        write(const unicode_t code_point, const uint32_t offset) {
          uint8_t utf8[4];
          const size_t nbytes = write_utf8(code_point, utf8);
          write(utf8, nbytes, offset);
        }

        inline void
        write(const uint8_t utf8[4], const size_t nbytes, const uint32_t offset) {
          for (size_t i = 0; i != nbytes; ++i)
            _buffer.write(utf8[i], (i == 0) ? offset : 0);
          _start = nullptr;
        }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(LexBuffer);
      };

      const EncodingResult &_encoding_result;
      Pool &_pool;

      RagelState<> _state;

      LexBuffer _attr_name_buffer;
      LexBuffer _tag_name_buffer;
      LexBuffer _text_buffer;

      SGMLishAttribute *_attribute;
      SGMLishNode *_node;

      inline void _attr_name_end(const uint8_t *fpc) { _attr_name_buffer.set_end(fpc); }
      inline void _attr_name_start(const uint8_t *fpc) { _attr_name_buffer.set_start(_encoding_result.utf8(), fpc); }
      inline void _character_end(const uint8_t *fpc) { _text_buffer.set_end(fpc); }
      inline void _character_start(const uint8_t *fpc) { _text_buffer.set_start(_encoding_result.utf8(), fpc); }
      inline void _tag_name_end(const uint8_t *fpc) { _tag_name_buffer.set_end(fpc); }
      inline void _tag_name_start(const uint8_t *fpc) { _tag_name_buffer.set_start(_encoding_result.utf8(), fpc); }

      void _character_reference_decimal(const uint8_t *const fpc);
      void _character_reference_hex(const uint8_t *const fpc);
      void _character_reference_named(const uint8_t *const fpc);

      void _init(void);

      PooledOffsetBuffer *_create_poold_tag_name(void);

      void _create_attr(void);
      void _create_cdata_node(void);
      void _create_comment_node(void);
      void _create_empty_tag_node(void);
      void _create_end_tag_node(void);
      void _create_start_tag_node(void);
      void _create_text_node(void);
      void _create_xml_decl_node(void);

    public:
      SGMLishLexer(const EncodingResult &er, Pool &pool);

      inline bool at_eof(void) const { return _state.at_eof(); }
      inline const uint8_t *get_p(void) const { return _state.p; }
      inline const uint8_t *get_pe(void) const { return _state.pe; }
      inline const RagelState<> &get_state(void) const { return _state; }

      SGMLishNode *lex(void);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(SGMLishLexer);
    };


    /**
     * \code{.cpp}
       Pool pool(...);
       SGMLishParser parser(..., pool);
       for (SGMLishNode *root = nullptr; (root = parser.parse()) != nullptr; ) {
         ...
         pool.clear();  // NOTE: once the pool is cleared, all SGMLishNode objects and their data (recursively) become invalid.
       }
       if (!parser.eof()) {
         throw std::runtime_error("Parser stopped parsing but did not hit EOF");
       }
     * \endcode
     **/
    class SGMLishParser {
    public:
      using PooledOffsetBuffer = OffsetBuffer<PoolAllocator<uint8_t>>;

    private:
      SGMLishLexer _lexer;
      SGMLishNode *_root;

    public:
      SGMLishParser(const EncodingResult &er, Pool &pool);

      inline bool eof(void) const { return _lexer.at_eof(); }
      inline const uint8_t *get_p(void) const { return _lexer.get_p(); }
      inline const uint8_t *get_pe(void) const { return _lexer.get_pe(); }
      inline SGMLishNode *root(void) const { return _root; }

      SGMLishNode *parse(void);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(SGMLishParser);
    };


  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_SGML_H_
