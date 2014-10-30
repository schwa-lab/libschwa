/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/formats/plain-text.h>


namespace schwa {
namespace formats {

PlainTextLexer::PlainTextLexer(const EncodingResult &er) :
    _encoding_result(er),
    _state(er.utf8(), er.utf8() + er.nbytes())
  { }


void
PlainTextLexer::_create_paragraph(void) {
  std::cout << "PlainTextLexer::_create_paragraph ts=" << static_cast<const void *>(_state.ts) << " te=" << static_cast<const void *>(_state.te) << std::endl;
  _paragraphs.emplace_back(std::make_pair(_state.ts, _state.te));
}

}  // namesapce formats
}  // namespace schwa
