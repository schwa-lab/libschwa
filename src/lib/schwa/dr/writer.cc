/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/dr.h>

#include <boost/smart_ptr.hpp>

namespace mp = schwa::msgpack;


namespace schwa { namespace dr {

struct WireKlass {
  Schema *schema;
  size_t klass_id;

  WireKlass(Schema *schema=nullptr, size_t klass_id=0) : schema(schema), klass_id(klass_id) { }

  inline std::string
  name(void) const {
    return schema->is_document() ? "__meta__" : schema->serial();
  }
};


void
Writer::write(const Document &d) {
  std::cout << "Writer::write(" << &d << ")" << std::endl;

  const size_t NKLASSES = 1 + _reg.size();
  boost::scoped_array<WireKlass> klasses(new WireKlass[NKLASSES]);

  // setup each of the WireKlasses
  size_t klass_id = 1;
  klasses[0].schema = &_reg.doc_schema();
  for (auto &it : _reg) {
    klasses[klass_id].schema = it;
    klasses[klass_id].klass_id = klass_id;
    ++klass_id;
  }

  for (size_t i = 0; i != NKLASSES; ++i) {
    std::cout << klasses[i].name() << " " << klasses[i].schema->type().name << std::endl;
    for (auto &field : *klasses[i].schema) {
      std::cout << "  " << field->name() << " " << field->is_pointer() << " " << field->is_store() << " " << field->is_slice();
      if (field->is_pointer())
        std::cout << " " << field->pointer_type().name;
      std::cout << std::endl;
    }
  }

}

} }
