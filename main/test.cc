#include <schwa/std.h>
#include <schwa/pool.h>
#include <schwa/hash.h>
#include <schwa/hashtable.h>
#include <schwa/docrep.h>

int
main(void) {
  msgpack::object v1(42);
  msgpack::object v2("hello, world");

  schwa::docrep::Token t1;
  t1.add("my_int", v1);
  t1.add("my_str", v2);

  std::ofstream fout("/tmp/test.msgpack");
  t1.serialize(fout);
  fout.close();

  std::cerr << "switching!" << std::endl;

  std::ifstream fin("/tmp/test.msgpack");
  schwa::docrep::WireIStream in(fin);
  schwa::docrep::Token t2;
  t2.unserialize(in);

  std::cout << "eof? " << in.eof() << std::endl;
  std::cout << "==? " << (t1 == t2) << std::endl;

  return 0;
}
