/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>

#include <schwa/config.h>
#include <schwa/io/logging.h>
#include <schwa/unsupervised/brown_clusters.h>

namespace cf = schwa::config;
namespace io = schwa::io;


namespace {

void
main(std::istream &in, std::ostream &out, const unsigned int nclusters) {
  schwa::unsupervised::BrownClusterer clusterer;
  clusterer.run(nclusters, in, out);
}

}  // namespace


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("brown-clusterer", "Construct Brown clusters from sentences of text. See Brown et al, 1992.");
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<unsigned int> nclusters(cfg, "nclusters", 'c', "The number of clusters to create", 1000);

  // Parse argv.
  input.position_arg_precedence(0);
  cfg.main<io::PrettyLogger>(argc, argv);

  // Dispatch to main function.
  try {
    main(input.file(), output.file(), nclusters());
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return 0;
}
