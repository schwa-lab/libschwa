#include <schwa/docrep.h>

namespace schwa {
namespace docrep {

size_t
Collection::add_doc(Document *const doc) {
  _docs.push_back(doc);
  return _docs.size();
}


bool
Collection::remove_doc(const Document *const doc) {
  for (std::vector<Document *>::iterator it(_docs.begin()); it != _docs.end(); ++it)
    if (*it == doc) {
      _docs.erase(it);
      return true;
    }
  return false;
}


std::ostream &
Collection::serialize(std::ostream &out) const {
  msgpack::packer<std::ostream> packer(out);
  std::cerr << "Collection::serialize begin" << std::endl;
  packer.pack_array(_docs.size());
  for (std::vector<Document *>::const_iterator it(_docs.begin()); it != _docs.end(); ++it)
    (*it)->serialize(out);
  std::cerr << "Collection::serialize end" << std::endl;
  return out;
}

}
}
