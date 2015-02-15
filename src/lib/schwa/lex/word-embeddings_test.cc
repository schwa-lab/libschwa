/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <sstream>

#include <schwa/lex/word-embeddings.h>
#include <schwa/msgpack.h>
#include <schwa/unicode.h>

namespace mp = ::schwa::msgpack;


namespace schwa {
namespace lex {

SUITE(schwa__lex__word_embeddings) {

TEST(word_embeddings_with_unknown) {
  Pool pool(4 * 1024 * 1024);
  StringPool sp(pool);
  WordEmbeddings word_embeddings(sp);

  CHECK_EQUAL(true, word_embeddings.empty());
  CHECK_EQUAL(0, word_embeddings.size());
  CHECK_EQUAL(0, word_embeddings.ndimensions());

  std::stringstream ss;
  mp::write_map_size(ss, 4);
  mp::write_str(ss, "hello");
  mp::write_array_size(ss, 3);
  mp::write_float(ss, 1.23f);
  mp::write_float(ss, 4.56f);
  mp::write_float(ss, 7.89f);
  mp::write_str(ss, "Hello");
  mp::write_array_size(ss, 3);
  mp::write_float(ss, -1.23f);
  mp::write_float(ss, -4.56f);
  mp::write_float(ss, -7.89f);
  mp::write_str(ss, "world");
  mp::write_array_size(ss, 3);
  mp::write_float(ss, 0.12f);
  mp::write_float(ss, 0.23f);
  mp::write_float(ss, 0.34f);
  mp::write_str(ss, "*UNKNOWN*");
  mp::write_array_size(ss, 3);
  mp::write_float(ss, 100.0f);
  mp::write_float(ss, -88.88f);
  mp::write_float(ss, 77.77f);
  ss.seekg(0);

  word_embeddings.load(ss);
  CHECK_EQUAL(false, word_embeddings.empty());
  CHECK_EQUAL(4, word_embeddings.size());
  CHECK_EQUAL(3, word_embeddings.ndimensions());
  CHECK_EQUAL(4, sp.size());

  const float *f;
  f = word_embeddings.get_embeddings("hello");
  CHECK(f != nullptr);
  CHECK_CLOSE(0.0123f, f[0], 1e-5);
  CHECK_CLOSE(0.0456f, f[1], 1e-5);
  CHECK_CLOSE(0.0789f, f[2], 1e-5);

  f = word_embeddings.get_embeddings("Hello");
  CHECK(f != nullptr);
  CHECK_CLOSE(-0.0123f, f[0], 1e-5);
  CHECK_CLOSE(-0.0456f, f[1], 1e-5);
  CHECK_CLOSE(-0.0789f, f[2], 1e-5);

  f = word_embeddings.get_embeddings("world");
  CHECK(f != nullptr);
  CHECK_CLOSE(0.0012f, f[0], 1e-5);
  CHECK_CLOSE(0.0023f, f[1], 1e-5);
  CHECK_CLOSE(0.0034f, f[2], 1e-5);

  f = word_embeddings.get_embeddings("meow?");
  CHECK(f != nullptr);
  CHECK_CLOSE(1.0f, f[0], 1e-5);
  CHECK_CLOSE(-0.8888f, f[1], 1e-5);
  CHECK_CLOSE(0.7777f, f[2], 1e-5);
}


TEST(word_embeddings_without_unknown) {
  Pool pool(4 * 1024 * 1024);
  StringPool sp(pool);
  WordEmbeddings word_embeddings(sp);

  CHECK_EQUAL(true, word_embeddings.empty());
  CHECK_EQUAL(0, word_embeddings.size());
  CHECK_EQUAL(0, word_embeddings.ndimensions());

  std::stringstream ss;
  mp::write_map_size(ss, 1);
  mp::write_str(ss, "hello");
  mp::write_array_size(ss, 3);
  mp::write_float(ss, 2.0f);
  mp::write_float(ss, 4.0f);
  mp::write_float(ss, 6.0f);
  ss.seekg(0);

  word_embeddings.load(ss);
  CHECK_EQUAL(false, word_embeddings.empty());
  CHECK_EQUAL(1, word_embeddings.size());
  CHECK_EQUAL(3, word_embeddings.ndimensions());
  CHECK_EQUAL(1, sp.size());

  const float *f;
  f = word_embeddings.get_embeddings("hello");
  CHECK(f != nullptr);
  CHECK_CLOSE(2.0f / 6.0f, f[0], 1e-5);
  CHECK_CLOSE(4.0f / 6.0f, f[1], 1e-5);
  CHECK_CLOSE(6.0f / 6.0f, f[2], 1e-5);

  f = word_embeddings.get_embeddings("Hello");
  CHECK(f == nullptr);

  f = word_embeddings.get_embeddings("world");
  CHECK(f == nullptr);

  f = word_embeddings.get_embeddings("meow?");
  CHECK(f == nullptr);
}


TEST(word_embeddings_double_load) {
  Pool pool(4 * 1024 * 1024);
  StringPool sp(pool);
  WordEmbeddings word_embeddings(sp);

  std::stringstream ss;
  mp::write_map_size(ss, 1);
  mp::write_str(ss, "hello");
  mp::write_array_size(ss, 3);
  mp::write_float(ss, 1.23f);
  mp::write_float(ss, 4.56f);
  mp::write_float(ss, 7.89f);
  ss.seekg(0);

  word_embeddings.load(ss);
  CHECK_EQUAL(false, word_embeddings.empty());
  CHECK_EQUAL(1, word_embeddings.size());
  CHECK_EQUAL(3, word_embeddings.ndimensions());
  CHECK_EQUAL(1, sp.size());

  ss.seekg(0);
  CHECK_THROW(word_embeddings.load(ss), ValueException);
}


TEST(word_embeddings_not_same_dimensionality) {
  Pool pool(4 * 1024 * 1024);
  StringPool sp(pool);
  WordEmbeddings word_embeddings(sp);

  std::stringstream ss;
  mp::write_map_size(ss, 2);
  mp::write_str(ss, "hello");
  mp::write_array_size(ss, 3);
  mp::write_float(ss, 1.23f);
  mp::write_float(ss, 4.56f);
  mp::write_float(ss, 7.89f);
  mp::write_str(ss, "Hello");
  mp::write_array_size(ss, 2);
  mp::write_float(ss, 1.23f);
  mp::write_float(ss, 7.89f);
  ss.seekg(0);

  CHECK_THROW(word_embeddings.load(ss), ValueException);
}

}  // SUITE

}  // namespace lex
}  // namespace schwa
