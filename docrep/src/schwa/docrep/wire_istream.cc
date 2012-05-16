#include <schwa/docrep.h>


namespace schwa { namespace docrep {

void
WireIStream::_next(void) {
  _has_next = _unpacker.next(&_result);
  std::cerr << "WireIStream::_next _has_next=" << _has_next << std::endl;
  if (!_has_next) {
    const std::streamsize read = _in.readsome(_unpacker.buffer(), _unpacker.buffer_capacity());
    std::cerr << "WireIStream::_next read=" << read << std::endl;
    if (read == 0)
      _eof = true;
    else {
      _unpacker.buffer_consumed(read);
      _has_next = _unpacker.next(&_result);
    }
  }
  std::cerr << "WireIStream::_next _has_next=" << _has_next << " _result=" << _result.get() << std::endl;
}


} }
