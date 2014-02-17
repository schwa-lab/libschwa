/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <fstream>
#include <iostream>
#include <memory>

#include <schwa/io/mmapped_source.h>


namespace schwa {
namespace io {

SUITE(schwa__io__mmapped_source) {

TEST(test_etc_passwd) {
  static const char *filename = "/etc/passwd";

  std::ifstream fin(filename);
  fin.seekg(0, std::ios::end);
  const size_t size = fin.tellg();
  fin.seekg(0, std::ios::beg);

  std::unique_ptr<char> buffer(new char[size]);
  fin.read(buffer.get(), size);
  fin.close();

  MMappedSource src(filename);
  CHECK_EQUAL(src.size(), size);
  CHECK(src.data() != static_cast<void *>(nullptr));

  CHECK_COMPARE_BYTES4(reinterpret_cast<uint8_t *>(buffer.get()), size, reinterpret_cast<const uint8_t *>(src.data()), src.size());
}

}  // SUITE

}  // namespace io
}  // namespace schwa
