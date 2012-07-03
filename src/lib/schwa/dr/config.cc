/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>


namespace schwa { namespace dr {

// ----------------------------------------------------------------------------
// DocrepFieldOp
// ----------------------------------------------------------------------------
DocrepFieldOp::DocrepFieldOp(DocrepClassOp &group, BaseDef &field) : config::OptionBase(field.name, field.help), _field(field) {
  group.add(this);
}


DocrepFieldOp::~DocrepFieldOp(void) { }


config::OptionBase *
DocrepFieldOp::find(const std::string &, const std::string key) {
  const size_t pos = key.find(_name);
  if (key.empty() || pos != 0 || key.size() != _name.size())
    return nullptr;
  return this;
}


void
DocrepFieldOp::help(std::ostream &out, const std::string &prefix, unsigned int) const {
  out << "  " << port::BOLD << prefix << _name << port::OFF << ": " << _desc;
    out << " (default: " << _field.serial << ")";
  out << std::endl;
}


void
DocrepFieldOp::set(const std::string &value) {
  _field.serial = value;
}


void
DocrepFieldOp::validate(void) { }


// ----------------------------------------------------------------------------
// DocrepClassOp
// ----------------------------------------------------------------------------
DocrepClassOp::DocrepClassOp(config::OpGroup &group, BaseDocSchema &schema) : config::OptionBase(schema.name, schema.help), _schema(schema) {
  group.add(this);
  for (auto &f : schema.fields())
    new DocrepFieldOp(*this, *f);
  for (auto &s : schema.stores())
    new DocrepFieldOp(*this, *s);
}


DocrepClassOp::DocrepClassOp(config::OpGroup &group, BaseSchema &schema) : config::OptionBase(schema.name, schema.help), _schema(schema) {
  group.add(this);
  for (auto &f : schema.fields())
    new DocrepFieldOp(*this, *f);
}


DocrepClassOp::~DocrepClassOp(void) {
  for (auto &c : _children)
    delete c;
}


config::OptionBase *
DocrepClassOp::find(const std::string &orig_key, const std::string key) {
  const size_t pos = key.find(_name);
  if (pos != 0)
    return nullptr;
  else if (key.size() == _name.size())
    return this;
  else if (key[_name.size()] != '-')
    throw config::ConfigException("Invalid option key", orig_key);

  const std::string new_key = key.substr(_name.size() + 1);
  if (new_key.empty())
    throw config::ConfigException("Invalid option key", orig_key);

  for (auto &child : _children) {
    config::OptionBase *const p = child->find(orig_key, new_key);
    if (p != nullptr)
      return p;
  }
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
DocrepClassOp::set(const std::string &value) {
  _schema.serial = value;
}


void
DocrepClassOp::validate(void) { }


// ----------------------------------------------------------------------------
// DocrepOpGroup
// ----------------------------------------------------------------------------
DocrepOpGroup::DocrepOpGroup(OpGroup &group, BaseDocSchema &dschema, const std::string &name, const std::string &desc) : config::OpGroup(group, name, desc), _dschema(dschema) {
  new DocrepClassOp(*this, dschema);
  for (auto &s : dschema.schemas())
    new DocrepClassOp(*this, *s);
}

} }
