/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/containers.h>
#include "test_utils.h"

#include <boost/test/unit_test.hpp>

namespace ct = schwa::containers;


BOOST_AUTO_TEST_SUITE(schwa__containers__block_vector)

BOOST_AUTO_TEST_CASE(BlockVector_int__create) {
  ct::BlockVector<int> v;
  BOOST_CHECK_EQUAL(v.nblocks(), 0);
  BOOST_CHECK_EQUAL(v.size(), 0);

  auto &block1 = v.reserve(5);
  BOOST_CHECK_EQUAL(v.nblocks(), 1);
  BOOST_CHECK_EQUAL(v.size(), 0);
  BOOST_CHECK_EQUAL(block1.size(), 0);
  BOOST_CHECK_EQUAL(block1.capacity(), 5);

  int &a = block1.create();
  BOOST_CHECK_EQUAL(block1.size(), 1);
  BOOST_CHECK_EQUAL(v.size(), 1);
  a = 20;
  BOOST_CHECK_EQUAL(block1[0], 20);

  int &b = block1.create();
  BOOST_CHECK_EQUAL(block1.size(), 2);
  BOOST_CHECK_EQUAL(v.size(), 2);
  b = 5;
  BOOST_CHECK_EQUAL(block1[1], 5);

  int &c = block1.create();
  BOOST_CHECK_EQUAL(block1.size(), 3);
  BOOST_CHECK_EQUAL(v.size(), 3);
  c = 12345678;
  BOOST_CHECK_EQUAL(block1[2], 12345678);

  int &d = block1.create();
  BOOST_CHECK_EQUAL(block1.size(), 4);
  BOOST_CHECK_EQUAL(v.size(), 4);
  d = -9e7;
  BOOST_CHECK_EQUAL(block1[3], -9e7);

  int &e = block1.create();
  BOOST_CHECK_EQUAL(block1.size(), 5);
  BOOST_CHECK_EQUAL(v.size(), 5);
  e = -21;
  BOOST_CHECK_EQUAL(block1[4], -21);

  BOOST_CHECK_EQUAL(block1[0], 20);
  BOOST_CHECK_EQUAL(block1[1], 5);
  BOOST_CHECK_EQUAL(block1[2], 12345678);
  BOOST_CHECK_EQUAL(block1[3], -9e7);
  BOOST_CHECK_EQUAL(block1[4], -21);

  BOOST_CHECK_EQUAL(v[0], block1[0]);
  BOOST_CHECK_EQUAL(v[1], block1[1]);
  BOOST_CHECK_EQUAL(v[2], block1[2]);
  BOOST_CHECK_EQUAL(v[3], block1[3]);
  BOOST_CHECK_EQUAL(v[4], block1[4]);

  auto &block2 = v.reserve(2);
  BOOST_CHECK_EQUAL(v.nblocks(), 2);
  BOOST_CHECK_EQUAL(v.size(), 5);
  BOOST_CHECK_EQUAL(block1.size(), 5);
  BOOST_CHECK_EQUAL(block1.capacity(), 5);
  BOOST_CHECK_EQUAL(block2.size(), 0);
  BOOST_CHECK_EQUAL(block2.capacity(), 2);

  int &f = block2.create();
  BOOST_CHECK_EQUAL(block2.size(), 1);
  BOOST_CHECK_EQUAL(v.size(), 6);
  f = 6786723;
  BOOST_CHECK_EQUAL(block2[0], 6786723);

  int &g = block2.create();
  BOOST_CHECK_EQUAL(block2.size(), 2);
  BOOST_CHECK_EQUAL(v.size(), 7);
  g = 0;
  BOOST_CHECK_EQUAL(block2[1], 0);

  BOOST_CHECK_EQUAL(v[0], block1[0]);
  BOOST_CHECK_EQUAL(v[1], block1[1]);
  BOOST_CHECK_EQUAL(v[2], block1[2]);
  BOOST_CHECK_EQUAL(v[3], block1[3]);
  BOOST_CHECK_EQUAL(v[4], block1[4]);
  BOOST_CHECK_EQUAL(v[5], block2[0]);
  BOOST_CHECK_EQUAL(v[6], block2[1]);
}


BOOST_AUTO_TEST_CASE(BlockVector_int__block_iterator) {
  ct::BlockVector<int> v;
  auto fn = [](int i) -> int { return 2*i + 3; };

  auto &block1 = v.reserve(20);
  for (int i = 0; i != 13; ++i)
    block1.create(fn(i));
  BOOST_CHECK_EQUAL(v.size(), 13);
  BOOST_CHECK_EQUAL(block1.size(), 13);
  BOOST_CHECK_EQUAL(block1.capacity(), 20);

  BOOST_REQUIRE_EQUAL(std::distance(block1.begin(), block1.end()), 13);
  auto it = block1.begin();
  BOOST_CHECK_EQUAL(*it, fn(0)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(1)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(2)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(3)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(4)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(5)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(6)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(7)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(8)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(9)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(10)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(11)); ++it;
  BOOST_CHECK_EQUAL(*it, fn(12)); ++it;
  BOOST_CHECK_EQUAL(it, block1.end());

  BOOST_CHECK_EQUAL(fn(0), block1.front());
  BOOST_CHECK_EQUAL(fn(12), block1.back());
}


BOOST_AUTO_TEST_CASE(BlockVector_int__vector_iterator) {
  ct::BlockVector<int> v;
  auto fn = [](int i) -> int { return 2*i + 3; };

  auto &block1 = v.reserve(3);
  for (size_t i = 0; i != block1.capacity(); ++i)
    block1.create(fn(i));
  auto &block2 = v.reserve(5);
  for (size_t i = 0; i != block2.capacity(); ++i)
    block2.create(fn(i));
  auto &block3 = v.reserve(4);
  for (size_t i = 0; i != block3.capacity(); ++i)
    block3.create(fn(i));

  BOOST_REQUIRE_EQUAL(v.nblocks(), 3);
  BOOST_REQUIRE_EQUAL(v.size(), 12);

  ct::BlockVector<int>::iterator it = v.begin();
  BOOST_CHECK_EQUAL(*it, block1[0]); ++it;
  BOOST_CHECK_EQUAL(*it, block1[1]); ++it;
  BOOST_CHECK_EQUAL(*it, block1[2]); ++it;
  BOOST_CHECK_EQUAL(*it, block2[0]); ++it;
  BOOST_CHECK_EQUAL(*it, block2[1]); ++it;
  BOOST_CHECK_EQUAL(*it, block2[2]); ++it;
  BOOST_CHECK_EQUAL(*it, block2[3]); ++it;
  BOOST_CHECK_EQUAL(*it, block2[4]); ++it;
  BOOST_CHECK_EQUAL(*it, block3[0]); ++it;
  BOOST_CHECK_EQUAL(*it, block3[1]); ++it;
  BOOST_CHECK_EQUAL(*it, block3[2]); ++it;
  BOOST_CHECK_EQUAL(*it, block3[3]); ++it;
  BOOST_CHECK_EQUAL(it, v.end());

  BOOST_CHECK_EQUAL(std::distance(v.begin(), v.end()), 12);
}

BOOST_AUTO_TEST_SUITE_END()
