/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <algorithm>
#include <functional>
#include <map>
#include <set>
#include <sstream>
#include <string>

#include <schwa/io/logging.h>
#include <schwa/learn.h>


namespace schwa {
namespace learn {

SUITE(schwa__learn__features) {

struct Entry {
  uint32_t last_iteration;
  uint32_t current;
  uint32_t total;

  Entry(void) : last_iteration(0), current(0), total(0) {
    LOG(INFO) << "[Entry::Entry|" << this << "]" << std::endl;
  }
  ~Entry(void) {
    LOG(INFO) << "[Entry::~Entry|" << this << "]" << std::endl;
  }

  inline void
  deserialise(std::istream &in) {
    const uint32_t nitems = msgpack::read_array_size(in);
    assert(nitems == 3); (void)nitems;
    last_iteration = msgpack::read_uint(in);
    current = msgpack::read_uint(in);
    total = msgpack::read_uint(in);
  }

  inline void
  serialise(std::ostream &out) const {
    msgpack::write_array_size(out, 3);
    msgpack::write_uint(out, last_iteration);
    msgpack::write_uint(out, current);
    msgpack::write_uint(out, total);
  }

  friend std::ostream &operator <<(std::ostream &out, const Entry &entry);
};

inline std::ostream &
operator <<(std::ostream &out, const Entry &e) {
  return out << "Entry(this=" << &e << " last_iteration=" << e.last_iteration << " current=" << e.current << " total=" << e.total << ")";
}


TEST(featurehashtable_test) {
  const FeatureType FT_ZERO(0);
  const FeatureType FT_ONE(1);
  const Label L_ZERO = 0;
  const Label L_ONE = 1;
  const Label L_TWO = 2;

  FeatureHashtable<Entry> table(3);
  CHECK(table.empty());
  CHECK_EQUAL(0, table.size());
  table.pprint(std::cout);

  Entry &e0_0 = table.get(FT_ZERO, "meow", L_ZERO);
  CHECK(!table.empty());
  CHECK_EQUAL(1, table.size());
  table.pprint(std::cout);

  Entry &e0_1 = table.get(FT_ZERO, "meow", L_ONE);
  CHECK(!table.empty());
  CHECK_EQUAL(2, table.size());
  table.pprint(std::cout);
  table.get(FT_ZERO, "meow", L_ONE);
  CHECK(!table.empty());
  CHECK_EQUAL(2, table.size());
  table.pprint(std::cout);

  Entry &e1_2 = table.get(FT_ONE, "meow", L_TWO);
  CHECK(!table.empty());
  CHECK_EQUAL(3, table.size());
  table.pprint(std::cout);

  Entry &e1_1 = table.get(FT_ONE, "meow", L_ONE);
  CHECK(!table.empty());
  CHECK_EQUAL(4, table.size());
  table.pprint(std::cout);

  CHECK(&e0_0 != &e0_1);
  CHECK(&e0_0 != &e1_2);
  CHECK(&e0_0 != &e1_1);
  CHECK(&e0_1 != &e1_2);
  CHECK(&e0_1 != &e1_1);
  CHECK(&e1_2 != &e1_1);

  Entry &e1 = table.get(FT_ZERO, std::string("meow"), L_ZERO);
  CHECK_EQUAL(4, table.size());
  CHECK_EQUAL(&e0_0, &e1);
  table.pprint(std::cout);

  Entry &e2 = table.get(FT_ZERO, std::string("woof"), L_ZERO);
  CHECK(!table.empty());
  CHECK_EQUAL(5, table.size());
  table.pprint(std::cout);

  CHECK(&e0_0 != &e2);
  CHECK(&e0_1 != &e2);
  CHECK(&e1_2 != &e2);
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
  CHECK_EQUAL(5, table.size());
  table.pprint(std::cout);

  const std::set<Entry *> entries = {&e0_0, &e0_1, &e1_2, &e1_1, &e2};
  std::set<Entry *> entries_seen;
  CHECK_EQUAL(table.size(), entries.size());
  for (size_t i = 0; i != entries.size(); ++i) {
    CHECK(it != table.end());
    std::cout << "(" << i << ") " << it << " " << *it << std::endl;
    Entry *const ptr = &(*it);
    entries_seen.insert(ptr);
    CHECK(entries.count(ptr) == 1);
    ++it;
  }
  std::cout << "(end) " << it << std::endl;
  CHECK_EQUAL(table.end(), it);
  CHECK_EQUAL(table.size(), entries.size());

  CHECK(table.find(FT_ZERO, "meow", L_ZERO) != table.end());
  CHECK(table.find(FT_ZERO, "woof", L_ZERO) != table.end());
  CHECK(table.find(FT_ZERO, "squeek", L_ZERO) == table.end());
  CHECK(table.find(FT_ZERO, "meow", L_TWO) == table.end());

  const decltype(table) &t = table;
  CHECK(t.find(FT_ZERO, "meow", L_ZERO) != t.end());
  CHECK(t.find(FT_ZERO, "woof", L_ZERO) != t.end());
  CHECK(t.find(FT_ZERO, "squeek", L_ZERO) == t.end());
  CHECK(t.find(FT_ZERO, "meow", L_TWO) == t.end());

  e0_0.last_iteration = 1;
  e0_1.last_iteration = 2;
  e1_2.last_iteration = 3;
  e1_1.last_iteration = 4;
  e2.last_iteration = 5;

  std::map<Label, uint32_t> seen;
  const auto &visitor = [&seen](const Label l, const Entry &e) {
    seen[l] = e.last_iteration;
  };

  seen.clear();
  table.for_each_label(FT_ZERO, "meow", L_ZERO, L_TWO + 1, visitor);
  CHECK_EQUAL(2, seen.size());
  CHECK(seen.find(L_ZERO) != seen.end());
  CHECK_EQUAL(1, seen[L_ZERO]);
  CHECK(seen.find(L_ONE) != seen.end());
  CHECK_EQUAL(2, seen[L_ONE]);
  CHECK(seen.find(L_TWO) == seen.end());

  seen.clear();
  table.for_each_label(FT_ONE, "meow", L_ZERO, L_TWO + 1, visitor);
  CHECK_EQUAL(2, seen.size());
  CHECK(seen.find(L_ZERO) == seen.end());
  CHECK(seen.find(L_ONE) != seen.end());
  CHECK_EQUAL(4, seen[L_ONE]);
  CHECK(seen.find(L_TWO) != seen.end());
  CHECK_EQUAL(3, seen[L_TWO]);

  seen.clear();
  table.for_each_label(FT_ONE, "chicken", L_ZERO, L_TWO + 1, visitor);
  CHECK_EQUAL(0, seen.size());

  {
    std::ostringstream out;
    table.serialise(out);
    std::istringstream in(out.str());
    FeatureHashtable<Entry> table2(3);
    table2.deserialise(in);
    CHECK_EQUAL(table.size(), table2.size());
  }
}


template <typename F>
static void
populate_features(F &f) {
  f("a");
  f("b");
  f("c");
  f("d", 3.2);
}


TEST(no_transform) {
  Features<> f;
  populate_features(f);

  std::stringstream ss;
  f.dump_crfsuite(ss);
  ss.seekg(0);
  const std::string s = ss.str();
  CHECK_EQUAL(4, std::count(s.begin(), s.end(), '\t'));
}


TEST(hasher_transform_default_hash) {
  Features<HasherTransform<>> f(HasherTransform<>(4));
  populate_features(f);

  std::stringstream ss;
  f.dump_crfsuite(ss);
  ss.seekg(0);
  const std::string s = ss.str();

  std::hash<std::string> h;
  std::set<std::hash<std::string>::result_type> unique;
  unique.insert(h("a") % (1 << 4));
  unique.insert(h("b") % (1 << 4));
  unique.insert(h("c") % (1 << 4));
  unique.insert(h("d") % (1 << 4));
  CHECK_EQUAL(unique.size(), std::count(s.begin(), s.end(), '\t'));
}


TEST(hasher_transform_custom_hash) {
  struct MyHash {
    using result_type = size_t;
    using argument_type = std::string;

    inline result_type
    operator ()(const argument_type &) const {
      return 42;
    }
  };

  Features<HasherTransform<MyHash>> f(HasherTransform<MyHash>(4));
  populate_features(f);

  std::stringstream ss;
  f.dump_crfsuite(ss);
  ss.seekg(0);
  CHECK_EQUAL("\t10:6.2", ss.str());
}

}  // SUITE

}  // namespace learn
}  // namespace schwa
