/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/tokenizer.h>
#include <schwa/dr/types.h>

using namespace schwa;

class Token : public dr::Annotation {
public:
  using dr::Annotation::Annotation;
};


int
main(void) {
  dr::Slice<uint64_t> slice;
  dr::Slice<Token *> ptr_slice;
  dr::Pointer<Token> ptr;
  dr::Pointers<Token> ptrs;
  dr::Store<Token> tokens;

  return 0;
}
