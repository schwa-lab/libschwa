/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
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
namespace dr_tail {

static void
main(std::istream &in, std::ostream &out, const uint32_t count) {
  // Allocate a circular array to store the last `count` docs in.
  std::unique_ptr<std::stringstream[]> bufs(new std::stringstream[count]);
  size_t bufs_upto = 0;

  // Read the documents off the input stream into the circular array.
  uint32_t nread = 0;
  while (true) {
    // Attempt to read the next document into temp storage.
    std::stringstream tmp;
    if (!dr::read_lazy_doc(in, tmp))
      break;

    // Move the read in data into the next buffer element, and increment the counts.
    bufs[bufs_upto].str(tmp.str());
    bufs[bufs_upto].clear();
    bufs_upto = (bufs_upto + 1) % count;
    ++nread;
  }

  // Output the last min(`count`, `nread`) docs.
  if (nread < count)
    bufs_upto = 0;
  for (uint32_t i = 0; i != std::min(nread, count); ++i) {
    // Rewind the temp storage and write it out.
    std::stringstream &tmp = bufs[bufs_upto];
    tmp.seekp(0);
    out << tmp.rdbuf();

    // Go to the next element in the circular array.
    bufs_upto = (bufs_upto + 1) % count;
  }
}


static void
main(const std::vector<std::string> &input_paths, io::OutputStream &out, const uint32_t count) {
  for (const std::string &input_path : input_paths) {
    io::InputStream in(input_path);
    main(in, out, count);
  }
}

}  // namespace dr_tail
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-tail", "A `tail` tool for docrep streams.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::Op<uint32_t> count(cfg, "count", 'n', "How many documents to keep", 1);

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
    schwa::dr_tail::main(input_paths, out, count());
  })
  return 0;
}
