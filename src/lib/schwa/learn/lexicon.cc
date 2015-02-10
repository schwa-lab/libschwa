/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/learn/lexicon.h>

#include <iostream>
#include <list>

#include <schwa/io/logging.h>
#include <schwa/msgpack.h>

namespace mp = ::schwa::msgpack;


namespace schwa {
namespace learn {

void
Lexicon::cull(const unsigned int min_freq) {
  // Build up a list of keys to delete.
  std::list<std::string> to_delete;
  for (const auto &it : _counts)
    if (it.second < min_freq)
      to_delete.push_back(it.first);

  // Delete the keys.
  const size_t size_before = size();
  for (const auto &key : to_delete)
    _counts.erase(key);
  const size_t size_after = size();

  LOG(INFO) << "Lexicon '" << _name << "' culled " << size_before << " down to " << size_after << " at frequency < " << min_freq << " (" << ((100.0 * (size_before - size_after)) / size_before) << "% reduction)." << std::endl;
}


void
Lexicon::deserialise(std::istream &in) {
  // Clear out any current data.
  _counts.clear();

  // Read in the size of the map and pre-allocate that amount of space.
  const uint32_t npairs = mp::read_map_size(in);
  _counts.reserve(npairs);

  // Read in each of the pairs and insert them into the underlying map.
  for (uint32_t i = 0; i != npairs; ++i) {
    std::string key = mp::read_str(in);
    unsigned int val = mp::read_uint(in);
    _counts.emplace(key, val);
  }
}


void
Lexicon::serialise(std::ostream &out) const {
  // Write out all of the items and their counts in a MessagePack map structure.
  mp::write_map_size(out, _counts.size());
  for (const auto &pair : _counts) {
    mp::write_str(out, pair.first);
    mp::write_uint(out, pair.second);
  }
}

}  // namespace learn
}  // namespace schwa
