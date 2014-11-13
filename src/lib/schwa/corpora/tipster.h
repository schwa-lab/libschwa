/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CORPORA_TIPSTER_H_
#define SCHWA_CORPORA_TIPSTER_H_

#include <string>
#include <utility>
#include <vector>

#include <schwa/_base.h>
#include <schwa/corpora/importer.h>
#include <schwa/utils/buffer.h>
#include <schwa/utils/ragel.h>


namespace schwa {
  namespace corpora {

    class TipsterImporter : public Importer {
    private:
      class Impl;
      Impl *_impl;

    public:
      explicit TipsterImporter(const std::string &path);
      virtual ~TipsterImporter(void);

      virtual canonical_schema::Doc *import(void) override;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(TipsterImporter);
    };


    class TipsterTextLexer {
    private:
      RagelState<BaseOffsetBuffer::iterator> _state;
      size_t _par_start_index;
      std::vector<std::pair<size_t, size_t>> _paragraphs;

      void _create_paragraph(void);
      void _create_paragraph(size_t start_index, size_t end_index);
      bool _lex(void);

    public:
      TipsterTextLexer(void);

      inline bool at_eof(void) const { return _state.at_eof(); }
      inline BaseOffsetBuffer::iterator get_p(void) const { return _state.p; }
      inline BaseOffsetBuffer::iterator get_pe(void) const { return _state.pe; }

      inline const std::vector<std::pair<size_t, size_t>> &paragraph_indexes(void) const { return _paragraphs; }
      inline const RagelState<BaseOffsetBuffer::iterator> &state(void) const { return _state; }

      void lex(const BaseOffsetBuffer &buffer);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(TipsterTextLexer);
    };

  }  // namespace corpora
}  // namespace schwa

#endif  // SCHWA_CORPORA_TIPSTER_H_
