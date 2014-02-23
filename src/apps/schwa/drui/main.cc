/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/drui/drui.h>
#include <schwa/exception.h>
#include <schwa/io/logging.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace schwa {
namespace drui {

static void
main(std::istream &input, std::ostream &output, cf::Op<unsigned int> &limit) {
  // Construct a docrep reader over the provided input stream.
  FauxDoc doc;
  FauxDoc::Schema schema;
  FauxDocProcessor processor(output);
  dr::Reader reader(input, schema);

  // Read the documents off the input stream.
  for (unsigned int i = 0; reader >> doc; ++i) {
    if (limit.was_assigned() && i == limit())
      break;
    processor(doc, i);
  }
}

}
}


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-ui", "A visualiser for docrep streams");
  cf::OpIStream input(cfg, "input", "The input file");
  cf::OpOStream output(cfg, "output", "The output file");
  cf::Op<unsigned int> limit(cfg, "limit", "Limit on how many documents to process", cf::Flags::OPTIONAL);

  // Parse argv.
  cfg.main<io::PrettyLogger>(argc, argv);

  // Dispatch to main function.
  try {
    schwa::drui::main(input.file(), output.file(), limit);
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return 0;
}
