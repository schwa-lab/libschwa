/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <sstream>

#include <schwa/config.h>
#include <schwa/dr/reader.h>
#include <schwa/io/logging.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace {

static void
main(std::istream &input, std::ostream &output, const uint32_t count) {
  // Allocate a circular array to store the last `count` docs in.
  std::unique_ptr<std::stringstream[]> bufs(new std::stringstream[count]);
  size_t bufs_upto = 0;

  // Read the documents off the input stream into the circular array.
  uint32_t nread = 0;
  while (true) {
    // Attempt to read the next document into temp storage.
    std::stringstream tmp;
    if (!dr::read_lazy_doc(input, tmp))
      break;

    // Move the read in data into the next buffer element, and increment the counts.
    // XXX Annoyingly, libstdc++ (gcc) hasn't implemented move semantics for iostreams yet.
    // XXX bufs[bufs_upto].swap(tmp);
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
    output << tmp.rdbuf();

    // Go to the next element in the circular array.
    bufs_upto = (bufs_upto + 1) % count;
  }
}

}  // namespace


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-tail", "A `tail` tool for docrep streams.");
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
