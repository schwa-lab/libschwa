/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CORPORA_TIPSTER_H_
#define SCHWA_CORPORA_TIPSTER_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/canonical-schema.h>


namespace schwa {
  namespace corpora {

    class TipsterImporter {
    private:
      class Impl;
      Impl *_impl;

    public:
      explicit TipsterImporter(const std::string &path);
      ~TipsterImporter(void);

      canonical_schema::Doc *import(void) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(TipsterImporter);
    };

  }  // namespace corpora
}  // namespace schwa

#endif  // SCHWA_CORPORA_TIPSTER_H_
