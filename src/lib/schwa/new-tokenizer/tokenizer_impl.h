/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_NEW_TOKENIZER_TOKENIZER_IMPL_H_
#define SCHWA_NEW_TOKENIZER_TOKENIZER_IMPL_H_

#include <algorithm>

#include <schwa/utils/enums.h>


namespace schwa {
  namespace new_tokenizer {

    template <typename ALLOC>
    OffsetInputStream<ALLOC>::OffsetInputStream(const size_t nitems_grow, const size_t initial_offset, const allocator_type &allocator) :
        _allocator(allocator),
        _nitems_grow(nitems_grow),
        _nitems_allocd(0),
        _nitems_used(0),
        _initial_offset(initial_offset),
        _bytes(nullptr),
        _offsets(nullptr),
        _flags(nullptr)
      { }

    template <typename ALLOC>
    OffsetInputStream<ALLOC>::~OffsetInputStream(void) {
      _allocator.deallocate(_bytes, _nitems_allocd*sizeof(uint8_t));
      _allocator.deallocate(reinterpret_cast<uint8_t *>(_offsets), _nitems_allocd*sizeof(uint32_t));
      _allocator.deallocate(reinterpret_cast<uint8_t *>(_flags), _nitems_allocd*sizeof(BreakFlag));
    }


    template <typename ALLOC>
    inline void
    OffsetInputStream<ALLOC>::_grow(const size_t nitems_grow) {
      uint8_t *bytes = _allocator.allocate((_nitems_allocd + nitems_grow)*sizeof(uint8_t));
      uint32_t *offsets = reinterpret_cast<uint32_t *>(_allocator.allocate((_nitems_allocd + nitems_grow)*sizeof(uint32_t)));
      BreakFlag *flags = reinterpret_cast<BreakFlag *>(_allocator.allocate((_nitems_allocd + nitems_grow)*sizeof(BreakFlag)));
      std::memcpy(bytes, _bytes, _nitems_used*sizeof(uint8_t));
      std::memcpy(offsets, _offsets, _nitems_used*sizeof(uint32_t));
      std::memcpy(flags, _flags, _nitems_used*sizeof(BreakFlag));
      _allocator.deallocate(_bytes, _nitems_allocd*sizeof(uint8_t));
      _allocator.deallocate(reinterpret_cast<uint8_t *>(_offsets), _nitems_allocd*sizeof(uint32_t));
      _allocator.deallocate(reinterpret_cast<uint8_t *>(_flags), _nitems_allocd*sizeof(BreakFlag));
      _bytes = bytes;
      _offsets = offsets;
      _flags = flags;
      _nitems_allocd += nitems_grow;
    }


    template <typename ALLOC> template <typename A>
    inline void
    OffsetInputStream<ALLOC>::write(const OffsetBuffer<A> &buffer) {
      // Copy across the bytes and their corresponding offsets.
      const size_t nitems_free = _nitems_allocd - _nitems_used;
      if (nitems_free < buffer.nitems_used())
        _grow(std::max(_nitems_grow, buffer.nitems_used() - nitems_free));
      std::memcpy(_bytes + _nitems_used, buffer.bytes(), buffer.nitems_used()*sizeof(uint8_t));
      std::memcpy(_offsets + _nitems_used, buffer.offsets(), buffer.nitems_used()*sizeof(uint32_t));
      std::memset(_flags + _nitems_used, BreakFlag::NONE, buffer.nitems_used()*sizeof(BreakFlag));

      // Either update our initial offset or the first copied offset.
      if (_nitems_used == 0)
        _initial_offset = buffer.initial_offset();
      else
        _offsets[_nitems_used - 1] += buffer.initial_offset();

      // Update the number of items we contain.
      _nitems_used += buffer.nitems_used();
    }


    template <typename ALLOC>
    inline void
    OffsetInputStream<ALLOC>::write(const BaseOffsetBuffer::iterator &begin, const BaseOffsetBuffer::iterator &end) {
      // Copy across the bytes and their corresponding offsets.
      const size_t nitems_incoming = end - begin;
      const size_t nitems_free = _nitems_allocd - _nitems_used;
      if (nitems_free < nitems_incoming)
        _grow(std::max(_nitems_grow, nitems_incoming - nitems_free));
      std::memcpy(_bytes + _nitems_used, begin.get_bytes(), nitems_incoming*sizeof(uint8_t));
      std::memcpy(_offsets + _nitems_used, begin.get_offsets(), nitems_incoming*sizeof(uint32_t));
      std::memset(_flags + _nitems_used, to_underlying(BreakFlag::NONE), nitems_incoming*sizeof(BreakFlag));

      // Either update our initial offset or the first copied offset.
      if (_nitems_used == 0)
        _initial_offset = begin.get_summed_offset();
      else
        _offsets[_nitems_used - 1] += begin.get_summed_offset();

      // Update the number of items we contain.
      _nitems_used += nitems_incoming;
    }


    template <typename ALLOC>
    inline void
    OffsetInputStream<ALLOC>::write(const uint8_t byte, const uint32_t offset, const BreakFlag flag) {
      // Insert the byte, offset, and flag.
      if (SCHWA_UNLIKELY(_nitems_used == _nitems_allocd))
        _grow();
      _bytes[_nitems_used] = byte;
      _offsets[_nitems_used] = offset;
      _flags[_nitems_used] = flag;

      // Update the number of items we contain.
      ++_nitems_used;
    }

  }  // namespace new_tokenizer
}  // namespace schwa

#endif  // SCHWA_NEW_TOKENIZER_TOKENIZER_H_
