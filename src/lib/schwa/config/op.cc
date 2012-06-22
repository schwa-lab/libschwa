/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>


namespace schwa { namespace config {

OpBase::OpBase(OpGroup &group, const std::string &name, const std::string &desc, const bool has_default) : OptionBase(name, desc), _has_default(has_default), _is_set(false) {
  group.add(this);
}

OptionBase *
OpBase::find(const std::string &, const std::string key) {
  const size_t pos = key.find(_name);
  if (key.empty() || pos != 0 || key.size() != _name.size())
    return nullptr;
  return this;
}


void
OpBase::set(const std::string &value) {
  _set(value);
  _is_set = true;
}


void
OpBase::validate(void) {
  if (!_is_set) {
    if (!_has_default)
      throw ConfigException("Unset configuration option", _name);
    set_default();
  }
  _validate();
}

} }
