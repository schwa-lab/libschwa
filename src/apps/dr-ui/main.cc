/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/port.h>

#include <dr-ui/main.h>
#include <dr-ui/processor.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;
namespace port = schwa::port;


namespace {

static void
main(std::istream &input, std::ostream &output, const cf::Op<uint32_t> &limit) {
  // Construct a docrep reader over the provided input stream.
  dr::FauxDoc doc;
  dr::FauxDoc::Schema schema;
  dr::Reader reader(input, schema);

  // Construct the processor functor to produce the pretty output.
  schwa::dr_ui::Processor processor(output);

  // Read the documents off the input stream.
  for (uint32_t i = 0; reader >> doc; ++i) {
    if (limit.was_assigned() && i == limit())
      break;
    processor(doc, i);
  }
}

}  // namespace


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg(schwa::dr_ui::PROGRAM_NAME, schwa::dr_ui::PROGRAM_DESC);
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<uint32_t> limit(cfg, "limit", 'n', "Limit on how many documents to process", cf::Flags::OPTIONAL);

  // Parse argv.
  input.position_arg_precedence(0);
  cfg.main<io::PrettyLogger>(argc, argv);

  // Dispatch to main function.
  try {
    port::make_stdout_pager(output.file());
    main(input.file(), output.file(), limit);
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return 0;
}
