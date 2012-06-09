/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tokenizer.h>
#include <schwa/tokenizer/streams/debug_text.h>


namespace schwa { namespace tokenizer {

const int DebugTextStream::NCOLOURS = 7;
const char *const DebugTextStream::COLOURS[] = {
  "\033[0;31m", "\033[0;32m", "\033[1;33m", "\033[1;34m",
  "\033[0;35m", "\033[0;36m", "\033[1;37m"
};
const char *const DebugTextStream::OFF = "\033[0m";


void
DebugTextStream::add(Type, const char *raw, offset_type, offset_type len, const char *norm) {
  if (!_new_sentence)
    _out << ' ';
  _new_sentence = false;

  if (norm)
    _out << COLOURS[_colour++ % NCOLOURS] << norm << OFF;
  else
    (_out << COLOURS[_colour++ % NCOLOURS]).write(raw, len) << OFF;
}

void
DebugTextStream::error(const char *raw, offset_type, offset_type len) {
  if (!_new_sentence)
    _out << ' ';
  (_out << "<error>").write(raw, len) << "</error>";
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

} }
