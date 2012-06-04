/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/base.h>
#include <schwa/msgpack.h>
#include <schwa/dr.h>

namespace mp = schwa::msgpack;


namespace schwa { namespace dr {

static void
write_klass_header(std::ostream &out, const BaseSchema &s, const bool is_doc_schema, const std::map<TypeInfo, size_t> &types) {
  // <klass> ::= ( <klass_name>, <fields> )
  mp::write_array_header(out, 2);

  // <klass_name>
  const std::string name = is_doc_schema ? "__meta__" : s.serial;
  mp::write_raw(out, name.c_str(), name.size());

  // <fields> ::= [ <field> ]
  mp::write_array_header(out, s.fields().size());

  for (auto &field : s.fields()) {
    // <field> ::= { <field_type> : <field_val> }
    const size_t nitems = 1 + field->is_pointer + field->is_slice;
    mp::write_map_header(out, nitems);

    // <field_type> ::= 0 # NAME => the name of the field
    mp::write_uint_fixed(out, 0);
    mp::write_raw(out, field->serial.c_str(), field->serial.size());

    // <field_type> ::= 1 # POINTER_TO => the <type_id> that this type points to
    if (field->is_pointer) {
      const auto it = types.find(s.type);
      assert(it != types.end());
      mp::write_uint_fixed(out, 1);
      mp::write_uint(out, it->second);
    }

    // <field_type> ::= 2 # IS_SLICE => whether or not this field is a "Slice" field
    if (field->is_slice) {
      mp::write_uint_fixed(out, 2);
      mp::write_boolean(out, true);
    }
  }
}


static void
write_instance(std::ostream &out, const Document &doc, const BaseSchema &schema, const void *const obj) {
  std::stringstream ss;
  size_t nfields = 0;
  unsigned int key = 0;
  for (auto &writer : schema.writers()) {
    const bool wrote = writer(ss, key, obj, static_cast<const void *>(&doc));
    if (wrote)
      ++nfields;
    ++key;
  }

  mp::write_map_header(out, nfields);
  out << ss.rdbuf();
}


void
Writer::write(const Document &doc) {
  std::cout << "Writer::write(" << &doc << ")" << std::endl;

  // map each of the types to their unique klass id within the header
  std::map<TypeInfo, size_t> typeid_map;
  std::map<TypeInfo, const BaseSchema *> schema_map;
  size_t type_id = 0;
  typeid_map[_dschema.type] = type_id++;
  for (auto &s : _dschema.schemas()) {
    const TypeInfo &type = s->type;
    assert(typeid_map.find(type) == typeid_map.end());
    typeid_map[type] = type_id++;
    schema_map[type] = s;
  }

  // <klasses> ::= [ <klass> ]
  mp::write_array_header(_out, typeid_map.size());
  write_klass_header(_out, _dschema, true, typeid_map);
  for (auto &s : _dschema.schemas())
    write_klass_header(_out, *s, false, typeid_map);

  // <stores> ::= [ <store> ]
  mp::write_array_header(_out, _dschema.stores().size());

  // <store> ::= ( <store_name>, <type_id>, <store_nelem> )
  for (auto &store : _dschema.stores()) {
    const auto it = typeid_map.find(store->pointer_type());
    assert(it != typeid_map.end());

    mp::write_array_header(_out, 3);
    mp::write_raw(_out, store->serial.c_str(), store->serial.size());
    mp::write_uint(_out, it->second);
    mp::write_uint(_out, store->size(doc));
  }

  // <doc_instance> ::= <instances_nbytes> <instance>
  std::stringstream ss;
  write_instance(ss, doc, _dschema, &doc);
  mp::write_uint(_out, ss.tellp());
  _out << ss.rdbuf();

  // <instances_groups> ::= <instances_group>*
  for (auto &store : _dschema.stores()) {
    // reset the stringstream
    ss.str("");
    ss.clear();

    auto it = schema_map.find(store->pointer_type());
    assert(it != schema_map.end());
    store->write(ss, doc, *(it->second), &write_instance);

    // <instances_group> ::= <instances_nbytes> <instances>
    mp::write_uint(_out, ss.tellp());
    _out << ss.rdbuf();
  }

  //std::cout << _dschema << std::endl;

  // flush since we've finished writing a whole document
  _out.flush();
}

} }
