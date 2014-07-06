/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRCOUNT_PROCESSOR_H_
#define SCHWA_DRCOUNT_PROCESSOR_H_

#include <iosfwd>

#include <schwa/_base.h>


namespace schwa {
  namespace dr {
    class Doc;
  }

  namespace dr_count {

    class Processor {
    public:
      enum class Formatting {
        ALIGNED, TABS,
      };

    private:
      class Impl;

      Impl *_impl;

    public:
      Processor(std::ostream &out, bool all_stores, const std::string &store, bool count_bytes, bool cumulative, bool per_doc, Formatting formatting, const std::string &doc_id);
      ~Processor(void);

      void finalise(void);
      void process_doc(const dr::Doc &doc, const std::string &path);

      inline void operator ()(const dr::Doc &doc, const std::string &path) { process_doc(doc, path); }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Processor);
    };

  }
}

#endif  // SCHWA_DRCOUNT_PROCESSOR_H_
