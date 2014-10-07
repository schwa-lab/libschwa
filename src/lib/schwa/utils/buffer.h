/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_BUFFER_H_
#define SCHWA_UTILS_BUFFER_H_

#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include <schwa/_base.h>


namespace schwa {

  class Buffer {
  public:
    static constexpr const size_t DEFAULT_ALIGNMENT = 32;

  private:
    const size_t _alignment;
    const size_t _grow;
    size_t _allocd;
    size_t _used;
    uint8_t *_buffer;

    inline void
    _write(const uint8_t *const data, const size_t nbytes) {
      if (SCHWA_UNLIKELY(_used + nbytes > _allocd)) {
        void *b;
        const int ret = ::posix_memalign(&b, _alignment, _allocd + _grow);
        if (SCHWA_UNLIKELY(ret != 0))
          throw std::bad_alloc();
        std::memcpy(b, _buffer, _used);
        std::free(_buffer);
        _buffer = static_cast<uint8_t *>(b);
        _allocd += _grow;
      }
      std::memcpy(_buffer + _used, data, nbytes);
      _used += nbytes;
    }

  public:
    explicit Buffer(size_t grow_nbytes, size_t alignment=DEFAULT_ALIGNMENT) : _alignment(alignment), _grow(grow_nbytes), _allocd(0), _used(0), _buffer(nullptr) { }
    ~Buffer(void) {
      std::free(_buffer);
    }

    inline size_t allocd(void) const { return _allocd; }
    inline uint8_t *buffer(void) const { return _buffer; }
    inline size_t grow(void) const { return _grow; }
    inline size_t used(void) const { return _used; }

    inline void clear(void) { _used = 0; }
    inline bool empty(void) const { return _used == 0; }

    inline void write(const uint8_t byte) { _write(&byte, 1); }
    inline void write(const uint8_t *const data, const size_t nbytes) { _write(data, nbytes); }
    inline void write(const uint8_t *const start, const uint8_t *const end) { _write(start, end - start); }
  };

}  // namespace schwa

#endif  // SCHWA_UTILS_BUFFER_H_
