/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/io/write_buffer.h>


namespace schwa {
namespace io {

SUITE(schwa__io__write_buffer) {

TEST(test_construction_default) {
  WriteBuffer b;
  CHECK_EQUAL(0, b.size());
  CHECK_EQUAL(WriteBuffer::DEFAULT_NBLOCKS, b.nblocks());
  CHECK_EQUAL(WriteBuffer::DEFAULT_BLOCK_SIZE, b.block_size());
  CHECK_EQUAL(0, b.block_upto());
  CHECK_EQUAL(WriteBuffer::DEFAULT_NPOOLS, b.npools());
  CHECK_EQUAL(WriteBuffer::DEFAULT_POOL_SIZE, b.pool_size());
  CHECK_EQUAL(0, b.pool_upto());
}


TEST(test_construction_custom) {
  WriteBuffer b(4, 1, 8, 2);
  CHECK_EQUAL(0, b.size());
  CHECK_EQUAL(1, b.nblocks());
  CHECK_EQUAL(4, b.block_size());
  CHECK_EQUAL(0, b.block_upto());
  CHECK_EQUAL(2, b.npools());
  CHECK_EQUAL(8, b.pool_size());
  CHECK_EQUAL(0, b.pool_upto());
}


TEST(test_put) {
  WriteBuffer b(4, 1, 8, 2);

  b.put('0');
  b.put('1');

  CHECK_EQUAL(2, b.size());
  CHECK_EQUAL(1, b.nblocks());
  CHECK_EQUAL(0, b.block_upto());
  CHECK_EQUAL(2, b.npools());
  CHECK_EQUAL(0, b.pool_upto());

  b.put('2');
  b.put('3');

  CHECK_EQUAL(4, b.size());
  CHECK_EQUAL(1, b.nblocks());
  CHECK_EQUAL(0, b.block_upto());
  CHECK_EQUAL(2, b.npools());
  CHECK_EQUAL(0, b.pool_upto());

  b.put('4');

  CHECK_EQUAL(5, b.size());
  CHECK_EQUAL(2, b.nblocks());
  CHECK_EQUAL(1, b.block_upto());
  CHECK_EQUAL(2, b.npools());
  CHECK_EQUAL(0, b.pool_upto());

  b.put('5');
  b.put('6');
  b.put('7');

  CHECK_EQUAL(8, b.size());
  CHECK_EQUAL(2, b.nblocks());
  CHECK_EQUAL(1, b.block_upto());
  CHECK_EQUAL(2, b.npools());
  CHECK_EQUAL(0, b.pool_upto());

  b.put('8');

  CHECK_EQUAL(9, b.size());
  CHECK_EQUAL(4, b.nblocks());
  CHECK_EQUAL(2, b.block_upto());
  CHECK_EQUAL(2, b.npools());
  CHECK_EQUAL(1, b.pool_upto());

  b.put('9');
  b.put('A');
  b.put('B');
  b.put('C');
  b.put('D');
  b.put('E');
  b.put('F');

  CHECK_EQUAL(16, b.size());
  CHECK_EQUAL(4, b.nblocks());
  CHECK_EQUAL(3, b.block_upto());
  CHECK_EQUAL(2, b.npools());
  CHECK_EQUAL(1, b.pool_upto());

  b.put('x');

  CHECK_EQUAL(17, b.size());
  CHECK_EQUAL(8, b.nblocks());
  CHECK_EQUAL(4, b.block_upto());
  CHECK_EQUAL(4, b.npools());
  CHECK_EQUAL(2, b.pool_upto());
}


TEST(test_copyto) {
  WriteBuffer b(4, 1, 8, 2);
  for (char c = 'a'; c <= 'z'; ++c)
    b.put(c);

  std::stringstream ss;
  b.copy_to(ss);
  CHECK_EQUAL("abcdefghijklmnopqrstuvwxyz", ss.str());
}


TEST(test_copyfrom) {
  WriteBuffer b1(4, 1, 8, 2);
  for (char c = 'a'; c <= 'z'; ++c)
    b1.put(c);

  WriteBuffer b2;
  b2.write("XXXXX", 5);
  b2.copy_from(b1);
  b2.write("YYYY", 4);

  std::stringstream ss;
  b2.copy_to(ss);

  CHECK_EQUAL("XXXXXabcdefghijklmnopqrstuvwxyzYYYY", ss.str());
}

}  // SUITE

}  // namespace io
}  // namespace schwa
