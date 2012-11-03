/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "test_utils.h"

#include <schwa/containers/block_vector.h>

namespace ct = schwa::containers;


namespace schwatest {

SUITE(schwa__containers__block_vector) {

TEST(BlockVector_int__create) {
  ct::BlockVector<int> v;
  CHECK_EQUAL(0, v.nblocks());
  CHECK_EQUAL(0, v.size());

  auto &block1 = v.reserve(5);
  CHECK_EQUAL(1, v.nblocks());
  CHECK_EQUAL(0, v.size());
  CHECK_EQUAL(0, block1.size());
  CHECK_EQUAL(5, block1.capacity());

  int &a = block1.create();
  CHECK_EQUAL(1, block1.size());
  CHECK_EQUAL(1, v.size());
  a = 20;
  CHECK_EQUAL(20, block1[0]);

  int &b = block1.create();
  CHECK_EQUAL(2, block1.size());
  CHECK_EQUAL(2, v.size());
  b = 5;
  CHECK_EQUAL(5, block1[1]);

  int &c = block1.create();
  CHECK_EQUAL(3, block1.size());
  CHECK_EQUAL(3, v.size());
  c = 12345678;
  CHECK_EQUAL(12345678, block1[2]);

  int &d = block1.create();
  CHECK_EQUAL(4, block1.size());
  CHECK_EQUAL(4, v.size());
  d = -9e7;
  CHECK_EQUAL(-9e7, block1[3]);

  int &e = block1.create();
  CHECK_EQUAL(5, block1.size());
  CHECK_EQUAL(5, v.size());
  e = -21;
  CHECK_EQUAL(-21, block1[4]);

  CHECK_EQUAL(20, block1[0]);
  CHECK_EQUAL(5, block1[1]);
  CHECK_EQUAL(12345678, block1[2]);
  CHECK_EQUAL(-9e7, block1[3]);
  CHECK_EQUAL(-21, block1[4]);

  CHECK_EQUAL(block1[0], v[0]);
  CHECK_EQUAL(block1[1], v[1]);
  CHECK_EQUAL(block1[2], v[2]);
  CHECK_EQUAL(block1[3], v[3]);
  CHECK_EQUAL(block1[4], v[4]);

  auto &block2 = v.reserve(2);
  CHECK_EQUAL(2, v.nblocks());
  CHECK_EQUAL(5, v.size());
  CHECK_EQUAL(5, block1.size());
  CHECK_EQUAL(5, block1.capacity());
  CHECK_EQUAL(0, block2.size());
  CHECK_EQUAL(2, block2.capacity());

  int &f = block2.create();
  CHECK_EQUAL(1, block2.size());
  CHECK_EQUAL(6, v.size());
  f = 6786723;
  CHECK_EQUAL(6786723, block2[0]);

  int &g = block2.create();
  CHECK_EQUAL(2, block2.size());
  CHECK_EQUAL(7, v.size());
  g = 0;
  CHECK_EQUAL(0, block2[1]);

  CHECK_EQUAL(block1[0], v[0]);
  CHECK_EQUAL(block1[1], v[1]);
  CHECK_EQUAL(block1[2], v[2]);
  CHECK_EQUAL(block1[3], v[3]);
  CHECK_EQUAL(block1[4], v[4]);
  CHECK_EQUAL(block2[0], v[5]);
  CHECK_EQUAL(block2[1], v[6]);
}


TEST(BlockVector_int__block_iterator) {
  ct::BlockVector<int> v;
  auto fn = [](int i) -> int { return 2*i + 3; };

  auto &block1 = v.reserve(20);
  for (int i = 0; i != 13; ++i)
    block1.create(fn(i));
  CHECK_EQUAL(13, v.size());
  CHECK_EQUAL(13, block1.size());
  CHECK_EQUAL(20, block1.capacity());

  CHECK_EQUAL(13, std::distance(block1.begin(), block1.end()));
  auto it = block1.begin();
  CHECK_EQUAL(fn(0), *it); ++it;
  CHECK_EQUAL(fn(1), *it); ++it;
  CHECK_EQUAL(fn(2), *it); ++it;
  CHECK_EQUAL(fn(3), *it); ++it;
  CHECK_EQUAL(fn(4), *it); ++it;
  CHECK_EQUAL(fn(5), *it); ++it;
  CHECK_EQUAL(fn(6), *it); ++it;
  CHECK_EQUAL(fn(7), *it); ++it;
  CHECK_EQUAL(fn(8), *it); ++it;
  CHECK_EQUAL(fn(9), *it); ++it;
  CHECK_EQUAL(fn(10), *it); ++it;
  CHECK_EQUAL(fn(11), *it); ++it;
  CHECK_EQUAL(fn(12), *it); ++it;
  CHECK_EQUAL(block1.end(), it);

  CHECK_EQUAL(block1.front(), fn(0));
  CHECK_EQUAL(block1.back(), fn(12));
}


TEST(BlockVector_int__vector_iterator) {
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

  CHECK_EQUAL(3, v.nblocks());
  CHECK_EQUAL(12, v.size());

  ct::BlockVector<int>::iterator it = v.begin();
  CHECK_EQUAL(block1[0], *it); ++it;
  CHECK_EQUAL(block1[1], *it); ++it;
  CHECK_EQUAL(block1[2], *it); ++it;
  CHECK_EQUAL(block2[0], *it); ++it;
  CHECK_EQUAL(block2[1], *it); ++it;
  CHECK_EQUAL(block2[2], *it); ++it;
  CHECK_EQUAL(block2[3], *it); ++it;
  CHECK_EQUAL(block2[4], *it); ++it;
  CHECK_EQUAL(block3[0], *it); ++it;
  CHECK_EQUAL(block3[1], *it); ++it;
  CHECK_EQUAL(block3[2], *it); ++it;
  CHECK_EQUAL(block3[3], *it); ++it;
  CHECK_EQUAL(v.end(), it);

  CHECK_EQUAL(12, std::distance(v.begin(), v.end()));
}


TEST(BlockVector_int__vector_iterator_empty_blocks) {
  ct::BlockVector<int> v;
  auto fn = [](int i) -> int { return 4*i*i - 3*i + 7; };

  auto &block0 = v.reserve(0);
  for (size_t i = 0; i != block0.capacity(); ++i)
    block0.create(fn(i));
  auto &block1 = v.reserve(3);
  for (size_t i = 0; i != block1.capacity(); ++i)
    block1.create(fn(i));
  auto &block2 = v.reserve(0);
  for (size_t i = 0; i != block2.capacity(); ++i)
    block2.create(fn(i));
  auto &block3 = v.reserve(0);
  for (size_t i = 0; i != block3.capacity(); ++i)
    block3.create(fn(i));
  auto &block4 = v.reserve(4);
  for (size_t i = 0; i != block4.capacity(); ++i)
    block4.create(fn(i));
  auto &block5 = v.reserve(0);
  for (size_t i = 0; i != block5.capacity(); ++i)
    block5.create(fn(i));

  CHECK_EQUAL(6, v.nblocks());
  CHECK_EQUAL(7, v.size());

  CHECK_EQUAL(7, std::distance(v.begin(), v.end()));

  ct::BlockVector<int>::iterator it = v.begin();
  CHECK_EQUAL(block1[0], *it); ++it;
  CHECK_EQUAL(block1[1], *it); ++it;
  CHECK_EQUAL(block1[2], *it); ++it;
  CHECK_EQUAL(block4[0], *it); ++it;
  CHECK_EQUAL(block4[1], *it); ++it;
  CHECK_EQUAL(block4[2], *it); ++it;
  CHECK_EQUAL(block4[3], *it); ++it;
  CHECK_EQUAL(v.end(), it);
  static_cast<void>(block0);
  static_cast<void>(block2);
  static_cast<void>(block3);
  static_cast<void>(block5);
}


TEST(BlockVector_int__index_of) {
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

  CHECK_EQUAL(3, v.nblocks());
  CHECK_EQUAL(12, v.size());

  CHECK_EQUAL(0, v.index_of(block1[0]));
  CHECK_EQUAL(1, v.index_of(block1[1]));
  CHECK_EQUAL(2, v.index_of(block1[2]));
  CHECK_EQUAL(3, v.index_of(block2[0]));
  CHECK_EQUAL(4, v.index_of(block2[1]));
  CHECK_EQUAL(5, v.index_of(block2[2]));
  CHECK_EQUAL(6, v.index_of(block2[3]));
  CHECK_EQUAL(7, v.index_of(block2[4]));
  CHECK_EQUAL(8, v.index_of(block3[0]));
  CHECK_EQUAL(9, v.index_of(block3[1]));
  CHECK_EQUAL(10, v.index_of(block3[2]));
  CHECK_EQUAL(11, v.index_of(block3[3]));
}


TEST(BlockVector_int__get) {
  ct::BlockVector<int> v;
  auto fn = [](int i) -> int { return 2*i + 3; };

  auto &block0 = v.reserve(0);
  for (size_t i = 0; i != block0.capacity(); ++i)
    block0.create(fn(i));
  auto &block1 = v.reserve(3);
  for (size_t i = 0; i != block1.capacity(); ++i)
    block1.create(fn(i));
  auto &block2 = v.reserve(0);
  for (size_t i = 0; i != block2.capacity(); ++i)
    block2.create(fn(i));
  auto &block3 = v.reserve(0);
  for (size_t i = 0; i != block3.capacity(); ++i)
    block3.create(fn(i));
  auto &block4 = v.reserve(4);
  for (size_t i = 0; i != block4.capacity(); ++i)
    block4.create(fn(i));
  auto &block5 = v.reserve(0);
  for (size_t i = 0; i != block5.capacity(); ++i)
    block5.create(fn(i));

  CHECK_EQUAL(7, v.size());
  CHECK_EQUAL(fn(0), v.get(0));
  CHECK_EQUAL(fn(1), v.get(1));
  CHECK_EQUAL(fn(2), v.get(2));
  CHECK_EQUAL(fn(0), v.get(3));
  CHECK_EQUAL(fn(1), v.get(4));
  CHECK_EQUAL(fn(2), v.get(5));
  CHECK_EQUAL(fn(3), v.get(6));
}


}  // SUITE

}  // namespace schwatest
