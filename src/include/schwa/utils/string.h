/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {

  inline void
  lower(std::string &s){
    for (std::string::iterator i = s.begin(); i != s.end(); ++i)
      *i = std::tolower(*i);
  }

}
