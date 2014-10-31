/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_ENCODING_IMPL_H_
#define SCHWA_ENCODING_IMPL_H_


namespace schwa {

  template <typename ALLOC>
  void
  EncodingResult::copy_to(OffsetBuffer<ALLOC> &buffer) {
    // FIXME this can be more optimised later.
    const uint8_t *utf8 = _utf8;
    const uint8_t *deltas = _deltas;
    for (size_t i = 0; i != _consumed; ++i)
      buffer.write(*utf8++, *deltas++);
  }

}  // namespace schwa

#endif  // SCHWA_ENCODING_IMPL_H_
