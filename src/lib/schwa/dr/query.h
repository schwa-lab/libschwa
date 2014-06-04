/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_QUERY_H_
#define SCHWA_DR_QUERY_H_

#include <deque>
#include <string>
#include <utility>

#include <schwa/_base.h>
#include <schwa/exception.h>
#include <schwa/msgpack.h>
#include <schwa/pool.h>

#include <re2/re2.h>


namespace schwa {
  namespace dr {
    class Doc;

    namespace query {

      class Expr;
      class VariableExpr;

      enum class TokenType : uint32_t {
        LITERAL_INTEGER, LITERAL_REGEX, LITERAL_STRING,
        OP_BOOLEAN, OP_COMPARISON, OP_NUMERIC3, OP_NUMERIC4,
        FUNCTION, VAR, VAR_ATTRIBUTE,
        OPEN_PAREN, CLOSE_PAREN,
        COMMA,
      };


      class CompileError : public Exception {
      public:
        explicit CompileError(const std::string &msg) : Exception(msg) { }
        CompileError(const CompileError &o) : Exception(o) { }
        virtual ~CompileError(void) throw() { }
      };


      class RuntimeError : public Exception {
      public:
        explicit RuntimeError(const std::string &msg) : Exception(msg) { }
        RuntimeError(const RuntimeError &o) : Exception(o) { }
        virtual ~RuntimeError(void) throw() { }
      };


      using ValueType = uint32_t;
      constexpr const ValueType TYPE_ANN     = 1 << 0;
      constexpr const ValueType TYPE_DOC     = 1 << 1;
      constexpr const ValueType TYPE_INTEGER = 1 << 2;
      constexpr const ValueType TYPE_MISSING = 1 << 3;
      constexpr const ValueType TYPE_REGEX   = 1 << 4;
      constexpr const ValueType TYPE_STORE   = 1 << 5;
      constexpr const ValueType TYPE_STRING  = 1 << 6;
      constexpr const ValueType TYPE_ANY     = (1 << 7) - 1;

      const char *valuetype_name(ValueType type);


      class Value {
      public:
        ValueType type;
        union {
          int64_t _int;
          const char *_str;
          const re2::RE2 *_re;
          const VariableExpr *_variable;
          const Doc *_doc;
          const msgpack::Map *_map;
        } via;

      protected:
        Value(ValueType type, int64_t value) : type(type) { via._int = value; }
        Value(ValueType type, const char *value) : type(type) { via._str = value; }
        Value(ValueType type, const VariableExpr *value) : type(type) { via._variable = value; }
        Value(ValueType type, const Doc *value) : type(type) { via._doc = value; }
        Value(ValueType type, const msgpack::Map *value) : type(type) { via._map = value; }
        Value(ValueType type, const re2::RE2 *value) : type(type) { via._re = value; }

      public:
        Value(const Value &o) : type(o.type), via(o.via) { }
        Value(const Value &&o) : type(o.type), via(o.via) { }

        bool to_bool(void) const;
        inline operator bool(void) const { return to_bool(); }

        static inline Value as_ann(const msgpack::Map *value) { return Value(TYPE_ANN, value); }
        static inline Value as_doc(const Doc *value) { return Value(TYPE_DOC, value); }
        static inline Value as_int(int64_t value) { return Value(TYPE_INTEGER, value); }
        static inline Value as_missing(const VariableExpr *value) { return Value(TYPE_MISSING, value); }
        static inline Value as_re(const re2::RE2 *value) { return Value(TYPE_REGEX, value); }
        static inline Value as_store(const VariableExpr *value) { return Value(TYPE_STORE, value); }
        static inline Value as_str(const char *value) { return Value(TYPE_STRING, value); }
      };


      class Interpreter {
      private:
        Pool _pool;
        std::deque<std::pair<TokenType, const char *>> _tokens;
        Expr *_expr;

        Expr *_parse_e1(void);
        Expr *_parse_e2(void);
        Expr *_parse_e3(void);
        Expr *_parse_e4(void);
        Expr *_parse_e5(void);

        void _parse(void);
        void _push_token(const TokenType type, const char *ts, const char *te);
        void _tokenise(const char *str, size_t len);
        void _throw_compile_error(const char *p, const char *pe);

      public:
        Interpreter(void);
        ~Interpreter(void);

        void compile(const char *str, size_t len);
        inline void compile(const std::string &str) { compile(str.c_str(), str.size()); }

        Value eval(const Doc &doc, uint32_t doc_num) const;
        inline Value operator ()(const Doc &doc, uint32_t doc_num) const { return eval(doc, doc_num); }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(Interpreter);
      };

    }
  }
}

#endif  // SCHWA_DR_QUERY_H_
