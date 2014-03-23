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
  attribute_access = ("ann" | "doc") "." [_0-9a-zA-Z]+ ;
  function_cast = "int" | "str" ;
  function_store = "all" | "any" ;

  main := |*
    literal_int => { PUSH_TOKEN(LITERAL_INTEGER); };
    literal_regex => { PUSH_TOKEN(LITERAL_REGEX); };
    literal_str => { PUSH_TOKEN(LITERAL_STRING); };

    op_boolean => { PUSH_TOKEN(OP_BOOLEAN); };
    op_comparison => { PUSH_TOKEN(OP_COMPARISON); };
    op_numeric3 => { PUSH_TOKEN(OP_NUMERIC3); };
    op_numeric4 => { PUSH_TOKEN(OP_NUMERIC4); };

    attribute_access => { PUSH_TOKEN(ATTRIBUTE_ACCESS); };
    function_cast => { PUSH_TOKEN(FUNCTION_CAST); };
    function_store => { PUSH_TOKEN(FUNCTION_STORE); };
    "index" => { PUSH_TOKEN(INDEX); };

    "(" => { PUSH_TOKEN(OPEN_PAREN); };
    ")" => { PUSH_TOKEN(CLOSE_PAREN); };
    "," => { PUSH_TOKEN(COMMA); };

    space+ ;
  *|;
}%%


#include <dr-grep/language.h>

namespace schwa {
namespace dr_grep {

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

}  // namesapce dr_grep
}  // namespace schwa
