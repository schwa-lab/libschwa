/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TOKENIZER_STREAMS_TEXT_H_
#define SCHWA_TOKENIZER_STREAMS_TEXT_H_

#include <iosfwd>

#include <schwa/_base.h>
#include <schwa/tokenizer/stream.h>

namespace schwa {
  namespace tokenizer {

    class TextStream : public Stream {
    protected:
      std::ostream &_out;
      bool _normalise;
      bool _new_document;
      bool _new_sentence;

    public:
      TextStream(std::ostream &out, bool normalise=true);
      virtual ~TextStream(void) { }

      virtual void add(Type, const char *raw, offset_type, offset_type len, const char *norm=0) override;
      virtual void error(const char *, offset_type, offset_type) override;

      virtual void begin_sentence(void) override;
      virtual void end_sentence(void) override;

      virtual void begin_paragraph(void) override;

      virtual void end_paragraph(void) override;

      virtual void begin_heading(int) override;
      virtual void end_heading(int) override;

      virtual void begin_list(void) override;
      virtual void end_list(void) override;

      virtual void begin_item(void) override;
      virtual void end_item(void) override;

      virtual void begin_document(void) override;
      virtual void end_document(void) override;
    };

  }
}

#endif  // SCHWA_TOKENIZER_STREAMS_TEXT_H_
