/* -*- Mode: C++; indent-tabs-mode: nil -*- */
/** @file */
#ifndef SCHWA_UTILS_SHLEX_H_
#define SCHWA_UTILS_SHLEX_H_

#include <string>

namespace schwa {
  namespace utils {

    /**
     * Return a shell-escaped version of the string \p s with quotes added if required.
     **/
    std::string shlex_quote(const std::string &s);

  }
}

#endif  // SCHWA_UTILS_SHLEX_H_
