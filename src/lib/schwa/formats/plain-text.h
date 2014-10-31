/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_PLAIN_TEXT_H_
#define SCHWA_FORMATS_PLAIN_TEXT_H_

#include <utility>
#include <vector>

#include <schwa/_base.h>
#include <schwa/encoding.h>
#include <schwa/utils/ragel.h>


namespace schwa {
  namespace formats {

    class PlainTextLexer {
    private:
      RagelState<BaseOffsetBuffer::iterator> _state;
      size_t _par_start_index;
      std::vector<std::pair<size_t, size_t>> _paragraphs;

      void _create_paragraph(void);
      void _create_paragraph(size_t start_index, size_t end_index);
      bool _lex(void);

    public:
      PlainTextLexer(void);

      inline bool at_eof(void) const { return _state.at_eof(); }
      inline OffsetBuffer<>::iterator get_p(void) const { return _state.p; }
      inline OffsetBuffer<>::iterator get_pe(void) const { return _state.pe; }

      inline const std::vector<std::pair<size_t, size_t>> &paragraph_indexes(void) const { return _paragraphs; }
      inline const RagelState<OffsetBuffer<>::iterator> &state(void) const { return _state; }

      void lex(const BaseOffsetBuffer &buffer);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(PlainTextLexer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_PLAIN_TEXT_H_

