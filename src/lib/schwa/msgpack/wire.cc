/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/msgpack.h>


namespace schwa { namespace msgpack {

static constexpr WireType FN = WireType::FIXNUM_NEGATIVE;
static constexpr WireType FP = WireType::FIXNUM_POSITIVE;
static constexpr WireType AF = WireType::ARRAY_FIXED;
static constexpr WireType MF = WireType::MAP_FIXED;
static constexpr WireType RF = WireType::RAW_FIXED;
constexpr WireType TABLE[256] = {
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


} }
