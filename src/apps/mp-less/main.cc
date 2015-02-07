/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <string>
#include <vector>

#include <schwa/config.h>
#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/port.h>

#include "processor.h"

namespace cf = schwa::config;
namespace io = schwa::io;
namespace port = schwa::port;


namespace schwa {
namespace mp_less {

static void
main(const std::vector<std::string> &input_paths, io::OutputStream &out, const bool annotations, const bool colour) {
  // Make the output stream invoke ${PAGER} if it's a stdout tty.
  port::make_stdout_pager(out);

  // Construct the processor functor to produce the pretty output.
  Processor processor(annotations, colour);

  // Open each of the input paths and process them in turn.
  for (const std::string &input_path : input_paths) {
    io::InputStream in(input_path);
    processor(in, out);
  }
}

}  // namespace mp_less
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("mp-less", "A visualiser for MessagePack payloads.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::Op<bool> annotations(cfg, "annotations", 'a', "Add MessagePack type annotations", true);
  cf::Op<bool> colour(cfg, "colour", 'c', "Colourful output", true);
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
    schwa::mp_less::main(input_paths, out, annotations(), colour());
  })
  return 0;
}
