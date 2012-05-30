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


template <typename T>
static void
write_instance(std::ostream &out, const T &obj, const BaseSchema &schema) {
  size_t nfields = schema.fields().size(); // TODO this needs to be computed to account not writing out default fields
  mp::write_map_header(out, nfields);
  for (auto &field : schema.fields()) {
    // TODO
    (void)obj;
    (void)field;
  }
}


void
Writer::write(const Document &doc) {
  std::cout << "Writer::write(" << &doc << ")" << std::endl;

  // map each of the types to their unique klass id within the header
  std::map<TypeInfo, size_t> type_map;
  size_t type_id = 0;
  type_map[_dschema.type] = type_id++;
  for (auto &s : _dschema.schemas()) {
    const TypeInfo &type = s->type;
    assert(type_map.find(type) == type_map.end());
    type_map[type] = type_id++;
  }

  // <klasses> ::= [ <klass> ]
  mp::write_array_header(_out, type_map.size());
  write_klass_header(_out, _dschema, true, type_map);
  for (auto &s : _dschema.schemas())
    write_klass_header(_out, *s, false, type_map);

  // <stores> ::= [ <store> ]
  mp::write_array_header(_out, _dschema.stores().size());

  // <store> ::= ( <store_name>, <type_id>, <store_nelem> )
  for (auto &store : _dschema.stores()) {
    const auto it = type_map.find(store->pointer_type());
    assert(it != type_map.end());

    mp::write_array_header(_out, 3);
    mp::write_raw(_out, store->serial.c_str(), store->serial.size());
    mp::write_uint(_out, it->second);
    mp::write_uint(_out, store->size(doc));
  }

  // <doc_instance> ::= <instances_nbytes> <instance>
  std::stringstream ss;
  write_instance(ss, doc, _dschema);
  mp::write_uint(_out, ss.tellp());
  _out << ss.rdbuf();

  // <instances_groups> ::= <instances_group>*
  for (auto &store : _dschema.stores()) {
    // reset the stringstream
    ss.str("");
    ss.clear();

    const auto it = type_map.find(store->pointer_type());
    (void)it;

    // <instances_group> ::= <instances_nbytes> <instances>
  }

  std::cout << _dschema << std::endl;

  // flush since we've finished writing a whole document
  _out.flush();
}

} }
