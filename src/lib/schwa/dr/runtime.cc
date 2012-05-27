/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/dr.h>


namespace schwa { namespace dr {

void
TypeRegistry::finalise(void) {
  if (_finalised)
    return;

  std::set<std::string> required;
  for (auto &def : *_doc_schema)
    std::cout << def << std::endl;

  _finalised = true;
}

} }
