/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRCOUNT_PROCESSOR_H_
#define SCHWA_DRCOUNT_PROCESSOR_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>
#include <schwa/dr/query.h>


namespace schwa {
  namespace dr {
    class Doc;
  }

  namespace dr_count {
    class Formatter;

    enum class Formatting {
      ALIGNED, TABS,
    };


    class Processor {
    private:
      std::ostream &_out;
      const bool _all_stores;
      const bool _count_bytes;
      const bool _cumulative;
      const bool _no_header;
      const bool _no_footer;
      const bool _no_ndocs;
      const int _every;
      const Formatting _formatting;
      const std::string _store;
      const std::string _doc_id;

      dr::query::Interpreter _interpreter;

      Formatter *_formatter;
      std::vector<uint64_t> _counts;
      std::vector<uint32_t> _local_counts;
      std::vector<bool> _output_stores;

      std::string _get_doc_id(const dr::Doc &doc) const;

    public:
      Processor(std::ostream &out, bool all_stores, const std::string &store, bool count_bytes, bool cumulative, int every, Formatting formatting, const std::string &doc_id, bool no_header, bool no_footer, bool no_ndocs);
      ~Processor(void);

      void finalise(void);
      void process_doc(const dr::Doc &doc, const std::string &path);
      void reset(void);

      inline bool output_doc_id(void) const { return _every == 1 && !_doc_id.empty(); }
      inline bool output_footer(void) const { return !_no_footer; }
      inline bool output_header(void) const { return !_no_header; }
      inline bool output_ndocs(void) const { return !_no_ndocs; }
      inline bool output_store(const size_t i) { return _output_stores[i]; }

      inline void operator ()(const dr::Doc &doc, const std::string &path) { process_doc(doc, path); }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Processor);
    };

  }
}

#endif  // SCHWA_DRCOUNT_PROCESSOR_H_
