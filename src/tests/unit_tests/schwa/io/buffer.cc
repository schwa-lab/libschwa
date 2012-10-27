/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "test_utils.h"

#include <schwa/io/write_buffer.h>

#include <boost/test/unit_test.hpp>

namespace io = schwa::io;


namespace schwatest {

BOOST_AUTO_TEST_SUITE(schwa__io__write_buffer)

BOOST_AUTO_TEST_CASE(test_construction_default) {
  io::WriteBuffer b;
  BOOST_CHECK_EQUAL(b.size(), 0);
  BOOST_CHECK_EQUAL(b.nblocks(), io::WriteBuffer::DEFAULT_NBLOCKS);
  BOOST_CHECK_EQUAL(b.block_size(), io::WriteBuffer::DEFAULT_BLOCK_SIZE);
  BOOST_CHECK_EQUAL(b.block_upto(), 0);
  BOOST_CHECK_EQUAL(b.npools(), io::WriteBuffer::DEFAULT_NPOOLS);
  BOOST_CHECK_EQUAL(b.pool_size(), io::WriteBuffer::DEFAULT_POOL_SIZE);
  BOOST_CHECK_EQUAL(b.pool_upto(), 0);
}


BOOST_AUTO_TEST_CASE(test_construction_custom) {
  io::WriteBuffer b(4, 1, 8, 2);
  BOOST_CHECK_EQUAL(b.size(), 0);
  BOOST_CHECK_EQUAL(b.nblocks(), 1);
  BOOST_CHECK_EQUAL(b.block_size(), 4);
  BOOST_CHECK_EQUAL(b.block_upto(), 0);
  BOOST_CHECK_EQUAL(b.npools(), 2);
  BOOST_CHECK_EQUAL(b.pool_size(), 8);
  BOOST_CHECK_EQUAL(b.pool_upto(), 0);
}


BOOST_AUTO_TEST_CASE(test_put) {
  io::WriteBuffer b(4, 1, 8, 2);

  b.put('0');
  b.put('1');

  BOOST_CHECK_EQUAL(b.size(), 2);
  BOOST_CHECK_EQUAL(b.nblocks(), 1);
  BOOST_CHECK_EQUAL(b.block_upto(), 0);
  BOOST_CHECK_EQUAL(b.npools(), 2);
  BOOST_CHECK_EQUAL(b.pool_upto(), 0);

  b.put('2');
  b.put('3');

  BOOST_CHECK_EQUAL(b.size(), 4);
  BOOST_CHECK_EQUAL(b.nblocks(), 1);
  BOOST_CHECK_EQUAL(b.block_upto(), 0);
  BOOST_CHECK_EQUAL(b.npools(), 2);
  BOOST_CHECK_EQUAL(b.pool_upto(), 0);

  b.put('4');

  BOOST_CHECK_EQUAL(b.size(), 5);
  BOOST_CHECK_EQUAL(b.nblocks(), 2);
  BOOST_CHECK_EQUAL(b.block_upto(), 1);
  BOOST_CHECK_EQUAL(b.npools(), 2);
  BOOST_CHECK_EQUAL(b.pool_upto(), 0);

  b.put('5');
  b.put('6');
  b.put('7');

  BOOST_CHECK_EQUAL(b.size(), 8);
  BOOST_CHECK_EQUAL(b.nblocks(), 2);
  BOOST_CHECK_EQUAL(b.block_upto(), 1);
  BOOST_CHECK_EQUAL(b.npools(), 2);
  BOOST_CHECK_EQUAL(b.pool_upto(), 0);

  b.put('8');

  BOOST_CHECK_EQUAL(b.size(), 9);
  BOOST_CHECK_EQUAL(b.nblocks(), 4);
  BOOST_CHECK_EQUAL(b.block_upto(), 2);
  BOOST_CHECK_EQUAL(b.npools(), 2);
  BOOST_CHECK_EQUAL(b.pool_upto(), 1);

  b.put('9');
  b.put('A');
  b.put('B');
  b.put('C');
  b.put('D');
  b.put('E');
  b.put('F');

  BOOST_CHECK_EQUAL(b.size(), 16);
  BOOST_CHECK_EQUAL(b.nblocks(), 4);
  BOOST_CHECK_EQUAL(b.block_upto(), 3);
  BOOST_CHECK_EQUAL(b.npools(), 2);
  BOOST_CHECK_EQUAL(b.pool_upto(), 1);

  b.put('x');

  BOOST_CHECK_EQUAL(b.size(), 17);
  BOOST_CHECK_EQUAL(b.nblocks(), 8);
  BOOST_CHECK_EQUAL(b.block_upto(), 4);
  BOOST_CHECK_EQUAL(b.npools(), 4);
  BOOST_CHECK_EQUAL(b.pool_upto(), 2);
}


BOOST_AUTO_TEST_CASE(test_copyto) {
  io::WriteBuffer b(4, 1, 8, 2);
  for (char c = 'a'; c <= 'z'; ++c)
    b.put(c);

  std::stringstream ss;
  b.copy_to(ss);
  BOOST_CHECK_EQUAL(ss.str(), "abcdefghijklmnopqrstuvwxyz");
}


BOOST_AUTO_TEST_CASE(test_copyfrom) {
  io::WriteBuffer b1(4, 1, 8, 2);
  for (char c = 'a'; c <= 'z'; ++c)
    b1.put(c);

  io::WriteBuffer b2;
  b2.write("XXXXX", 5);
  b2.copy_from(b1);
  b2.write("YYYY", 4);

  std::stringstream ss;
  b2.copy_to(ss);

  BOOST_CHECK_EQUAL(ss.str(), "XXXXXabcdefghijklmnopqrstuvwxyzYYYY");
}


BOOST_AUTO_TEST_SUITE_END()

}  // namespace schwatest
