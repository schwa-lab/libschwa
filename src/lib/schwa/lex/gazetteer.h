/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEX_GAZETTEER_H_
#define SCHWA_LEX_GAZETTEER_H_

#include <string>
#include <unordered_map>
#include <vector>

#include <schwa/_base.h>
#include <schwa/utils/hash.h>
#include <schwa/utils/string-pool.h>


namespace schwa {
  namespace lex {

    /**
     * TODO
     * { token0 : [ [ token ] ] }
     **/
    class Gazetteer {
    private:
      StringPool &_string_pool;
      unsigned int _longest_ngram;
      std::unordered_map<const uint8_t *, std::vector<std::vector<const uint8_t *>>, uint8str_hash, uint8str_equal_to> _map;

    public:
      explicit Gazetteer(StringPool &string_pool);
      ~Gazetteer(void);

      void load(std::istream &in);

      const std::vector<std::vector<const uint8_t *>> *get_ngrams(const std::string &token0) const;

      inline unsigned int longest_ngram(void) const { return _longest_ngram; }
      inline StringPool &string_pool(void) { return _string_pool; }
      inline const StringPool &string_pool(void) const { return _string_pool; }
    };

  }
}

#endif  // SCHWA_LEX_GAZETTEER_H_
