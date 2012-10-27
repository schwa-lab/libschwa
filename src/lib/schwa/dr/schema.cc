/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr/schema.h>

#include <iostream>

#include <schwa/dr/field_defs.h>
#include <schwa/dr/runtime.h>


namespace schwa {
namespace dr {

// ========================================================================
// BaseSchema
// ========================================================================
BaseSchema::BaseSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) :
    name(name),
    help(help),
    serial(serial),
    type(type)
  { }


std::ostream &
BaseSchema::dump(std::ostream &out) const {
  for (auto &field : _fields) {
    out << "  name='" << field->name << '\'';
    out << " is_slice=" << field->is_slice;
    out << " is_pointer=" << field->is_pointer;
    if (field->is_pointer)
      out << '<' << field->pointer_type().name << '>';
    out << std::endl;
  }
  return out;
}


std::ostream &
operator <<(std::ostream &out, const BaseSchema &s) {
  return s.dump(out);
}


// ========================================================================
// Lazy
// ========================================================================
Lazy::Lazy(void) : Lazy(nullptr, 0, 0) { }

Lazy::Lazy(const char *lazy, uint32_t nelem, uint32_t nbytes) :
    _lazy(lazy),
    _lazy_nelem(nelem),
    _lazy_nbytes(nbytes)
  { }

Lazy::Lazy(const Lazy &o) :
    _lazy(o._lazy),
    _lazy_nelem(o._lazy_nelem),
    _lazy_nbytes(o._lazy_nbytes)
  { }

Lazy::Lazy(const Lazy &&o) :
    _lazy(o._lazy),
    _lazy_nelem(o._lazy_nelem),
    _lazy_nbytes(o._lazy_nbytes)
  { }

Lazy &
Lazy::operator =(const Lazy &o) {
  _lazy = o._lazy;
  _lazy_nelem = o._lazy_nelem;
  _lazy_nbytes = o._lazy_nbytes;
  return *this;
}


// ========================================================================
// BaseAnnSchema
// ========================================================================
BaseAnnSchema::BaseAnnSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) :
    BaseSchema(name, help, serial, type)
  { }


std::ostream &
BaseAnnSchema::dump(std::ostream &out) const {
  out << serial << ' ' << type << std::endl;
  return BaseSchema::dump(out);
}


// ========================================================================
// BaseDocSchema
// ========================================================================
BaseDocSchema::BaseDocSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) :
    BaseSchema(name, help, serial, type)
  { }

BaseDocSchema::~BaseDocSchema(void) {
  for (auto &s : _schemas)
    delete s;
}


std::ostream &
BaseDocSchema::dump(std::ostream &out) const {
  out << "__meta__ " << type << std::endl;
  BaseSchema::dump(out);
  for (auto &schema : _schemas)
    out << *schema;
  return out;
}


// ========================================================================
// Doc
// ========================================================================
Doc::Doc(void) : Lazy(), _rt(nullptr) { }

Doc::~Doc(void) {
  delete _rt;
}

}  // namespace dr
}  // namespace schwa
