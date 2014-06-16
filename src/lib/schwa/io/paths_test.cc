/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <stdlib.h>

#include <schwa/io/paths.h>
#include <schwa/unittest.h>


namespace schwa {
namespace io {

SUITE(schwa__io__paths) {

TEST(abspath_to_argv0) {
  const std::string path = abspath_to_argv0();
  CHECK(!path.empty());
}


TEST(get_env_paths) {
  std::vector<std::string> paths;
  int ret;

  paths.clear();
  ret = ::setenv("TEST_GET_ENV_PATH", "/foo/bar:baz:.:/usr:/bin", 1);
  CHECK_EQUAL(0, ret);
  get_env_paths(paths, "TEST_GET_ENV_PATH");
  CHECK_EQUAL(5, paths.size());
  CHECK_EQUAL("/foo/bar", paths[0]);
  CHECK_EQUAL("baz", paths[1]);
  CHECK_EQUAL(".", paths[2]);
  CHECK_EQUAL("/usr", paths[3]);
  CHECK_EQUAL("/bin", paths[4]);

  paths.clear();
  ret = ::unsetenv("TEST_GET_ENV_PATH");
  CHECK_EQUAL(0, ret);
  get_env_paths(paths, "TEST_GET_ENV_PATH");
  CHECK_EQUAL(0, paths.size());

  paths.clear();
  ret = ::setenv("TEST_GET_ENV_PATH", "", 1);
  CHECK_EQUAL(0, ret);

  ::unsetenv("TEST_GET_ENV_PATH");
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

}  // namespace io
}  // namespace schwa
