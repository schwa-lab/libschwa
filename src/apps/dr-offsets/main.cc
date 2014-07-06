/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <sstream>

#include <schwa/config.h>
#include <schwa/dr/reader.h>
#include <schwa/io/logging.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace schwa {
namespace dr_offsets {

static void
main(io::InputStream &in, io::OutputStream &out, const bool include_bytes) {
  // Read the documents off the input stream.
  size_t offset = 0;
  std::stringstream tmp;
  while (true) {
    // Read the next document into temp storage.
    if (!dr::read_lazy_doc(in, tmp))
      break;
    const size_t nbytes = tmp.tellp();
    tmp.str("");

    // Output the offset and number of bytes.
    *out << offset;
    if (include_bytes)
      *out << " " << nbytes;
    *out << std::endl;

    // Update the offset.
    offset += nbytes;
  }
}

}  // namespace dr_offsets
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-offsets", "Outputs the byte offsets of each of the documents in a given docrep stream.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::Op<bool> nbytes(cfg, "nbytes", 'n', "Output the number of bytes of each document as well as its byte offset.", false);

  input_path.set_positional_precedence(0);

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Open the input and output streams.
    io::InputStream in(input_path());
    io::OutputStream out(output_path());

    // Dispatch to main function.
    schwa::dr_offsets::main(in, out, nbytes());
  })
  return 0;
}
