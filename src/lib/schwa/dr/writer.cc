/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>

namespace mp = schwa::msgpack;


namespace schwa { namespace dr {

static void
write_klass_header(std::ostream &out, const BaseSchema &s, const bool is_doc_schema, const Document &doc, const BaseDocumentSchema &dschema) {
  // <klass> ::= ( <klass_name>, <fields> )
  mp::write_array_size(out, 2);

  // <klass_name>
  const std::string name = is_doc_schema ? "__meta__" : s.serial;
  mp::write_raw(out, name);

  // <fields> ::= [ <field> ]
  mp::write_array_size(out, s.fields().size());

  for (auto &field : s.fields()) {
    // <field> ::= { <field_type> : <field_val> }
    const size_t nitems = 1 + field->is_pointer + field->is_slice;
    mp::write_map_size(out, nitems);

    // <field_type> ::= 0 # NAME => the name of the field
    mp::write_uint_fixed(out, 0);
    mp::write_raw(out, field->serial);

    // <field_type> ::= 1 # POINTER_TO => the <store_id> that this field points into
    if (field->is_pointer) {
      const ptrdiff_t field_store_offset = field->store_offset(doc);
      size_t store_id = 0;
      for (auto &store : dschema.stores()) {
        if (store->store_offset(doc) == field_store_offset)
          break;
        ++store_id;
      }
      assert(store_id != dschema.stores().size());

      mp::write_uint_fixed(out, 1);
      mp::write_uint(out, store_id);
    }

    // <field_type> ::= 2 # IS_SLICE => whether or not this field is a "Slice" field
    if (field->is_slice) {
      mp::write_uint_fixed(out, 2);
      mp::write_bool(out, true);
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

  mp::write_map_size(out, nfields);
  if (nfields != 0)
    out << ss.rdbuf();
}


void
Writer::write(const Document &doc) {
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
  mp::write_array_size(_out, typeid_map.size());
  write_klass_header(_out, _dschema, true, doc, _dschema);
  for (auto &s : _dschema.schemas())
    write_klass_header(_out, *s, false, doc, _dschema);

  // <stores> ::= [ <store> ]
  mp::write_array_size(_out, _dschema.stores().size());

  // <store> ::= ( <store_name>, <type_id>, <store_nelem> )
  for (auto &store : _dschema.stores()) {
    const auto it = typeid_map.find(store->pointer_type());
    assert(it != typeid_map.end());

    mp::write_array_size(_out, 3);
    mp::write_raw(_out, store->serial);
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

  // flush since we've finished writing a whole document
  _out.flush();
}

} }
