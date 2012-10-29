/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "test_utils.h"

#include <fstream>
#include <iostream>
#include <memory>

#include <schwa/io/mmapped_source.h>

#include <boost/test/unit_test.hpp>

namespace io = schwa::io;


namespace schwatest {

BOOST_AUTO_TEST_SUITE(schwa__io__mmapped_source)

BOOST_AUTO_TEST_CASE(test_etc_passwd) {
  static const char *filename = "/etc/passwd";

  std::ifstream fin(filename);
  fin.seekg(0, std::ios::end);
  const size_t size = fin.tellg();
  fin.seekg(0, std::ios::beg);

  std::unique_ptr<char> buffer(new char[size]);
  fin.read(buffer.get(), size);
  fin.close();

  io::MMappedSource src(filename);
  BOOST_CHECK_EQUAL(src.size(), size);
  BOOST_CHECK_NE(src.data(), static_cast<void *>(nullptr));

  BOOST_CHECK( compare_bytes(reinterpret_cast<const uint8_t *>(src.data()), src.size(), reinterpret_cast<uint8_t *>(buffer.get()), size) );
}

BOOST_AUTO_TEST_SUITE_END()

}  // namespace schwatest
