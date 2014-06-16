/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/unittest.h>

#include <algorithm>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

#include <schwa/learn.h>
#include <schwa/unicode.h>


namespace schwa {
namespace learn {

SUITE(schwa__learn__extract) {

class X {
public:
  unsigned int value;

  X(void) : value(0) { }
  explicit X(unsigned int value) : value(value) { }
};


template <typename TRANSFORM>
static std::vector<std::string>
_get_crfsuite_vector(const Features<TRANSFORM> &f) {
  std::stringstream ss;
  f.dump_crfsuite(ss);
  ss.seekg(0);

  std::vector<std::string> units;
  std::string tmp;
  for (const char c : ss.str()) {
    if (c == '\t') {
      if (!tmp.empty()) {
        units.push_back(tmp);
        tmp.clear();
      }
    }
    else
      tmp.push_back(c);
  }
  if (!tmp.empty()) {
    units.push_back(tmp);
    tmp.clear();
  }

  std::sort(units.begin(), units.end());
  return units;
}


TEST(sentinel_offsets) {
  std::vector<X> xs(20);
  for (unsigned int i = 0; i != xs.size(); ++i)
    xs[i].value = i;

  const SentinelOffsets<X> o(&xs.front(), &xs.back() + 1, [](const X *const x) {
      std::stringstream ss;
      ss << x->value;
      return ss.str();
  });
  CHECK_EQUAL("0", o(0, 0));
  CHECK_EQUAL("1", o(0, 1));
  CHECK_EQUAL(SENTINEL, o(0, -1));

  CHECK_EQUAL("5", o(5, 0));
  CHECK_EQUAL("6", o(5, 1));
  CHECK_EQUAL("4", o(5, -1));

  CHECK_EQUAL("19", o(19, 0));
  CHECK_EQUAL(SENTINEL, o(19, 1));
  CHECK_EQUAL("18", o(19, -1));
}


TEST(window) {
  std::vector<X> xs(20);
  for (unsigned int i = 0; i != xs.size(); ++i)
    xs[i].value = i;

  const SentinelOffsets<X> o(&xs.front(), &xs.back() + 1, [](const X *const x) {
      std::stringstream ss;
      ss << x->value;
      return ss.str();
  });
  const contextual_callback<X> unigram = create_unigram_callback<X>();

  Features<> f;
  window("n", 0, -2, 3, o, f, unigram);

  const std::vector<std::string> units = _get_crfsuite_vector(f);
  CHECK_EQUAL(6, units.size());
  CHECK_EQUAL("n[i+1]=1:1", units[0]);
  CHECK_EQUAL("n[i+2]=2:1", units[1]);
  CHECK_EQUAL("n[i+3]=3:1", units[2]);
  CHECK_EQUAL("n[i-1]=__SENTINEL__:1", units[3]);
  CHECK_EQUAL("n[i-2]=__SENTINEL__:1", units[4]);
  CHECK_EQUAL("n[i]=0:1", units[5]);
}


TEST(word_form) {
  CHECK_EQUAL("Ll", word_form("hello"));
  CHECK_EQUAL("Lu", word_form("HELLO"));
  CHECK_EQUAL("LuLl", word_form("Hello"));
  CHECK_EQUAL("LlLuLlLuLl", word_form("hElLo"));
  CHECK_EQUAL("LuLlNd", word_form("Hell9"));
  CHECK_EQUAL("", word_form(""));

  CHECK_EQUAL("Ll", word_form(UnicodeString(U"hello")));
  CHECK_EQUAL("Lu", word_form(UnicodeString(U"HELLO")));
  CHECK_EQUAL("LuLl", word_form(UnicodeString(U"Hello")));
  CHECK_EQUAL("LlLuLlLuLl", word_form(UnicodeString(U"hElLo")));
  CHECK_EQUAL("LuLlNd", word_form(UnicodeString(U"Hell9")));
  CHECK_EQUAL("", word_form(UnicodeString(U"")));
}


TEST(add_affix_features) {
  Features<> f;
  std::vector<std::string> units;

  f.clear();
  add_affix_features(f, 3, 2, u8"Argentina");
  units = _get_crfsuite_vector(f);
  CHECK_EQUAL(5, units.size());
  CHECK_EQUAL("prefix=A:1", units[0]);
  CHECK_EQUAL("prefix=Ar:1", units[1]);
  CHECK_EQUAL("prefix=Arg:1", units[2]);
  CHECK_EQUAL("suffix=a:1", units[3]);
  CHECK_EQUAL("suffix=an:1", units[4]);

  f.clear();
  add_affix_features(f, 3, 2, u8"a");
  units = _get_crfsuite_vector(f);
  CHECK_EQUAL(2, units.size());
  CHECK_EQUAL("prefix=a:1", units[0]);
  CHECK_EQUAL("suffix=a:1", units[1]);

  f.clear();
  add_affix_features(f, 4, 4, u8"κόσμε");
  units = _get_crfsuite_vector(f);
  CHECK_EQUAL(8, units.size());
  CHECK_EQUAL("prefix=κ:1", units[0]);
  CHECK_EQUAL("prefix=κό:1", units[1]);
  CHECK_EQUAL("prefix=κόσ:1", units[2]);
  CHECK_EQUAL("prefix=κόσμ:1", units[3]);
  CHECK_EQUAL("suffix=ε:1", units[4]);
  CHECK_EQUAL("suffix=εμ:1", units[5]);
  CHECK_EQUAL("suffix=εμσ:1", units[6]);
  CHECK_EQUAL("suffix=εμσό:1", units[7]);
}

}  // SUITE

}  // namespace learn
}  // namespace schwa
