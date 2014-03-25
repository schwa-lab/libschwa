/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <dr-count/processor.h>

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>

#include <schwa/dr.h>

namespace dr = schwa::dr;


namespace schwa {
namespace dr_count {

// ============================================================================
// Processor::Impl
// ============================================================================
class Processor::Impl {
public:
  static const size_t MIN_WIDTH;
  static const size_t NDOCS_WIDTH;

private:
  std::ostream &_out;
  const bool _all_stores;
  const bool _count_bytes;
  const bool _cumulative;
  const bool _per_doc;
  const Formatting _formatting;

  uint32_t _ndocs;
  std::unordered_map<std::string, uint64_t> _counts;
  std::unordered_map<std::string, uint64_t> _local_counts;
  std::map<std::string, size_t> _widths;

public:
  Impl(std::ostream &out, bool all_stores, bool count_bytes, bool cumulative, bool per_doc, Formatting formatting) :
      _out(out),
      _all_stores(all_stores),
      _count_bytes(count_bytes),
      _cumulative(cumulative),
      _per_doc(per_doc),
      _formatting(formatting),
      _ndocs(0)
    { }

  void finalise(void);
  void process_doc(const dr::Doc &doc);
};

const size_t Processor::Impl::MIN_WIDTH = 10;
const size_t Processor::Impl::NDOCS_WIDTH = 10;


void
Processor::Impl::process_doc(const dr::Doc &doc) {
  const dr::RTManager &rt = *(doc.rt());
  const dr::RTSchema &schema = *(rt.doc);

  // Increment the number of docs processed.
  ++_ndocs;

  // If we are outputting counts per store and we are the first document to be processed, initialise that state.
  if (_all_stores && _ndocs == 1) {
    // Calculate the widths per column.
    for (auto &store : schema.stores)
      _widths[store->serial] = std::max(MIN_WIDTH, store->serial.size());

    // Output the column headings.
    if (_formatting == Formatting::ALIGNED) {
      _out << std::setw(NDOCS_WIDTH) << "ndocs";
      for (auto &pair : _widths)
        _out << ' ' << std::setw(pair.second) << pair.first;
      _out << std::endl;
    }
    else {
      _out << "ndocs";
      for (auto &pair : _widths)
        _out << '\t' << pair.first;
      _out << std::endl;
    }
  }

  // Output the document count if per_doc.
  if (_per_doc && _all_stores) {
    const uint32_t count = _cumulative ? _ndocs : 1;
    if (_formatting == Formatting::ALIGNED)
      _out << std::setw(NDOCS_WIDTH) << count;
    else
      _out << count;
  }

  // For each store, add its counts to the stream counts.
  _local_counts.clear();
  for (auto &store : schema.stores) {
    uint64_t count = _count_bytes ? store->lazy_nbytes : store->lazy_nelem;
    _counts[store->serial] += count;
    _local_counts[store->serial] = count;
  }

  // Output the counts per store.
  if (_per_doc && _all_stores) {
    for (auto &pair : _widths) {
      const uint64_t count = _cumulative ? _counts[pair.first] : _local_counts[pair.first];
      if (_formatting == Formatting::ALIGNED)
        _out << ' ' << std::setw(pair.second) << count;
      else
        _out << '\t' << count;
    }
    _out << std::endl;
  }
}


void
Processor::Impl::finalise(void) {
  // Output the document count.
  if (_all_stores) {
    if (_formatting == Formatting::ALIGNED)
      _out << std::setw(NDOCS_WIDTH) << _ndocs;
    else
      _out << _ndocs;
  }
  else
    _out << _ndocs;

  // Output the counts per store.
  if (_all_stores) {
    for (auto &pair : _widths) {
      const uint64_t count = _counts[pair.first];
      if (_formatting == Formatting::ALIGNED)
        _out << ' ' << std::setw(pair.second) << count;
      else
        _out << '\t' << count;
    }
  }
  _out << std::endl;
}


// ============================================================================
// Processor
// ============================================================================
Processor::Processor(std::ostream &out, bool all_stores, bool count_bytes, bool cumulative, bool per_doc, Formatting formatting) : _impl(new Processor::Impl(out, all_stores, count_bytes, cumulative, per_doc, formatting)) { }

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

}  // namespace dr_count
}  // namespace schwa
