/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <memory>
#include <string>

#include <schwa/canonical-schema.h>
#include <schwa/config.h>
#include <schwa/dr/config.h>
#include <schwa/encoding.h>
#include <schwa/formats/plain-text.h>
#include <schwa/io/logging.h>
#include <schwa/tokenizer/tokenizer.h>
#include <schwa/utils/buffer.h>

namespace cf = ::schwa::config;
namespace cs = ::schwa::canonical_schema;
namespace dr = ::schwa::dr;
namespace fm = ::schwa::formats;
namespace io = ::schwa::io;
namespace tk = ::schwa::tokenizer;

namespace schwa {
namespace tokenizer {

class Printer {
protected:
  std::ostream &_out;

public:
  explicit Printer(std::ostream &out) : _out(out) { }
  virtual ~Printer(void) { }

  virtual void write(const cs::Doc &doc) = 0;

  inline void operator <<(const cs::Doc &doc) { write(doc); }

private:
  SCHWA_DISALLOW_COPY_AND_ASSIGN(Printer);
};


class DebugPrinter : public Printer {
public:
  explicit DebugPrinter(std::ostream &out) : Printer(out) { }
  virtual ~DebugPrinter(void) { }

  void
  write(const cs::Doc &doc) override {
    (void)doc;
  }
};


class DocrepPrinter : public Printer {
protected:
  dr::Writer _writer;

public:
  DocrepPrinter(std::ostream &out, cs::Doc::Schema &schema) : Printer(out), _writer(_out, schema) { }
  virtual ~DocrepPrinter(void) { }

  void
  write(const cs::Doc &doc) override {
    _writer << doc;
  }
};



class TextPrinter : public Printer {
public:
  explicit TextPrinter(std::ostream &out) : Printer(out) { }
  virtual ~TextPrinter(void) { }

  void
  write(const cs::Doc &doc) override {
    for (auto &paragraph : doc.paragraphs) {
      // Output a blank line between each paragraph.
      if (&paragraph != &doc.paragraphs[0])
        _out << std::endl;

      for (auto &sentence : paragraph.span) {
        for (auto &token : sentence.span) {
          if (&token != sentence.span.start)
            _out << ' ';
          _out << (token.norm.empty() ? token.raw : token.norm);
        }
        _out << std::endl;
      }
    }
  }
};


static void
tokenize(io::InputStream &in, Printer &printer, const Encoding &encoding) {
  // Read the input stream into a buffer, and re-encode it into UTF-8.
  OffsetBuffer<> in_utf8(1 * 1024 * 1024);
  {
    Buffer<> in_bytes(1 * 1024 * 1024);
    in_bytes.consume(in);

    EncodingResult er;
    to_utf8(encoding, in_bytes, er);
    er.copy_to(in_utf8);
  }

  // Run the plain text lexer over the input to get paragraph bounds.
  fm::PlainTextLexer lexer;
  lexer.lex(in_utf8);

  // Construct the document object.
  cs::Doc doc;

  // Construct the tokenizer object.
  Tokenizer tokenizer;

  // For each paragraph boundary pair.
  for (const auto &pair : lexer.paragraph_indexes()) {
    // Find the newline boundaries in the paragraph.
    OffsetInputStream<> ois(pair.second - pair.first);
    ois.write(in_utf8.begin() + pair.first, in_utf8.begin() + pair.second);

    // Tokenize the paragraph.
    const size_t nsentences_before = doc.sentences.size();
    tokenizer.tokenize(ois, doc);
    const size_t nsentences_after = doc.sentences.size();

    // Create the Paragraph and Block objects, and add them to the document.
    if (nsentences_before != nsentences_after) {
      cs::Paragraph paragraph;
      paragraph.span.start = reinterpret_cast<cs::Sentence *>(nsentences_before);
      paragraph.span.stop = reinterpret_cast<cs::Sentence *>(nsentences_after);
      doc.paragraphs.push_back(paragraph);

      cs::Block block;
      block.paragraph = reinterpret_cast<cs::Paragraph *>(doc.paragraphs.size() - 1);
      doc.blocks.push_back(block);
    }
  }

  // Unswizzle the document pointers and output the document.
  doc.unswizzle_pointers();
  printer << doc;
}

}  // namespace tokenizer
}  // namespace schwa


int
main(int argc, char **argv) {
  // Instantiate a document schema for use in the docrep config framework.
  cs::Doc::Schema schema;

  // Construct an option parser.
  cf::Main cfg("schwa-tokenizer", "Schwa Lab PTB-style tokenizer. Tokenizes plain text input into docrep or plain text formats.");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> input_encoding(cfg, "input-encoding", 'e', "The encoding of the input stream", "utf-8");
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::OpChoices<std::string> printer_name(cfg, "printer", 'p', "Which printer to use as output", {"text", "debug", "docrep"}, "text");
  dr::DocrepGroup dr(cfg, schema);

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Convert the input encoding string to an encoding enum value.
    const auto encoding = schwa::get_encoding(input_encoding());

    // Open the input and output streams.
    io::InputStream in(input_path());
    io::OutputStream out(output_path());

    // Create the tokenizer printer object.
    std::unique_ptr<tk::Printer> printer;
    if (printer_name() == "text")
      printer.reset(new tk::TextPrinter(out));
    else if (printer_name() == "debug")
      printer.reset(new tk::DebugPrinter(out));
    else if (printer_name() == "docrep")
      printer.reset(new tk::DocrepPrinter(out, schema));

    // Dispatch to the main function.
    tokenize(in, *printer, encoding);
  })
  return 0;
}
