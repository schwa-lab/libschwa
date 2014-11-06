/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/new-tokenizer/tokenizer.h>

#include <cassert>
#include <sstream>
#include <string>

#include <schwa/exception.h>
#include <schwa/utils/enums.h>

namespace cs = ::schwa::canonical_schema;


namespace schwa {
namespace new_tokenizer {

// ============================================================================
// Tokenizer::State
// ============================================================================
Tokenizer::State::State(void) : RagelState<OffsetInputStream<>::iterator>() { }

void
Tokenizer::State::reset(void) {
  suffix = 0;
  n1 = n2 = nullptr;
}


void
Tokenizer::State::reset(iterator start, iterator end) {
  RagelState<OffsetInputStream<>::iterator>::reset(start, end);
  reset();
}


std::ostream &
Tokenizer::State::dump(std::ostream &out) const {
  out << "{p=" << p.get_index() << " pe=" << pe.get_index() << " t='";
  for (iterator it = ts; it != te; ++it)
    out << *it;
  out << "' suffix=" << suffix << "}";
  return out;
}


inline std::ostream &
operator <<(std::ostream &out, const Tokenizer::State &state) {
  return state.dump(out);
}


// ============================================================================
// Tokenizer
// ============================================================================
Tokenizer::Tokenizer(void) : _ois(nullptr), _doc(nullptr) { }

Tokenizer::~Tokenizer(void) { }


void
Tokenizer::_create_sentence(void) {
  // Don't create an empty sentence.
  if (_ntokens_before == _doc->tokens.size())
    return;

  // Create the Sentence object and add it to the document.
  cs::Sentence sentence;
  sentence.span.start = reinterpret_cast<cs::Token *>(_ntokens_before);
  sentence.span.stop = reinterpret_cast<cs::Token *>(_doc->tokens.size());
  _doc->sentences.push_back(sentence);

  // Set the index of the start of the next sentence.
  _ntokens_before = _doc->tokens.size();
}


void
Tokenizer::_create_token(OffsetInputStream<>::iterator ts, OffsetInputStream<>::iterator te, const uint8_t *const norm) {
  // Create the Token object and add it to the document.
  cs::Token token;
  token.span.start = ts.get_index();
  token.span.stop = te.get_index();
  token.raw = std::string(reinterpret_cast<const char *>(ts.get_bytes()), reinterpret_cast<const char *>(te.get_bytes()));
  if (norm != nullptr)
    token.norm = std::string(reinterpret_cast<const char *>(norm));
  _doc->tokens.push_back(token);
}


inline void
Tokenizer::_flush_sentence(void) {
  if (_seen_terminator) {
    _create_sentence();
    _seen_terminator = false;
  }
}


void
Tokenizer::_contraction(void) {
  assert(_state.suffix != 0);
  assert(_state.n2 != nullptr);
  _flush_sentence();
  _create_token(_state.ts, _state.te - _state.suffix, _state.n1);
  _create_token(_state.te - _state.suffix, _state.te, _state.n2);
  _state.reset();
}


void
Tokenizer::_close_double_quote(void) {
  _create_token(_state.ts, _state.te, reinterpret_cast<const uint8_t *>(u8"”"));
  _state.reset();
}


void
Tokenizer::_close_single_quote(void) {
  _create_token(_state.ts, _state.te, reinterpret_cast<const uint8_t *>(u8"’"));
  _state.reset();
}


void
Tokenizer::_double_quote(void) {
  _flush_sentence();
  _create_token(_state.ts, _state.te, reinterpret_cast<const uint8_t *>(u8"\""));
  _state.reset();
}


void
Tokenizer::_open_double_quote(void) {
  _flush_sentence();
  _create_token(_state.ts, _state.te, reinterpret_cast<const uint8_t *>(u8"“"));
  _state.reset();
}


void
Tokenizer::_open_single_quote(void) {
  _flush_sentence();
  _create_token(_state.ts, _state.te, reinterpret_cast<const uint8_t *>(u8"‘"));
  _state.reset();
}

void
Tokenizer::_punctuation(const uint8_t *norm) {
  _create_token(_state.ts, _state.te, norm != nullptr ? norm : _state.n1);
  _state.reset();
}


void
Tokenizer::_single_quote(void) {
  _flush_sentence();
  _create_token(_state.ts, _state.te, reinterpret_cast<const uint8_t *>(u8"'"));
  _state.reset();
}


void
Tokenizer::_split(void) {
  assert(_state.suffix != 0);
  _flush_sentence();
  _create_token(_state.ts, _state.te - _state.suffix, _state.n1);
  _create_token(_state.te - _state.suffix, _state.te, _state.n2);
  _state.reset();
}


void
Tokenizer::_terminator(const uint8_t *norm) {
  _create_token(_state.ts, _state.te, norm != nullptr ? norm : _state.n1);
  _state.reset();
  _seen_terminator = true;
}


void
Tokenizer::_word(void) {
  _flush_sentence();
  _create_token(_state.ts, _state.te, _state.n1);
  _state.reset();
}


void
Tokenizer::tokenize(OffsetInputStream<> &ois, cs::Doc &doc) {
  // Reset our local state.
  _state.reset(ois.begin(), ois.end());
  _ois = &ois;
  _doc = &doc;
  _ntokens_before = doc.tokens.size();
  _seen_terminator = false;

  // Run the Ragel-generated tokenizer.
  const bool success = _tokenize();
  if (!success)
    throw RagelException("Failed to tokenize using Tokenizer");

  // Ensure the last sentence is closed.
  _create_sentence();
}

}  // namespace new-tokenizer
}  // namespace schwa
