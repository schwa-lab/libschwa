/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <sstream>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/io/logging.h>

#include "processor.h"

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;

using Formatting = schwa::dr_count::Processor::Formatting;


namespace {

static void
main(std::istream &input, std::ostream &output, bool all_stores, const std::string &store, bool count_bytes, bool cumulative, bool per_doc, Formatting formatting, const std::string &doc_id) {
  // Construct a docrep reader over the provided input stream.
  dr::FauxDoc doc;
  dr::FauxDoc::Schema schema;
  dr::Reader reader(input, schema);

  // Construct the document processor.
  schwa::dr_count::Processor processor(output, all_stores, store, count_bytes, cumulative, per_doc, formatting, doc_id);

  // Read the documents off the input stream.
  while (reader >> doc)
    processor(doc);
  processor.finalise();
}

}


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-count", "Count the number of documents or annotations in stores on a docrep stream.");
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<bool> all_stores(cfg, "all", 'a', "Count docs and elements in all stores found on the first doc", false);
  cf::Op<std::string> store(cfg, "store", 's', "Count docs and elements in the provided store only", cf::Flags::OPTIONAL);
  cf::Op<bool> per_doc(cfg, "per-doc", 'e', "Show counts per doc instead of for the stream", false);
  cf::Op<bool> count_bytes(cfg, "bytes", 'b', "Count bytes instead of number of instances", false);
  cf::Op<bool> cumulative(cfg, "cumulative", 'c', "Show cumulative counts per doc", false);
  cf::OpChoices<std::string> format(cfg, "format", 'f', "How to format the output data", {"aligned", "tabs"}, "aligned");
  cf::Op<std::string> doc_id(cfg, "doc-id", 'd', "Output this expression before each document instead when outputting per-document counts", cf::Flags::OPTIONAL);

  // Parse argv.
  input.position_arg_precedence(0);
  cfg.main<io::PrettyLogger>(argc, argv);

  // Construct the formatting enum value.
  Formatting formatting = Formatting::ALIGNED;
  if (format() == "aligned")
    formatting = Formatting::ALIGNED;
  else if (format() == "tabs")
    formatting = Formatting::TABS;

  // Dispatch to main function.
  try {
    main(input.file(), output.file(), all_stores(), store(), count_bytes(), cumulative(), per_doc(), formatting, doc_id());
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return 0;
}
