/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr/config.h>

#include <iostream>
#include <sstream>

#include <schwa/config/exception.h>
#include <schwa/dr/field_defs.h>
#include <schwa/port.h>

namespace cf = schwa::config;


namespace schwa {
namespace dr {

// ============================================================================
// OpDocrepField
// ============================================================================
OpDocrepField::OpDocrepField(DocrepClassGroup &group, BaseDef &field) :
    cf::Op<std::string>(group, field.name, field.help, field.serial),
    _field(field)
  { }


bool
OpDocrepField::_validate(const cf::Main &main) {
  if (!cf::Op<std::string>::_validate(main))
    return false;
  if (_value.empty()) {
    std::ostringstream ss;
    ss << "Error setting value for \"" << _name << "\": value is empty";
    throw cf::ConfigException(ss.str());
  }
  _field.serial = _value;
  return true;
}


// ============================================================================
// DocrepClassGroup
// ============================================================================
DocrepClassGroup::DocrepClassGroup(DocrepGroup &group, BaseDocSchema &schema) :
    cf::Group(group, schema.name, schema.help),
    _schema(schema) {
  for (auto &f : schema.fields())
    new OpDocrepField(*this, *f);
  for (auto &s : schema.stores())
    new OpDocrepField(*this, *s);
}

DocrepClassGroup::DocrepClassGroup(DocrepGroup &group, BaseSchema &schema) :
    cf::Group(group, schema.name, schema.help),
    _schema(schema) {
  for (auto &f : schema.fields())
    new OpDocrepField(*this, *f);
}


void
DocrepClassGroup::_help_self(std::ostream &out, unsigned int depth) const {
  Group::_help_self(out, depth);
  out << port::DARK_GREY << " (default: " << _schema.name << ")" << port::OFF;
}


bool
DocrepClassGroup::accepts_assignment(void) const {
  return true;
}


bool
DocrepClassGroup::accepts_mention(void) const {
  return true;
}


void
DocrepClassGroup::_assign(const std::string &value) {
  if (value.empty()) {
    std::ostringstream ss;
    ss << "Error setting value for \"" << _name << "\": value is empty";
    throw cf::ConfigException(ss.str());
  }
  _schema.serial = value;
}


void
DocrepClassGroup::_mention(void) { }


void
DocrepClassGroup::serialise(std::ostream &out) const {
  if (_was_assigned)
    out << _full_name << '=' << _schema.serial << '\n';
  Group::serialise(out);
}


// ============================================================================
// DocrepGroup
// ============================================================================
DocrepGroup::DocrepGroup(Group &group, BaseDocSchema &dschema, const std::string &name, const std::string &desc) :
    cf::Group(group, name, desc),
    _dschema(dschema) {
  new DocrepClassGroup(*this, dschema);
  for (auto &s : dschema.schemas())
    new DocrepClassGroup(*this, *s);
}

}  // namespace dr
}  // namespace schwa
