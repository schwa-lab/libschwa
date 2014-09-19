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
    private:
      const std::string COLOUR_BOLD;
      const std::string COLOUR_DARK_GREY;
      const std::string COLOUR_OFF;
      const std::string COLOUR_RED;
      const std::string REPR_NIL;
      const std::string REPR_UNKNOWN;
      const std::string SEP;
      const bool _annotations;

      unsigned int _indent;
      std::ostream *_out;

      std::ostream &_write_indent(void);
      void _write_value(const msgpack::Value &value, bool add_description=true);

    public:
      Processor(bool annotations, bool colour);
      ~Processor(void);

      void process(std::istream &in, std::ostream &out);
      inline void operator ()(std::istream &in, std::ostream &out) { process(in, out); }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Processor);
    };

  }
}

#endif  // SCHWA_MPLESS_PROCESSOR_H_
