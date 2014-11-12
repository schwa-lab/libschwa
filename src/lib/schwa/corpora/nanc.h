/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CORPORA_NANC_H_
#define SCHWA_CORPORA_NANC_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/canonical-schema.h>


namespace schwa {
  namespace corpora {

    class NANCImporter {
    private:
      class Impl;
      Impl *_impl;

    public:
      explicit NANCImporter(const std::string &path);
      ~NANCImporter(void);

      canonical_schema::Doc *import(void) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(NANCImporter);
    };

  }  // namespace corpora
}  // namespace schwa

#endif  // SCHWA_CORPORA_NANC_H_
