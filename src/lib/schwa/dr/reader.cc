/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>

namespace mp = schwa::msgpack;


namespace schwa { namespace dr {

#if 0
static void
read_instance(std::istream &in, Document &doc, const BaseSchema &schema, const void *const obj) {
  const size_t nfields = mp::read_map_header(in);
  for (size_t i = 0; i != nfields; ++i) {
    const size_t key = mp::read_uint(in);

  }
}
#endif


Reader &
Reader::read(Document &doc) {
  if (_in.eof()) {
    _has_more = false;
    return *this;
  }

  // map of each of the registered types
  std::map<std::string, const BaseSchema *> klass_name_map;
  klass_name_map["__meta__"] = &_dschema;
  for (auto &s : _dschema.schemas())
    klass_name_map[s->serial] = s;


  // read the klasses header
  // <klasses> ::= [ <klass> ]
  const size_t nklasses = mp::read_array_header(_in);
  std::vector<const BaseSchema *> klasses;
  std::vector<std::vector<const BaseFieldDef *>> klass_fields;
  klasses.reserve(nklasses);
  klass_fields.resize(nklasses);

  for (size_t k = 0; k != nklasses; ++k) {
    // <klass> ::= ( <klass_name>, <fields> )
    const size_t npair = mp::read_array_header(_in);
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
    klasses.push_back(schema);

    // <fields> ::= [ <field> ]
    const size_t nfields = mp::read_array_header(_in);

    for (size_t f = 0; f != nfields; ++f) {
      std::string field_name;
      size_t store_id;
      bool is_pointer = false, is_slice = false;

      // <field> ::= { <field_type> : <field_val> }
      const size_t nitems = mp::read_map_header(_in);
      for (size_t i = 0; i != nitems; ++i) {
        const uint8_t key = mp::read_uint_fixed(_in);
        switch (key) {
        case 0: field_name = mp::read_raw(_in); break;
        case 1: store_id = mp::read_uint(_in); is_pointer = true; break;
        case 2: is_slice = mp::read_boolean(_in); break;
        default:
          std::stringstream ss;
          ss << "Unknown value " << static_cast<unsigned int>(key) << " as key in <field> map";
          throw ReaderException(ss.str());
        }
      }

      // see if the read in field exists on the registered class's schema
      const BaseFieldDef *field = nullptr;
      for (auto &f : schema->fields()) {
        if (f->serial == field_name) {
          field = f;
          break;
        }
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

      klass_fields[k].push_back(field);
    } // for each field

  } // for each klass



  // read the stores header
  // <stores> ::= [ <store> ]
  const size_t nstores = mp::read_array_header(_in);
  for (size_t n = 0; n != nstores; ++n) {
    // <store> ::= ( <store_name>, <klass_id>, <store_nelem> )
    const size_t ntriple = mp::read_array_header(_in);
    if (ntriple != 3) {
      std::stringstream ss;
      ss << "Invalid sized tuple read in: expected 3 elements but found " << ntriple;
      throw ReaderException(ss.str());
    }
    const std::string store_name = mp::read_raw(_in);
    const size_t klass_id = mp::read_uint(_in);
    const size_t nitems = mp::read_uint(_in);

    // lookup the store on the Document class
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
    const TypeInfo &klass_ptr_type = klasses[klass_id]->type;
    if (store_ptr_type != klass_ptr_type) {
      std::stringstream ss;
      ss << "Store '" << store_name << "' points to " << store_ptr_type << " but the stream says it points to " << klass_ptr_type;
      throw ReaderException(ss.str());
    }

    // resize the store to house the correct number of instances
    store->resize(doc, nitems);
    std::cout << "resizing " << store_name << " to " << nitems << std::endl;
  }


  // read the document instance
  // <doc_instance> ::= <instances_nbytes> <instance>
  const size_t doc_nbytes = mp::read_uint(_in);
  (void)doc_nbytes;


  _has_more = true;
  return *this;
}

} }
