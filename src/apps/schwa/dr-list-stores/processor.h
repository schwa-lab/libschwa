/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRLISTSTORES_PROCESSOR_H_
#define SCHWA_DRLISTSTORES_PROCESSOR_H_

#include <iosfwd>
#include <vector>

#include <schwa/_base.h>
#include <schwa/dr.h>


namespace schwa {
  namespace dr_list_stores {

    class Processor {
    private:
      class Impl;

      Impl *_impl;

    public:
      Processor(std::ostream &out, bool per_doc);
      ~Processor(void);

      void finalise(void);
      void process_doc(const dr::FauxDoc &doc);

      inline void operator ()(const dr::FauxDoc &doc) { process_doc(doc); }

    private:
      DISALLOW_COPY_AND_ASSIGN(Processor);
    };

  }
}

#endif  // SCHWA_DRLISTSTORES_PROCESSOR_H_
