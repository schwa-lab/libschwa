/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr/config.h>

#include <iostream>

#include <schwa/config/exception.h>
#include <schwa/dr/field_defs.h>
#include <schwa/port.h>

namespace cf = schwa::config;


namespace schwa {
namespace dr {

// ============================================================================
// DocrepFieldOp
// ============================================================================
DocrepFieldOp::DocrepFieldOp(DocrepClassOp &group, BaseDef &field) :
    cf::ConfigNode(field.name, field.help),
    _field(field) {
  //group.add(this);  FIXME
  (void)group;
}

DocrepFieldOp::~DocrepFieldOp(void) { }


cf::ConfigNode *
DocrepFieldOp::find(const std::string &key) {
  (void)key;
  // FIXME
  return nullptr;
}


void
DocrepFieldOp::help(std::ostream &out, const std::string &prefix, unsigned int) const {
  out << "  " << port::BOLD << prefix << _name << port::OFF << ": " << _desc;
    out << " (default: " << _field.serial << ")";
  out << std::endl;
}


void
DocrepFieldOp::assign(const std::string &value) {
  _field.serial = value;
}


bool
DocrepFieldOp::accepts_assignment(void) const {
  return true;
}


bool
DocrepFieldOp::accepts_mention(void) const {
  return true;
}


void
DocrepFieldOp::mention(void) { }


bool
DocrepFieldOp::validate(const cf::Main &) {
  return true;
}


// ============================================================================
// DocrepClassOp
// ============================================================================
DocrepClassOp::DocrepClassOp(cf::Group &group, BaseDocSchema &schema) :
    cf::ConfigNode(schema.name, schema.help),
    _schema(schema) {
  //group.add(this);  FIXME
  (void)group;
  for (auto &f : schema.fields())
    new DocrepFieldOp(*this, *f);
  for (auto &s : schema.stores())
    new DocrepFieldOp(*this, *s);
}

DocrepClassOp::DocrepClassOp(cf::Group &group, BaseSchema &schema) :
    cf::ConfigNode(schema.name, schema.help),
    _schema(schema) {
  //group.add(this);  FIXME
  (void)group;
  for (auto &f : schema.fields())
    new DocrepFieldOp(*this, *f);
}

DocrepClassOp::~DocrepClassOp(void) {
  for (auto &c : _children)
    delete c;
}


void
DocrepClassOp::add(DocrepFieldOp *const child) {
  _children.push_back(child);
}


cf::ConfigNode *
DocrepClassOp::find(const std::string &key) {
  // FIXME
  (void)key;
  return nullptr;
}


void
DocrepClassOp::help(std::ostream &out, const std::string &prefix, unsigned int depth) const {
  const std::string me = prefix + _name + "-";
  if (depth != 0)
    out << std::endl;
  out << port::BOLD << prefix << _name << port::OFF << ": " << _desc;
  out << " (default: " << _schema.serial << ")" << std::endl;
  for (auto &child : _children)
    child->help(out, me, depth + 1);
}


void
DocrepClassOp::assign(const std::string &value) {
  _schema.serial = value;
}


bool
DocrepClassOp::accepts_assignment(void) const {
  return true;
}


bool
DocrepClassOp::accepts_mention(void) const {
  return true;
}


void
DocrepClassOp::mention(void) { }


bool
DocrepClassOp::validate(const cf::Main &) {
  return true;
}


// ============================================================================
// DocrepOpGroup
// ============================================================================
DocrepOpGroup::DocrepOpGroup(Group &group, BaseDocSchema &dschema, const std::string &name, const std::string &desc) : cf::Group(group, name, desc), _dschema(dschema) {
  new DocrepClassOp(*this, dschema);
  for (auto &s : dschema.schemas())
    new DocrepClassOp(*this, *s);
}

}  // namespace dr
}  // namespace schwa
