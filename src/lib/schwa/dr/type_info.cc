/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr/type_info.h>

#include <iostream>

#include <schwa/port.h>


namespace schwa {
namespace dr {

#ifdef USE_DEMANGLE_TYPEID
TypeInfo::TypeInfo(const std::string &typeid_name) :
    name(port::demangle_typeid(typeid_name.c_str()))
  { }
#else
TypeInfo::TypeInfo(const std::string &typeid_name) :
    name(typeid_name)
  { }
#endif

TypeInfo::TypeInfo(const TypeInfo &o) :
    name(o.name)
  { }

std::ostream &
operator <<(std::ostream &out, const TypeInfo &type) {
  return out << type.name;
}

}  // namespace dr
}  // namespace schwa
