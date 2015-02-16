/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/lex/brown-clusters.h>

#include <algorithm>
#include <cstring>

#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/msgpack.h>

namespace mp = ::schwa::msgpack;


namespace schwa {
namespace lex {

const std::array<unsigned int, 4> BrownClusters::DEFAULT_PATH_LENGTHS = {{4, 6, 10, 20}};

BrownClusters::BrownClusters(StringPool &string_pool, const unsigned int min_freq) :
    _string_pool(string_pool),
    _min_freq(min_freq),
    _path_lengths(DEFAULT_PATH_LENGTHS.begin(), DEFAULT_PATH_LENGTHS.end())
  { }

BrownClusters::~BrownClusters(void) { }


void
BrownClusters::load(std::istream &in) {
  if (SCHWA_UNLIKELY(!_map.empty()))
    throw ValueException("BrownClusters instance is already initialised");

  // Read how many path lengths are in the map.
  const uint32_t npaths = mp::read_map_size(in);

  // Read in each of the paths.
  for (uint32_t i = 0; i != npaths; ++i) {
    // Read in the path and convert it to a canonical string.
    const std::string path = mp::read_str(in);
    const uint8_t *const canonical_path = _string_pool.get(path);

    // Read in how many tokens have this path.
    const uint32_t ntokens = mp::read_map_size(in);

    // Read in each of the token/count pairs.
    for (uint32_t j = 0; j != ntokens; ++j) {
      const std::string token = mp::read_str(in);
      const unsigned int freq = mp::read_uint(in);

      // Keep the token if it occurs at least min_freq times.
      if (freq >= _min_freq) {
        const uint8_t *canonical_token = _string_pool.get(token);
        _map.emplace(canonical_token, canonical_path);
      }
    }
  }

  LOG(INFO) << "BrownClusters::load loaded " << _map.size() << " token/path pairs with threshold >= " << _min_freq << std::endl;
}


void
BrownClusters::set_min_freq(const unsigned int min_freq) {
  if (SCHWA_UNLIKELY(!_map.empty()))
    throw ValueException("Minimum frequency cannot be set as this BrownClusters instance has already been initialised");
  _min_freq = min_freq;
}


size_t
BrownClusters::get_paths(const std::string &word, const uint8_t **const path, unsigned int *const lengths) const {
  // Lookup the word in the map and return zero paths if it's not found.
  const auto &it = _map.find(reinterpret_cast<const uint8_t *>(word.c_str()));
  if (it == _map.end())
    return 0;

  // Set the path pointer to the canonical path string.
  *path = it->second;
  const unsigned int path_length = std::strlen(reinterpret_cast<const char *>(it->second));

  // Work out what path lengths this path can utilise.
  size_t npaths;
  for (npaths = 0; npaths != _path_lengths.size(); ++npaths) {
    if (npaths != 0 && _path_lengths[npaths - 1] >= path_length)
      break;
    lengths[npaths] = std::min(_path_lengths[npaths], path_length);
  }
  return npaths;
}

}  // namespace lex
}  // namespace schwa
