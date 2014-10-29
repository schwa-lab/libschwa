/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_PLAIN_TEXT_H_
#define SCHWA_FORMATS_PLAIN_TEXT_H_

#include <utility>
#include <vector>

#include <schwa/_base.h>
#include <schwa/encoding.h>


namespace schwa {
  namespace formats {

    class PlainTextLexer {
    public:
      class RagelState {
      public:
        const uint8_t *p;
        const uint8_t *pe;
        const uint8_t *eof;
        const uint8_t *ts;
        const uint8_t *te;
        int act;
        int cs;

        RagelState(const uint8_t *const data, const size_t nbytes) :
            p(data),
            pe(data + nbytes),
            eof(pe),
            ts(nullptr),
            te(nullptr),
            act(0),
            cs(0)
          { }

        inline bool at_eof(void) const { return p == pe; }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(RagelState);
      };

    private:
      const EncodingResult &_encoding_result;
      RagelState _state;
      std::vector<std::pair<const uint8_t *, const uint8_t *>> _paragraphs;

      void _init(void);
      void _create_paragraph(void);

    public:
      const PlainTextLexer(const EncodingResult &er);

      inline bool at_eof(void) const { return _state.at_eof(); }
      inline const uint8_t *get_p(void) const { return _state.p; }
      inline const uint8_t *get_pe(void) const { return _state.pe; }
      inline const RagelState &get_state(void) const { return _state; }

      bool lex(void);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(PlainTextLexer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_PLAIN_TEXT_H_

