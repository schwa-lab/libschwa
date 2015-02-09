/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/learn/lexicon.h>

#include <list>

#include <schwa/io/logging.h>


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

}  // namespace learn
}  // namespace schwa
