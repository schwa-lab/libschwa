/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRGREP_LANGUAGE_H_
#define SCHWA_DRGREP_LANGUAGE_H_

#include <deque>
#include <string>
#include <utility>

#include <schwa/_base.h>
#include <schwa/exception.h>
#include <schwa/pool.h>


namespace schwa {
  namespace dr {
    class Doc;
  }

  namespace dr_grep {

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


    class Expr;


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

      bool eval(uint64_t index, const dr::Doc &doc) const;
      inline bool operator ()(uint64_t index, const dr::Doc &doc) const { return eval(index, doc); }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Interpreter);
    };

  }
}

#endif  // SCHWA_DRGREP_LANGUAGE_H_
