/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/port.h>

#include "processor.h"

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;
namespace port = schwa::port;


namespace schwa {
namespace dr_less {

static void
main(std::istream &input, std::ostream &output, const cf::Op<uint32_t> &limit) {
  // Make the output stream invoke ${PAGER} if it's a stdout tty.
  port::make_stdout_pager(output);

  // Construct a docrep reader over the provided input stream.
  dr::FauxDoc doc;
  dr::FauxDoc::Schema schema;
  dr::Reader reader(input, schema);

  // Construct the processor functor to produce the pretty output.
  Processor processor(output);

  // Read the documents off the input stream.
  for (uint32_t i = 0; reader >> doc; ++i) {
    if (limit.was_assigned() && i == limit())
      break;
    processor(doc, i);
  }
}

}  // namespace dr_less
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-less", "A visualiser for docrep streams.");
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<uint32_t> limit(cfg, "limit", 'n', "Limit on how many documents to process", cf::Flags::OPTIONAL);

  input.set_positional_precedence(0);

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Dispatch to main function.
    schwa::dr_less::main(input.file(), output.file(), limit);
  })
  return 0;
}
