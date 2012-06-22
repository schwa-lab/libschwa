/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>


namespace schwa { namespace dr {

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
BaseAnnotationSchema::dump(std::ostream &out) const {
  out << serial << ' ' << type << std::endl;
  return BaseSchema::dump(out);
}


std::ostream &
BaseDocumentSchema::dump(std::ostream &out) const {
  out << "__meta__ " << type << std::endl;
  BaseSchema::dump(out);
  for (auto &schema : _schemas)
    out << *schema;
  return out;
}

} }
