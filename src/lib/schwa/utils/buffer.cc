#include <schwa/utils/buffer.h>

#include <cstring>
#include <iostream>
#include <new>


namespace schwa {

void
Buffer::_write(const uint8_t *const data, const size_t nbytes) {
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


void
Buffer::consume(std::istream &in) {
  uint8_t buf[4096];
  while (true) {
    in.read(reinterpret_cast<char *>(buf), sizeof(buf));
    const auto nbytes = in.gcount();
    if (nbytes < 1)
      break;
    write(buf, nbytes);
  }
}

}  // namespace schwa
