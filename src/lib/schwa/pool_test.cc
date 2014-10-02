/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/pool.h>


namespace schwa {

SUITE(schwa__pool) {

TEST(test) {
  Pool p(64);
  CHECK_EQUAL(1, p.nblocks());
  //CHECK_EQUAL(64, p.allocd());
  //CHECK_EQUAL(0, p.used());

  void *ptr = p.alloc(60);
  CHECK_EQUAL(1, p.nblocks());
  //CHECK_EQUAL(64, p.allocd());
  //CHECK_EQUAL(60, p.used());
  CHECK(reinterpret_cast<size_t>(ptr) % Pool::DEFAULT_ALIGNMENT == 0);

  p.alloc(4);
  CHECK_EQUAL(1, p.nblocks());
  //CHECK_EQUAL(64, p.allocd());
  //CHECK_EQUAL(64, p.used());

  p.alloc(80);
  CHECK_EQUAL(2, p.nblocks());
  //CHECK_EQUAL(144, p.allocd());
  //CHECK_EQUAL(144, p.used());

  p.alloc(32);
  CHECK_EQUAL(3, p.nblocks());
  //CHECK_EQUAL(208, p.allocd());
  //CHECK_EQUAL(176, p.used());

  p.alloc(31);
  CHECK_EQUAL(3, p.nblocks());
  //CHECK_EQUAL(208, p.allocd());
  //CHECK_EQUAL(207, p.used());

  p.alloc(2);
  CHECK_EQUAL(4, p.nblocks());
  //CHECK_EQUAL(272, p.allocd());
  //CHECK_EQUAL(209, p.used());
}

}  // SUITE

}  // namespace schwa
