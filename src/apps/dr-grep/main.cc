/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <sstream>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/io/logging.h>

#include <dr-grep/language.h>
#include <dr-grep/main.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace {

void
main(std::istream &input, std::ostream &output, const std::string &expression) {
  // Construct a docrep reader and writer over the provided streams.
  dr::FauxDoc doc;
  dr::FauxDoc::Schema schema;
  dr::Reader reader(input, schema);
  dr::Writer writer(output, schema);

  // Construct an interpreter and compile the expression.
  schwa::dr_grep::Interpreter interpreter;
  interpreter.compile(expression);

  // Read the documents off the input stream and evaluate them against the expression.
  for (uint64_t i = 0; reader >> doc; ++i) {
    if (interpreter(i, doc))
      writer << doc;
  }
}

}  // namespace


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg(schwa::dr_grep::PROGRAM_NAME, schwa::dr_grep::PROGRAM_DESC);
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<std::string> expression(cfg, "expression", 'e', "The expression to filter on");

  // Parse argv.
  cfg.main<io::PrettyLogger>(argc, argv);

  // Dispatch to main function.
  try {
    main(input.file(), output.file(), expression());
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }
  return 0;
}
