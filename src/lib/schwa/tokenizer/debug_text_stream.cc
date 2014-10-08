/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tokenizer/debug_text_stream.h>

#include <iostream>


namespace schwa {
namespace tokenizer {

const std::array<const char *, 7> DebugTextStream::COLOURS = {{
  "\033[0;31m", "\033[0;32m", "\033[1;33m",
  "\033[1;34m", "\033[0;35m", "\033[0;36m",
  "\033[1;37m",
}};
const char *DebugTextStream::OFF = "\033[0m";


DebugTextStream::DebugTextStream(std::ostream &out) :
    _out(out),
    _new_sentence(true),
    _colour(0)
  { }

void
DebugTextStream::add(Type, const uint8_t *raw, size_t, size_t len, const uint8_t *norm) {
  if (!_new_sentence)
    _out << ' ';
  _new_sentence = false;

  if (norm)
    _out << COLOURS[_colour] << norm << OFF;
  else
    (_out << COLOURS[_colour]).write(reinterpret_cast<const char *>(raw), len) << OFF;
  _colour = (_colour + 1) % COLOURS.size();
}

void
DebugTextStream::error(const uint8_t *raw, size_t, size_t len) {
  if (!_new_sentence)
    _out << ' ';
  (_out << "<error>").write(reinterpret_cast<const char *>(raw), len) << "</error>";
}

void
DebugTextStream::begin_sentence(void) {
  _out << "<s>";
  _new_sentence = true;
  _colour = 0;
}

void
DebugTextStream::end_sentence(void) {
  _out << "</s>\n";
}

void
DebugTextStream::begin_list(void) {
  _out << "<ul>\n";
}

void
DebugTextStream::end_list(void) {
  _out << "</ul>\n";
}

void
DebugTextStream::begin_item(void) {
  _out << "<li>\n";
}

void
DebugTextStream::end_item(void) {
  _out << "</li>\n";
}

void
DebugTextStream::begin_paragraph(void) {
  _out << "<p>\n";
}

void
DebugTextStream::end_paragraph(void) {
  _out << "</p>\n";
}

void
DebugTextStream::begin_heading(int depth) {
  _out << "<h" << depth << ">\n";
}
void
DebugTextStream::end_heading(int depth) {
  _out << "</h" << depth << ">\n";
}

void
DebugTextStream::begin_document(void) {
  _out << "<doc>\n";
}

void
DebugTextStream::end_document(void) {
  _out << "</doc>\n";
}

}  // namespace tokenizer
}  // namespace schwa
