/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tokenizer/tokenizer.h>

#include <cassert>
#include <sstream>
#include <string>

#include <schwa/exception.h>
#include <schwa/utils/enums.h>

//#define TOKENIZER_DEBUG 1

namespace cs = ::schwa::canonical_schema;


namespace schwa {
namespace tokenizer {

// ============================================================================
// Punctuation normalisation constants.
// ============================================================================
const uint8_t *const NORMALISED_CLOSE_DOUBLE_QUOTE = reinterpret_cast<const uint8_t *>(u8"”");
const uint8_t *const NORMALISED_CLOSE_SINGLE_QUOTE = reinterpret_cast<const uint8_t *>(u8"’");
const uint8_t *const NORMALISED_DASH = reinterpret_cast<const uint8_t *>(u8"--");
const uint8_t *const NORMALISED_ELLIPSIS = reinterpret_cast<const uint8_t *>(u8"...");
const uint8_t *const NORMALISED_EXCLAMATION_MARK = reinterpret_cast<const uint8_t *>(u8"!");
const uint8_t *const NORMALISED_INVERTED_EXCLAMATION_MARK = reinterpret_cast<const uint8_t *>(u8"¡");
const uint8_t *const NORMALISED_INVERTED_QUESTION_MARK = reinterpret_cast<const uint8_t *>(u8"¿");
const uint8_t *const NORMALISED_OPEN_DOUBLE_QUOTE = reinterpret_cast<const uint8_t *>(u8"“");
const uint8_t *const NORMALISED_OPEN_SINGLE_QUOTE = reinterpret_cast<const uint8_t *>(u8"‘");
const uint8_t *const NORMALISED_PERIOD = reinterpret_cast<const uint8_t *>(u8".");
const uint8_t *const NORMALISED_QUESTION_MARK = reinterpret_cast<const uint8_t *>(u8"?");
const uint8_t *const NORMALISED_SINGLE_QUOTE = reinterpret_cast<const uint8_t *>(u8"'");


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


// ============================================================================
// Tokenizer
// ============================================================================
Tokenizer::Tokenizer(void) : _ois(nullptr), _doc(nullptr) { }

Tokenizer::~Tokenizer(void) { }


void
Tokenizer::_abbreviation(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_abbreviation] "; _state.dump(std::cerr) << std::endl;
#endif
  _flush_sentence();
  _create_token(_state.ts, _state.te, _state.n1, false);
  _state.reset();
  _prev_was_abbrev = true;
}


void
Tokenizer::_create_sentence(void) {
  // Don't create an empty sentence.
  if (_ntokens_before == _doc->tokens.size())
    return;

  // Create the Sentence object and add it to the document.
  cs::Sentence sentence;
  sentence.span.start = reinterpret_cast<cs::Token *>(_ntokens_before + 1);
  sentence.span.stop = reinterpret_cast<cs::Token *>(_doc->tokens.size() + 1);
  _doc->sentences.push_back(sentence);

  // Set the index of the start of the next sentence.
  _ntokens_before = _doc->tokens.size();
}


void
Tokenizer::_create_token(OffsetInputStream<>::iterator ts, OffsetInputStream<>::iterator te, const uint8_t *const norm, const bool maybe_break_on_caps) {
  // If the first code point is upper case and the previous token was an abbreviation, force a new sentence.
  if (maybe_break_on_caps && _prev_was_abbrev && !_in_brackets) {
    const uint8_t *start = ts.get_bytes();
    const unicode_t first = read_utf8(&start, te.get_bytes());
    if (unicode::is_upper(first))
      _create_sentence();
  }

  // Create the Token object and add it to the document.
  cs::Token token;
  token.span.start = ts.get_summed_offset();
  token.span.stop = te.get_summed_offset();
  token.raw = std::string(reinterpret_cast<const char *>(ts.get_bytes()), reinterpret_cast<const char *>(te.get_bytes()));
  if (norm != nullptr)
    token.norm = std::string(reinterpret_cast<const char *>(norm));
  _doc->tokens.push_back(token);

  // Reset state.
  _prev_was_abbrev = false;
  _prev_was_close_punctuation = false;
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
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_contraction] "; _state.dump(std::cerr) << std::endl;
#endif
  // Maybe flush sentence.
  if (_seen_terminator && _prev_was_close_punctuation) {
    const uint8_t *start = _state.ts.get_bytes();
    const unicode_t first = read_utf8(&start, _state.te.get_bytes());
    if (unicode::is_lower(first))
      _seen_terminator = false;
    else
      _flush_sentence();
  }
  else
    _flush_sentence();

  assert(_state.suffix != 0);
  assert(_state.n2 != nullptr);
  _create_token(_state.ts, _state.te - _state.suffix, _state.n1);
  _create_token(_state.te - _state.suffix, _state.te, _state.n2);
  _state.reset();
}


void
Tokenizer::_close_bracket(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_close_bracket] "; _state.dump(std::cerr) << std::endl;
#endif
  _create_token(_state.ts, _state.te, _state.n1);
  _state.reset();
  _in_brackets = false;
  _prev_was_close_punctuation = true;
}


void
Tokenizer::_close_double_quote(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_close_double_quote] "; _state.dump(std::cerr) << std::endl;
#endif
  _create_token(_state.ts, _state.te, NORMALISED_CLOSE_DOUBLE_QUOTE);
  _state.reset();
  _in_double_quotes = false;
  _prev_was_close_punctuation = true;
}


void
Tokenizer::_close_single_quote(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_close_single_quote] "; _state.dump(std::cerr) << std::endl;
#endif
  _create_token(_state.ts, _state.te, NORMALISED_CLOSE_SINGLE_QUOTE);
  _state.reset();
  _in_single_quotes = false;
  _prev_was_close_punctuation = true;
}


void
Tokenizer::_double_quote(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_double_quote] "; _state.dump(std::cerr) << std::endl;
#endif
  if (_seen_terminator && !_in_double_quotes)
    _flush_sentence();
  if (_in_double_quotes)
    _close_double_quote();
  else
    _open_double_quote();
}


void
Tokenizer::_ignore(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_ignore] "; _state.dump(std::cerr) << std::endl;
#endif
  // TODO actually take into account BreakFlags.
}


void
Tokenizer::_month_day(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_month_day] "; _state.dump(std::cerr) << std::endl;
#endif
  _flush_sentence();
  _create_token(_state.ts, _state.b1, _state.n1);
  _create_token(_state.b2, _state.te - 1, _state.n2);
  _create_token(_state.te - 1, _state.te, _state.n2);
  _state.reset();
  _seen_terminator = true;
}


void
Tokenizer::_open_bracket(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_open_bracket] "; _state.dump(std::cerr) << std::endl;
#endif
  _flush_sentence();
  _create_token(_state.ts, _state.te, _state.n1);
  _state.reset();
  _in_brackets = true;
}


void
Tokenizer::_open_double_quote(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_open_double_quote] "; _state.dump(std::cerr) << std::endl;
#endif
  _flush_sentence();
  _create_token(_state.ts, _state.te, NORMALISED_OPEN_DOUBLE_QUOTE);
  _state.reset();
  _in_double_quotes = true;
}


void
Tokenizer::_open_single_quote(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_open_single_quote] "; _state.dump(std::cerr) << std::endl;
#endif
  _flush_sentence();
  _create_token(_state.ts, _state.te, NORMALISED_OPEN_SINGLE_QUOTE);
  _state.reset();
  _in_single_quotes = true;
}

void
Tokenizer::_punctuation(const uint8_t *const norm) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_punctuation] "; _state.dump(std::cerr) << std::endl;
#endif
  // Maybe flush sentence.
  if (_seen_terminator)
    _seen_terminator = false;

  _create_token(_state.ts, _state.te, norm != nullptr ? norm : _state.n1);
  _state.reset();
}


void
Tokenizer::_single_quote(void) {
  if (_in_single_quotes) {
    _create_token(_state.ts, _state.te, NORMALISED_CLOSE_SINGLE_QUOTE);
    _in_single_quotes = false;
    _prev_was_close_punctuation = true;
  }
  else {
    _flush_sentence();
    if (_ntokens_before != _doc->tokens.size() && _state.ts.get_summed_offset() == _doc->tokens.back().span.stop) {
      _create_token(_state.ts, _state.te, NORMALISED_SINGLE_QUOTE);
      _in_single_quotes = false;
    }
    else {
      _create_token(_state.ts, _state.te, NORMALISED_OPEN_SINGLE_QUOTE);
      _in_single_quotes = true;
    }
  }
  _state.reset();
}


void
Tokenizer::_split(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_split] "; _state.dump(std::cerr) << std::endl;
#endif
  assert(_state.suffix != 0);
  _flush_sentence();
  _create_token(_state.ts, _state.te - _state.suffix, _state.n1);
  _create_token(_state.te - _state.suffix, _state.te, _state.n2);
  _state.reset();
}


void
Tokenizer::_terminator(const uint8_t *const norm) {
  _create_token(_state.ts, _state.te, norm);
  _state.reset();
  if (norm == NORMALISED_PERIOD) {
    if (!_in_single_quotes)
      _seen_terminator = true;
  }
  else {
    if (!(_in_brackets || _in_single_quotes))
      _seen_terminator = true;
  }
}


void
Tokenizer::_word(void) {
#ifdef TOKENIZER_DEBUG
  std::cerr << "[_word] "; _state.dump(std::cerr) << std::endl;
#endif
  // Maybe flush sentence.
  if (_seen_terminator && _prev_was_close_punctuation) {
    const uint8_t *start = _state.ts.get_bytes();
    const unicode_t first = read_utf8(&start, _state.te.get_bytes());
    if (unicode::is_lower(first))
      _seen_terminator = false;
    else
      _flush_sentence();
  }
  else
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
  _in_brackets = false;
  _in_double_quotes = false;
  _in_single_quotes = false;
  _prev_was_abbrev = false;
  _prev_was_close_punctuation = false;
  _seen_terminator = false;

  // Run the Ragel-generated tokenizer.
  const bool success = _tokenize();
  if (!success)
    throw RagelException("Failed to tokenize using Tokenizer");

  // Ensure the last sentence is closed.
  _create_sentence();
}

}  // namespace tokenizer
}  // namespace schwa
