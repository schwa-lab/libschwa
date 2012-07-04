/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/msgpack.h>


namespace schwa { namespace msgpack {

static constexpr WireType FN = WireType::FIXNUM_NEGATIVE;
static constexpr WireType FP = WireType::FIXNUM_POSITIVE;
static constexpr WireType AF = WireType::ARRAY_FIXED;
static constexpr WireType MF = WireType::MAP_FIXED;
static constexpr WireType RF = WireType::RAW_FIXED;
const WireType TABLE[256] = {
  FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP,
  FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP,
  FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP,
  FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP,
  FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP,
  FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP,
  FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP,
  FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP, FP,
  MF, MF, MF, MF, MF, MF, MF, MF, MF, MF, MF, MF, MF, MF, MF, MF,
  AF, AF, AF, AF, AF, AF, AF, AF, AF, AF, AF, AF, AF, AF, AF, AF,
  RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF,
  RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF, RF,
  WireType::NIL  , WireType::RESERVED, WireType::FALSE , WireType::TRUE  , WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::FLOAT , WireType::DOUBLE, WireType::UINT_8  , WireType::UINT_16 , WireType::UINT_32, WireType::UINT_64,
  WireType::INT_8, WireType::INT_16  , WireType::INT_32, WireType::INT_64, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RESERVED, WireType::RAW_16, WireType::RAW_32, WireType::ARRAY_16, WireType::ARRAY_32, WireType::MAP_16 , WireType::MAP_32 ,
  FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN,
  FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN, FN
};


bool
read_lazy(std::istream &in, std::ostream &out, WireType &type) {
  char buf[8];
  uint16_t s16;
  uint32_t s32;
  bool recurse = false, raw = false;

  const int h = in.get();
  if (h == EOF)
    return false;
  out.put(h);

  type = header_type(h);
  switch (type) {
  case WireType::FIXNUM_POSITIVE:
  case WireType::FIXNUM_NEGATIVE:
  case WireType::NIL:
  case WireType::TRUE:
  case WireType::FALSE:
    break;
  case WireType::INT_8:
  case WireType::UINT_8:
    in.read(buf, 1);
    out.write(buf, 1);
    break;
  case WireType::INT_16:
  case WireType::UINT_16:
    in.read(buf, 2);
    out.write(buf, 2);
    break;
  case WireType::FLOAT:
  case WireType::INT_32:
  case WireType::UINT_32:
    in.read(buf, 4);
    out.write(buf, 4);
    break;
  case WireType::DOUBLE:
  case WireType::INT_64:
  case WireType::UINT_64:
    in.read(buf, 8);
    out.write(buf, 8);
    break;
  case WireType::ARRAY_FIXED:
    s32 = h & 0x0F;
    recurse = true;
    break;
  case WireType::ARRAY_16:
    read_bytes16(in, s16);
    s32 = s16;
    recurse = true;
    break;
  case WireType::ARRAY_32:
    read_bytes32(in, s32);
    recurse = true;
    break;
  case WireType::MAP_FIXED:
    s32 = h & 0x0F;
    s32 *= 2;
    recurse = true;
    break;
  case WireType::MAP_16:
    read_bytes16(in, s16);
    s32 = 2 * s16;
    recurse = true;
    break;
  case WireType::MAP_32:
    read_bytes32(in, s32);
    s32 *= 2;
    recurse = true;
    break;
  case WireType::RAW_FIXED:
    s32 = h & 0x1F;
    raw = true;
    break;
  case WireType::RAW_16:
    read_bytes16(in, s16);
    s32 = s16;
    raw = true;
    break;
  case WireType::RAW_32:
    read_bytes32(in, s32);
    raw = true;
    break;
  case WireType::RESERVED:
    break;
  }

  if (recurse) {
    WireType t;
    for (uint32_t i = 0; i != s32; ++i)
      if (!read_lazy(in, out, t))
        return false;
  }
  else if (raw) {
    for (uint32_t i = 0; i != s32; ++i) {
      int c = in.get();
      if (c == EOF)
        return false;
      out.put(c);
    }
  }

  return true;
}

} }
