/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_NEW_TOKENIZER_TOKENIZER_H_
#define SCHWA_NEW_TOKENIZER_TOKENIZER_H_

#include <iostream>
#include <string>

#include <schwa/_base.h>
#include <schwa/canonical-schema.h>
#include <schwa/memory.h>
#include <schwa/utils/buffer.h>
#include <schwa/utils/ragel.h>


namespace schwa {
  namespace new_tokenizer {

    enum class BreakFlag : uint8_t {
      NONE = 0,
      FORCE_SENTENCE_BREAK,
    };


    template <typename ALLOC=schwa::AlignedAllocator<uint8_t>>
    class OffsetInputStream {
    private:
      class Iterator {
      private:
        const OffsetInputStream *_ois;
        size_t _index;

      public:
        Iterator(void) : _ois(nullptr), _index(0) { }
        Iterator(const OffsetInputStream &ois, const size_t index) : _ois(&ois), _index(index) { }
        Iterator(const Iterator &o) : _ois(o._ois), _index(o._index) { }
        Iterator(const Iterator &&o) : _ois(o._ois), _index(o._index) { }
        ~Iterator(void) { }

        inline Iterator &
        operator =(const Iterator &o) {
          _ois = o._ois;
          _index = o._index;
          return *this;
        }

        inline Iterator &
        operator =(int) {  // Needed for Ragel-generated code (state.ts = 0).
          _ois = nullptr;
          _index = 0;
          return *this;
        }

        inline Iterator &
        operator ++(void) {
          ++_index;
          return *this;
        }

        inline Iterator
        operator ++(int) {
          Iterator it(*this);
          ++(*this);
          return it;
        }

        inline Iterator &
        operator +=(const size_t delta) {
          _index += delta;
          return *this;
        }

        inline Iterator &
        operator --(void) {
          --_index;
          return *this;
        }

        inline Iterator
        operator --(int) {
          Iterator it(*this);
          --(*this);
          return it;
        }

        inline Iterator &
        operator -=(const size_t delta) {
          _index -= delta;
          return *this;
        }

        inline Iterator
        operator +(const size_t delta) {
          Iterator it(*this);
          it += delta;
          return it;
        }

        inline Iterator
        operator -(const size_t delta) {
          Iterator it(*this);
          it -= delta;
          return it;
        }

        inline uint8_t
        operator *(void) const {
          return get_byte();
        }

        inline bool
        operator ==(const Iterator &o) const {
          return _ois == o._ois && _index == o._index;
        }

        inline bool
        operator !=(const Iterator &o) const {
          return _ois != o._ois || _index != o._index;
        }

        inline uint8_t
        get_byte(void) const {
          if (SCHWA_UNLIKELY(_ois == nullptr))
            return 0;
          return _ois->bytes()[_index];
        }

        inline uint8_t *
        get_bytes(void) const {
          if (SCHWA_UNLIKELY(_ois == nullptr))
            return nullptr;
          return _ois->bytes() + _index;
        }

        inline BreakFlag
        get_flag(void) const {
          if (SCHWA_UNLIKELY(_ois == nullptr))
            return BreakFlag::NONE;
          return _ois->flags()[_index];
        }

        inline size_t
        get_index(void) const {
          return _index;
        }

        inline uint32_t
        get_offset(void) const {
          if (SCHWA_UNLIKELY(_ois == nullptr))
            return 0;
          return _ois->offsets()[_index];
        }
      };

    public:
      using allocator_type = ALLOC;
      using iterator = Iterator;

    private:
      allocator_type _allocator;
      const size_t _nitems_grow;
      size_t _nitems_allocd;
      size_t _nitems_used;
      size_t _initial_offset;
      uint8_t *_bytes;
      uint32_t *_offsets;
      BreakFlag *_flags;

      template <typename U> friend class OffsetInputStream;

      void _grow(void) { _grow(_nitems_grow); }
      void _grow(size_t nitems_grow);

    public:
      explicit OffsetInputStream(size_t nitems_grow, size_t initial_offset=0, const allocator_type &allocator=allocator_type());
      ~OffsetInputStream(void);

      inline allocator_type &allocator(void) const { return _allocator; }
      inline uint8_t *bytes(void) const { return _bytes; }
      inline bool empty(void) const { return _nitems_used == 0; }
      inline BreakFlag *flags(void) const { return _flags; }
      inline size_t initial_offset(void) const { return _initial_offset; }
      inline size_t nitems_allocd(void) const { return _nitems_allocd; }
      inline size_t nitems_grow(void) const { return _nitems_grow; }
      inline size_t nitems_used(void) const { return _nitems_used; }
      inline uint32_t *offsets(void) const { return _offsets; }

      inline void clear(void) { _nitems_used = 0; }
      inline void set_initial_offset(const size_t initial_offset) { _initial_offset = initial_offset; }

      inline iterator begin(void) const { return iterator(*this, 0); }
      inline iterator end(void) const { return iterator(*this, _nitems_used); }

      template <typename A>
      void write(const OffsetBuffer<A> &buffer);
      void write(const BaseOffsetBuffer::iterator &begin, const BaseOffsetBuffer::iterator &end);
      void write(uint8_t byte, uint32_t offset, BreakFlag flag=BreakFlag::NONE);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(OffsetInputStream);
    };


    class Tokenizer {
    public:
      class State : public RagelState<OffsetInputStream<>::iterator> {
      public:
        using RagelState<OffsetInputStream<>::iterator>::iterator;

        unsigned int suffix;
        const uint8_t *n1;
        const uint8_t *n2;

        State(void);

        std::ostream &dump(std::ostream &out) const;
        void reset(void);
        void reset(iterator start, iterator end);

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(State);
      };

    private:
      State _state;
      OffsetInputStream<> *_ois;
      canonical_schema::Doc *_doc;
      size_t _ntokens_before;
      bool _seen_terminator;

      bool _tokenize(void);

      void _create_sentence(void);
      void _create_token(OffsetInputStream<>::iterator ts, OffsetInputStream<>::iterator te, const uint8_t *norm);
      void _flush_sentence(void);

      void _close_double_quote(void);
      void _close_single_quote(void);
      void _contraction(void);
      void _double_quote(void);
      void _open_double_quote(void);
      void _open_single_quote(void);
      void _punctuation(const uint8_t *norm=nullptr);
      void _single_quote(void);
      void _split(void);
      void _terminator(const uint8_t *norm=nullptr);
      void _word(void);

    public:
      Tokenizer(void);
      ~Tokenizer(void);

      void tokenize(OffsetInputStream<> &ois, canonical_schema::Doc &doc);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Tokenizer);
    };

  }  // namespace new_tokenizer
}  // namespace schwa

#include <schwa/new-tokenizer/tokenizer_impl.h>

#endif  // SCHWA_NEW_TOKENIZER_TOKENIZER_H_
