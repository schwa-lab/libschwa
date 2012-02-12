#include <schwa/base.h>
#include <schwa/msgpack.h>

namespace schwa { namespace msgpack {

std::ostream &
write_array(std::ostream &out, const ArrayObject &arr) {
  if (arr.size <= 31)
    out.put(static_cast<unsigned char>(header::ARRAY_FIXED | arr.size));
  else if (arr.size <= 65535)
    write_raw_uint16(out, arr.size);
  else
    write_raw_uint32(out, arr.size);
  for (uint32_t i = 0; i != arr.size; ++i)
    write_object(out, arr.items[i]);
  return out;
}


std::ostream &
write_map(std::ostream &out, const MapObject &map) {
  if (map.size <= 31)
    out.put(static_cast<unsigned char>(header::MAP_FIXED | map.size));
  else if (map.size <= 65535)
    write_raw_uint16(out, map.size);
  else
    write_raw_uint32(out, map.size);
  for (uint32_t i = 0; i != map.size; ++i) {
    write_object(out, map.keys[i]);
    write_object(out, map.vals[i]);
  }
  return out;
}

std::ostream &
write_raw(std::ostream &out, const RawObject &raw) {
  if (raw.size <= 31)
    out.put(static_cast<unsigned char>(header::RAW_FIXED | raw.size));
  else if (raw.size <= std::numeric_limits<uint16_t>::max())
    write_raw_uint16(out, raw.size);
  else
    write_raw_uint32(out, raw.size);
  return out.write(raw.data, raw.size);
}


std::ostream &
write_object(std::ostream &out, const Object &obj) {
  switch (obj.type) {
  case TYPE_NIL:
    return write_nil(out);
  case TYPE_BOOLEAN:
    return write_boolean(out, obj.via._bool);
  case TYPE_UINT:
    return write_uint(out, obj.via._uint);
  case TYPE_INT:
    return write_int(out, obj.via._int);
  case TYPE_FLOAT:
    return write_float(out, obj.via._float);
  case TYPE_DOUBLE:
    return write_double(out, obj.via._double);
  case TYPE_RAW:
    return write_raw(out, obj.via._raw);
  case TYPE_ARRAY:
    return write_array(out, obj.via._array);
  case TYPE_MAP:
    return write_map(out, obj.via._map);
  };
};



} }
