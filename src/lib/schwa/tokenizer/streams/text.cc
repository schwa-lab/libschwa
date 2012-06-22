/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tokenizer.h>
#include <schwa/tokenizer/streams/text.h>


namespace schwa { namespace tokenizer {

void
TextStream::add(Type, const char *raw, offset_type, offset_type len, const char *norm) {
  if (!_new_sentence)
    _out << ' ';
  _new_sentence = false;

  if (norm && _normalise)
    _out << norm;
  else
    _out.write(raw, len);
}

void
TextStream::error(const char *, offset_type, offset_type) { }

void
TextStream::begin_sentence(void) {
  _new_sentence = true;
}

void
TextStream::end_sentence(void) {
  _out << '\n';
}

void
TextStream::begin_paragraph(void) {
  if (!_new_document)
    _out << '\n';
  _new_document = false;
}

void
TextStream::end_paragraph(void) { }

void
TextStream::begin_heading(int) {
  begin_paragraph();
}

void
TextStream::end_heading(int) {
  end_paragraph();
}

void
TextStream::begin_list(void) {
  begin_paragraph();
}

void
TextStream::end_list(void) { }

void
TextStream::begin_item(void) {
  if (_normalise)
    _out << "* ";
}

void
TextStream::end_item(void) { }

void
TextStream::begin_document(void) {
  _new_document = true;
}

void
TextStream::end_document(void) { }

} }
