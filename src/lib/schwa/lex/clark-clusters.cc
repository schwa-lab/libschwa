/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/lex/clark-clusters.h>

#include <algorithm>
#include <sstream>

#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/msgpack.h>

namespace mp = ::schwa::msgpack;


namespace schwa {
namespace lex {

ClarkClusters::ClarkClusters(StringPool &string_pool) : _string_pool(string_pool) { }

ClarkClusters::~ClarkClusters(void) { }


void
ClarkClusters::load(std::istream &in) {
  if (SCHWA_UNLIKELY(!_map.empty()))
    throw ValueException("ClarkClusters instance is already initialised");
  unsigned int max_cluster = 0;

  // Read how many tokens are in the map.
  const uint32_t ntokens = mp::read_map_size(in);

  // Read in each of the tokens.
  for (uint32_t i = 0; i != ntokens; ++i) {
    // Read in the token and convert it to a canonical string.
    const std::string token = mp::read_str(in);
    const uint8_t *const canonical_token = _string_pool.get(token);

    // Read in how many tokens have this path.
    const uint32_t nitems = mp::read_array_size(in);
    if (SCHWA_UNLIKELY(nitems != 2)) {
      std::ostringstream ss;
      ss << "Invalid msgpack structure: nitems should be 2, but is " << nitems << std::endl;
      throw ValueException(ss.str());
    }

    // Store the data.
    const unsigned int cluster = mp::read_uint(in);
    const double prob = mp::read_double(in);
    _map.emplace(canonical_token, std::make_tuple(cluster, prob));

    // Keep track of the largest cluster ID.
    if (cluster > max_cluster)
      max_cluster = cluster;
  }

  LOG(INFO) << "ClarkClusters::load loaded " << _map.size() << " tokens with maximum seen cluster ID " << max_cluster << std::endl;
}


bool
ClarkClusters::get_cluster(const std::string &word, unsigned int *cluster, double *prob) const {
  // Lookup the word in the map.
  const auto &it = _map.find(reinterpret_cast<const uint8_t *>(word.c_str()));
  if (it == _map.end())
    return false;

  // Set the cluster ID.
  if (cluster != nullptr)
    *cluster = std::get<0>(it->second);
  if (prob != nullptr)
    *prob = std::get<1>(it->second);
  return true;
}

}  // namespace lex
}  // namespace schwa
