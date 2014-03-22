/* -*- Mode: C++; indent-tabs-mode: nil -*- */
/*
Below is the grammar definition for the query language. This ragel machine
defined in src/ragel/dr-grep/language.rl performs the lexing and populates
the `_tokens' deque. A recursive descent parse over the tokens is performed
here, buildling up an expresssion tree as it goes along.

<e1> ::= <e2> (<op_boolean> <e1>)?
<e2> ::= <e2> (<op_comparison> <e3>)?
<e3> ::= <e3> (<op_numeric3> <e3>)?
<e4> ::= <e4> (<op_numeric4> <e4>)?
<e5> ::= "(" <e1> ")"
       | "index"
       | attribute_access
       | function "(" attribute_access "," <e1> ")"
       | literal_int
       | literal_regex
       | literal_str

*/
#include <dr-grep/language.h>

#include <cassert>
#include <cstring>
#include <cstdio>
#include <regex>
#include <sstream>

#include <schwa/dr.h>
#include <schwa/utils/enums.h>

namespace dr = schwa::dr;


namespace schwa {
namespace dr_grep {

// ============================================================================
// Value object
// ============================================================================
using type_t = unsigned int;
static const type_t TYPE_INT   = 1 << 0;
static const type_t TYPE_STR   = 1 << 1;
static const type_t TYPE_REGEX = 1 << 2;
static const type_t TYPE_STORE = 1 << 3;

struct Value {
  type_t type;
  int64_t _int;
  std::string _str;
};


// ============================================================================
// Expression nodes
// ============================================================================
class Expr {
protected:
  const TokenType _token_type;
  const char *const _token;

  Expr(TokenType token_type, const char *token) : _token_type(token_type), _token(token) { }

public:
  virtual ~Expr(void) { }

  virtual bool eval(uint64_t index, const dr::FauxDoc &doc) const = 0;
};


class AttributeAccessExpr : public Expr {
protected:
  const bool _is_ann;
  const char *const _attribute;

public:
  AttributeAccessExpr(TokenType token_type, const char *token) :
      Expr(token_type, token),
      _is_ann(std::memcmp(_token, "ann", 3) == 0),
      _attribute(_token + 4) { }
  virtual ~AttributeAccessExpr(void) { }

  virtual bool
  eval(const uint64_t index, const dr::FauxDoc &doc) const override {
    (void)index;  // TODO
    (void)doc;
    return false;
  }
};


class BinaryOperatorExpr : public Expr {
protected:
  Expr *const _left;
  Expr *const _right;

public:
  BinaryOperatorExpr(TokenType token_type, const char *token, Expr *left, Expr *right) :
      Expr(token_type, token),
      _left(left),
      _right(right) { }
  virtual ~BinaryOperatorExpr(void) { }

  virtual bool
  eval(const uint64_t index, const dr::FauxDoc &doc) const override {
    (void)index;  // TODO
    (void)doc;
    return false;
  }
};


class FunctionExpr : public Expr {
protected:
  Expr *const _attribute_access;
  Expr *const _expr;

public:
  FunctionExpr(TokenType token_type, const char *token, Expr *attribute_access, Expr *expr) :
      Expr(token_type, token),
      _attribute_access(attribute_access),
      _expr(expr) { }
  virtual ~FunctionExpr(void) { }

  virtual bool
  eval(const uint64_t index, const dr::FauxDoc &doc) const override {
    (void)index;  // TODO
    (void)doc;
    return false;
  }
};


class LiteralIntegerExpr : public Expr {
protected:
  int64_t _value;

public:
  LiteralIntegerExpr(TokenType token_type, const char *token) : Expr(token_type, token) {
    const int ret = std::sscanf(_token, "%lld", &_value);
    assert(ret == 1);
  }
  virtual ~LiteralIntegerExpr(void) { }

  virtual bool
  eval(const uint64_t index, const dr::FauxDoc &doc) const override {
    (void)index;  // TODO
    (void)doc;
    return false;
  }
};


class LiteralRegexExpr : public Expr {
protected:
  std::regex _re;

public:
  LiteralRegexExpr(TokenType token_type, const char *token) : Expr(token_type, token) {
    try {
      _re = std::regex(_token + 1, std::strlen(_token) - 2, std::regex::nosubs | std::regex::ECMAScript);
    }
    catch (std::regex_error &e) {
      throw CompileError(e.what());
    }
  }
  virtual ~LiteralRegexExpr(void) { }

  virtual bool
  eval(const uint64_t index, const dr::FauxDoc &doc) const override {
    (void)index;  // TODO
    (void)doc;
    return false;
  }
};


class LiteralStringExpr : public Expr {
public:
  LiteralStringExpr(TokenType token_type, const char *token) : Expr(token_type, token) { }
  virtual ~LiteralStringExpr(void) { }

  virtual bool
  eval(const uint64_t index, const dr::FauxDoc &doc) const override {
    (void)index;  // TODO
    (void)doc;
    return false;
  }
};




// ============================================================================
// Interpreter
// ============================================================================
Interpreter::Interpreter(void) : _pool(4 * 1024), _expr(nullptr) { }

Interpreter::~Interpreter(void) {
  delete _expr;
}


Expr *
Interpreter::_parse_e1(void) {
  Expr *left = _parse_e2();
  if (!_tokens.empty() && _tokens.front().first == TokenType::OP_BOOLEAN) {
    decltype(_tokens)::value_type pair = _tokens.front();
    _tokens.pop_front();
    Expr *right = _parse_e1();
    left = new (_pool) BinaryOperatorExpr(pair.first, pair.second, left, right);
  }
  return left;
}


Expr *
Interpreter::_parse_e2(void) {
  Expr *left = _parse_e3();
  if (!_tokens.empty() && _tokens.front().first == TokenType::OP_COMPARISON) {
    decltype(_tokens)::value_type pair = _tokens.front();
    _tokens.pop_front();
    Expr *right = _parse_e3();
    left = new (_pool) BinaryOperatorExpr(pair.first, pair.second, left, right);
  }
  return left;
}


Expr *
Interpreter::_parse_e3(void) {
  Expr *left = _parse_e4();
  if (!_tokens.empty() && _tokens.front().first == TokenType::OP_NUMERIC3) {
    decltype(_tokens)::value_type pair = _tokens.front();
    _tokens.pop_front();
    Expr *right = _parse_e3();
    left = new (_pool) BinaryOperatorExpr(pair.first, pair.second, left, right);
  }
  return left;
}


Expr *
Interpreter::_parse_e4(void) {
  Expr *left = _parse_e5();
  if (!_tokens.empty() && _tokens.front().first == TokenType::OP_NUMERIC4) {
    decltype(_tokens)::value_type pair = _tokens.front();
    _tokens.pop_front();
    Expr *right = _parse_e4();
    left = new (_pool) BinaryOperatorExpr(pair.first, pair.second, left, right);
  }
  return left;
}


Expr *
Interpreter::_parse_e5(void) {
  std::ostringstream msg;

  if (_tokens.empty())
    throw CompileError("Expected <e5> but no more tokens available");

  decltype(_tokens)::value_type pair = _tokens.front();
  _tokens.pop_front();

  Expr *expr = nullptr;
  switch (pair.first) {
  case TokenType::OPEN_PAREN:
    expr = _parse_e1();
    if (_tokens.empty())
      throw CompileError("Expected CLOSE_PAREN in <e5> but no more tokens available");
    pair = _tokens.front();
    _tokens.pop_front();
    if (pair.first != TokenType::CLOSE_PAREN) {
      msg << "Expected CLOSE_PAREN in <e5> but found token type " << to_underlying(pair.first) << " instead";
      throw CompileError(msg.str());
    }
    break;

  case TokenType::ATTRIBUTE_ACCESS:
    expr = new (_pool) AttributeAccessExpr(pair.first, pair.second);
    break;

  case TokenType::FUNCTION:
    {
      // "("
      if (_tokens.empty())
        throw CompileError("Expected OPEN_PAREN in <e5> function but no more tokens available");
      pair = _tokens.front();
      _tokens.pop_front();
      if (pair.first != TokenType::OPEN_PAREN) {
        msg << "Expected OPEN_PAREN in <e5> function but found token type " << to_underlying(pair.first) << " instead";
        throw CompileError(msg.str());
      }
      // attribute_access
      if (_tokens.empty())
        throw CompileError("Expected attribute_access in <e5> function but no more tokens available");
      pair = _tokens.front();
      _tokens.pop_front();
      if (pair.first != TokenType::ATTRIBUTE_ACCESS) {
        msg << "Expected attribute_access in <e5> function but found token type " << to_underlying(pair.first) << " instead";
        throw CompileError(msg.str());
      }
      Expr *const attribute_access = new (_pool) AttributeAccessExpr(pair.first, pair.second);
      // ","
      if (_tokens.empty())
        throw CompileError("Expected COMMA in <e5> function but no more tokens available");
      pair = _tokens.front();
      _tokens.pop_front();
      if (pair.first != TokenType::COMMA) {
        msg << "Expected COMMA in <e5> function but found token type " << to_underlying(pair.first) << " instead";
        throw CompileError(msg.str());
      }
      // attribute_access
      Expr *const e1 = _parse_e1();
      // ")"
      if (_tokens.empty())
        throw CompileError("Expected CLOSE_PAREN in <e5> function but no more tokens available");
      pair = _tokens.front();
      _tokens.pop_front();
      if (pair.first != TokenType::CLOSE_PAREN) {
        msg << "Expected CLOSE_PAREN in <e5> function but found token type " << to_underlying(pair.first) << " instead";
        throw CompileError(msg.str());
      }
      expr = new (_pool) FunctionExpr(pair.first, pair.second, attribute_access, e1);
    }
    break;

  case TokenType::LITERAL_INTEGER:
    expr = new (_pool) LiteralIntegerExpr(pair.first, pair.second);
    break;

  case TokenType::LITERAL_REGEX:
    expr = new (_pool) LiteralRegexExpr(pair.first, pair.second);
    break;

  case TokenType::LITERAL_STRING:
    expr = new (_pool) LiteralStringExpr(pair.first, pair.second);
    break;

  default:
    msg << "Expected <e5> token but found token type " << to_underlying(pair.first) << " instead";
    throw CompileError(msg.str());
  }
  return expr;
}


void
Interpreter::_parse(void) {
  // Parse and consume the tokens.
  if (!_tokens.empty())
    _expr = _parse_e1();

  // Ensure there are no more tokens left to consume.
  if (!_tokens.empty()) {
    const auto &pair = _tokens.front();
    _throw_compile_error(pair.second, std::strlen(pair.second));
  }
}


void
Interpreter::_push_token(const TokenType type, const char *const ts, const char *const te) {
  const size_t len = te - ts;
  char *const str = _pool.alloc<char *>(len + 1);
  std::memcpy(str, ts, len);
  str[len] = '\0';
  _tokens.push_back(decltype(_tokens)::value_type(type, str));
}


void
Interpreter::_throw_compile_error(const char *const str, const size_t len) {
  std::ostringstream ss;
  ss << "Invalid input found at '";
  ss.write(str, len);
  ss << "'";
  throw CompileError(ss.str());
}


void
Interpreter::_throw_compile_error(const char *const ts, const char *const te) {
  _throw_compile_error(ts, te - ts);
}


void
Interpreter::compile(const char *const str, const size_t len) {
  _tokenise(str, len);
  _parse();
}


bool
Interpreter::eval(const uint64_t index, const dr::FauxDoc &doc) const {
  if (_expr == nullptr)
    return true;
  else
    return _expr->eval(index, doc);
}

}  // namesapce dr_grep
}  // namespace schwa
