/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_IO_H_
#define SCHWA_LEARN_IO_H_

#include <vector>

#include <schwa/_base.h>
#include <schwa/dr.h>
#include <schwa/exception.h>
#include <schwa/io/streams.h>


namespace schwa {
  namespace learn {

    /**
     * A basic wrapper around a dr::Reader instance which can be reset. Upon resetting, documents
     * from the start of the stream and yielded again. The documents can either be stored in
     * memory after the first read (useful in the case of reading from a pipe), or the underlying
     * file can be seeked back to the beginning and be read from again.
     **/
    template <typename DOC>
    class ResettableDocrepReader {
    private:
      std::vector<DOC *> _docs;
      io::InputStream &_in;
      const typename DOC::Schema &_schema;
      dr::Reader *_reader;
      DOC *_doc;
      const bool _retain_docs;
      bool _seen_eof;
      size_t _docs_upto;

    public:
      ResettableDocrepReader(io::InputStream &in, const typename DOC::Schema &schema, bool retain_docs=false);
      ~ResettableDocrepReader(void);

      /**
       * Returns nullptr or a pointer to a DOC instance, depending on whether another document is
       * available to be returned.
       **/
      DOC *next(void);

      /**
       * Resets the reader so that yielded docs start from the beginning of the underlying stream
       * again.
       **/
      void reset(void);
    };

  }
}

#include <schwa/learn/io_impl.h>

#endif  // SCHWA_LEARN_IO_H_
