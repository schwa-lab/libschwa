/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include "processor.h"

#include <algorithm>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

#include <schwa/dr.h>
#include <schwa/dr/query.h>

namespace dr = schwa::dr;
namespace dq = schwa::dr::query;


namespace schwa {
namespace dr_count {

template <typename T>
static inline std::string
to_str(const T &value) {
  std::ostringstream ss;
  ss << value;
  return ss.str();
}


// ============================================================================
// Formatter
// ============================================================================
class Formatter {
protected:
  std::ostream &_out;
  std::vector<std::string> _columns;
  std::vector<std::string> _values;

public:
  explicit Formatter(std::ostream &out) : _out(out) { }
  virtual ~Formatter(void) { }

  virtual void
  add_column(const std::string &column) {
    _columns.push_back(column);
  }

  virtual void
  reset(void) {
    _columns.clear();
    _values.clear();
  }

  virtual void
  write_header(void) {
    std::copy(_columns.cbegin(), _columns.cend(), std::back_insert_iterator<std::vector<std::string>>(_values));
    write_row();
    _values.clear();
  }

  virtual void write_row(void) = 0;

  template <typename T>
  inline void
  add_value(const T &value) {
    _values.push_back(to_str(value));
  }
};


// ============================================================================
// AlignedFormatter
// ============================================================================
class AlignedFormatter : public Formatter {
public:
  static const size_t MIN_WIDTH;

protected:
  std::vector<size_t> _widths;

public:
  explicit AlignedFormatter(std::ostream &out) : Formatter(out) { }
  virtual ~AlignedFormatter(void) { }

  virtual void
  add_column(const std::string &column) override {
    Formatter::add_column(column);
    _widths.push_back(std::max(MIN_WIDTH, column.size()));
  }

  virtual void
  reset(void) override {
    Formatter::reset();
    _widths.clear();
  }

  virtual void
  write_row(void) override {
    if (!_values.empty()) {
      for (size_t i = 0; i != _values.size(); ++i) {
        if (i != 0)
          _out << ' ';
        _out << std::setw(_widths[i]) << _values[i];
      }
      _out << std::endl;
    }
    _values.clear();
  }
};

const size_t AlignedFormatter::MIN_WIDTH = 10;


// ============================================================================
// TabbedFormatter
// ============================================================================
class TabbedFormatter : public Formatter {
public:
  explicit TabbedFormatter(std::ostream &out) : Formatter(out) { }
  virtual ~TabbedFormatter(void) { }

  virtual void
  write_row(void) override {
    if (!_values.empty()) {
      for (size_t i = 0; i != _values.size(); ++i) {
        if (i != 0)
          _out << '\t';
        _out << _values[i];
      }
      _out << std::endl;
    }
    _values.clear();
  }
};


// ============================================================================
// Processor
// ============================================================================
Processor::Processor(std::ostream &out, bool all_stores, const std::string &store, bool count_bytes, bool cumulative, int every, Formatting formatting, const std::string &doc_id, bool no_header, bool no_footer, bool no_ndocs) :
    _out(out),
    _all_stores(all_stores),
    _count_bytes(count_bytes),
    _cumulative(cumulative),
    _no_header(no_header || !store.empty()),
    _no_footer(no_footer),
    _no_ndocs(no_ndocs || !store.empty()),
    _every(every),
    _formatting(formatting),
    _store(store),
    _doc_id(doc_id),
    _ndocs(0),
    _local_ndocs(0)
  {
  // Construct the appropriate row formatter.
  if (_formatting == Formatting::ALIGNED)
    _formatter = new AlignedFormatter(_out);
  else
    _formatter = new TabbedFormatter(_out);

  // Compile the doc ID query.
  if (_every == 1 && !_doc_id.empty())
    _interpreter.compile(_doc_id);
}

Processor::~Processor(void) {
  delete _formatter;
}


std::string
Processor::_get_doc_id(const dr::Doc &doc) const {
  const dq::Value v = _interpreter(doc, _ndocs - 1);
  switch (v.type) {
  case dq::TYPE_STRING: return v.via._str;
  case dq::TYPE_INTEGER: return to_str(v.via._int);
  default: return std::string("<") + dq::valuetype_name(v.type) + ">";
  }
}


void
Processor::initialise(void) {
  // Maybe add the doc_id column.
  if (output_doc_id())
    _formatter->add_column("doc_id");
  // Maybe add the ndocs column.
  if (output_ndocs())
    _formatter->add_column("ndocs");
}


void
Processor::process_doc(const dr::Doc &doc, const std::string &) {
  const dr::RTManager &rt = *(doc.rt());
  const dr::RTSchema &schema = *(rt.doc);

  // If we have not processed any documents yet, initialise the column names.
  if (_ndocs == 0) {
    // Work out which of the stores we should be outputting.
    _output_stores.clear();
    for (auto &store : schema.stores) {
      bool keep = _all_stores;
      if (!_store.empty())
        keep = store->serial == _store;
      _output_stores.push_back(keep);
    }

    // Maybe add each store as a column.
    for (size_t i = 0; i != schema.stores.size(); ++i)
      if (output_store(i))
        _formatter->add_column(schema.stores[i]->serial);

    // Reset the counts.
    _counts.resize(schema.stores.size());
    _local_counts.resize(_counts.size());

    // Output the header row if not suppressed.
    if (output_header())
      _formatter->write_header();
  }

  // Increment the number of docs processed.
  ++_ndocs;
  ++_local_ndocs;

  // Update the counts for each store.
  for (size_t i = 0; i != schema.stores.size(); ++i) {
    const auto &store = *schema.stores[i];
    const uint32_t count = _count_bytes ? store.lazy_nbytes : store.lazy_nelem;
    _counts[i] += count;
    _local_counts[i] += count;
  }

  // If we want to output every N documents, and we're at a multiple of N.
  if (_every != -1 && _local_ndocs % _every == 0) {
    // Add the doc_id column value to the row.
    if (output_doc_id()) {
      const std::string doc_id = _get_doc_id(doc);
      _formatter->add_value(doc_id);
    }

    // Add the ndocs column value to the row.
    if (output_ndocs()) {
      const uint32_t count = _cumulative ? _ndocs : _local_ndocs;
      _formatter->add_value(count);
    }

    // Add each of the store counts to the row.
    for (size_t i = 0; i != schema.stores.size(); ++i) {
      if (output_store(i)) {
        const uint64_t count = _cumulative ? _counts[i] : _local_counts[i];
        _formatter->add_value(count);
      }
    }

    // Write out the row.
    _formatter->write_row();

    // Reset the local counts.
    _local_ndocs = 0;
    std::fill(_local_counts.begin(), _local_counts.end(), 0);
  }
}


void
Processor::finalise(void) {
  // Skip the footer row?
  if (!output_footer())
    return;

  // If we did not process any documents, output the header row if not suppressed.
  if (_ndocs == 0 && output_header())
    _formatter->write_header();

  // Add the doc_id column value to the row.
  if (output_doc_id())
    _formatter->add_value("");

  // Add the ndocs column value to the row.
  if (output_ndocs())
    _formatter->add_value(_ndocs);

  // Add each of the store counts to the row.
  for (size_t i = 0; i != _counts.size(); ++i)
    if (output_store(i))
      _formatter->add_value(_counts[i]);

  // Write out the row.
  _formatter->write_row();
}


void
Processor::reset(void) {
  _formatter->reset();
  _counts.clear();
  _local_counts.clear();
  _ndocs = 0;
  _local_ndocs = 0;
}

}  // namespace dr_count
}  // namespace schwa
