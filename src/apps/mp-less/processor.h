/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MPLESS_PROCESSOR_H_
#define SCHWA_MPLESS_PROCESSOR_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>


namespace schwa {
  namespace msgpack {
    class Value;
  }

  namespace mp_less {

    class Processor {
    public:
      static const std::string SEP;
      static const std::string REPR_NIL;
      static const std::string REPR_UNKNOWN;

    private:
      unsigned int _indent;
      std::ostream *_out;

      std::ostream &_write_indent(void);
      void _write_value(const msgpack::Value &value, bool add_description=true);

    public:
      Processor(void);
      ~Processor(void);

      void process(std::istream &in, std::ostream &out);
      inline void operator ()(std::istream &in, std::ostream &out) { process(in, out); }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Processor);
    };

  }
}

#endif  // SCHWA_MPLESS_PROCESSOR_H_
