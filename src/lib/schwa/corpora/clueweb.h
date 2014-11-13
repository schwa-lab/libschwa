/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CORPORA_CLUEWEB_H_
#define SCHWA_CORPORA_CLUEWEB_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/corpora/importer.h>


namespace schwa {
  namespace corpora {

    class ClueWebImporter : public Importer {
    private:
      class Impl;
      Impl *_impl;

    public:
      explicit ClueWebImporter(const std::string &path);
      virtual ~ClueWebImporter(void);

      virtual canonical_schema::Doc *import(void) override;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(ClueWebImporter);
    };

  }  // namespace corpora
}  // namespace schwa

#endif  // SCHWA_CORPORA_CLUEWEB_H_
