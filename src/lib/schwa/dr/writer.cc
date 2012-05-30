/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/dr.h>

namespace mp = schwa::msgpack;


namespace schwa { namespace dr {

static void
debug_schema(const BaseSchema &s, const std::map<TypeInfo, size_t> &klass_map) {
  std::cout << (s.is_document_schema ? "__meta__" : s.serial) << " " << s.type << std::endl;
  for (auto &f : s) {
    std::cout << "  " << f->name << " " << f->is_pointer() << " " << f->is_store() << " " << f->is_slice();
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
Writer::write_klass_header(const BaseSchema &s, const std::map<TypeInfo, size_t> &types) {
  // <klass> ::= ( <klass_name>, <fields> )
  mp::write_array_header(_out, 2);

  // <klass_name>
  const std::string name = s.is_document_schema ? "__meta__" : s.serial;
  mp::write_raw(_out, name.c_str(), name.size());

  // <fields> ::= [ <field> ]
  size_t nfields = 0;
  for (auto &field : s)
    if (!field->is_store())
      ++nfields;
  mp::write_array_header(_out, nfields);

  for (auto &field : s) {
    if (field->is_store())
      continue;

    // <field> ::= { <field_type> : <field_val> }
    const size_t nitems = 1 + field->is_pointer() + field->is_slice();
    mp::write_map_header(_out, nitems);

    // <field_type> ::= 0 # NAME => the name of the field
    mp::write_uint_fixed(_out, 0);
    mp::write_raw(_out, field->serial.c_str(), field->serial.size());

    // <field_type> ::= 1 # POINTER_TO => the <klass_id> that this type points to
    if (field->is_pointer()) {
      const auto it = types.find(s.type);
      assert(it != types.end());
      mp::write_uint_fixed(_out, 1);
      mp::write_uint(_out, it->second);
    }

    // <field_type> ::= 2 # IS_SLICE => whether or not this field is a "Slice" field
    if (field->is_slice()) {
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
  write_klass_header(_dschema, klass_map);
  for (auto &s : _dschema.schemas())
    write_klass_header(*s, klass_map);

  // <stores> ::= [ <store> ]
  size_t nstores = 0;
  for (auto &field : _dschema)
    if (field->is_store())
      ++nstores;
  mp::write_array_header(_out, nstores);

  // <store> ::= ( <store_name>, <klass_id>, <store_nelem> )
  for (auto &field : _dschema) {
    if (!field->is_store())
      continue;

    const auto it = klass_map.find(field->pointer_type());
    assert(it != klass_map.end());

    mp::write_array_header(_out, 3);
    mp::write_raw(_out, field->serial.c_str(), field->serial.size());
    mp::write_uint(_out, it->second);
    mp::write_uint(_out, field->store_size(&doc));
  }

  // <doc_instance> ::= <instances_nbytes> <instance>
  // TODO

  std::cout << std::endl;
  debug_schema(_dschema, klass_map);
  for (auto &s : _dschema.schemas())
    debug_schema(*s, klass_map);

  // flush since we've finished writing a whole document
  _out.flush();
}

} }
