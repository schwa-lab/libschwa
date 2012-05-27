/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/dr.h>


namespace schwa { namespace dr {

void
TypeRegistry::finalise(void) {
  std::set<std::string> seen;
  std::map<std::string, std::string> unfulfilled;

  _finalised = true;
}

} }
