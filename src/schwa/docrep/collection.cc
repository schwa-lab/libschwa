#include <schwa/docrep.hpp>

namespace schwa {
namespace docrep {

size_t
Collection::add_doc(Document *const doc) {
  _docs.push_back(doc);
  return _docs.size();
}


bool
Collection::remove_doc(const Document *const doc) {
  for (auto it(_docs.begin()); it != _docs.end(); ++it)
    if (*it == doc) {
      _docs.erase(it);
      return true;
    }
  return false;
}

}
}
