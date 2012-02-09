#include <schwa/docrep.h>

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
  for (std::map<std::string, msgpack::object>::const_iterator it(_map.begin()); it != _map.end(); ++it) {
    packer.pack(it->first);
    packer.pack(it->second);
  }
  return out;
}


WireIStream &
Token::unserialize(WireIStream &in) {

  msgpack::object obj(in.get());
  std::cerr << obj << std::endl;
  if (obj.type != msgpack::type::MAP)
    throw new DeserializeException();

  const uint32_t size = obj.via.map.size;
  msgpack::object_kv *values = obj.via.map.ptr;
  std::cerr << size << " " << values << std::endl;
  for (uint32_t i = 0; i != size; ++i) {
    std::cerr << "i=" << i << std::endl;
    if (values[i].key.type != msgpack::type::RAW)
      throw new DeserializeException();
    std::string key;
    values[i].key.convert(&key);
    _map[key] = values[i].val;
  }

  in.next();

  std::cerr << "Token::unserialize has " << _map.size() << " objects" << std::endl;
  return in;
}


} }
