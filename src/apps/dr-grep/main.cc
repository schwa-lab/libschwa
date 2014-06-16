/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/dr/query.h>
#include <schwa/io/logging.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace dq = schwa::dr::query;
namespace io = schwa::io;


namespace schwa {
namespace dr_grep {

void
main(const std::vector<std::string> &input_paths, std::ostream &out, const std::string &expression) {
  // Construct a docrep writer over the provided streams.
  dr::FauxDoc doc;
  dr::FauxDoc::Schema schema;
  dr::Writer writer(out, schema);

  // Construct an interpreter and compile the expression.
  dq::Interpreter interpreter;
  interpreter.compile(expression);

  // Process each input stream.
  for (const std::string &input_path : input_paths) {
    io::InputStream in(input_path);
    dr::Reader reader(in, schema);

    // Read the documents off the input stream and evaluate them against the expression.
    for (uint32_t i = 0; reader >> doc; ++i) {
      const auto v = interpreter(doc, i);
      if (v)
        writer << doc;
    }
  }
}

}  // namespace dr_grep
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-grep", "Filter documents in a docrep stream.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::Op<std::string> expression(cfg, "expression", 'e', "The expression to filter on");

  expression.set_positional_precedence(0);
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
    schwa::dr_grep::main(input_paths, out, expression());
  })
  return 0;
}
