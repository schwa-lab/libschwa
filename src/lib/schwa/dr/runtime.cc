/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/dr.h>


namespace schwa { namespace dr {

static void
_finalise(const std::set<TypeInfo> &seen, const Schema &schema) {
  for (auto &field : schema) {
    if (field->is_pointer() || field->is_store()) {
      const TypeInfo &type = field->pointer_type();
      const auto it = seen.find(type);
      if (it == seen.end()) {
        std::stringstream ss;
        ss << "Type '" << type << "' is used by field '" << schema.name << "::" << field->name();
        ss << "', but it is not registered in the type registry";
        throw DependencyException(ss.str());
      }
    }
  }
}


void
TypeRegistry::finalise(void) {
  if (_finalised)
    return;

  // find all of the types stored in the registry
  std::set<TypeInfo> seen;
  seen.insert(_doc_schema->type);
  for (auto &schema : _schemas)
    seen.insert(schema->type);

  // find all pointer fields
  _finalise(seen, *_doc_schema);
  for (auto &schema : _schemas)
    _finalise(seen, *schema);

  _finalised = true;
}

} }
