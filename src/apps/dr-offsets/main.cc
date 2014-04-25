/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <sstream>

#include <schwa/config.h>
#include <schwa/dr/reader.h>
#include <schwa/io/logging.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace {

static void
main(std::istream &input, std::ostream &output, const bool include_bytes) {
  // Read the documents off the input stream.
  size_t offset = 0;
  std::stringstream tmp;
  while (true) {
    // Read the next document into temp storage.
    if (!dr::read_lazy_doc(input, tmp))
      break;
    const size_t nbytes = tmp.tellp();
    tmp.str("");

    // Output the offset and number of bytes.
    output << offset;
    if (include_bytes)
      output << " " << nbytes;
    output << std::endl;

    // Update the offset.
    offset += nbytes;
  }
}

}  // namespace


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-offsets", "Outputs the byte offsets of each of the documents in a given docrep stream.");
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<bool> nbytes(cfg, "nbytes", 'n', "Output the number of bytes of each document as well as its byte offset.", false);

  // Parse argv.
  input.position_arg_precedence(0);
  cfg.main<io::PrettyLogger>(argc, argv);

  // Dispatch to main function.
  try {
    main(input.file(), output.file(), nbytes());
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return 0;
}
