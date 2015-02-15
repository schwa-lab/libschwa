/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/learn/extract.h>


namespace schwa {
namespace learn {

const std::string SENTINEL = "__SENTINEL__";

// ============================================================================
// Windower
// ============================================================================
Windower::Windower(const std::string &name, const int8_t delta_left, const int8_t delta_right) :
    _delta_left(delta_left),
    _delta_right(delta_right),
    _name(name)
  { }

}  // namespace learn
}  // namespace schwa
