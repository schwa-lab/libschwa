/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_BUFFER_H_
#define SCHWA_UTILS_BUFFER_H_

#include <cstdlib>
#include <iosfwd>

#include <schwa/_base.h>
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

    void _write(const uint8_t *const data, const size_t nbytes);

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

    void consume(std::istream &in);

    inline void write(const uint8_t byte) { _write(&byte, 1); }
    inline void write(const uint8_t *const data, const size_t nbytes) { _write(data, nbytes); }
    inline void write(const uint8_t *const start, const uint8_t *const end) { _write(start, end - start); }

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(Buffer);
  };


  template <typename ALLOC=schwa::AlignedAllocator<uint8_t>>
  class OffsetBuffer {
  public:
    using allocator_type = ALLOC;

  private:
    allocator_type _allocator;
    const size_t _nitems_grow;
    size_t _nitems_allocd;
    size_t _nitems_used;
    size_t _initial_offset;
    uint8_t *_bytes;
    uint32_t *_offsets;

    template <typename U> friend class OffsetBuffer;

  public:
    explicit OffsetBuffer(size_t nitems_grow, size_t initial_offset=0, const allocator_type &allocator=allocator_type());
    OffsetBuffer(const OffsetBuffer &o);
    ~OffsetBuffer(void);

    inline allocator_type &allocator(void) const { return _allocator; }
    inline uint8_t *bytes(void) const { return _bytes; }
    inline bool empty(void) const { return _nitems_used == 0; }
    inline size_t initial_offset(void) const { return _initial_offset; }
    inline size_t nitems_allocd(void) const { return _nitems_allocd; }
    inline size_t nitems_grow(void) const { return _nitems_grow; }
    inline size_t nitems_used(void) const { return _nitems_used; }
    inline uint32_t *offsets(void) const { return _offsets; }

    inline void clear(void) { _nitems_used = 0; }
    inline void set_initial_offset(const size_t initial_offset) { _initial_offset = initial_offset; }

    void write(uint8_t byte, uint32_t offset);
  };

}  // namespace schwa

#include <schwa/utils/buffer_impl.h>

#endif  // SCHWA_UTILS_BUFFER_H_
