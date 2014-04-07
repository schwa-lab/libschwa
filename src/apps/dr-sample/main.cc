/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>

#include <schwa/config.h>
#include <schwa/dr/reader.h>
#include <schwa/io/logging.h>

#include <dr-sample/main.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace {

static void
main(std::istream &input, std::ostream &output, const uint32_t count) {
  // Construct the random number generator with the current time as seed.
  const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);

  // Create an array to store the `count` random docs in.
  std::unique_ptr<std::stringstream[]> bufs(new std::stringstream[count]);

  // Use reservoir sampling to sample the random docs from the input stream.
  uint32_t nread = 0;
  while (true) {
    // Attempt to read the next document into temp storage.
    std::stringstream tmp;
    if (!dr::read_lazy_doc(input, tmp))
      break;

    // Work out where to place the document, if anywhere.
    bool keep = true;
    uint32_t index = nread;
    if (nread >= count) {
      const uint32_t r = generator();
      if ((r % (nread + 1)) < count)
        index = r % count;
      else
        keep = false;
    }

    // Move the read in data into the appropriate buffer element, and increment the counts.
    if (keep) {
      bufs[index].str(tmp.str());
      bufs[index].clear();
    }
    ++nread;
  }

  // Output the sampled docs.
  for (uint32_t i = 0; i != std::min(nread, count); ++i) {
    // Rewind the temp storage and write it out.
    std::stringstream &tmp = bufs[i];
    tmp.seekp(0);
    output << tmp.rdbuf();
  }
}

}  // namespace


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg(schwa::dr_sample::PROGRAM_NAME, schwa::dr_sample::PROGRAM_DESC);
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<uint32_t> count(cfg, "count", 'n', "How many documents to keep", 1);

  // Parse argv.
  input.position_arg_precedence(0);
  cfg.main<io::PrettyLogger>(argc, argv);

  // Dispatch to main function.
  try {
    main(input.file(), output.file(), count());
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return 0;
}
