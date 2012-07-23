/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>

namespace mp = schwa::msgpack;


namespace schwa { namespace dr {

static void
write_instance(io::WriteBuffer &out, const Doc &doc, const RTSchema &schema, const void *const obj) {
  io::WriteBuffer buf;
  const Lazy *const lazy_obj = reinterpret_cast<const Lazy *>(obj);

  uint32_t nfields_new = 0;
  for (auto &field : schema.fields) {
    if (!field->is_lazy() && field->def->mode == FieldMode::READ_WRITE) {
      const bool wrote = field->def->writer(buf, field->field_id, obj, static_cast<const void *>(&doc));
      if (wrote)
        ++nfields_new;
    }
  }

  const uint32_t nfields = nfields_new + lazy_obj->lazy_nelem();
  mp::write_map_size(out, nfields);
  if (nfields != 0) {
    if (lazy_obj->lazy_nbytes() != 0)
      out.write(lazy_obj->lazy_data(), lazy_obj->lazy_nbytes());
    out.copy_from(buf);
  }
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
      const uint32_t nelem = 1 + (field->pointer != nullptr) + field->is_slice;
      mp::write_map_size(_out, nelem);

      // <field_type> ::= 0 # NAME => the name of the field
      mp::write_uint_fixed(_out, 0);
      mp::write_raw(_out, field->is_lazy() ? field->serial : field->def->serial);

      // <field_type> ::= 1 # POINTER_TO => the <store_id> that this field points into
      if (field->pointer != nullptr) {
        mp::write_uint_fixed(_out, 1);
        mp::write_uint(_out, field->pointer->store_id);
      }

      // <field_type> ::= 2 # IS_SLICE => whether or not this field is a "Slice" field
      if (field->is_slice) {
        mp::write_uint_fixed(_out, 2);
        mp::write_bool(_out, true);
      }
    } // for each field
  } // for each klass


  // <stores> ::= [ <store> ]
  mp::write_array_size(_out, rtdschema->stores.size());
  for (auto &store : rtdschema->stores) {
    // <store> ::= ( <store_name>, <type_id>, <store_nelem> )
    mp::write_array_size(_out, 3);
    mp::write_raw(_out, store->is_lazy() ? store->serial : store->def->serial);
    mp::write_uint(_out, store->klass->klass_id);
    mp::write_uint(_out, store->is_lazy() ? store->lazy_nelem : store->def->size(doc));
  }

  // <doc_instance> ::= <instances_nbytes> <instance>
  {
    io::WriteBuffer buf;
    write_instance(buf, doc, *rtdschema, &doc);
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
      store->def->write(buf, doc, *store->klass, &write_instance);
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
