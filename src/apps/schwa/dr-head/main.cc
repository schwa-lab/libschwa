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
main(std::istream &input, std::ostream &output, const unsigned int count, const unsigned int skip) {
  // Read the documents off the input stream.
  unsigned int nread = 0;
  for (unsigned int i = 0; ; ++i) {
    std::stringstream tmp;
    if (nread == count)
      break;
    else if (!dr::read_lazy_doc(input, tmp))
      break;
    else if (i < skip)
      continue;

    tmp.seekp(0);
    output << tmp.rdbuf();
    ++nread;
  }
}

}


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-head", "A `head` tool for docrep streams.");
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<unsigned int> count(cfg, "count", 'n', "How many documents to keep", 10);
  cf::Op<unsigned int> skip(cfg, "skip", 's', "How many documents to skip before counting", 0);

  // Parse argv.
  cfg.main<io::PrettyLogger>(argc, argv);

  // Dispatch to main function.
  try {
    main(input.file(), output.file(), count(), skip());
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return 0;
}
