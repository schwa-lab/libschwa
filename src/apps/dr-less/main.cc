/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <string>
#include <vector>

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
main(const std::vector<std::string> &input_paths, io::OutputStream &out, const cf::Op<uint32_t> &limit) {
  // Make the output stream invoke ${PAGER} if it's a stdout tty.
  port::make_stdout_pager(out);

  // Construct the processor functor to produce the pretty output.
  Processor processor(out);

  dr::FauxDoc doc;
  dr::FauxDoc::Schema schema;
  for (const std::string &input_path : input_paths) {
    // Construct a docrep reader over the provided input stream.
    io::InputStream in(input_path);
    dr::Reader reader(in, schema);

    // Read the documents off the input stream.
    for (uint32_t i = 0; reader >> doc; ++i) {
      if (limit.was_assigned() && i == limit())
        break;
      processor(doc, i);
    }
  }
}

}  // namespace dr_less
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-less", "A visualiser for docrep streams.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::Op<uint32_t> limit(cfg, "limit", 'n', "Limit on how many documents to process", cf::Flags::OPTIONAL);

  cfg.allow_unclaimed_args("[input-path...]");

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Work out which input paths to read from.
    std::vector<std::string> input_paths;
    if (input_path.was_mentioned() || cfg.unclaimed_args().empty())
      input_paths.push_back(input_path());
    else
      input_paths = cfg.unclaimed_args();

    // Open the ouptut stream.
    io::OutputStream out(output_path());

    // Dispatch to main function.
    schwa::dr_less::main(input_paths, out, limit);
  })
  return 0;
}
