/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TOKENIZER_STREAMS_DEBUG_TEXT_H_
#define SCHWA_TOKENIZER_STREAMS_DEBUG_TEXT_H_

#include <iosfwd>

#include <schwa/_base.h>
#include <schwa/tokenizer/stream.h>

namespace schwa {
  namespace tokenizer {

    class DebugTextStream : public Stream {
    public:
      static const int NCOLOURS;
      static const char *const COLOURS[];
      static const char *const OFF;

    protected:
      std::ostream &_out;
      bool _new_sentence;
      unsigned int _colour;

    public:
      DebugTextStream(std::ostream &out);
      virtual ~DebugTextStream(void) { }

      virtual void add(Type, const char *raw, offset_type, offset_type len, const char *norm=0) override;
      virtual void error(const char *raw, offset_type, offset_type len) override;

      virtual void begin_sentence(void) override;
      virtual void end_sentence(void) override;

      virtual void begin_list(void) override;
      virtual void end_list(void) override;

      virtual void begin_item(void) override;
      virtual void end_item(void) override;

      virtual void begin_paragraph(void) override;
      virtual void end_paragraph(void) override;

      virtual void begin_heading(int depth) override;
      virtual void end_heading(int depth) override;

      virtual void begin_document(void) override;
      virtual void end_document(void) override;

    private:
      DISALLOW_COPY_AND_ASSIGN(DebugTextStream);
    };

  }
}

#endif  // SCHWA_TOKENIZER_STREAMS_DEBUG_TEXT_H_
