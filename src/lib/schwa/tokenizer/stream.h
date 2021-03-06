/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TOKENIZER_STREAM_H_
#define SCHWA_TOKENIZER_STREAM_H_

#include <schwa/_base.h>
#include <schwa/tokenizer/common.h>


namespace schwa {
  namespace tokenizer {

    class Stream {
    protected:
      Stream(void) { }

    public:
      virtual ~Stream(void) { }

      virtual void add(Type type, const char *raw, size_t begin, size_t len, const char *norm=nullptr) = 0;
      virtual void error(const char *raw, size_t begin, size_t len) = 0;

      virtual void begin_sentence(void) { }
      virtual void end_sentence(void) { }

      virtual void begin_list(void) { }
      virtual void end_list(void) { }

      virtual void begin_item(void) { }
      virtual void end_item(void) { }

      virtual void begin_paragraph(void) { }
      virtual void end_paragraph(void) { end_sentence(); }

      virtual void begin_heading(int) { end_paragraph(); }
      virtual void end_heading(int) { end_sentence(); }

      virtual void begin_document(void) { }
      virtual void end_document(void) { end_paragraph(); }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Stream);
    };

  }
}

#endif  // SCHWA_TOKENIZER_STREAM_H_
