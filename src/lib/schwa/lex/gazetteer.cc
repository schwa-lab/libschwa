/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/lex/gazetteer.h>

#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/msgpack.h>

namespace mp = ::schwa::msgpack;


namespace schwa {
namespace lex {

Gazetteer::Gazetteer(StringPool &string_pool) : _string_pool(string_pool), _longest_ngram(0) { }

Gazetteer::~Gazetteer(void) {
  for (auto &map : _maps)
    delete map;
}


void
Gazetteer::load(std::istream &in) {
  // Allocate a new map for this gazetter.
  map_type *const map = new map_type();
  _maps.push_back(map);

  // Read how many pairs are in the map.
  const uint32_t npairs = mp::read_map_size(in);
  unsigned int nloaded = 0;

  // Read in each of the pairs.
  for (uint32_t i = 0; i != npairs; ++i) {
    // Read in the first token key.
    const std::string token0 = mp::read_str(in);
    const uint8_t *const canonical_token0 = _string_pool.get(token0);

    // Insert the item into the map.
    std::vector<std::vector<const uint8_t *>> &ngrams = (*map)[canonical_token0];

    // Read in how many n-grams start with this token.
    const uint32_t nngrams = mp::read_array_size(in);
    ngrams.resize(nngrams);

    // Read in each of the n-grams.
    for (uint32_t j = 0; j != nngrams; ++j, ++nloaded) {
      // Read in how many tokens the current n-gram is.
      const uint32_t ngram_length = mp::read_array_size(in);
      ngrams[j].resize(ngram_length);
      if (ngram_length > _longest_ngram)
        _longest_ngram = ngram_length;

      // Read in each of the tokens of the n-gram.
      for (uint32_t k = 0; k != ngram_length; ++k) {
        const std::string token = mp::read_str(in);
        const uint8_t *canonical_token = _string_pool.get(token);
        ngrams[j][k] = canonical_token;
      }
    }
  }

  LOG(INFO) << "Gazetteer::load loaded " << nloaded << " n-grams, longest is " << _longest_ngram << " tokens" << std::endl;
}


const std::vector<std::vector<const uint8_t *>> *
Gazetteer::get_ngrams(const unsigned int ngazetteer, const std::string &token0) const {
  const auto &it = _maps[ngazetteer]->find(reinterpret_cast<const uint8_t *>(token0.c_str()));
  return (it == _maps[ngazetteer]->end()) ? nullptr : &it->second;
}

}  // namespace lex
}  // namespace schwa
