/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/new-tokenizer/tokenizer.h>

#include <sstream>

#include <schwa/exception.h>
#include <schwa/utils/enums.h>


namespace schwa {
namespace new_tokenizer {

// ============================================================================
// Functions
// ============================================================================
std::string
tokentype_name(const TokenType type) {
  switch (type) {
  case TokenType::ABBREVIATION: return "ABBREVIATION";
  case TokenType::CONTRACTION: return "CONTRACTION";
  case TokenType::DASH: return "DASH";
  case TokenType::NUMBER: return "NUMBER";
  case TokenType::POSSESSIVE: return "POSSESSIVE";
  case TokenType::PUNCTUATION: return "PUNCTUATION";
  case TokenType::QUOTE: return "QUOTE";
  case TokenType::TERMINATOR: return "TERMINATOR";
  case TokenType::UNIT: return "UNIT";
  case TokenType::WORD: return "WORD";
  default:
    std::ostringstream ss;
    ss << "Unknown TokenType value " << to_underlying(type);
    throw ValueException(ss.str());
  }
}


// ============================================================================
// Tokenizer::State
// ============================================================================
Tokenizer::State::State(void) : RagelState<OffsetInputStream<>::iterator>() { }

void
Tokenizer::State::reset(iterator start, iterator end) {
  RagelState<OffsetInputStream<>::iterator>::reset(start, end);
  suffix = 0;
  n1 = n2 = nullptr;
  is_contraction = false;
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
Tokenizer::Tokenizer(void) { }

Tokenizer::~Tokenizer(void) { }


void
Tokenizer::_close_double_quote(void) {
  std::cout << "[_close_double_quote] state=" << _state << std::endl;
}


void
Tokenizer::_close_single_quote(void) {
  std::cout << "[_close_single_quote] state=" << _state << std::endl;
}


void
Tokenizer::_double_quote(void) {
  std::cout << "[_double_quote] state=" << _state << std::endl;
}


void
Tokenizer::_end(TokenType type) {
  (void)type;
  std::cout << "[_end] type=" << type << " state=" << _state << std::endl;
}


void
Tokenizer::_open_double_quote(void) {
  std::cout << "[_open_double_quote] state=" << _state << std::endl;
}


void
Tokenizer::_open_single_quote(void) {
  std::cout << "[_open_single_quote] state=" << _state << std::endl;
}

void
Tokenizer::_punct(TokenType type, const uint8_t *norm) {
  (void)type;
  (void)norm;
  std::cout << "[_punct] type=" << type << " state=" << _state << " norm=";
  if (norm == nullptr)
    std::cout << reinterpret_cast<const void *>(norm);
  else
    std::cout << "'" << reinterpret_cast<const char *>(norm) << "'";
  std::cout << std::endl;
}


void
Tokenizer::_single_quote(void) {
  std::cout << "[_single_quote] state=" << _state << std::endl;
}


void
Tokenizer::_split(TokenType type1, TokenType type2) {
  (void)type1;
  (void)type2;
  std::cout << "[_split] type1=" << type1 << " type2=" << type2 << " state=" << _state << std::endl;
  _state.suffix = 0;
}


void
Tokenizer::_terminator(const uint8_t *norm) {
  (void)norm;
  std::cout << "[_terminator] state=" << _state << " norm='" << reinterpret_cast<const char *>(norm) << "'" << std::endl;
}


void
Tokenizer::_word(TokenType type) {
  (void)type;
  std::cout << "[_word] type=" << type << " state=" << _state << std::endl;
}


void
Tokenizer::tokenize(OffsetInputStream<> &ois) {
  // Reset our state.
  _state.reset(ois.begin(), ois.end());

  // Run the Ragel-generated tokenizer.
  const bool success =  _tokenize();
  if (!success)
    throw RagelException("Failed to tokenize using Tokenizer");
}

}  // namespace new-tokenizer
}  // namespace schwa
