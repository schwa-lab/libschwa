/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_STRING_H_
#define SCHWA_UTILS_STRING_H_

#include <algorithm>
#include <cctype>
#include <functional>
#include <locale>
#include <string>


namespace schwa {

  inline std::string &
  ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    return s;
  }


  inline std::string &
  rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    return s;
  }


  inline std::string &
  trim(std::string &s) {
    return ltrim(rtrim(s));
  }

}

#endif  // SCHWA_UTILS_STRING_H_
