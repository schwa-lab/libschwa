// vim: ft=ragel:
/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#define PUSH_TOKEN(type) _push_token(TokenType::type, ts, te)

%%{
  machine language;

  # ===========================================================================
  # ECMAScript regular expression definition, used by C++ regular expressions.
  # See http://www.ecma-international.org/ecma-262/5.1/#sec-7.8.5
  # ===========================================================================
  regex_non_terminator = [^\n] ;
  regex_backslash = "\\" regex_non_terminator ;
  regex_class_char = regex_non_terminator -- ("]" | "\\")
                   | regex_backslash
                   ;
  regex_class = "[" regex_class_char* "]" ;
  regex_first_char = regex_non_terminator -- ("*" | "\\" | "/" | "[")
                   | regex_backslash
                   | regex_class
                   ;
  regex_nonfirst_char = regex_non_terminator -- ("\\" | "/" | "[")
                      | regex_backslash
                      | regex_class
                      ;
  regex = "/" regex_first_char regex_nonfirst_char* "/";


  # ===========================================================================
  # Language definition.
  # ===========================================================================
  literal_int = ("-" | "+")? ("0" | [1-9][0-9]*) ;
  literal_regex = regex ;
  literal_str = '"' ('\\"' | [^"])* '"' ;

  op_boolean = "&&" | "||" ;
  op_comparison = "==" | "!=" | "<" | "<=" | ">" | ">=" | "~" | "~=" ;
  op_numeric3 = "+" | "-" | "%" ;
  op_numeric4 = "*" | "/" ;

  function = "all" | "any" | "int" | "len" | "str" ;
  var = [_a-zA-Z][_9a-zA-Z0-9]* ;
  var_attribute = "." [_a-zA-Z][_9a-zA-Z0-9]* ;

  main := |*
    literal_int => { PUSH_TOKEN(LITERAL_INTEGER); };
    literal_regex => { _push_token(TokenType::LITERAL_REGEX, ts + 1, te - 1); };
    literal_str => { _push_token(TokenType::LITERAL_STRING, ts + 1, te - 1); };

    op_boolean => { PUSH_TOKEN(OP_BOOLEAN); };
    op_comparison => { PUSH_TOKEN(OP_COMPARISON); };
    op_numeric3 => { PUSH_TOKEN(OP_NUMERIC3); };
    op_numeric4 => { PUSH_TOKEN(OP_NUMERIC4); };

    function => { PUSH_TOKEN(FUNCTION); };
    var => { PUSH_TOKEN(VAR); };
    var_attribute => { _push_token(TokenType::VAR_ATTRIBUTE, ts + 1, te); };

    "(" => { PUSH_TOKEN(OPEN_PAREN); };
    ")" => { PUSH_TOKEN(CLOSE_PAREN); };
    "," => { PUSH_TOKEN(COMMA); };

    space+ ;
  *|;
}%%


#include <schwa/dr/query.h>

namespace schwa {
namespace dr {
namespace query {

%% write data;

void
Interpreter::_tokenise(const char *const str, const size_t len) {
  int cs;
  int act;
  const char *ts, *te;
  const char *p = str;
  const char *pe = p + len;
  const char *eof = pe;

  // Initialise and run the FSA.
  %% write init;
  %% write exec;

  // Did the FSA terminate on an accepting state?
  if (cs < language_first_final)
    _throw_compile_error(p, pe);
}

}  // namespace query
}  // namesapce dr
}  // namespace schwa
