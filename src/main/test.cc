/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/msgpack.h>

using namespace schwa;


class Token : public dr::Annotation {
public:
  dr::Slice<uint64_t> slice;
  dr::Slice<Token *> slice2;
  std::string raw;
  std::string norm;
  dr::Pointer<Token> parent;

  class Schema;
};


class X : public dr::Annotation {
};


class Doc : public dr::Document {
public:
  std::string filename;
  dr::Store<Token> tokens;
  dr::Store<Token> tokens2;
  dr::Store<X> xs;

  class Schema;
};


class Token::Schema : public dr::AnnotationSchema<Token> {
public:
  DR_FIELD(&Token::slice) slice;
  DR_POINTER(&Token::slice2, &Doc::tokens) slice2;
  DR_FIELD(&Token::raw) raw;
  DR_FIELD(&Token::norm) norm;
  DR_POINTER(&Token::parent, &Doc::tokens) parent;

  Schema(void) :
    dr::AnnotationSchema<Token>("Token", "Some help text about Token", "Token"),
    slice(*this, "slice", "some help text about slice", dr::LOAD_RO, "slice"),
    slice2(*this, "slice2", "some help text about slice2", dr::LOAD_RO, "slice2"),
    raw(*this, "raw", "some help text about raw", dr::LOAD_RW, "raw"),
    norm(*this, "norm", "some help text about norm", dr::LOAD_RW, "norm"),
    parent(*this, "parent", "some help text about parent", dr::LOAD_RW, "parent")
    { }
  virtual ~Schema(void) { }
};


class Doc::Schema : public dr::DocumentSchema<Doc> {
public:
  DR_FIELD(&Doc::filename) filename;
  DR_STORE(&Doc::tokens) tokens;
  DR_STORE(&Doc::tokens2) tokens2;

  Schema(void) :
    dr::DocumentSchema<Doc>("Document", "Some help text about this Document class"),
    filename(*this, "filename", "some help text about filename", dr::LOAD_RO, "filename"),
    tokens(*this, "tokens", "some help text about Token store", dr::LOAD_RW, "tokens"),
    tokens2(*this, "tokens2", "some help text about Token2 store", dr::LOAD_RW, "tokens2")
    { }
  virtual ~Schema(void) { }
};


// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------
static void
do_write(std::ostream &out) {
  Doc::Schema schema;
  schema.filename.serial = "foo";
  schema.types<Token>().serial = "PTBToken";
  schema.types<Token>().raw.serial = "real_raw";

  Doc d;
  d.tokens.create(2);
  Token &t1 = d.tokens[0];
  Token &t2 = d.tokens[1];
  t1.raw = "hello";
  t2.raw = "world";
  t2.parent = &t1;

  dr::Writer writer(out, schema);
  writer << d;
}


static void
do_read(std::istream &in, std::ostream &out) {
  Doc::Schema schema;
  schema.filename.serial = "foo";
  schema.types<Token>().serial = "PTBToken";
  schema.types<Token>().raw.serial = "real_raw";

  dr::Reader reader(in, schema);
  dr::Writer writer(out, schema);
  while (true) {
    Doc d;
    if (!(reader >> d))
      break;
    std::cerr << "read ..." << std::endl;
    writer << d;
  }
}


int
main(int argc, char *argv[]) {
  config::OpGroup cfg("test", "this is the toplevel help");
  config::EnumOp<std::string> op_mode(cfg, "mode", "The mode of operation", {"read", "write"}, "write");
  config::IStreamOp op_in(cfg, "input", "The input file");
  config::OStreamOp op_out(cfg, "output", "The output file");
  try {
    if (!cfg.process(argc - 1, argv + 1))
      return 1;
  }
  catch (config::ConfigException &e) {
    std::cerr << print_exception("ConfigException", e);
    cfg.help(std::cerr);
    return 1;
  }

  try {
    if (op_mode() == "write")
      do_write(op_out.file());
    else
      do_read(op_in.file(), op_out.file());
  }
  catch (Exception &e) {
    std::cerr << print_exception(e) << std::endl;
    return 1;
  }

  return 0;
}
