/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_BUFFER_H_
#define SCHWA_UTILS_BUFFER_H_

#include <cstdlib>
#include <iosfwd>
#include <numeric>

#include <schwa/_base.h>
#include <schwa/io/streams.h>
#include <schwa/memory.h>


namespace schwa {

  template <typename ALLOC=schwa::AlignedAllocator<uint8_t>>
  class Buffer {
  public:
    using allocator_type = ALLOC;

  private:
    allocator_type _allocator;
    const size_t _nbytes_grow;
    size_t _nbytes_allocd;
    size_t _nbytes_used;
    uint8_t *_bytes;

    void _grow(size_t nbytes);
    void _write(const uint8_t *data, size_t nbytes);

  public:
    explicit Buffer(size_t nbytes_grow, const allocator_type &allocator=allocator_type());
    ~Buffer(void);

    inline allocator_type &allocator(void) const { return _allocator; }
    inline size_t allocd(void) const { return _nbytes_allocd; }
    inline uint8_t *bytes(void) const { return _bytes; }
    inline size_t nbytes_grow(void) const { return _nbytes_grow; }
    inline size_t used(void) const { return _nbytes_used; }

    inline void clear(void) { _nbytes_used = 0; }
    inline bool empty(void) const { return _nbytes_used == 0; }

    void consume(io::InputStream &in);
    void consume(std::istream &in);
    void reserve(size_t nbytes);

    inline void write(const uint8_t byte) { _write(&byte, 1); }
    inline void write(const uint8_t *const data, const size_t nbytes) { _write(data, nbytes); }
    inline void write(const uint8_t *const start, const uint8_t *const end) { _write(start, end - start); }

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(Buffer);
  };


  class BaseOffsetBuffer {
  protected:
    class Iterator {
    private:
      const BaseOffsetBuffer *_buffer;
      size_t _index;

    public:
      Iterator(void) : _buffer(nullptr), _index(0) { }
      Iterator(const BaseOffsetBuffer &buffer, const size_t index) : _buffer(&buffer), _index(index) { }
      Iterator(const Iterator &o) : _buffer(o._buffer), _index(o._index) { }
      Iterator(const Iterator &&o) : _buffer(o._buffer), _index(o._index) { }
      ~Iterator(void) { }

      inline Iterator &
      operator =(const Iterator &o) {
        _buffer = o._buffer;
        _index = o._index;
        return *this;
      }

      inline Iterator &
      operator =(int) {  // Needed for Ragel-generated code (state.ts = 0).
        _buffer = nullptr;
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
      operator +(const size_t delta) const {
        Iterator it(*this);
        it += delta;
        return it;
      }

      inline Iterator
      operator -(const size_t delta) const {
        Iterator it(*this);
        it -= delta;
        return it;
      }

      inline ssize_t
      operator -(const Iterator &o) const {
        return _index - o._index;
      }

      inline uint8_t
      operator *(void) const {
        return get_byte();
      }

      inline bool
      operator ==(const Iterator &o) const {
        return _buffer == o._buffer && _index == o._index;
      }

      inline bool
      operator !=(const Iterator &o) const {
        return _buffer != o._buffer || _index != o._index;
      }

      inline uint8_t
      get_byte(void) const {
        if (SCHWA_UNLIKELY(_buffer == nullptr))
          return 0;
        return _buffer->bytes()[_index];
      }

      inline uint8_t *
      get_bytes(void) const {
        if (SCHWA_UNLIKELY(_buffer == nullptr))
          return nullptr;
        return _buffer->bytes() + _index;
      }

      inline size_t
      get_index(void) const {
        return _index;
      }

      inline uint32_t
      get_offset(void) const {
        if (SCHWA_UNLIKELY(_buffer == nullptr))
          return 0;
        return _buffer->offsets()[_index];
      }

      inline uint32_t *
      get_offsets(void) const {
        if (SCHWA_UNLIKELY(_buffer == nullptr))
          return nullptr;
        return _buffer->offsets() + _index;
      }

      inline size_t
      get_summed_offset(void) const {
        if (SCHWA_UNLIKELY(_buffer == nullptr))
          return 0;
        return _buffer->get_summed_offset(_index);
      }
    };

  public:
    using iterator = Iterator;

  protected:
    const size_t _nitems_grow;
    size_t _nitems_allocd;
    size_t _nitems_used;
    size_t _initial_offset;
    uint8_t *_bytes;
    uint32_t *_offsets;

  public:
    explicit BaseOffsetBuffer(size_t nitems_grow, size_t initial_offset=0);
    BaseOffsetBuffer(const BaseOffsetBuffer &o);
    ~BaseOffsetBuffer(void) { }

    inline uint8_t *bytes(void) const { return _bytes; }
    inline bool empty(void) const { return _nitems_used == 0; }
    inline size_t initial_offset(void) const { return _initial_offset; }
    inline size_t nitems_allocd(void) const { return _nitems_allocd; }
    inline size_t nitems_grow(void) const { return _nitems_grow; }
    inline size_t nitems_used(void) const { return _nitems_used; }
    inline uint32_t *offsets(void) const { return _offsets; }

    inline iterator begin(void) const { return iterator(*this, 0); }
    inline iterator end(void) const { return iterator(*this, _nitems_used); }

    inline size_t get_summed_offset(const size_t index) const { return std::accumulate(_offsets, _offsets + index, _initial_offset); }

    inline void clear(void) { _nitems_used = 0; }
    inline void set_initial_offset(const size_t initial_offset) { _initial_offset = initial_offset; }
  };


  template <typename ALLOC=schwa::AlignedAllocator<char>>
  class OffsetBuffer : public BaseOffsetBuffer {
  public:
    using allocator_type = ALLOC;
    using iterator = BaseOffsetBuffer::iterator;

  private:
    allocator_type _allocator;

    template <typename U> friend class OffsetBuffer;

    void _grow(size_t nitems);

  public:
    explicit OffsetBuffer(size_t nitems_grow, size_t initial_offset=0, const allocator_type &allocator=allocator_type());
    template <typename A> OffsetBuffer(const OffsetBuffer<A> &o, const allocator_type &allocator=allocator_type());
    ~OffsetBuffer(void);

    inline allocator_type &allocator(void) const { return _allocator; }
    void reserve(size_t nitems);
    void write(uint8_t byte, uint32_t offset);
  };

}  // namespace schwa

#include <schwa/utils/buffer_impl.h>

#endif  // SCHWA_UTILS_BUFFER_H_
