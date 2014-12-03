/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_BUFFER_IMPL_H_
#define SCHWA_UTILS_BUFFER_IMPL_H_

#include <cstring>
#include <iostream>


namespace schwa {

  // ============================================================================
  // Buffer
  // ============================================================================
  template <typename ALLOC>
  Buffer<ALLOC>::Buffer(const size_t nbytes_grow, const allocator_type &allocator) :
      _allocator(allocator),
      _nbytes_grow(nbytes_grow),
      _nbytes_allocd(0),
      _nbytes_used(0),
      _bytes(nullptr)
    { }

  template <typename ALLOC>
  Buffer<ALLOC>::~Buffer(void) {
    _allocator.deallocate(_bytes, _nbytes_allocd);
  }


  template <typename ALLOC>
  inline void
  Buffer<ALLOC>::_grow(const size_t nbytes) {
    uint8_t *bytes = _allocator.allocate(_nbytes_allocd + nbytes);
    std::memcpy(bytes, _bytes, _nbytes_used);
    _allocator.deallocate(_bytes, _nbytes_allocd);
    _bytes = bytes;
    _nbytes_allocd += nbytes;
  }


  template <typename ALLOC>
  inline void
  Buffer<ALLOC>::_write(const uint8_t *const data, const size_t nbytes) {
    if (SCHWA_UNLIKELY(_nbytes_used + nbytes > _nbytes_allocd))
      _grow(_nbytes_grow);
    std::memcpy(_bytes + _nbytes_used, data, nbytes);
    _nbytes_used += nbytes;
  }


  template <typename ALLOC>
  inline void
  Buffer<ALLOC>::consume(io::InputStream &in) {
    if (in.is_stdin() || in.is_tty()) {
      Buffer<ALLOC>::consume(static_cast<std::istream &>(in));
      return;
    }

    if (SCHWA_UNLIKELY(_nbytes_used + in.nbytes() > _nbytes_allocd))
      _grow(in.nbytes());
    (*in).read(reinterpret_cast<char *>(_bytes + _nbytes_used), in.nbytes());
    _nbytes_used += (*in).gcount();
  }


  template <typename ALLOC>
  inline void
  Buffer<ALLOC>::consume(std::istream &in) {
    uint8_t buf[4096];
    while (true) {
      in.read(reinterpret_cast<char *>(buf), sizeof(buf));
      const auto nbytes = in.gcount();
      if (nbytes < 1)
        break;
      write(buf, nbytes);
    }
  }


  template <typename ALLOC>
  inline void
  Buffer<ALLOC>::reserve(const size_t nbytes) {
    if (_nbytes_used + nbytes > _nbytes_allocd)
      _grow(nbytes);
  }


  // ============================================================================
  // OffsetBuffer
  // ============================================================================
  template <typename ALLOC>
  OffsetBuffer<ALLOC>::OffsetBuffer(const size_t nitems_grow, const size_t initial_offset, const allocator_type &allocator) :
      BaseOffsetBuffer(nitems_grow, initial_offset),
      _allocator(allocator)
    { }

  template <typename ALLOC> template <typename A>
  OffsetBuffer<ALLOC>::OffsetBuffer(const OffsetBuffer<A> &o, const allocator_type &allocator) :
      BaseOffsetBuffer(o),
      _allocator(allocator)
    {
    _nitems_allocd = _nitems_used;
    _bytes = reinterpret_cast<uint8_t *>(_allocator.allocate(_nitems_allocd*sizeof(uint8_t)));
    _offsets = reinterpret_cast<uint32_t *>(_allocator.allocate(_nitems_allocd*sizeof(uint32_t)));
    std::memcpy(_bytes, o._bytes, _nitems_used*sizeof(uint8_t));
    std::memcpy(_offsets, o._offsets, _nitems_used*sizeof(uint32_t));
  }

  template <typename ALLOC>
  OffsetBuffer<ALLOC>::~OffsetBuffer(void) {
    _allocator.deallocate(reinterpret_cast<char *>(_bytes), _nitems_allocd*sizeof(uint8_t));
    _allocator.deallocate(reinterpret_cast<char *>(_offsets), _nitems_allocd*sizeof(uint32_t));
  }


  template <typename ALLOC>
  inline void
  OffsetBuffer<ALLOC>::_grow(const size_t nitems) {
    const size_t new_nitems_allocd = _nitems_allocd + nitems;
    uint8_t *bytes = reinterpret_cast<uint8_t *>(_allocator.allocate(new_nitems_allocd*sizeof(uint8_t)));
    uint32_t *offsets = reinterpret_cast<uint32_t *>(_allocator.allocate(new_nitems_allocd*sizeof(uint32_t)));
    std::memcpy(bytes, _bytes, _nitems_used*sizeof(uint8_t));
    std::memcpy(offsets, _offsets, _nitems_used*sizeof(uint32_t));
    _allocator.deallocate(reinterpret_cast<char *>(_bytes), _nitems_allocd*sizeof(uint8_t));
    _allocator.deallocate(reinterpret_cast<char *>(_offsets), _nitems_allocd*sizeof(uint32_t));
    _bytes = bytes;
    _offsets = offsets;
    _nitems_allocd = new_nitems_allocd;
  }


  template <typename ALLOC>
  inline void
  OffsetBuffer<ALLOC>::reserve(const size_t nitems) {
    if (_nitems_allocd - _nitems_used < nitems)
      _grow(std::max(nitems, _nitems_grow));
  }


  template <typename ALLOC>
  inline void
  OffsetBuffer<ALLOC>::write(const uint8_t byte, const uint32_t offset) {
    if (SCHWA_UNLIKELY(_nitems_used == _nitems_allocd))
      _grow(_nitems_grow);
    _bytes[_nitems_used] = byte;
    _offsets[_nitems_used] = offset;
    ++_nitems_used;
  }

}  // namespace schwa

#endif  // SCHWA_UTILS_BUFFER_IMPL_H_
