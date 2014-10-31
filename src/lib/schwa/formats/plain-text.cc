/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/formats/plain-text.h>


namespace schwa {
namespace formats {

PlainTextLexer::PlainTextLexer(void) { }


void
PlainTextLexer::_create_paragraph(void) {
  std::cout << "PlainTextLexer::_create_paragraph ts=" << _state.ts.get_index() << " te=" << _state.te.get_index() << std::endl;
  _create_paragraph(_par_start_index, _state.ts.get_index());
  _par_start_index = _state.te.get_index();
}


void
PlainTextLexer::_create_paragraph(const size_t start_index, const size_t end_index) {
  _paragraphs.emplace_back(std::make_pair(start_index, end_index));
}


void
PlainTextLexer::lex(const BaseOffsetBuffer &buffer) {
  // Reset our internal state.
  _state.reset(buffer.begin(), buffer.end());
  _par_start_index = 0;
  _paragraphs.clear();

  // Run the Ragel lexer.
  const bool success = _lex();
  if (!success)
    throw RagelException("Failed to lex using PlainTextLexer");

  // Add the final paragraph.
  if (_par_start_index != buffer.nitems_used())
    _create_paragraph(_par_start_index, buffer.nitems_used());

  std::cout << "[PlainTextLexer::lex] paragraphs=";
  for (const auto &pair : _paragraphs)
    std::cout << " (" << pair.first << "," << pair.second << ")";
  std::cout << std::endl;
}

}  // namesapce formats
}  // namespace schwa
