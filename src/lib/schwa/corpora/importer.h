/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CORPORA_IMPORTER_H_
#define SCHWA_CORPORA_IMPORTER_H_

#include <schwa/_base.h>
#include <schwa/canonical-schema.h>


namespace schwa {
  namespace corpora {

    class Importer {
    public:
      Importer(void);
      virtual ~Importer(void);

      virtual canonical_schema::Doc *import(void) = 0;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Importer);
    };

  }  // namespace corpora
}  // namespace schwa

#endif  // SCHWA_CORPORA_IMPORTER_H_
