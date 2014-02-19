/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>
#include <schwa/dr.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;
namespace mp = schwa::msgpack;


#if 0
class Token : public dr::Ann {
public:
  dr::Slice<uint64_t> slice;
  dr::Slice<Token *> slice2;
  std::string raw;
  std::string norm;
  dr::Pointer<Token> parent;

  class Schema;
};


class X : public dr::Ann {
};


class Doc : public dr::Doc {
public:
  std::string filename;
  dr::Store<Token> tokens;
  dr::Store<Token> tokens2;
  dr::Store<X> xs;

  class Schema;
};


class Token::Schema : public dr::Ann::Schema<Token> {
public:
  DR_FIELD(&Token::slice) slice;
  DR_POINTER(&Token::slice2, &Doc::tokens) slice2;
  DR_FIELD(&Token::raw) raw;
  DR_FIELD(&Token::norm) norm;
  DR_POINTER(&Token::parent, &Doc::tokens) parent;

  Schema(void) :
    dr::Ann::Schema<Token>("Token", "Some help text about Token", "Token"),
    slice(*this, "slice", "some help text about slice", dr::FieldMode::READ_ONLY, "slice"),
    slice2(*this, "slice2", "some help text about slice2", dr::FieldMode::READ_ONLY, "slice2"),
    raw(*this, "raw", "some help text about raw", dr::FieldMode::READ_WRITE, "raw"),
    norm(*this, "norm", "some help text about norm", dr::FieldMode::READ_WRITE, "norm"),
    parent(*this, "parent", "some help text about parent", dr::FieldMode::READ_WRITE, "parent")
    { }
  virtual ~Schema(void) { }
};


class Doc::Schema : public dr::Doc::Schema<Doc> {
public:
  DR_FIELD(&Doc::filename) filename;
  DR_STORE(&Doc::tokens) tokens;
  DR_STORE(&Doc::tokens2) tokens2;

  Schema(void) :
    dr::Doc::Schema<Doc>("Doc", "Some help text about this Doc class"),
    filename(*this, "filename", "some help text about filename", dr::FieldMode::READ_ONLY, "filename"),
    tokens(*this, "tokens", "some help text about Token store", dr::FieldMode::READ_WRITE, "tokens"),
    tokens2(*this, "tokens2", "some help text about Token2 store", dr::FieldMode::READ_WRITE, "tokens2")
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
  cf::Main cfg("test", "this is the toplevel help");
  cf::ChoicesOp<std::string> op_mode(cfg, "mode", "The mode of operation", {"read", "write"}, "write");
  cf::IStreamOp op_in(cfg, "input", "The input file");
  cf::OStreamOp op_out(cfg, "output", "The output file");

  cfg.main(argc, argv);

  try {
    if (op_mode() == "write")
      do_write(op_out.file());
    else
      do_read(op_in.file(), op_out.file());
  }
  catch (schwa::Exception &e) {
    std::cerr << schwa::print_exception(e) << std::endl;
    return 1;
  }

  return 0;
}
#endif


int
main(int argc, char **argv) {

  cf::Main cfg("test", "this is the toplevel help");
  cf::ChoicesOp<std::string> mode(cfg, "mode", "The mode of operation", {"read", "write"}, "write");
  cf::IStreamOp input(cfg, "input", "The input file");
  cf::OStreamOp output(cfg, "output", "The output file");

  cf::Group group1(cfg, "group1", "help text for some group");
  cf::Op<uint32_t> g1a(group1, "g1a", "meow", 8);
  cf::Op<uint32_t> g1b(group1, "g1b", "meow", 8);

  cf::Group group1_1(group1, "group11", "help text for some group");
  cf::Op<uint32_t> g1_1a(group1_1, "g1a", "meow", 8);
  cf::Op<uint32_t> g1_1b(group1_1, "g1b", "meow", 8);

  cf::Group group2(cfg, "group2", "help text for some group");
  cf::Op<uint32_t> g2a(group2, "g2a", "meow", 0);
  cf::Op<uint32_t> g2b(group2, "g2b", "meow", 0);

  cfg.main<io::PrettyLogger>(argc, argv);

  LOG(DEBUG) << "This is a debug statement." << std::endl;
  LOG(INFO) << "This is a info statement." << std::endl;
  LOG(WARNING) << "This is a warning statement." << std::endl;
  LOG(ERROR) << "This is a error statement." << std::endl;
  LOG(CRITICAL) << "This is a critical statement." << std::endl;

  return 0;
}
