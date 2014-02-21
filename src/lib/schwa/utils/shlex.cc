/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/utils/shlex.h>

#include <cstring>
#include <sstream>

namespace schwa {
namespace utils {

std::string
shlex_quote(const std::string &s) {
  bool needs_quoting = false, contains_quote = false;
  for (const char c : s) {
    if (std::isspace(c) || c == '!' || c == '"' || c == '#' || c == '$' || c == '&' || c == '\'' || c == '(' || c == ')' || c == ';' || c == '<' || c == '>' || c == '=' || c == '?' || c == '[' || c == ']' || c == '\\' || c == '`' || c == '{' || c == '}' || c == '|' || c == '~') {
      needs_quoting = true;
      if (c == '\'')
        contains_quote = true;
    }
  }

  std::string out = s;
  if (contains_quote) {
    std::ostringstream ss;
    for (const char c : s) {
      if (c == '\'')
        ss << "\"'\"";
      else
        ss << c;
    }
    out = ss.str();
  }
  if (needs_quoting)
    out = "'" + out + "'";
  return out;
}

}
}
