/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/dr.h>

namespace mp = schwa::msgpack;


namespace schwa { namespace dr {

static void
debug_schema(const BaseSchema &s, const bool is_doc_schema, const std::map<TypeInfo, size_t> &klass_map) {
  std::cout << (is_doc_schema ? "__meta__" : s.serial) << " " << s.type << std::endl;
  for (auto &f : s.fields()) {
    std::cout << "  " << f->name << " " << f->is_pointer << " " << f->is_slice;
    if (f->is_pointer) {
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
Writer::write_klass_header(const BaseSchema &s, const bool is_doc_schema, const std::map<TypeInfo, size_t> &types) {
  // <klass> ::= ( <klass_name>, <fields> )
  mp::write_array_header(_out, 2);

  // <klass_name>
  const std::string name = is_doc_schema ? "__meta__" : s.serial;
  mp::write_raw(_out, name.c_str(), name.size());

  // <fields> ::= [ <field> ]
  mp::write_array_header(_out, s.fields().size());

  for (auto &field : s.fields()) {
    // <field> ::= { <field_type> : <field_val> }
    const size_t nitems = 1 + field->is_pointer + field->is_slice;
    mp::write_map_header(_out, nitems);

    // <field_type> ::= 0 # NAME => the name of the field
    mp::write_uint_fixed(_out, 0);
    mp::write_raw(_out, field->serial.c_str(), field->serial.size());

    // <field_type> ::= 1 # POINTER_TO => the <klass_id> that this type points to
    if (field->is_pointer) {
      const auto it = types.find(s.type);
      assert(it != types.end());
      mp::write_uint_fixed(_out, 1);
      mp::write_uint(_out, it->second);
    }

    // <field_type> ::= 2 # IS_SLICE => whether or not this field is a "Slice" field
    if (field->is_slice) {
      mp::write_uint_fixed(_out, 2);
      mp::write_boolean(_out, true);
    }
  }
}


void
Writer::write(const Document &doc) {
  std::cout << "Writer::write(" << &doc << ")" << std::endl;

  // map each of the types to their unique klass id within the header
  std::map<TypeInfo, size_t> klass_map;
  size_t klass_id = 0;
  klass_map[_dschema.type] = klass_id++;
  for (auto &s : _dschema.schemas()) {
    const TypeInfo &type = s->type;
    assert(klass_map.find(type) == klass_map.end());
    klass_map[type] = klass_id++;
  }

  // <klasses> ::= [ <klass> ]
  mp::write_array_header(_out, klass_map.size());
  write_klass_header(_dschema, true, klass_map);
  for (auto &s : _dschema.schemas())
    write_klass_header(*s, false, klass_map);

  // <stores> ::= [ <store> ]
  mp::write_array_header(_out, _dschema.stores().size());

  // <store> ::= ( <store_name>, <klass_id>, <store_nelem> )
  for (auto &store : _dschema.stores()) {
    const auto it = klass_map.find(store->pointer_type());
    assert(it != klass_map.end());

    mp::write_array_header(_out, 3);
    mp::write_raw(_out, store->serial.c_str(), store->serial.size());
    mp::write_uint(_out, it->second);
    mp::write_uint(_out, store->size(doc));
  }

  // <doc_instance> ::= <instances_nbytes> <instance>
  // TODO

  std::cout << std::endl;
  debug_schema(_dschema, true, klass_map);
  for (auto &s : _dschema.schemas())
    debug_schema(*s, false, klass_map);

  // flush since we've finished writing a whole document
  _out.flush();
}

} }
