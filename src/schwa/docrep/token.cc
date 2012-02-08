#include <schwa/std.hpp>
#include <schwa/docrep.hpp>

namespace schwa { namespace docrep {

void
Token::add(const std::string &name, const msgpack::object &value) {
  _map[name] = value;
}


std::ostream &
Token::serialize(std::ostream &out) const {
  msgpack::packer<std::ostream> packer(out);
  std::cerr << "Token::serialize writing " << _map.size() << " objects" << std::endl;
  packer.pack_map(_map.size());
  for (auto &pair : _map) {
    packer.pack(pair.first);
    packer.pack(pair.second);
  }
  return out;
}


std::istream &
Token::unserialize(std::istream &in) {
  msgpack::unpacker unpacker;

  while (in) {
    std::cerr << "Looping..." << std::endl;
    unpacker.reserve_buffer(32 * 1024);
    std::streamsize read = in.readsome(unpacker.buffer(), unpacker.buffer_capacity());
    std::cerr << "read in " << read << std::endl;
    if (read == 0)
      break;
    unpacker.buffer_consumed(read);

    msgpack::unpacked result;
    while (unpacker.next(&result)) {
      msgpack::object obj = result.get();
    }
  }

  std::cerr << "Token::unserialize has " << _map.size() << " objects" << std::endl;
  return in;
}


} }
