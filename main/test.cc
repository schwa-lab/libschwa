#include <schwa/std.hpp>
//#include <schwa/pool.hpp>
//#include <schwa/hash.hpp>
//#include <schwa/hashtable.hpp>
#include <schwa/docrep.hpp>

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
  schwa::docrep::Token t2;
  t2.unserialize(fin);
  fin.close();

  return 0;
}
