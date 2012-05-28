/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/dr.h>

namespace mp = schwa::msgpack;


namespace schwa { namespace dr {

static void
debug_schema(const Schema &s, const std::map<TypeInfo, size_t> &klass_map) {
  std::cout << (s.is_document() ? "__meta__" : s.serial()) << " " << s.type().name << std::endl;
  for (auto &f : s) {
    std::cout << "  " << f->name() << " " << f->is_pointer() << " " << f->is_store() << " " << f->is_slice();
    if (f->is_pointer()) {
      std::cout << " " << f->pointer_type().name;
      const auto it = klass_map.find(f->pointer_type());
      if (it == klass_map.end())
        std::cout << " <unknown!>";
      else
        std::cout << " " << it->second;
    }
    std::cout << std::endl;
  }
}


void
Writer::write(const Document &d) {
  std::cout << "Writer::write(" << &d << ")" << std::endl;
  _reg.finalise();

  std::map<TypeInfo, size_t> klass_map;

  size_t klass_id = 0;
  klass_map[_reg.doc_schema().type()] = klass_id++;
  for (auto &it : _reg) {
    const TypeInfo &type = it->type();
    const auto i = klass_map.find(type);
    if (i == klass_map.end())
      klass_map[type] = klass_id++;
  }

  debug_schema(_reg.doc_schema(), klass_map);
  for (auto &s : _reg)
    debug_schema(*s, klass_map);

}

} }
