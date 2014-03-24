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
#include <string>

#include <schwa/dr.h>
#include <schwa/msgpack.h>
#include <schwa/utils/enums.h>

namespace dr = schwa::dr;
namespace io = schwa::io;
namespace mp = schwa::msgpack;


namespace schwa {
namespace dr_grep {

using ValueType = uint32_t;
static constexpr const ValueType TYPE_INTEGER = 1 << 0;
static constexpr const ValueType TYPE_STRING  = 1 << 1;
static constexpr const ValueType TYPE_REGEX   = 1 << 2;
static constexpr const ValueType TYPE_STORE   = 1 << 3;
static constexpr const ValueType TYPE_MISSING = 1 << 4;
static constexpr const ValueType TYPE_ANY = TYPE_INTEGER | TYPE_STRING | TYPE_REGEX | TYPE_STORE | TYPE_MISSING;

static const char *
type_t_name(const ValueType type) {
  switch (type) {
  case TYPE_INTEGER: return "integer";
  case TYPE_STRING: return "string";
  case TYPE_REGEX: return "regex";
  case TYPE_STORE: return "store";
  case TYPE_MISSING: return "missing";
  default: return "";
  }
}


// ============================================================================
// Exception helpers.
// ============================================================================
static int64_t
cast_to_int(const std::string &value) {
  char *end;
  const int64_t v_int = std::strtoll(value.c_str(), &end, 10);
  if (*end != '\0') {
    std::ostringstream ss;
    ss << "Failed to convert string value '" << value << "' to an integer";
    throw RuntimeError(ss.str());
  }
  return v_int;
}


static std::string
cast_to_str(const int64_t value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}


static void
check_accepts(const char *const msg, const ValueType type, const ValueType accepts_mask) {
  if ((type & accepts_mask) == 0) {
    std::ostringstream ss;
    ss << "Invalid input type to " << msg << ": found " << type_t_name(type);
    throw RuntimeError(ss.str());
  }
}


static void
check_same_accepts(const char *const msg, const ValueType t1, const ValueType t2, const ValueType accepts_mask) {
  if ((t1 & accepts_mask) == 0) {
    std::ostringstream ss;
    ss << "Invalid left type to " << msg << ": found " << type_t_name(t1);
    throw RuntimeError(ss.str());
  }
  if ((t2 & accepts_mask) == 0) {
    std::ostringstream ss;
    ss << "Invalid right type to " << msg << ": found " << type_t_name(t2);
    throw RuntimeError(ss.str());
  }
  if (t1 != t2) {
    std::ostringstream ss;
    ss << "Left and right types to " << msg << " do not match. Found " << type_t_name(t1) << " and " << type_t_name(t2);
    throw RuntimeError(ss.str());
  }
}


static void
throw_compile_error(const char *const str, const size_t len) {
  std::ostringstream ss;
  ss << "Invalid input found at '";
  ss.write(str, len);
  ss << "'";
  throw CompileError(ss.str());
}


// ============================================================================
// Value object
// ============================================================================
class AttributeAccessExpr;

class Value {
public:
  const ValueType type;
  const int64_t v_int;
  const std::string v_str;
  const std::regex *const v_re;
  const AttributeAccessExpr *const v_attribute;

protected:
  Value(ValueType type, int64_t v_int, const std::string &v_str, const std::regex *v_re, const AttributeAccessExpr *v_attribute) : type(type), v_int(v_int), v_str(v_str), v_re(v_re), v_attribute(v_attribute) { }

public:
  Value(const Value &o) : type(o.type), v_int(o.v_int), v_str(o.v_str), v_re(o.v_re), v_attribute(o.v_attribute) { }
  Value(const Value &&o) : type(o.type), v_int(o.v_int), v_str(o.v_str), v_re(o.v_re), v_attribute(o.v_attribute) { }

  static inline Value as_int(int64_t value) { return Value(TYPE_INTEGER, value, "", nullptr, nullptr); }
  static inline Value as_str(const std::string &value) { return Value(TYPE_STRING, 0, value, nullptr, nullptr); }
  static inline Value as_re(const std::regex *value) { return Value(TYPE_REGEX, 0, "", value, nullptr); }
  static inline Value as_store(const AttributeAccessExpr *value) { return Value(TYPE_STORE, 0, "", nullptr, value); }
  static inline Value as_missing(const AttributeAccessExpr *value) { return Value(TYPE_MISSING, 0, "", nullptr, value); }
};


// ============================================================================
// Expression nodes
// ============================================================================
class Expr {
protected:
  const char *const _token;

  explicit Expr(const char *token) : _token(token) { }
public:
  virtual ~Expr(void) { }

  virtual Value eval(uint64_t index, const dr::Doc &doc) const = 0;
};


class AttributeAccessExpr : public Expr {
protected:
  const bool _is_ann;
  const std::string _attribute;
  mutable const dr::RTManager *_rt;
  mutable const dr::RTSchema *_rtdschema;
  mutable const dr::RTStoreDef *_rtstore;

public:
  explicit AttributeAccessExpr(const char *token) :
      Expr(token),
      _is_ann(std::memcmp(_token, "ann", 3) == 0),
      _attribute(_token + 4),
      _rt(nullptr),
      _rtdschema(nullptr) { }
  virtual ~AttributeAccessExpr(void) { }

  inline bool is_ann(void) const { return _is_ann; }
  inline bool is_doc(void) const { return !_is_ann; }

  inline uint32_t store_nelem(void) const { return _rtstore->lazy_nelem; }

  virtual Value
  eval(const uint64_t, const dr::Doc &doc) const override {
    _rt = doc.rt();
    if (is_doc())
      return eval_doc();
    return Value::as_int(0);
  }

  Value
  eval_doc(void) const {
    _rtdschema = _rt->doc;

    // Decode the lazy document values into dynamic msgpack objects.
    Pool pool(4096);
    io::ArrayReader reader(_rtdschema->lazy_data, _rtdschema->lazy_nbytes);
    mp::Value *value = mp::read_dynamic(reader, pool);
    assert(value != nullptr);
    assert(mp::is_map(value->type));
    const mp::Map &map = *value->via._map;

    // <instance> ::= { <field_id> : <obj_val> }
    for (uint32_t j = 0; j != map.size(); ++j) {
      assert(mp::is_uint(map.get(j).key.type));
      const mp::Map::Pair &pair = map.get(j);
      const dr::RTFieldDef &field = *_rtdschema->fields[pair.key.via._uint64];
      if (field.serial == _attribute) {
        if (field.is_slice || field.points_into != nullptr || field.is_self_pointer) {
          std::ostringstream ss;
          ss << "Field '" << _attribute << "' is of an invalid type";
          throw RuntimeError(ss.str());
        }

        if (mp::is_bool(pair.value.type))
          return Value::as_int(pair.value.via._bool);
        else if (mp::is_sint(pair.value.type))
          return Value::as_int(pair.value.via._int64);
        else if (mp::is_uint(pair.value.type))
          return Value::as_int(static_cast<int64_t>(pair.value.via._uint64));
        else if (mp::is_raw(pair.value.type)) {
          const mp::Raw &raw = *pair.value.via._raw;
          return Value::as_str(std::string(raw.value(), raw.size()));
        }
        else {
          std::ostringstream ss;
          ss << "Field '" << _attribute << "' is of an invalid type";
          throw RuntimeError(ss.str());
        }
      }
    }

    for (const dr::RTStoreDef *store : _rtdschema->stores) {
      if (store->serial == _attribute) {
        this->_rtstore = store;
        return Value::as_store(this);
      }
    }

    return Value::as_missing(this);
  }
};


class BinaryOperatorExpr : public Expr {
protected:
  Expr *const _left;
  Expr *const _right;

public:
  BinaryOperatorExpr(const char *token, Expr *left, Expr *right) :
      Expr(token),
      _left(left),
      _right(right) { }
  virtual ~BinaryOperatorExpr(void) { }

  virtual Value
  eval(const uint64_t index, const dr::Doc &doc) const override {
    const Value v1 = _left->eval(index, doc);
    const Value v2 = _right->eval(index, doc);
    if (std::strcmp(_token, "&&") == 0) {
      check_same_accepts("BinaryOperatorExpr(&&)", v1.type, v2.type, TYPE_INTEGER);
      return Value::as_int(v1.v_int && v2.v_int);
    }
    else if (std::strcmp(_token, "||") == 0) {
      check_same_accepts("BinaryOperatorExpr(||)", v1.type, v2.type, TYPE_INTEGER);
      return Value::as_int(v1.v_int || v2.v_int);
    }
    else if (std::strcmp(_token, "==") == 0) {
      check_same_accepts("BinaryOperatorExpr(==)", v1.type, v2.type, TYPE_INTEGER | TYPE_STRING);
      if (v1.type == TYPE_INTEGER)
        return Value::as_int(v1.v_int == v2.v_int);
      else
        return Value::as_int(v1.v_str == v2.v_str);
    }
    else if (std::strcmp(_token, "!=") == 0) {
      check_same_accepts("BinaryOperatorExpr(!=)", v1.type, v2.type, TYPE_INTEGER | TYPE_STRING);
      if (v1.type == TYPE_INTEGER)
        return Value::as_int(v1.v_int != v2.v_int);
      else
        return Value::as_int(v1.v_str != v2.v_str);
    }
    else if (std::strcmp(_token, "<=") == 0) {
      check_same_accepts("BinaryOperatorExpr(<=)", v1.type, v2.type, TYPE_INTEGER);
      return Value::as_int(v1.v_int <= v2.v_int);
    }
    else if (std::strcmp(_token, "<") == 0) {
      check_same_accepts("BinaryOperatorExpr(<)", v1.type, v2.type, TYPE_INTEGER);
      return Value::as_int(v1.v_int < v2.v_int);
    }
    else if (std::strcmp(_token, ">=") == 0) {
      check_same_accepts("BinaryOperatorExpr(>=)", v1.type, v2.type, TYPE_INTEGER);
      return Value::as_int(v1.v_int >= v2.v_int);
    }
    else if (std::strcmp(_token, ">") == 0) {
      check_same_accepts("BinaryOperatorExpr(>)", v1.type, v2.type, TYPE_INTEGER);
      return Value::as_int(v1.v_int > v2.v_int);
    }
    else if (std::strcmp(_token, "~=") == 0) {
      check_accepts("BinaryOperatorExpr(~= lhs)", v1.type, TYPE_STRING);
      check_accepts("BinaryOperatorExpr(~= rhs)", v2.type, TYPE_REGEX);
      std::smatch m;
      const bool found = std::regex_search(v1.v_str, m, *v2.v_re);
      return Value::as_int(found && static_cast<size_t>(m.length()) == v1.v_str.size());
    }
    else if (std::strcmp(_token, "~") == 0) {
      check_accepts("BinaryOperatorExpr(~ lhs)", v1.type, TYPE_STRING);
      check_accepts("BinaryOperatorExpr(~ rhs)", v2.type, TYPE_REGEX);
      return Value::as_int(std::regex_search(v1.v_str, *v2.v_re));
    }
    else if (std::strcmp(_token, "+") == 0) {
      check_same_accepts("BinaryOperatorExpr(+)", v1.type, v2.type, TYPE_INTEGER | TYPE_STRING);
      if (v1.type == TYPE_INTEGER)
        return Value::as_int(v1.v_int + v2.v_int);
      else
        return Value::as_str(v1.v_str + v2.v_str);
    }
    else if (std::strcmp(_token, "-") == 0) {
      check_same_accepts("BinaryOperatorExpr(-)", v1.type, v2.type, TYPE_INTEGER);
      return Value::as_int(v1.v_int - v2.v_int);
    }
    else if (std::strcmp(_token, "%") == 0) {
      check_same_accepts("BinaryOperatorExpr(%)", v1.type, v2.type, TYPE_INTEGER);
      return Value::as_int(v1.v_int % v2.v_int);
    }
    else if (std::strcmp(_token, "*") == 0) {
      check_same_accepts("BinaryOperatorExpr(*)", v1.type, v2.type, TYPE_INTEGER);
      return Value::as_int(v1.v_int * v2.v_int);
    }
    else if (std::strcmp(_token, "/") == 0) {
      check_same_accepts("BinaryOperatorExpr(/)", v1.type, v2.type, TYPE_INTEGER);
      return Value::as_int(v1.v_int / v2.v_int);
    }
    else {
      assert(!"Should never get here");
      return Value::as_int(0);
    }
  }
};


class FunctionUnaryExpr : public Expr {
protected:
  Expr *const _expr;

public:
  FunctionUnaryExpr(const char *token, Expr *expr) : Expr(token), _expr(expr) { }
  virtual ~FunctionUnaryExpr(void) { }

  virtual Value
  eval(const uint64_t index, const dr::Doc &doc) const override {
    const Value v = _expr->eval(index, doc);
    if (std::strcmp(_token, "int") == 0) {
      check_accepts("FunctionUnaryExpr(int)", v.type, TYPE_INTEGER | TYPE_MISSING | TYPE_STRING);
      switch (v.type) {
      case TYPE_INTEGER: return v;
      case TYPE_MISSING: return Value::as_int(0);
      case TYPE_STRING: return Value::as_int(cast_to_int(v.v_str));
      }
    }
    else if (std::strcmp(_token, "len") == 0) {
      check_accepts("FunctionUnaryExpr(len)", v.type, TYPE_MISSING | TYPE_STRING | TYPE_STORE);
      switch (v.type) {
      case TYPE_MISSING: return Value::as_int(0);
      case TYPE_STRING: return Value::as_int(v.v_str.size());
      case TYPE_STORE: return Value::as_int(v.v_attribute->store_nelem());
      }
    }
    else if (std::strcmp(_token, "str") == 0) {
      check_accepts("FunctionUnaryExpr(str)", v.type, TYPE_INTEGER | TYPE_MISSING | TYPE_STRING);
      switch (v.type) {
      case TYPE_INTEGER: return Value::as_str(cast_to_str(v.v_int));
      case TYPE_MISSING: return Value::as_str("");
      case TYPE_STRING: return v;
      }
    }
    assert(!"Should never get here");
    return Value::as_int(0);
  }
};


class FunctionStoreExpr : public Expr {
protected:
  Expr *const _attribute_access;
  Expr *const _expr;

public:
  FunctionStoreExpr(const char *token, Expr *attribute_access, Expr *expr) :
      Expr(token),
      _attribute_access(attribute_access),
      _expr(expr) { }
  virtual ~FunctionStoreExpr(void) { }

  virtual Value
  eval(const uint64_t index, const dr::Doc &doc) const override {
    if (std::strcmp(_token, "all") == 0) {
    }
    else if (std::strcmp(_token, "any") == 0) {
    }
    else
      assert(!"Should never get here");
    (void)index;  // TODO
    (void)doc;
    return Value::as_int(0);
  }
};


class IndexExpr : public Expr {
public:
  explicit IndexExpr(const char *token) : Expr(token) { }
  virtual ~IndexExpr(void) { }

  virtual Value
  eval(const uint64_t index, const dr::Doc &) const override {
    return Value::as_int(static_cast<int64_t>(index));
  }
};


class LiteralIntegerExpr : public Expr {
protected:
  int64_t _value;

public:
  explicit LiteralIntegerExpr(const char *token) : Expr(token) {
    const int ret = std::sscanf(_token, "%lld", &_value);
    assert(ret == 1);
  }
  virtual ~LiteralIntegerExpr(void) { }

  virtual Value
  eval(const uint64_t, const dr::Doc &) const override {
    return Value::as_int(_value);
  }
};


class LiteralRegexExpr : public Expr {
protected:
  std::regex _re;

public:
  explicit LiteralRegexExpr(const char *token) : Expr(token) {
    try {
      _re = std::regex(_token + 1, std::strlen(_token) - 2, std::regex::nosubs | std::regex::ECMAScript);
    }
    catch (std::regex_error &e) {
      throw CompileError(e.what());
    }
  }
  virtual ~LiteralRegexExpr(void) { }

  virtual Value
  eval(const uint64_t, const dr::Doc &) const override {
    return Value::as_re(&_re);
  }
};


class LiteralStringExpr : public Expr {
protected:
  const std::string _value;

public:
  explicit LiteralStringExpr(const char *token) : Expr(token), _value(token + 1, std::strlen(token) - 2) { }
  virtual ~LiteralStringExpr(void) { }

  virtual Value
  eval(const uint64_t, const dr::Doc &) const override {
    return Value::as_str(_value);
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
    left = new (_pool) BinaryOperatorExpr(pair.second, left, right);
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
    left = new (_pool) BinaryOperatorExpr(pair.second, left, right);
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
    left = new (_pool) BinaryOperatorExpr(pair.second, left, right);
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
    left = new (_pool) BinaryOperatorExpr(pair.second, left, right);
  }
  return left;
}


Expr *
Interpreter::_parse_e5(void) {
  std::ostringstream msg;

  if (_tokens.empty())
    throw CompileError("Expected <e5> but no more tokens available");

  const decltype(_tokens)::value_type pair = _tokens.front();
  decltype(_tokens)::value_type tmp_pair;
  _tokens.pop_front();

  Expr *expr = nullptr;
  switch (pair.first) {
  case TokenType::OPEN_PAREN:
    expr = _parse_e1();
    if (_tokens.empty())
      throw CompileError("Expected CLOSE_PAREN in <e5> but no more tokens available");
    tmp_pair = _tokens.front();
    _tokens.pop_front();
    if (tmp_pair.first != TokenType::CLOSE_PAREN) {
      msg << "Expected CLOSE_PAREN in <e5> but found token type " << to_underlying(tmp_pair.first) << " instead";
      throw CompileError(msg.str());
    }
    break;

  case TokenType::INDEX:
    expr = new (_pool) IndexExpr(pair.second);
    break;

  case TokenType::ATTRIBUTE_ACCESS:
    expr = new (_pool) AttributeAccessExpr(pair.second);
    break;

  case TokenType::FUNCTION_UNARY:
    {
      // "("
      if (_tokens.empty())
        throw CompileError("Expected OPEN_PAREN in <e5> function_unary but no more tokens available");
      tmp_pair = _tokens.front();
      _tokens.pop_front();
      if (tmp_pair.first != TokenType::OPEN_PAREN) {
        msg << "Expected OPEN_PAREN in <e5> function_unary but found token type " << to_underlying(tmp_pair.first) << " instead";
        throw CompileError(msg.str());
      }
      // expr
      Expr *const e1 = _parse_e1();
      // ")"
      if (_tokens.empty())
        throw CompileError("Expected CLOSE_PAREN in <e5> function_unary but no more tokens available");
      tmp_pair = _tokens.front();
      _tokens.pop_front();
      if (tmp_pair.first != TokenType::CLOSE_PAREN) {
        msg << "Expected CLOSE_PAREN in <e5> function_unary but found token type " << to_underlying(tmp_pair.first) << " instead";
        throw CompileError(msg.str());
      }
      expr = new (_pool) FunctionUnaryExpr(pair.second, e1);
    }
    break;

  case TokenType::FUNCTION_STORE:
    {
      // "("
      if (_tokens.empty())
        throw CompileError("Expected OPEN_PAREN in <e5> function_store but no more tokens available");
      tmp_pair = _tokens.front();
      _tokens.pop_front();
      if (tmp_pair.first != TokenType::OPEN_PAREN) {
        msg << "Expected OPEN_PAREN in <e5> function_store but found token type " << to_underlying(tmp_pair.first) << " instead";
        throw CompileError(msg.str());
      }
      // attribute_access
      if (_tokens.empty())
        throw CompileError("Expected attribute_access in <e5> function_store but no more tokens available");
      tmp_pair = _tokens.front();
      _tokens.pop_front();
      if (tmp_pair.first != TokenType::ATTRIBUTE_ACCESS) {
        msg << "Expected attribute_access in <e5> function_store but found token type " << to_underlying(tmp_pair.first) << " instead";
        throw CompileError(msg.str());
      }
      Expr *const attribute_access = new (_pool) AttributeAccessExpr(tmp_pair.second);
      // ","
      if (_tokens.empty())
        throw CompileError("Expected COMMA in <e5> function_store but no more tokens available");
      tmp_pair = _tokens.front();
      _tokens.pop_front();
      if (tmp_pair.first != TokenType::COMMA) {
        msg << "Expected COMMA in <e5> function_store but found token type " << to_underlying(tmp_pair.first) << " instead";
        throw CompileError(msg.str());
      }
      // expr
      Expr *const e1 = _parse_e1();
      // ")"
      if (_tokens.empty())
        throw CompileError("Expected CLOSE_PAREN in <e5> function_store but no more tokens available");
      tmp_pair = _tokens.front();
      _tokens.pop_front();
      if (tmp_pair.first != TokenType::CLOSE_PAREN) {
        msg << "Expected CLOSE_PAREN in <e5> function_store but found token type " << to_underlying(tmp_pair.first) << " instead";
        throw CompileError(msg.str());
      }
      expr = new (_pool) FunctionStoreExpr(pair.second, attribute_access, e1);
    }
    break;

  case TokenType::LITERAL_INTEGER:
    expr = new (_pool) LiteralIntegerExpr(pair.second);
    break;

  case TokenType::LITERAL_REGEX:
    expr = new (_pool) LiteralRegexExpr(pair.second);
    break;

  case TokenType::LITERAL_STRING:
    expr = new (_pool) LiteralStringExpr(pair.second);
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
    throw_compile_error(pair.second, std::strlen(pair.second));
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
Interpreter::_throw_compile_error(const char *const p, const char *const pe) {
  throw_compile_error(p, pe - p);
}


void
Interpreter::compile(const char *const str, const size_t len) {
  _tokenise(str, len);
  _parse();
}


bool
Interpreter::eval(const uint64_t index, const dr::Doc &doc) const {
  if (_expr == nullptr)
    return true;

  const Value v = _expr->eval(index, doc);
  switch (v.type) {
  case TYPE_INTEGER: return v.v_int;
  case TYPE_MISSING: return false;
  case TYPE_REGEX: return true;
  case TYPE_STORE: return true;
  case TYPE_STRING: return !v.v_str.empty();
  default:
    assert(!"Should not get here.");
    return false;
  }
}

}  // namesapce dr_grep
}  // namespace schwa
