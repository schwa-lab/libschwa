/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TOKENIZER_DEBUG_TEXT_STREAM_H_
#define SCHWA_TOKENIZER_DEBUG_TEXT_STREAM_H_

#include <array>
#include <iosfwd>

#include <schwa/_base.h>
#include <schwa/tokenizer/stream.h>


namespace schwa {
  namespace tokenizer {

    class DebugTextStream : public Stream {
    public:
      static const std::array<const char *, 7> COLOURS;
      static const char *OFF;

    protected:
      std::ostream &_out;
      bool _new_sentence;
      size_t _colour;

    public:
      DebugTextStream(std::ostream &out);
      virtual ~DebugTextStream(void) { }

      virtual void add(Type, const char *raw, size_t begin, size_t len, const char *norm=nullptr) override;
      virtual void error(const char *raw, size_t begin, size_t len) override;

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
      SCHWA_DISALLOW_COPY_AND_ASSIGN(DebugTextStream);
    };

  }
}

#endif  // SCHWA_TOKENIZER_DEBUG_TEXT_STREAM_H_
