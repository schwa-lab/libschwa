/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cstring>
#include <iomanip>
#include <type_traits>

#include <schwa/hashtable.h>
#include <schwa/unittest.h>


namespace schwa {

SUITE(schwa__hashtable) {

TEST(fasthashtable_test) {
  struct Entry {
    uint32_t hash;
    uint32_t last_iteration;
    float current;
    float total;

    Entry(void) : hash(0), last_iteration(0), current(0), total(0) { }

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(Entry);
  };

  FeatureHashtable<std::string, Entry> table;
  CHECK(table.empty());
  CHECK_EQUAL(0, table.size());

  Entry &e0 = table["meow"];
  CHECK(!table.empty());
  CHECK_EQUAL(1, table.size());

  Entry &e1 = table["meow"];
  CHECK(!table.empty());
  CHECK_EQUAL(1, table.size());
  CHECK_EQUAL(&e0, &e1);

  Entry &e2 = table["woof"];
  CHECK(!table.empty());
  CHECK_EQUAL(2, table.size());
  CHECK(&e0 != &e2);
  CHECK(&e1 != &e2);

  auto it = table.begin();
  CHECK_EQUAL(it, it);
  CHECK_EQUAL(table.begin(), table.begin());
  CHECK_EQUAL(table.end(), table.end());
  CHECK_EQUAL(table.cbegin(), table.cbegin());
  CHECK_EQUAL(table.cend(), table.cend());

  CHECK(it != table.end());
  CHECK(&(*it) == &e0 || &(*it) == &e2);
  ++it;
  CHECK(it != table.end());
  CHECK(&(*it) == &e0 || &(*it) == &e2);
  ++it;
  CHECK_EQUAL(it, table.end());

  CHECK(table.find("meow") != table.end());
  CHECK(table.find("woof") != table.end());
  CHECK(table.find("squeek") == table.end());

  const decltype(table) &t = table;
  CHECK(t.find("meow") != t.end());
  CHECK(t.find("woof") != t.end());
  CHECK(t.find("squeek") == t.end());
}

}  // SUITE

}  // namespace schwa
