/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <sstream>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/dr/query.h>
#include <schwa/io/logging.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace schwa {
namespace dr_grep {

void
main(std::istream &input, std::ostream &output, const std::string &expression) {
  // Construct a docrep reader and writer over the provided streams.
  dr::FauxDoc doc;
  dr::FauxDoc::Schema schema;
  dr::Reader reader(input, schema);
  dr::Writer writer(output, schema);

  // Construct an interpreter and compile the expression.
  dr::query::Interpreter interpreter;
  interpreter.compile(expression);

  // Read the documents off the input stream and evaluate them against the expression.
  for (uint32_t i = 0; reader >> doc; ++i) {
    const auto v = interpreter(doc, i);
    if (v)
      writer << doc;
  }
}

}  // namespace dr_grep
}  // namespace schwa


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("dr-grep", "Filter documents in a docrep stream.");
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<std::string> expression(cfg, "expression", 'e', "The expression to filter on");

  expression.set_positional_precedence(0);
  input.set_positional_precedence(1);

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Dispatch to main function.
    schwa::dr_grep::main(input.file(), output.file(), expression());
  })
  return 0;
}
