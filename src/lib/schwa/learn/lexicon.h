/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_LEXICON_H_
#define SCHWA_LEARN_LEXICON_H_

#include <iosfwd>
#include <string>
#include <unordered_map>

#include <schwa/_base.h>
#include <schwa/unicode.h>


namespace schwa {
  namespace learn {

    class Lexicon {
    private:
      std::string _name;
      std::unordered_map<std::string, unsigned int> _counts;

    public:
      explicit Lexicon(const std::string &name) : _name(name) { }

      inline void add(const std::string &utf8) { _counts[utf8] += 1; }
      inline void add(const UnicodeString &word) { _counts[word.to_utf8()] += 1; }
      inline void clear(void) { _counts.clear(); }
      void cull(unsigned int min_freq);

      inline unsigned int
      count(const std::string &utf8) const {
        const auto &it = _counts.find(utf8);
        return it == _counts.end() ? 0 : it->second;
      }
      inline unsigned int
      count(const UnicodeString &word) const {
        return count(word.to_utf8());
      }

      inline bool
      contains(const std::string &utf8) const {
        return _counts.find(utf8) != _counts.end();
      }
      inline bool contains(const UnicodeString &word) const { return contains(word.to_utf8()); }

      inline bool empty(void) const { return _counts.empty(); }
      inline const std::string &name(void) const { return _name; }
      inline size_t size(void) const { return _counts.size(); }

      void deserialise(std::istream &in);
      void serialise(std::ostream &out) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Lexicon);
    };

  }
}

#endif  // SCHWA_LEARN_LEXICON_H_
