/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UTILS_COUNTER_H__
#define SCHWA_UTILS_COUNTER_H__

#include <functional>
#include <string>
#include <tuple>
#include <unordered_map>

#include <schwa/_base.h>


namespace schwa {

  template <typename A>
  struct UnigramCounterHash {
    using argument_type = A;
    using result_type = size_t;

    std::hash<A> _a;

    inline result_type
    operator ()(const argument_type &arg) const {
      return _a(arg);
    }
  };


  template <typename A, typename B>
  struct BigramCounterHash {
    using argument_type = std::tuple<A, B>;
    using result_type = size_t;

    std::hash<A> _a;
    std::hash<B> _b;

    inline result_type
    operator ()(const argument_type &arg) const {
      result_type h = 23;
      h = 31*h + _a(std::get<0>(arg));
      h = 31*h + _b(std::get<1>(arg));
      return h;
    }
  };


  template <typename KEY, typename VALUE, class HASHER>
  class Counter {
  public:
    using key_type = KEY;
    using hasher_type = HASHER;
    using mapped_type = VALUE;

    using map_type = std::unordered_map<key_type, mapped_type, hasher_type>;
    using const_iterator = typename map_type::const_iterator;
    using iterator = typename map_type::iterator;
    using size_type = typename map_type::size_type;
    using value_type = typename map_type::value_type;

  private:
    map_type _counts;

  public:
    inline void clear(void) { _counts.clear(); }
    inline size_type size(void) const { return _counts.size(); }

    inline iterator begin(void) { return _counts.begin(); }
    inline iterator end(void) { return _counts.end(); }
    inline const_iterator begin(void) const { return _counts.begin(); }
    inline const_iterator end(void) const { return _counts.end(); }
    inline const_iterator cbegin(void) const { return _counts.begin(); }
    inline const_iterator cend(void) const { return _counts.end(); }

    inline mapped_type &operator [](const key_type &arg) { return _counts[arg]; }
    inline mapped_type
    operator [](const key_type &arg) const {
      const auto it = _counts.find(arg);
      return (it == _counts.end()) ? mapped_type() : it->second;
    }

    template <typename... T>
    mapped_type &
    operator ()(T&&... args) {
      return _counts[key_type(args...)];
    }

    template <typename... T>
    mapped_type
    operator ()(T&&... args) const {
      const auto it = _counts.find(key_type(args...));
      return (it == _counts.end()) ? mapped_type() : it->second;
    }
  };


  template <typename A, typename VALUE=unsigned int, class HASHER=UnigramCounterHash<A>>
  using UnigramCounter = Counter<A, VALUE, HASHER>;

  template <typename A, typename B, typename VALUE=unsigned int, class HASHER=BigramCounterHash<A, B>>
  using BigramCounter = Counter<std::tuple<A, B>, VALUE, HASHER>;
}

#endif  // SCHWA_UTILS_COUNTER_H__
