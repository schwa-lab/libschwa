/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/dr.h>


namespace schwa { namespace dr {

void
Writer::write(const Document &d) {
  (void)d;
  _out << "Hello" << std::endl;
}

} }
