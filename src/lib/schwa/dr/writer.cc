/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>

namespace mp = schwa::msgpack;


namespace schwa { namespace dr {

Writer::Writer(std::ostream &out, BaseDocSchema &dschema) : _out(out), _dschema(dschema) { }

static void
write_lazy(io::WriteBuffer &out, const Lazy &lazy_obj, const uint32_t nfields_new, io::WriteBuffer &buf) {
  const uint32_t nfields = nfields_new + lazy_obj.lazy_nelem();
  mp::write_map_size(out, nfields);
  if (nfields != 0) {
    if (lazy_obj.lazy_nbytes() != 0)
      out.write(lazy_obj.lazy_data(), lazy_obj.lazy_nbytes());
    out.copy_from(buf);
  }
}


static void
write_instance(io::WriteBuffer &out, const Doc &doc, const RTSchema &schema) {
  io::WriteBuffer buf;
  uint32_t nfields_new = 0;
  for (auto &field : schema.fields) {
    if (!field->is_lazy() && field->def->mode == FieldMode::READ_WRITE) {
      const bool wrote = field->def->write_field(buf, field->field_id, doc);
      if (wrote)
        ++nfields_new;
    }
  }
  write_lazy(out, doc, nfields_new, buf);
}


static void
write_instance(io::WriteBuffer &out, const Ann &obj, const IStore &current_store, const Doc &doc, const RTSchema &schema) {
  io::WriteBuffer buf;
  uint32_t nfields_new = 0;
  for (auto &field : schema.fields) {
    if (!field->is_lazy() && field->def->mode == FieldMode::READ_WRITE) {
      const bool wrote = field->def->write_field(buf, field->field_id, obj, current_store, doc);
      if (wrote)
        ++nfields_new;
    }
  }
  write_lazy(out, obj, nfields_new, buf);
}


void
Writer::write(const Doc &doc) {
  // get or construct the RTManager for the document
  RTManager *rt;
  if (doc._rt == nullptr)
    rt = build_rt(_dschema);
  else
    rt = merge_rt(doc._rt, _dschema);
  const RTSchema *const rtdschema = rt->doc;

  // <wire_version>
  mp::write_uint(_out, WIRE_VERSION);

  // <klasses> ::= [ <klass> ]
  mp::write_array_size(_out, rt->klasses.size());
  for (auto &schema : rt->klasses) {
    // <klass> ::= ( <klass_name>, <fields> )
    mp::write_array_size(_out, 2);

    // <klass_name>
    if (schema == rt->doc)
      mp::write_raw(_out, "__meta__");
    else if (schema->is_lazy())
      mp::write_raw(_out, schema->serial);
    else
      mp::write_raw(_out, schema->def->serial);

    // <fields> ::= [ <field> ]
    mp::write_array_size(_out, schema->fields.size());
    for (auto &field : schema->fields) {
      // <field> ::= { <field_type> : <field_val> }
      const uint32_t nelem = 1 + (field->points_into != nullptr) + field->is_slice + field->is_self_pointer + field->is_collection;
      mp::write_map_size(_out, nelem);

      // <field_type> ::= 0 # NAME => the name of the field
      mp::write_uint_fixed(_out, to_underlying(wire::NAME));
      mp::write_raw(_out, field->is_lazy() ? field->serial : field->def->serial);

      // <field_type> ::= 1 # POINTER_TO => the <store_id> that this field points into
      if (field->points_into != nullptr) {
        mp::write_uint_fixed(_out, to_underlying(wire::POINTER_TO));
        mp::write_uint(_out, field->points_into->store_id);
      }

      // <field_type> ::= 2 # IS_SLICE => whether or not this field is a "Slice" field
      if (field->is_slice) {
        mp::write_uint_fixed(_out, to_underlying(wire::IS_SLICE));
        mp::write_nil(_out);
      }

      // <field_type>  ::= 3 # IS_SELF_POINTER => whether or not this field is a self-pointer. POINTER_TO and IS_SELF_POINTER are mutually exclusive.
      if (field->is_self_pointer) {
        mp::write_uint_fixed(_out, to_underlying(wire::IS_SELF_POINTER));
        mp::write_nil(_out);
      }

      // <field_type>  ::= 4 # IS_COLLECTION => whether or not this field is a collection. IS_COLLECTION and IS_SLICE are mutually exclusive.
      if (field->is_collection) {
        mp::write_uint_fixed(_out, to_underlying(wire::IS_COLLECTION));
        mp::write_nil(_out);
      }
    } // for each field
  } // for each klass


  // <stores> ::= [ <store> ]
  mp::write_array_size(_out, rtdschema->stores.size());
  for (auto &store : rtdschema->stores) {
    // <store> ::= ( <store_name>, <type_id>, <store_nelem> )
    mp::write_array_size(_out, 3);
    if (store->is_lazy()) {
      mp::write_raw(_out, store->serial);
      mp::write_uint(_out, store->klass->klass_id);
      mp::write_uint(_out, store->lazy_nelem);
    }
    else {
      IStore &istore = store->def->istore(doc);
      mp::write_raw(_out, store->def->serial);
      mp::write_uint(_out, store->klass->klass_id);
      mp::write_uint(_out, istore.nelem());
    }
  }

  // <doc_instance> ::= <instances_nbytes> <instance>
  {
    io::WriteBuffer buf;
    write_instance(buf, doc, *rtdschema);
    mp::write_uint(_out, buf.size());
    buf.copy_to(_out);
  }

  // <instances_groups> ::= <instances_group>*
  for (auto &store : rtdschema->stores) {
    // <instances_group> ::= <instances_nbytes> <instances>
    if (store->is_lazy()) {
      mp::write_uint(_out, store->lazy_nbytes);
      _out.write(store->lazy_data, store->lazy_nbytes);
    }
    else {
      io::WriteBuffer buf;

      IStore &istore = store->def->istore(doc);
      IStore::typeless_iterator istore_it = istore.typeless_begin();
      const size_t nelem = istore.nelem();

      // <instances> ::= [ <instance> ]
      msgpack::write_array_size(buf, nelem);
      for (size_t i = 0; i != nelem; ++i, ++istore_it) {
        Ann &ann = *istore_it;
        write_instance(buf, ann, istore, doc, *store->klass);
      }

      mp::write_uint(_out, buf.size());
      buf.copy_to(_out);
    }
  }

  // delete the temp RTManager
  if (doc._rt == nullptr)
    delete rt;

  // flush since we've finished writing a whole document
  _out.flush();
}

} }
