/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/io/logging.h>

#include "processor.h"

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace schwa {
namespace dr_count {

static void
main(const std::vector<std::string> &input_paths, std::ostream &out, bool all_stores, const std::string &store, bool count_bytes, bool cumulative, int every, Formatting formatting, const std::string &doc_id, bool no_header, bool no_footer, bool no_ndocs) {
  // Construct the document processor.
  Processor processor(out, all_stores, store, count_bytes, cumulative, every, formatting, doc_id, no_header, no_footer, no_ndocs);

  dr::FauxDoc doc;
  dr::FauxDoc::Schema schema;

  // For each input path to read from.
  for (const auto &input_path : input_paths) {
    // Construct a docrep reader over the provided input stream.
    io::InputStream in(input_path);
    dr::Reader reader(*in, schema);

    // If there is more than one file to process, output the filename and reset the processors state.
    if (input_paths.size() > 1) {
      out << input_path << std::endl;
      processor.reset();
    }

    // Read the documents off the input stream.
    try {
      while (reader >> doc)
        processor(doc, in.path());
    }
    catch (dr::ReaderException &) {
      LOG(WARNING) << "Failed to read document from '" << in.path() << "'" << std::endl;
    }

    processor.finalise();
  }
}

}  // namespace dr_count
}  // namespace schwa


int
main(int argc, char **argv) {
  using Formatting = schwa::dr_count::Formatting;

  // Construct an option parser.
  cf::Main cfg("dr-count", "Count the number of documents or annotations in stores on a docrep stream.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::Op<bool> all_stores(cfg, "all", 'a', "Count docs and elements in all stores found on the first doc", false);
  cf::Op<std::string> store(cfg, "store", 's', "Count docs and elements in the provided store only", cf::Flags::OPTIONAL);
  cf::Op<uint32_t> every(cfg, "every", 'e', "Show counts per doc instead of for the stream", 1, cf::Flags::OPTIONAL);
  cf::Op<bool> count_bytes(cfg, "bytes", 'b', "Count bytes instead of number of instances", false);
  cf::Op<bool> cumulative(cfg, "cumulative", 'c', "Show cumulative counts per doc", false);
  cf::OpChoices<std::string> format(cfg, "format", 'f', "How to format the output data", {"aligned", "tabs"}, "aligned");
  cf::Op<std::string> doc_id(cfg, "doc-id", 'd', "Output this expression before each document instead when outputting per-document counts", cf::Flags::OPTIONAL);
  cf::Op<bool> no_header(cfg, "no-header", 'H', "Hide the column headings row", false);
  cf::Op<bool> no_footer(cfg, "no-footer", 'F', "Hide the cumulative total footer row", false);
  cf::Op<bool> no_ndocs(cfg, "no-ndocs", 'N', "Hide the ndocs column", false);

  cfg.allow_unclaimed_args("[input-path...]");

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Construct the formatting enum value.
    Formatting formatting = Formatting::ALIGNED;
    if (format() == "aligned")
      formatting = Formatting::ALIGNED;
    else if (format() == "tabs")
      formatting = Formatting::TABS;

    // Work out which input paths to read from.
    std::vector<std::string> input_paths;
    if (input_path.was_mentioned() || cfg.unclaimed_args().empty())
      input_paths.push_back(input_path());
    else
      input_paths = cfg.unclaimed_args();

    // Open the ouptut stream.
    io::OutputStream out(output_path());

    // Dispatch to main function.
    schwa::dr_count::main(input_paths, *out, all_stores(), store(), count_bytes(), cumulative(), every.was_mentioned() ? every() : -1, formatting, doc_id(), no_header(), no_footer(), no_ndocs());
  })
  return 0;
}
