/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_IO_IMPL_H_
#define SCHWA_LEARN_IO_IMPL_H_

#include <vector>

#include <schwa/_base.h>
#include <schwa/dr.h>
#include <schwa/exception.h>
#include <schwa/io/streams.h>


namespace schwa {
  namespace learn {

    template <typename DOC>
    ResettableDocrepReader<DOC>::ResettableDocrepReader(io::InputStream &in, const typename DOC::Schema &schema, bool retain_docs) :
        _in(in),
        _schema(schema),
        _reader(nullptr),
        _doc(nullptr),
        _retain_docs(retain_docs),
        _seen_eof(false),
        _docs_upto(0)
      {
      // Ensure that we can seek the input stream if we're not retaining the documents.
      if (!_retain_docs && !_in.is_seekable())
        throw schwa::ValueException("The input stream needs to be seekable when retain_docs is set to false.");

      // Construct a docrep reader over the input stream.
      _reader = new dr::Reader(_in, _schema);
    }

    template <typename DOC>
    ResettableDocrepReader<DOC>::~ResettableDocrepReader(void) {
      delete _reader;
      delete _doc;
      for (DOC *doc : _docs)
        delete doc;
    }


    template <typename DOC>
    inline DOC *
    ResettableDocrepReader<DOC>::next(void) {
      if (_retain_docs) {
        // Have we read in all of the docs already?
        if (_seen_eof) {
          // Are we at the end of the in-memory docs?
          if (_docs_upto == _docs.size())
            return nullptr;
          return _docs[_docs_upto++];
        }
        else {
          // Are we at the end of the in-memory docs?
          if (_docs_upto < _docs.size())
            return _docs[_docs_upto++];

          // Read in the next document.
          _doc = new DOC();
          const bool success = *_reader >> *_doc;
          if (success) {
            _docs.push_back(_doc);
            ++_docs_upto;
          }
          else
            _seen_eof = true;

          return success ? _doc : nullptr;
        }
      }
      else {
        // Recreate the doc instance.
        delete _doc;
        _doc = new DOC();

        const bool success = *_reader >> *_doc;
        return success ? _doc : nullptr;
      }
    }


    template <typename DOC>
    inline void
    ResettableDocrepReader<DOC>::reset(void) {
      // If we're retaining the documents, reset our index into our list of stored documents.
      if (_retain_docs) {
        _docs_upto = 0;
        return;
      }

      // Seek the input stream back to the beginning.
      _in.seek_start();

      // Recreate a docrep reader over the input stream.
      delete _reader;
      _reader = new dr::Reader(_in, _schema);
    }

  }
}

#endif  // SCHWA_LEARN_IO_IMPL_H_
