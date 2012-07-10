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
  FieldMode mode;

  explicit WireField(size_t index) : field(nullptr), index(index), store_id(0), is_pointer(false), mode(FieldMode::STREAM_ONLY) { }
  WireField(const BaseFieldDef *field, size_t index, size_t store_id, size_t is_pointer) : field(field), index(index), store_id(store_id), is_pointer(is_pointer), mode(field->mode) { }
  WireField(const WireField &o) : field(o.field), index(o.index), store_id(o.store_id), is_pointer(o.is_pointer), mode(o.mode) { }
  WireField(const WireField &&o) : field(o.field), index(o.index), store_id(o.store_id), is_pointer(o.is_pointer), mode(o.mode) { }
  WireField &operator =(const WireField &o) {
    field = o.field;
    index = o.index;
    store_id = o.store_id;
    is_pointer = o.is_pointer;
    mode = o.mode;
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
      std::stringstream msg;
      msg << "Invalid sized tuple read in: expected 2 elements but found " << npair;
      throw ReaderException(msg.str());
    }

    // read in the class name and check that we have a registered class with this name
    const std::string klass_name = mp::read_raw(_in);
    const auto &kit = klass_name_map.find(klass_name);
    if (kit == klass_name_map.end()) {
      std::stringstream msg;
      msg << "Class name '" << klass_name << "' found on input stream, but no known class matches that name";
      throw ReaderException(msg.str());
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
          std::stringstream msg;
          msg << "Unknown value " << static_cast<unsigned int>(key) << " as key in <field> map";
          throw ReaderException(msg.str());
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
#if 0
      if (field == nullptr) {
        std::stringstream msg;
        msg << "Field name '" << field_name << "' found on the input stream does not exist on the class '" << klass_name << "'";
        throw ReaderException(msg.str());
      }
#endif

      if (field == nullptr)
        klasses.back().add_field(WireField(index));
      else {
        // perform some sanity checks that the type of data on the stream is what we're expecting
        if (is_pointer && !field->is_pointer) {
          std::stringstream msg;
          msg << "Field '" << field_name << "' of class '" << klass_name << "' is marked as IS_POINTER on the stream, but not on the class's field";
          throw ReaderException(msg.str());
        }
        if (is_slice && !field->is_slice) {
          std::stringstream msg;
          msg << "Field '" << field_name << "' of class '" << klass_name << "' is marked as IS_SLICE on the stream, but not on the class's field";
          throw ReaderException(msg.str());
        }
        klasses.back().add_field(WireField(field, index, store_id, is_pointer));
      }
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
      std::stringstream msg;
      msg << "Invalid sized tuple read in: expected 3 elements but found " << ntriple;
      throw ReaderException(msg.str());
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
      std::stringstream msg;
      msg << "Store name '" << store_name << "' found on the input stream does not exist on the document '" << _dschema.name << "'";
      throw ReaderException(msg.str());
    }

    // sanity check on the value of the klass_id
    if (klass_id >= klasses.size()) {
      std::stringstream msg;
      msg << "klass_id value " << klass_id << " >= number of klasses (" << klasses.size() << ")";
      throw ReaderException(msg.str());
    }

    // ensure that the stream store and the static store agree on the klass they're storing
    const TypeInfo &store_ptr_type = store->pointer_type();
    const TypeInfo &klass_ptr_type = klasses[klass_id].klass()->type;
    if (store_ptr_type != klass_ptr_type) {
      std::stringstream msg;
      msg << "Store '" << store_name << "' points to " << store_ptr_type << " but the stream says it points to " << klass_ptr_type;
      throw ReaderException(msg.str());
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
          std::stringstream msg;
          msg << "store_id value " << field.store_id << " >= number of stores (" << stores.size() << ")";
          throw ReaderException(msg.str());
        }

        const ptrdiff_t field_store_offset = field.field->store_offset(nullptr);
        const ptrdiff_t store_store_offset = stores[field.store_id].first->store_offset(nullptr);
        if (field_store_offset != store_store_offset) {
          std::stringstream msg;
          msg << "field_store_offset (" << field_store_offset << ") != store_store_offset (" << store_store_offset << ")";
          throw ReaderException(msg.str());
        }
      }

  // buffer for reading the <instances> into
  const char *instances_bytes = nullptr;
  size_t instances_bytes_size = 0;

  // read the document instance
  // <doc_instance> ::= <instances_nbytes> <instance>
  {
    const size_t instances_nbytes = mp::read_uint(_in);

    // read in all of the instances data in one go into a buffer
    if (instances_nbytes > instances_bytes_size) {
      delete [] instances_bytes;
      instances_bytes = new char[instances_nbytes];
      instances_bytes_size = instances_nbytes;
      assert(instances_bytes != nullptr);
    }
    _in.read(const_cast<char *>(instances_bytes), instances_nbytes);
    if (!_in.good()) {
      std::stringstream msg;
      msg << "Failed to read in " << instances_nbytes << " from the input stream";
      throw ReaderException(msg.str());
    }

    // wrap the read in instances in a reader
    std::cout << "created reader to read " << instances_nbytes << " bytes" << std::endl;
    io::ArrayReader reader(instances_bytes, instances_nbytes);

    // allocate space to write the lazy attributes to
    char *const lazy_bytes = new char[instances_nbytes];
    assert(lazy_bytes != nullptr);
    uint32_t lazy_nfields = 0;

    // wrap the lazy bytes in a writer
    io::UnsafeArrayWriter lazy_writer(lazy_bytes);

    // <instance> ::= { <field_id> : <obj_val> }
    auto readers = _dschema.readers();
    const size_t size = mp::read_map_size(reader);
    for (size_t i = 0; i != size; ++i) {
      const size_t key = mp::read_uint(reader);
      WireField &field = klasses[klass_id_meta][key];

      // deserialize the field value if required
      const char *const before = reader.upto();
      if (field.mode != FieldMode::STREAM_ONLY)
        readers[field.index](reader, static_cast<void *>(&doc), static_cast<void *>(&doc));
      else {
        mp::WireType type;
        ++lazy_nfields;
        mp::write_uint(lazy_writer, key);
        mp::read_lazy(reader, lazy_writer, type);
      }
      const char *const after = reader.upto();

      // keep a lazy serialized copy of the field if required
      if (field.mode == FieldMode::READ_ONLY) {
        ++lazy_nfields;
        mp::write_uint(lazy_writer, key);
        lazy_writer.write(before, after - before);
      }
    }

    // check whether or not the lazy slab was used
    if (lazy_writer.data() == lazy_writer.upto())
      delete [] lazy_bytes;
    else {
      doc._lazy_buffers.push_back(lazy_bytes);
      doc._lazy = lazy_bytes;
      doc._lazy_nelem = lazy_nfields;
      doc._lazy_nbytes = lazy_writer.upto() - lazy_bytes;
    }
  }


  // read the store instances
  // <instances_groups> ::= <instances_group>*
  for (auto &pair : stores) {
    // <instances_group>  ::= <instances_nbytes> <instances>
    const size_t instances_nbytes = mp::read_uint(_in);
    std::cout << "[reader] reading <instances_groups> nbytes=" << instances_nbytes << std::endl;

    // read in all of the instances data in one go into a buffer
    if (instances_nbytes > instances_bytes_size) {
      delete [] instances_bytes;
      instances_bytes = new char[instances_nbytes];
      instances_bytes_size = instances_nbytes;
      assert(instances_bytes != nullptr);
    }
    _in.read(const_cast<char *>(instances_bytes), instances_nbytes);
    if (!_in.good()) {
      std::stringstream msg;
      msg << "Failed to read in " << instances_nbytes << " from the input stream";
      throw ReaderException(msg.str());
    }

    // wrap the read in instances in a reader
    io::ArrayReader reader(instances_bytes, instances_nbytes);

    // allocate space to write the lazy attributes to
    char *const lazy_bytes = new char[instances_nbytes];
    assert(lazy_bytes != nullptr);

    // wrap the lazy bytes in a writer
    io::UnsafeArrayWriter lazy_writer(lazy_bytes);


    const BaseStoreDef *const schema = pair.first;
    const size_t klass_id = pair.second;
    char *objects = schema->read_begin(doc);
    const size_t objects_delta = schema->read_size();
    auto readers = klasses[klass_id].klass()->readers();

    // <instances> ::= [ <instance> ]
    const size_t ninstances = mp::read_array_size(reader);
    for (size_t i = 0; i != ninstances; ++i) {
      const char *const lazy_before = lazy_writer.upto();
      uint32_t lazy_nfields = 0;

      // <instance> ::= { <field_id> : <obj_val> }
      const size_t size = mp::read_map_size(reader);
      for (size_t j = 0; j != size; ++j) {
        const size_t key = mp::read_uint(reader);
        WireField &field = klasses[klass_id][key];

        // deserialize the field value if required
        const char *const before = reader.upto();
        if (field.mode != FieldMode::STREAM_ONLY)
          readers[field.index](reader, objects, static_cast<void *>(&doc));
        else {
          mp::WireType type;
          ++lazy_nfields;
          mp::write_uint(lazy_writer, key);
          mp::read_lazy(reader, lazy_writer, type);
        }
        const char *const after = reader.upto();

        // keep a lazy serialized copy of the field if required
        if (field.mode == FieldMode::READ_ONLY) {
          ++lazy_nfields;
          mp::write_uint(lazy_writer, key);
          lazy_writer.write(before, after - before);
        }
      }

      // check whether or not the Ann instance used any of the lazy buffer
      const char *const lazy_after = lazy_writer.upto();
      if (lazy_after != lazy_before) {
        Ann &ann = *reinterpret_cast<Ann *>(objects);
        ann._lazy = lazy_before;
        ann._lazy_nelem = lazy_nfields;
        ann._lazy_nbytes = lazy_after - lazy_before;
      }

      // move on to the next annotation instance
      objects += objects_delta;
    }
  }

  delete [] instances_bytes;

  _has_more = true;
  return *this;
}

} }
