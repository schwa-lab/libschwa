/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/pool.h>


namespace schwa {

SUITE(schwa__pool) {

TEST(test) {
  Pool p(64);
  CHECK_EQUAL(0, p.nblocks());
  CHECK_EQUAL(0, p.nbytes_allocd());
  CHECK_EQUAL(0, p.nbytes_used());

  p.alloc(60);
  CHECK_EQUAL(1, p.nblocks());
  CHECK_EQUAL(64, p.nbytes_allocd());
  CHECK_EQUAL(60, p.nbytes_used());

  p.alloc(4);
  CHECK_EQUAL(1, p.nblocks());
  CHECK_EQUAL(64, p.nbytes_allocd());
  CHECK_EQUAL(64, p.nbytes_used());

  p.alloc(80);
  CHECK_EQUAL(2, p.nblocks());
  CHECK_EQUAL(144, p.nbytes_allocd());
  CHECK_EQUAL(144, p.nbytes_used());

  p.alloc(32);
  CHECK_EQUAL(3, p.nblocks());
  CHECK_EQUAL(208, p.nbytes_allocd());
  CHECK_EQUAL(176, p.nbytes_used());

  p.alloc(31);
  CHECK_EQUAL(3, p.nblocks());
  CHECK_EQUAL(208, p.nbytes_allocd());
  CHECK_EQUAL(207, p.nbytes_used());

  p.alloc(2);
  CHECK_EQUAL(4, p.nblocks());
  CHECK_EQUAL(272, p.nbytes_allocd());
  CHECK_EQUAL(209, p.nbytes_used());

  p.drain();
  CHECK_EQUAL(4, p.nblocks());
  CHECK_EQUAL(0, p.nblocks_used());
  CHECK_EQUAL(4, p.nblocks_unused());
  CHECK_EQUAL(272, p.nbytes_allocd());
  CHECK_EQUAL(0, p.nbytes_used());

  p.alloc(32);
  CHECK_EQUAL(4, p.nblocks());
  CHECK_EQUAL(1, p.nblocks_used());
  CHECK_EQUAL(3, p.nblocks_unused());
  CHECK_EQUAL(272, p.nbytes_allocd());
  CHECK_EQUAL(32, p.nbytes_used());

  p.alloc(32);
  CHECK_EQUAL(4, p.nblocks());
  CHECK_EQUAL(1, p.nblocks_used());
  CHECK_EQUAL(3, p.nblocks_unused());
  CHECK_EQUAL(272, p.nbytes_allocd());
  CHECK_EQUAL(64, p.nbytes_used());

  p.alloc(10);
  CHECK_EQUAL(4, p.nblocks());
  CHECK_EQUAL(2, p.nblocks_used());
  CHECK_EQUAL(2, p.nblocks_unused());
  CHECK_EQUAL(272, p.nbytes_allocd());
  CHECK_EQUAL(74, p.nbytes_used());

  p.alloc(1000);
  CHECK_EQUAL(5, p.nblocks());
  CHECK_EQUAL(3, p.nblocks_used());
  CHECK_EQUAL(2, p.nblocks_unused());
  CHECK_EQUAL(1272, p.nbytes_allocd());
  CHECK_EQUAL(1074, p.nbytes_used());
}

}  // SUITE

}  // namespace schwa
