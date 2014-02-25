/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRUI_PROCESSOR_H_
#define SCHWA_DRUI_PROCESSOR_H_

#include <iosfwd>
#include <vector>

#include <schwa/_base.h>


namespace schwa {
  namespace dr {
    class Doc;
  }

  namespace dr_ui {

    class Processor {
    private:
      class Impl;

      Impl *_impl;

    public:
      Processor(std::ostream &out);
      ~Processor(void);

      void process_doc(const dr::Doc &doc, unsigned int doc_num);
      inline void operator ()(const dr::Doc &doc, unsigned int doc_num) { process_doc(doc, doc_num); }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Processor);
    };

  }
}

#endif  // SCHWA_DRUI_PROCESSOR_H_
