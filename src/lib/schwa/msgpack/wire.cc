/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/msgpack.h>


namespace schwa { namespace msgpack {

std::ostream &
write_array(std::ostream &out, const ArrayObject &arr) {
  write_array_header(out, arr.size);
  for (uint32_t i = 0; i != arr.size; ++i)
    write_object(out, arr.items[i]);
  return out;
}


std::ostream &
write_map(std::ostream &out, const MapObject &map) {
  write_map_header(out, map.size);
  for (uint32_t i = 0; i != map.size; ++i) {
    write_object(out, map.keys[i]);
    write_object(out, map.vals[i]);
  }
  return out;
}


std::ostream &
write_raw(std::ostream &out, const char *const data, const size_t size) {
  if (size <= 31)
    out.put(static_cast<unsigned char>(header::RAW_FIXED | size));
  else if (size <= std::numeric_limits<uint16_t>::max()) {
    out.put(header::RAW_16);
    write_raw_uint16(out, size);
  }
  else {
    out.put(header::RAW_32);
    write_raw_uint32(out, size);
  }
  return out.write(data, size);
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
  return out;
}

} }
