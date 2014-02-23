/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr-list-stores/processor.h>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#include <schwa/dr.h>

namespace dr = schwa::dr;


namespace schwa {
namespace dr_list_stores {

// ============================================================================
// Processor::Impl
// ============================================================================
class Processor::Impl {
private:
  std::ostream &_out;
  const bool _per_doc;

  std::unordered_map<std::string, unsigned int> _counts;

public:
  Impl(std::ostream &out, const bool per_doc) : _out(out), _per_doc(per_doc) { }

  void finalise(void);
  void process_doc(const dr::Doc &doc);
};


void
Processor::Impl::process_doc(const dr::Doc &doc) {
  const dr::RTManager &rt = *(doc.rt());
  const dr::RTSchema &schema = *(rt.doc);

  // Add counts to the stream counter.
  for (auto &it : schema.stores) {
    const dr::RTStoreDef &store = *it;
    _counts[store.serial] += 1;
  }

  // Output the sorted store names for each doc, if required.
  if (_per_doc) {
    // Sort the names of the stores on the current doc.
    std::vector<std::string> names;
    for (auto &store : schema.stores)
      names.push_back(store->serial);
    std::sort(names.begin(), names.end());

    // Output, tab separated.
    for (decltype(names)::size_type i = 0; i != names.size(); ++i) {
      if (i != 0)
        _out << '\t';
      _out << names[i];
    }
    _out << std::endl;
  }
}


void
Processor::Impl::finalise(void) {
  // Sort the names of the found stores.
  std::vector<std::string> names;
  for (auto &it : _counts)
    names.push_back(it.first);
  std::sort(names.begin(), names.end());

  // Output, tab separated.
  if (_per_doc)
    _out << "=== total ===" << std::endl;
  for (auto &name : names)
    _out << name << '\t' << _counts[name] << std::endl;
}


// ============================================================================
// Processor
// ============================================================================
Processor::Processor(std::ostream &out, const bool per_doc) : _impl(new Processor::Impl(out, per_doc)) { }

Processor::~Processor(void) {
  delete _impl;
}

void
Processor::process_doc(const dr::Doc &doc) {
  _impl->process_doc(doc);
}

void
Processor::finalise(void) {
  _impl->finalise();
}

}  // namespace dr_list_stores
}  // namespace schwa
