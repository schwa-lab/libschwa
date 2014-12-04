/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <schwa/canonical-schema.h>
#include <schwa/config.h>
#include <schwa/corpora/clueweb.h>
#include <schwa/corpora/nanc.h>
#include <schwa/corpora/tipster.h>
#include <schwa/dr.h>
#include <schwa/dr/config.h>
#include <schwa/io/logging.h>
#include <schwa/io/streams.h>

namespace cf = ::schwa::config;
namespace cp = ::schwa::corpora;
namespace cs = ::schwa::canonical_schema;
namespace dr = ::schwa::dr;
namespace io = ::schwa::io;


int
main(int argc, char **argv) {
  // Instantiate a document schema for use in the docrep config framework.
  cs::Doc::Schema schema;

  // Construct an option parser.
  cf::Main cfg("corpora-to-dr", "Converts a file from a known corpus into docrep.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::OpChoices<std::string> corpus(cfg, "corpus", 'c', "Which corpus is input source from", {"clueweb", "nanc", "tipster"});
  dr::DocrepGroup dr(cfg, schema);
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

    // Create a docrep writer over the output stream.
    io::OutputStream out(output_path());
    dr::Writer writer(out, schema);

    std::unique_ptr<cp::Importer> importer;
    for (const auto &path : input_paths) {
      // Create the corpus importer object.
      if (corpus() == "clueweb")
        importer.reset(new cp::ClueWebImporter(path));
      else if (corpus() == "nanc")
        importer.reset(new cp::NANCImporter(path));
      else if (corpus() == "tipster")
        importer.reset(new cp::TipsterImporter(path));

      // Read in each of the documents from the importer, and write out the docrep.
      for (cs::Doc *doc = nullptr; (doc = importer->import()) != nullptr; ) {
        LOG(INFO) << "Read in doc '" << doc->doc_id << "'" << std::endl;
        writer << *doc;
      }
    }
  })
  return 0;
}
