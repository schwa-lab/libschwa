/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "processor.h"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>

#include <schwa/dr.h>
#include <schwa/dr/query.h>

namespace dr = schwa::dr;
namespace dq = schwa::dr::query;


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
  const std::string _store;
  const std::string _doc_id;
  size_t _doc_id_width;

  uint32_t _ndocs;
  std::unordered_map<std::string, uint64_t> _counts;
  std::unordered_map<std::string, uint32_t> _local_counts;
  std::map<std::string, size_t> _widths;

  dq::Interpreter _interpreter;

  std::string _get_doc_id(const dr::Doc &doc) const;
  static std::string _int_to_str(const dq::Value &v);

public:
  Impl(std::ostream &out, bool all_stores, const std::string &store, bool count_bytes, bool cumulative, bool per_doc, Formatting formatting, const std::string &doc_id) :
      _out(out),
      _all_stores(all_stores),
      _count_bytes(count_bytes),
      _cumulative(cumulative),
      _per_doc(per_doc),
      _formatting(formatting),
      _store(store),
      _doc_id(doc_id),
      _ndocs(0)
    {
    if (!_doc_id.empty())
      _interpreter.compile(_doc_id);
  }

  void finalise(void);
  void process_doc(const dr::Doc &doc, const std::string &path);
};

const size_t Processor::Impl::MIN_WIDTH = 10;
const size_t Processor::Impl::NDOCS_WIDTH = 10;


std::string
Processor::Impl::_int_to_str(const dq::Value &v) {
  std::ostringstream ss;
  ss << v.via._int;
  return ss.str();
}


std::string
Processor::Impl::_get_doc_id(const dr::Doc &doc) const {
  const dq::Value v = _interpreter(doc, _ndocs - 1);
  switch (v.type) {
  case dq::TYPE_STRING: return v.via._str;
  case dq::TYPE_INTEGER: return _int_to_str(v);
  default: return std::string("<") + dq::valuetype_name(v.type) + ">";
  }
}


void
Processor::Impl::process_doc(const dr::Doc &doc, const std::string &) {
  const dr::RTManager &rt = *(doc.rt());
  const dr::RTSchema &schema = *(rt.doc);

  // Initialise state.
  if (_ndocs == 0) {
    if (!_doc_id.empty())
      _doc_id_width = std::max(MIN_WIDTH, _get_doc_id(doc).size());

    if (_all_stores || !_store.empty()) {
      // Calculate the widths per column.
      for (auto &store : schema.stores) {
        bool keep = true;
        if (!_store.empty())
          keep = store->serial == _store;
        if (keep)
          _widths[store->serial] = std::max(MIN_WIDTH, store->serial.size());
      }
    }
  }

  // Output the column headings.
  if (_ndocs == 0 && !_widths.empty()) {
    if (_formatting == Formatting::ALIGNED) {
      if (!_doc_id.empty())
        _out << std::setw(_doc_id_width) << "doc-id ";
      _out << std::setw(NDOCS_WIDTH) << "ndocs";
      for (auto &pair : _widths)
        _out << ' ' << std::setw(pair.second) << pair.first;
      _out << std::endl;
    }
    else {
      if (!_doc_id.empty())
        _out << "doc-id\t";
      _out << "ndocs";
      for (auto &pair : _widths)
        _out << '\t' << pair.first;
      _out << std::endl;
    }
  }

  // Increment the number of docs processed.
  ++_ndocs;

  // Output the doc_id if per doc.
  if (_per_doc && !_doc_id.empty()) {
    const std::string doc_id = _get_doc_id(doc);
    if (_formatting == Formatting::ALIGNED)
      _out << std::setw(_doc_id_width) << doc_id << ' ';
    else
      _out << doc_id << '\t';
  }

  // Output the document count if per_doc.
  if (_per_doc && !_widths.empty()) {
    const uint32_t count = _cumulative ? _ndocs : 1;
    if (_formatting == Formatting::ALIGNED)
      _out << std::setw(NDOCS_WIDTH) << count;
    else
      _out << count;
  }

  // For each store, add its counts to the stream counts.
  _local_counts.clear();
  for (auto &store : schema.stores) {
    const uint32_t count = _count_bytes ? store->lazy_nbytes : store->lazy_nelem;
    _counts[store->serial] += count;
    _local_counts[store->serial] = count;
  }

  // Output the counts per store.
  if (_per_doc && !_widths.empty()) {
    for (auto &pair : _widths) {
      const uint64_t count = _cumulative ? _counts[pair.first] : _local_counts[pair.first];
      if (_formatting == Formatting::ALIGNED)
        _out << ' ' << std::setw(pair.second) << count;
      else
        _out << '\t' << count;
    }
  }
  if (_per_doc)
    _out << std::endl;
}


void
Processor::Impl::finalise(void) {
  // Output the doc_id.
  if (!_doc_id.empty()) {
    if (_formatting == Formatting::ALIGNED)
      _out << std::setw(_doc_id_width) << "" << ' ';
    else
      _out << '\t';
  }

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
Processor::Processor(std::ostream &out, bool all_stores, const std::string &store, bool count_bytes, bool cumulative, bool per_doc, Formatting formatting, const std::string &doc_id) : _impl(new Processor::Impl(out, all_stores, store, count_bytes, cumulative, per_doc, formatting, doc_id)) { }

Processor::~Processor(void) {
  delete _impl;
}

void
Processor::process_doc(const dr::Doc &doc, const std::string &path) {
  _impl->process_doc(doc, path);
}

void
Processor::finalise(void) {
  _impl->finalise();
}

}  // namespace dr_count
}  // namespace schwa
