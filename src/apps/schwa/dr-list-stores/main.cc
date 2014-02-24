/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <sstream>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/io/logging.h>

#include <schwa/dr-list-stores/processor.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;
namespace ls = schwa::dr_list_stores;

namespace {

static void
main(std::istream &input, std::ostream &output, const bool per_doc) {
  // Construct a docrep reader over the provided input stream.
  dr::FauxDoc doc;
  dr::FauxDoc::Schema schema;
  dr::Reader reader(input, schema);

  // Construct the document processor.
  ls::Processor processor(output, per_doc);

  // Read the documents off the input stream.
  while (reader >> doc)
    processor(doc);
  processor.finalise();
}

}


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-list-stores", "List the stores in a docrep stream.");
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<bool> per_doc(cfg, "per-doc", "Whether to output the stores per document or per stream", false);

  // Parse argv.
  cfg.main<io::PrettyLogger>(argc, argv);

  // Dispatch to main function.
  try {
    main(input.file(), output.file(), per_doc());
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return 0;
}
