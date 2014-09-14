/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cstring>
#include <iomanip>
#include <set>
#include <type_traits>

#include <schwa/hashtable.h>
#include <schwa/unittest.h>


namespace schwa {

SUITE(schwa__hashtable) {

TEST(fasthashtable_test) {
  struct Entry : public FeatureHashtableEntryBase {
    uint32_t last_iteration;
    float current;
    float total;

    Entry(void) : FeatureHashtableEntryBase(), last_iteration(0), current(0), total(0) { }

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(Entry);
  };

  const FeatureType FT_ZERO(0);
  const FeatureType FT_ONE(1);
  const Label L_ZERO = 0;
  const Label L_ONE = 1;
  const Label L_TWO = 2;

  FeatureHashtable<Entry> table;
  CHECK(table.empty());
  CHECK_EQUAL(0, table.size());

  Entry &e0_0 = table.get(FT_ZERO, "meow", L_ZERO);
  CHECK(!table.empty());
  CHECK_EQUAL(1, table.size());

  Entry &e0_1 = table.get(FT_ZERO, "meow", L_ONE);
  CHECK(!table.empty());
  CHECK_EQUAL(2, table.size());

  Entry &e1_0 = table.get(FT_ONE, "meow", L_ZERO);
  CHECK(!table.empty());
  CHECK_EQUAL(3, table.size());

  Entry &e1_1 = table.get(FT_ONE, "meow", L_ONE);
  CHECK(!table.empty());
  CHECK_EQUAL(4, table.size());

  CHECK(&e0_0 != &e0_1);
  CHECK(&e0_0 != &e1_0);
  CHECK(&e0_0 != &e1_1);
  CHECK(&e0_1 != &e1_0);
  CHECK(&e0_1 != &e1_1);
  CHECK(&e1_0 != &e1_1);

  Entry &e1 = table.get(FT_ZERO, std::string("meow"), L_ZERO);
  CHECK_EQUAL(4, table.size());
  CHECK_EQUAL(&e0_0, &e1);

  Entry &e2 = table.get(FT_ZERO, std::string("woof"), L_ZERO);
  CHECK(!table.empty());
  CHECK_EQUAL(5, table.size());
  CHECK(&e0_0 != &e2);
  CHECK(&e0_1 != &e2);
  CHECK(&e1_0 != &e2);
  CHECK(&e1_1 != &e2);
  CHECK(&e1 != &e2);

  CHECK(0 < table.load_factor());
  CHECK(1 > table.load_factor());

  auto it = table.begin();
  CHECK_EQUAL(it, it);
  CHECK_EQUAL(table.begin(), table.begin());
  CHECK_EQUAL(table.end(), table.end());
  CHECK_EQUAL(table.cbegin(), table.cbegin());
  CHECK_EQUAL(table.cend(), table.cend());

  const std::set<Entry *> entries = {&e0_0, &e0_1, &e1_0, &e1_1, &e2};
  for (size_t i = 0; i != entries.size(); ++i) {
    CHECK(it != table.end());
    CHECK(entries.count(&(*it)) == 1);
    ++it;
  }
  CHECK_EQUAL(it, table.end());

  CHECK(table.find(FT_ZERO, "meow", L_ZERO) != table.end());
  CHECK(table.find(FT_ZERO, "woof", L_ZERO) != table.end());
  CHECK(table.find(FT_ZERO, "squeek", L_ZERO) == table.end());
  CHECK(table.find(FT_ZERO, "meow", L_TWO) == table.end());

  const decltype(table) &t = table;
  CHECK(t.find(FT_ZERO, "meow", L_ZERO) != t.end());
  CHECK(t.find(FT_ZERO, "woof", L_ZERO) != t.end());
  CHECK(t.find(FT_ZERO, "squeek", L_ZERO) == t.end());
  CHECK(t.find(FT_ZERO, "meow", L_TWO) == t.end());
}

}  // SUITE

}  // namespace schwa
