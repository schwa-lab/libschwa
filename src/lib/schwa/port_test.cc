/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <schwa/port.h>


namespace schwa {
namespace port {

SUITE(schwa__port) {

TEST(abspath_to_argv0) {
  const std::string path = abspath_to_argv0();
  CHECK(!path.empty());
}


TEST(path_dirname) {
  std::string path;
  path = path_dirname(".");
  CHECK_EQUAL(".", path);
  path = path_dirname("../foo");
  CHECK_EQUAL("..", path);
  path = path_dirname("/this/is/a/path");
  CHECK_EQUAL("/this/is/a", path);
  path = path_dirname("foo");
  CHECK_EQUAL(".", path);
  path = path_dirname("./foo");
  CHECK_EQUAL(".", path);
}


TEST(path_join) {
  std::string path;
  path = path_join("one", "two");
  CHECK_EQUAL("one/two", path);
  path = path_join("", "two");
  CHECK_EQUAL("two", path);
  path = path_join("one", "/two");
  CHECK_EQUAL("/two", path);
}

}  // SUITE

}  // namespace port
}  // namespace schwa
