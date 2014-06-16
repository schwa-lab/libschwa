/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <chrono>
#include <iostream>
#include <memory>
#include <random>
#include <sstream>
#include <string>
#include <vector>

#include <schwa/config.h>
#include <schwa/dr/reader.h>
#include <schwa/io/logging.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace schwa {
namespace dr_sample {

using seed_type = std::mt19937::result_type;

static void
run(const std::vector<std::string> &input_paths, std::ostream &out, std::mt19937 &generator, const uint32_t count) {
  // Create an array to store the `count` random docs in.
  std::unique_ptr<std::stringstream[]> bufs(new std::stringstream[count]);

  // Use reservoir sampling to sample the random docs from the input streams.
  uint32_t nread = 0;
  for (const std::string &input_path : input_paths) {
    io::InputStream in(input_path);

    while (true) {
      // Attempt to read the next document into temp storage.
      std::stringstream tmp;
      if (!dr::read_lazy_doc(*in, tmp))
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
  }

  // Output the sampled docs.
  for (uint32_t i = 0; i != std::min(nread, count); ++i) {
    // Rewind the temp storage and write it out.
    std::stringstream &tmp = bufs[i];
    tmp.seekp(0);
    out << tmp.rdbuf();
  }
}


static void
main(const std::vector<std::string> &input_paths, std::ostream &out, const uint32_t count) {
  // Construct the random number generator with the current time as seed.
  const auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::mt19937 generator(seed);
  run(input_paths, out, generator, count);
}


static void
main(const std::vector<std::string> &input_paths, std::ostream &out, const uint32_t count, const seed_type seed) {
  // Construct the random number generator with the provided seed.
  std::mt19937 generator(seed);
  run(input_paths, out, generator, count);
}

}  // namespace dr_sample
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-sample", "Randomly samples n documents from a stream.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::Op<uint32_t> count(cfg, "count", 'n', "How many documents to keep", 1);
  cf::Op<schwa::dr_sample::seed_type> seed(cfg, "seed", 's', "The seed value for the random number generator", cf::Flags::OPTIONAL);

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

    // Open the ouptut stream.
    io::OutputStream out(output_path());

    // Dispatch to main function.
    if (seed.was_mentioned())
      schwa::dr_sample::main(input_paths, out, count(), seed());
    else
      schwa::dr_sample::main(input_paths, out, count());
  })
  return 0;
}
