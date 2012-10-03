/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>


namespace schwa { namespace dr {

BaseDef::BaseDef(const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) : name(name), help(help), mode(mode), serial(serial) {
  if (mode == FieldMode::STREAM_ONLY)
    throw ValueException("Invalid `mode' value: must either be READ_WRITE, READ_ONLY, or DELETE");
}


BaseFieldDef::BaseFieldDef(const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial, const bool is_pointer, const bool is_self_pointer, const bool is_slice) : BaseDef(name, help, mode, serial), is_pointer(is_pointer), is_self_pointer(is_self_pointer), is_slice(is_slice) { }


const TypeInfo &
BaseFieldDef::pointer_type(void) const {
  assert(!"this should never be invoked");
  return *static_cast<const TypeInfo *>(nullptr);
}


ptrdiff_t
BaseFieldDef::store_offset(const Doc *) const {
  assert(!"this should never be invoked");
  return -1;
}


BaseStoreDef::BaseStoreDef(const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) : BaseDef(name, help, mode, serial) { }

} }
