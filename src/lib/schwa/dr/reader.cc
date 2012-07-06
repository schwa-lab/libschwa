/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>

namespace mp = schwa::msgpack;


namespace schwa { namespace dr {

class WireField {
public:
  const BaseFieldDef *field;
  size_t index;
  size_t store_id;
  bool is_pointer;

  WireField(const BaseFieldDef *field, size_t index, size_t store_id, size_t is_pointer) : field(field), index(index), store_id(store_id), is_pointer(is_pointer) { }
  WireField(const WireField &o) : field(o.field), index(o.index), store_id(o.store_id), is_pointer(o.is_pointer) { }
  WireField(const WireField &&o) : field(o.field), index(o.index), store_id(o.store_id), is_pointer(o.is_pointer) { }
  WireField &operator =(const WireField &o) {
    field = o.field;
    index = o.index;
    store_id = o.store_id;
    is_pointer = o.is_pointer;
    return *this;
  }
};


class WireKlass {
private:
  const BaseSchema *_klass;
  std::vector<WireField> _fields;

public:
  WireKlass(const BaseSchema *klass) : _klass(klass) { }
  WireKlass(const WireKlass &o) : _klass(o._klass), _fields(o._fields) { }
  WireKlass(const WireKlass &&o) : _klass(o._klass), _fields(o._fields) { }
  WireKlass &operator =(const WireKlass &o) {
    _klass = o._klass;
    _fields = o._fields;
    return *this;
  }

  inline const BaseSchema *klass(void) const { return _klass; }
  inline const std::vector<WireField> &fields(void) const { return _fields; }

  inline void add_field(WireField f) { _fields.push_back(f); }

  inline WireField &operator [](size_t index) { return _fields[index]; }
};


Reader &
Reader::read(Doc &doc) {
  if (_in.peek() == EOF || _in.eof()) {
    _has_more = false;
    return *this;
  }

  // map of each of the registered types
  std::map<std::string, const BaseSchema *> klass_name_map;
  klass_name_map["__meta__"] = &_dschema;
  for (auto &s : _dschema.schemas())
    klass_name_map[s->serial] = s;

  // keep track of the klass_id of __meta__
  bool found_klass_id_meta = false;
  size_t klass_id_meta;

  // read the klasses header
  // <klasses> ::= [ <klass> ]
  const size_t nklasses = mp::read_array_size(_in);
  std::vector<WireKlass> klasses;

  for (size_t k = 0; k != nklasses; ++k) {
    // <klass> ::= ( <klass_name>, <fields> )
    const size_t npair = mp::read_array_size(_in);
    if (npair != 2) {
      std::stringstream ss;
      ss << "Invalid sized tuple read in: expected 2 elements but found " << npair;
      throw ReaderException(ss.str());
    }

    // read in the class name and check that we have a registered class with this name
    const std::string klass_name = mp::read_raw(_in);
    const auto &kit = klass_name_map.find(klass_name);
    if (kit == klass_name_map.end()) {
      std::stringstream ss;
      ss << "Class name '" << klass_name << "' found on input stream, but no known class matches that name";
      throw ReaderException(ss.str());
    };
    const BaseSchema *const schema = kit->second;
    klasses.push_back(WireKlass(schema));

    // keep track of the klass_id of __meta__
    if (klass_name == "__meta__") {
      found_klass_id_meta = true;
      klass_id_meta = k;
    }

    // <fields> ::= [ <field> ]
    const size_t nfields = mp::read_array_size(_in);

    for (size_t f = 0; f != nfields; ++f) {
      std::string field_name;
      size_t store_id = 0;
      bool is_pointer = false, is_slice = false;

      // <field> ::= { <field_type> : <field_val> }
      const size_t nitems = mp::read_map_size(_in);
      for (size_t i = 0; i != nitems; ++i) {
        const uint8_t key = mp::read_uint_fixed(_in);
        switch (key) {
        case 0: field_name = mp::read_raw(_in); break;
        case 1: store_id = mp::read_uint(_in); is_pointer = true; break;
        case 2: is_slice = mp::read_bool(_in); break;
        default:
          std::stringstream ss;
          ss << "Unknown value " << static_cast<unsigned int>(key) << " as key in <field> map";
          throw ReaderException(ss.str());
        }
      }

      // see if the read in field exists on the registered class's schema
      const BaseFieldDef *field = nullptr;
      size_t index = 0;
      for (auto &f : schema->fields()) {
        if (f->serial == field_name) {
          field = f;
          break;
        }
        ++index;
      }
      if (field == nullptr) {
        std::stringstream ss;
        ss << "Field name '" << field_name << "' found on the input stream does not exist on the class '" << klass_name << "'";
        throw ReaderException(ss.str());
      }

      // perform some sanity checks that the type of data on the stream is what we're expecting
      if (is_pointer && !field->is_pointer) {
        std::stringstream ss;
        ss << "Field '" << field_name << "' of class '" << klass_name << "' is marked as IS_POINTER on the stream, but not on the class's field";
        throw ReaderException(ss.str());
      }
      if (is_slice && !field->is_slice) {
        std::stringstream ss;
        ss << "Field '" << field_name << "' of class '" << klass_name << "' is marked as IS_SLICE on the stream, but not on the class's field";
        throw ReaderException(ss.str());
      }

      klasses.back().add_field(WireField(field, index, store_id, is_pointer));
    } // for each field

  } // for each klass

  if (!found_klass_id_meta)
    throw ReaderException("Did not read in a __meta__ class");


  // read the stores header
  // <stores> ::= [ <store> ]
  const size_t nstores = mp::read_array_size(_in);
  std::vector<std::pair<const BaseStoreDef *, size_t>> stores;
  stores.reserve(nstores);

  for (size_t n = 0; n != nstores; ++n) {
    // <store> ::= ( <store_name>, <klass_id>, <store_nelem> )
    const size_t ntriple = mp::read_array_size(_in);
    if (ntriple != 3) {
      std::stringstream ss;
      ss << "Invalid sized tuple read in: expected 3 elements but found " << ntriple;
      throw ReaderException(ss.str());
    }
    const std::string store_name = mp::read_raw(_in);
    const size_t klass_id = mp::read_uint(_in);
    const size_t nitems = mp::read_uint(_in);

    // lookup the store on the Doc class
    const BaseStoreDef *store = nullptr;
    for (auto &s : _dschema.stores()) {
      if (s->serial == store_name) {
        store = s;
        break;
      }
    }
    if (store == nullptr) {
      std::stringstream ss;
      ss << "Store name '" << store_name << "' found on the input stream does not exist on the document '" << _dschema.name << "'";
      throw ReaderException(ss.str());
    }

    // sanity check on the value of the klass_id
    if (klass_id >= klasses.size()) {
      std::stringstream ss;
      ss << "klass_id value " << klass_id << " >= number of klasses (" << klasses.size() << ")";
      throw ReaderException(ss.str());
    }

    // ensure that the stream store and the static store agree on the klass they're storing
    const TypeInfo &store_ptr_type = store->pointer_type();
    const TypeInfo &klass_ptr_type = klasses[klass_id].klass()->type;
    if (store_ptr_type != klass_ptr_type) {
      std::stringstream ss;
      ss << "Store '" << store_name << "' points to " << store_ptr_type << " but the stream says it points to " << klass_ptr_type;
      throw ReaderException(ss.str());
    }

    // resize the store to house the correct number of instances
    store->resize(doc, nitems);
    stores.push_back(std::make_pair(store, klass_id));
  }


  // sanity check each of the pointer fields point to valid stores
  for (auto &klass : klasses)
    for (auto &field : klass.fields())
      if (field.is_pointer) {
        // sanity check on the value of store_id
        if (field.store_id >= stores.size()) {
          std::stringstream ss;
          ss << "store_id value " << field.store_id << " >= number of stores (" << stores.size() << ")";
          throw ReaderException(ss.str());
        }

        const ptrdiff_t field_store_offset = field.field->store_offset(nullptr);
        const ptrdiff_t store_store_offset = stores[field.store_id].first->store_offset(nullptr);
        if (field_store_offset != store_store_offset) {
          std::stringstream ss;
          ss << "field_store_offset (" << field_store_offset << ") != store_store_offset (" << store_store_offset << ")";
          throw ReaderException(ss.str());
        }
      }


  // read the document instance
  // <doc_instance> ::= <instances_nbytes> <instance>
  {
    const size_t nbytes = mp::read_uint(_in);
    static_cast<void>(nbytes); // unused

    // <instance> ::= { <field_id> : <obj_val> }
    auto readers = _dschema.readers();
    const size_t size = mp::read_map_size(_in);
    for (size_t i = 0; i != size; ++i) {
      const size_t key = mp::read_uint(_in);
      const size_t index = klasses[klass_id_meta][key].index;
      readers[index](_in, static_cast<void *>(&doc), static_cast<void *>(&doc));
    }
  }


  // read the store instances
  // <instances_groups> ::= <instances_group>*
  for (auto &pair : stores) {
    // <instances_group>  ::= <instances_nbytes> <instances>
    const size_t nbytes = mp::read_uint(_in);
    static_cast<void>(nbytes); // unused

    const BaseStoreDef *const schema = pair.first;
    const size_t klass_id = pair.second;

    const size_t delta = schema->read_size();
    char *objects = schema->read_begin(doc);
    auto readers = klasses[klass_id].klass()->readers();

    // <instances> ::= [ <instance> ]
    const size_t ninstances = mp::read_array_size(_in);
    for (size_t i = 0; i != ninstances; ++i) {
      // <instance> ::= { <field_id> : <obj_val> }
      const size_t size = mp::read_map_size(_in);
      for (size_t j = 0; j != size; ++j) {
        const size_t key = mp::read_uint(_in);
        const size_t index = klasses[klass_id][key].index;
        readers[index](_in, objects, static_cast<void *>(&doc));
      }

      objects += delta;
    }
  }

  _has_more = true;
  return *this;
}

} }
