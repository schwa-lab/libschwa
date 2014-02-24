/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRCOUNT_PROCESSOR_H_
#define SCHWA_DRCOUNT_PROCESSOR_H_

#include <iosfwd>
#include <vector>

#include <schwa/_base.h>


namespace schwa {
  namespace dr {
    class Doc;
  }

  namespace dr_count {

    class Processor {
    private:
      class Impl;

      Impl *_impl;

    public:
      Processor(std::ostream &out, bool per_doc);
      ~Processor(void);

      void finalise(void);
      void process_doc(const dr::Doc &doc);

      inline void operator ()(const dr::Doc &doc) { process_doc(doc); }

    private:
      DISALLOW_COPY_AND_ASSIGN(Processor);
    };

  }
}

#endif  // SCHWA_DRCOUNT_PROCESSOR_H_
