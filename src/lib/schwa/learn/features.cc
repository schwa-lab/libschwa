/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/learn/features.h>

#include <iostream>
#include <string>


namespace schwa {
namespace learn {

void
dump_crfsuite_feature(const std::string &feature, std::ostream &out) {
  for (auto &c : feature) {
    if (c == ':')
      out << "\\:";
    else if (c == '\\')
      out << "\\\\";
    else
      out << c;
  }
}

}  // namespace learn
}  // namespace schwa
