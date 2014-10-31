/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/utils/buffer.h>


namespace schwa {

// ============================================================================
// BaseOffsetBuffer
// ============================================================================
BaseOffsetBuffer::BaseOffsetBuffer(const size_t nitems_grow, const size_t initial_offset) :
  _nitems_grow(nitems_grow),
  _nitems_allocd(0),
  _nitems_used(0),
  _initial_offset(initial_offset),
  _bytes(nullptr),
  _offsets(nullptr)
{ }

BaseOffsetBuffer::BaseOffsetBuffer(const BaseOffsetBuffer &o) :
  _nitems_grow(o._nitems_grow),
  _nitems_allocd(o._nitems_allocd),
  _nitems_used(o._nitems_used),
  _initial_offset(o._initial_offset),
  _bytes(nullptr),
  _offsets(nullptr)
{ }

}
