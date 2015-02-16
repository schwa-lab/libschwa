/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <sstream>

#include <schwa/lex/brown-clusters.h>
#include <schwa/msgpack.h>
#include <schwa/unicode.h>

namespace mp = ::schwa::msgpack;


namespace schwa {
namespace lex {

SUITE(schwa__lex__brown_clusters) {

TEST(brown_clusters_normal_load) {
  Pool pool(4 * 1024 * 1024);
  StringPool sp(pool);
  BrownClusters brown_clusters(sp);
  const uint8_t *path, *canonical0, *canonical1;
  unsigned int lengths[4];
  size_t npaths;

  CHECK_EQUAL(true, brown_clusters.empty());
  CHECK_EQUAL(0, brown_clusters.size());
  CHECK_EQUAL(5, brown_clusters.min_freq());
  CHECK_EQUAL(4, brown_clusters.npaths());
  CHECK_EQUAL( 4, brown_clusters.path_lengths()[0]);
  CHECK_EQUAL( 6, brown_clusters.path_lengths()[1]);
  CHECK_EQUAL(10, brown_clusters.path_lengths()[2]);
  CHECK_EQUAL(20, brown_clusters.path_lengths()[3]);

  std::stringstream ss;
  mp::write_map_size(ss, 2);
  mp::write_str(ss, "00100010");  // length 8
  mp::write_map_size(ss, 2);
  mp::write_str(ss, "hello");
  mp::write_uint(ss, 5);
  mp::write_str(ss, "Hello");
  mp::write_uint(ss, 10);
  mp::write_str(ss, "00101010101000101");  // length 17
  mp::write_map_size(ss, 3);
  mp::write_str(ss, "woRld");
  mp::write_uint(ss, 1);
  mp::write_str(ss, "WORLD");
  mp::write_uint(ss, 12);
  mp::write_str(ss, "world");
  mp::write_uint(ss, 100);
  ss.seekg(0);

  brown_clusters.load(ss);
  CHECK_EQUAL(false, brown_clusters.empty());
  CHECK_EQUAL(4, brown_clusters.size());
  CHECK_EQUAL(6, sp.size());

  canonical0 = sp.get("00100010");
  canonical1 = sp.get("00101010101000101");
  CHECK_EQUAL(6, sp.size());

  npaths = brown_clusters.get_paths("meow", &path, &lengths[0]);
  CHECK_EQUAL(0, npaths);
  CHECK_EQUAL(6, sp.size());

  npaths = brown_clusters.get_paths("hello", &path, &lengths[0]);
  CHECK_EQUAL(3, npaths);
  CHECK_EQUAL(6, sp.size());
  CHECK_EQUAL(reinterpret_cast<const void *>(canonical0), reinterpret_cast<const void *>(path));
  CHECK_EQUAL(4, lengths[0]);
  CHECK_EQUAL(6, lengths[1]);
  CHECK_EQUAL(8, lengths[2]);

  npaths = brown_clusters.get_paths("Hello", &path, &lengths[0]);
  CHECK_EQUAL(3, npaths);
  CHECK_EQUAL(6, sp.size());
  CHECK_EQUAL(reinterpret_cast<const void *>(canonical0), reinterpret_cast<const void *>(path));
  CHECK_EQUAL(4, lengths[0]);
  CHECK_EQUAL(6, lengths[1]);
  CHECK_EQUAL(8, lengths[2]);

  npaths = brown_clusters.get_paths("woRld", &path, &lengths[0]);
  CHECK_EQUAL(0, npaths);
  CHECK_EQUAL(6, sp.size());

  npaths = brown_clusters.get_paths("world", &path, &lengths[0]);
  CHECK_EQUAL(4, npaths);
  CHECK_EQUAL(6, sp.size());
  CHECK_EQUAL(reinterpret_cast<const void *>(canonical1), reinterpret_cast<const void *>(path));
  CHECK_EQUAL( 4, lengths[0]);
  CHECK_EQUAL( 6, lengths[1]);
  CHECK_EQUAL(10, lengths[2]);
  CHECK_EQUAL(17, lengths[3]);
}


TEST(brown_clusters_double_load) {
  Pool pool(4 * 1024 * 1024);
  StringPool sp(pool);
  BrownClusters brown_clusters(sp);

  std::stringstream ss;
  mp::write_map_size(ss, 1);
  mp::write_str(ss, "00100010");
  mp::write_map_size(ss, 1);
  mp::write_str(ss, "hello");
  mp::write_uint(ss, 5);
  ss.seekg(0);

  brown_clusters.load(ss);
  CHECK_EQUAL(false, brown_clusters.empty());
  CHECK_EQUAL(1, brown_clusters.size());
  CHECK_EQUAL(2, sp.size());

  ss.seekg(0);
  CHECK_THROW(brown_clusters.load(ss), ValueException);
}

}  // SUITE

}  // namespace lex
}  // namespace schwa
