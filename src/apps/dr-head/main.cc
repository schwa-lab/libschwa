/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
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
namespace dr_head {

static void
main(std::istream &in, std::ostream &out, const uint32_t count, const uint32_t skip) {
  // Read the documents off the input stream.
  uint32_t nread = 0;
  for (uint32_t i = 0; ; ++i) {
    std::stringstream tmp;
    if (nread == count)
      break;
    else if (!dr::read_lazy_doc(in, tmp))
      break;
    else if (i < skip)
      continue;

    tmp.seekp(0);
    out << tmp.rdbuf();
    ++nread;
  }
}


static void
main(const std::vector<std::string> &input_paths, io::OutputStream &out, const uint32_t count, const uint32_t skip) {
  for (const std::string &input_path : input_paths) {
    io::InputStream in(input_path);
    main(in, out, count, skip);
  }
}

}  // namespace dr_head
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-head", "A `head` tool for docrep streams.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::Op<uint32_t> count(cfg, "count", 'n', "How many documents to keep", 1);
  cf::Op<uint32_t> skip(cfg, "skip", 's', "How many documents to skip before counting", 0);

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
    schwa::dr_head::main(input_paths, out, count(), skip());
  })
  return 0;
}
