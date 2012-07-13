/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/dr.h>


namespace schwa { namespace dr {

// ============================================================================
// RTFieldDef
// ============================================================================
RTFieldDef::RTFieldDef(uint32_t field_id, const std::string &serial, const RTStoreDef *pointer, bool is_slice, const BaseFieldDef *def) : def(def), pointer(pointer), serial(serial), field_id(field_id), is_slice(is_slice) { }

RTFieldDef::RTFieldDef(const RTFieldDef &&o) : def(o.def), pointer(o.pointer), serial(o.serial), field_id(o.field_id), is_slice(o.is_slice) { }


// ============================================================================
// RTStoreDef
// ============================================================================
RTStoreDef::RTStoreDef(uint32_t store_id, const std::string &serial, const RTSchema *klass, const BaseStoreDef *def) : def(def), klass(klass), lazy_data(nullptr), lazy_nbytes(0), lazy_nelem(0), store_id(store_id), serial(serial) { }

RTStoreDef::RTStoreDef(uint32_t store_id, const std::string &serial, const RTSchema *klass, const char *lazy_data, uint32_t lazy_nbytes, uint32_t lazy_nelem) : def(nullptr), klass(klass), lazy_data(lazy_data), lazy_nbytes(lazy_nbytes), lazy_nelem(lazy_nelem), store_id(store_id), serial(serial) { }

RTStoreDef::RTStoreDef(const RTStoreDef &&o) : def(o.def), klass(o.klass), lazy_data(o.lazy_data), lazy_nbytes(o.lazy_nbytes), lazy_nelem(o.lazy_nelem), store_id(o.store_id), serial(o.serial) { }


// ============================================================================
// RTSchema
// ============================================================================
RTSchema::RTSchema(uint32_t klass_id, const std::string &serial, const BaseSchema *def) : def(def), klass_id(klass_id), serial(serial) { }

RTSchema::RTSchema(const RTSchema &&o) : def(o.def), fields(o.fields), stores(o.stores), klass_id(o.klass_id), serial(o.serial) { }

RTSchema::~RTSchema(void) {
  for (auto &f : fields)
    delete f;
  for (auto &s : stores)
    delete s;
}


// ============================================================================
// RTManager
// ============================================================================
RTManager::~RTManager(void) {
  for (auto &k : klasses)
    delete k;
  for (auto &p : lazy_buffers)
    delete [] p;
}


// ============================================================================
// Functions
// ============================================================================
static void
merge_rtschema_fields(RTSchema &rtschema, const BaseSchema &schema, const std::map<ptrdiff_t, const RTStoreDef *> &store_offsets) {
  uint32_t field_id = 0;
  std::map<const BaseFieldDef *, const RTFieldDef *> known_fields;
  if (!rtschema.fields.empty()) {
    for (auto &rtfield : rtschema.fields) {
      if (!rtfield->is_lazy())
        known_fields.insert(std::make_pair(rtfield->def, rtfield));
      if (rtfield->field_id > field_id)
        field_id = rtfield->field_id;
    }
    ++field_id;
  }

  for (auto &field : schema.fields()) {
    const auto &it = known_fields.find(field);
    if (it == known_fields.end()) {
      const RTStoreDef *pointer = nullptr;
      if (field->is_pointer)
        pointer = store_offsets.find(field->store_offset(nullptr))->second;
      RTFieldDef *const rtfield = new RTFieldDef(field_id, field->serial, pointer, field->is_slice, field);
      assert(rtfield != nullptr);
      rtschema.fields.push_back(rtfield);
      ++field_id;
    }
  }
  std::sort(rtschema.fields.begin(), rtschema.fields.end(), [](const RTFieldDef *a, const RTFieldDef *b) { return a->field_id < b->field_id; });
  assert(rtschema.fields.back()->field_id == rtschema.fields.size());
}


RTManager *
build_rt(const BaseDocSchema &dschema) {
  RTManager *const rt = new RTManager();
  assert(rt != nullptr);

  RTSchema *const rtdschema = new RTSchema(0, "__meta__", &dschema);
  assert(rtdschema != nullptr);
  rt->doc = rtdschema;
  rt->klasses.push_back(rtdschema);

  return merge_rt(rt, dschema);

#if 0
  // construct the RTStoreDef's
  uint32_t store_id = 0;
  std::map<ptrdiff_t, const RTStoreDef *> store_offsets;
  for (auto &store : dschema.stores()) {
    RTStoreDef *const rtstore = new RTStoreDef(store_id, store->name, nullptr, store);
    assert(rtstore != nullptr);
    store_offsets.insert(std::make_pair(store->store_offset(nullptr), rtstore));
    rtdschema->stores.push_back(rtstore);
    ++store_id;
  }

  // construct the documents RTFieldDef's
  merge_rtschema_fields(*rtdschema, dschema, store_offsets);

  // construct each of the klasses
  std::map<TypeInfo, const RTSchema *> typeinfo_to_schema;
  uint32_t klass_id = 1;
  for (auto &schema : dschema.schemas()) {
    RTSchema *const rtschema = new RTSchema(klass_id, schema);
    assert(rtschema != nullptr);
    merge_rtschema_fields(*rtschema, *schema, store_offsets);
    rt->klasses.push_back(rtschema);
    typeinfo_to_schema.insert(std::make_pair(schema->type, rtschema));
    ++klass_id;
  }

  // back-fill the RTStoreDef's RTSchema pointers now that they exist
  store_id = 0;
  for (auto &store : dschema.stores()) {
    const RTSchema *klass = typeinfo_to_schema.find(store->pointer_type())->second;
    rtdschema->stores[store_id]->klass = klass;
    ++store_id;
  }
#endif
}


RTManager *
merge_rt(RTManager *const rt, const BaseDocSchema &dschema) {
  RTSchema *const rtdschema = const_cast<RTSchema *>(rt->doc);

  uint32_t klass_id = 0;
  std::map<const BaseSchema *, const RTSchema *> known_klasses;
  if (!rt->klasses.empty()) {
    for (auto &rtschema : rt->klasses) {
      if (!rtschema->is_lazy())
        known_klasses.insert(std::make_pair(rtschema->def, rtschema));
      if (rtschema->klass_id > klass_id)
        klass_id = rtschema->klass_id;
    }
    ++klass_id;
  }

  uint32_t store_id = 0;
  std::map<const BaseStoreDef *, const RTStoreDef *> known_stores;
  if (!rt->doc->stores.empty()) {
    for (auto &rtstore : rt->doc->stores) {
      if (!rtstore->is_lazy())
        known_stores.insert(std::make_pair(rtstore->def, rtstore));
      if (rtstore->store_id > store_id)
        store_id = rtstore->store_id;
    }
    ++store_id;
  }

  // construct the RTStoreDef's
  std::map<ptrdiff_t, const RTStoreDef *> store_offsets;
  for (auto &store : dschema.stores()) {
    RTStoreDef *rtstore;
    const auto &it = known_stores.find(store);
    if (it == known_stores.end()) {
      rtstore = new RTStoreDef(store_id, store->name, nullptr, store);
      assert(rtstore != nullptr);
      rtdschema->stores.push_back(rtstore);
      known_stores.insert(std::make_pair(store, rtstore));
      ++store_id;
    }
    else
      rtstore = const_cast<RTStoreDef *>(it->second);
    store_offsets.insert(std::make_pair(store->store_offset(nullptr), rtstore));
  }
  std::sort(rtdschema->stores.begin(), rtdschema->stores.end(), [](const RTStoreDef *a, const RTStoreDef *b) { return a->store_id < b->store_id; });
  assert(rtdschema->stores.back()->store_id == rtdschema->stores.size());

  // construct the documents RTFieldDef's
  merge_rtschema_fields(*rtdschema, dschema, store_offsets);

  // construct each of the klasses
  std::map<TypeInfo, const RTSchema *> typeinfo_to_schema;
  for (auto &schema : dschema.schemas()) {
    RTSchema *rtschema;
    const auto &it = known_klasses.find(schema);
    if (it == known_klasses.end()) {
      rtschema = new RTSchema(klass_id, schema->serial, schema);
      assert(rtschema != nullptr);
      merge_rtschema_fields(*rtschema, *schema, store_offsets);
      rt->klasses.push_back(rtschema);
      ++klass_id;
    }
    else {
      rtschema = const_cast<RTSchema *>(it->second);
      merge_rtschema_fields(*rtschema, *schema, store_offsets);
    }
    typeinfo_to_schema.insert(std::make_pair(schema->type, rtschema));
  }
  std::sort(rt->klasses.begin(), rt->klasses.end(), [](const RTSchema *a, const RTSchema *b) { return a->klass_id < b->klass_id; });
  assert(rt->klasses.back()->klass_id == rt->klasses.size());

  // back-fill the RTStoreDef's RTSchema pointers now that they exist
  for (auto &store : dschema.stores()) {
    RTStoreDef *rtstore = const_cast<RTStoreDef *>(known_stores.find(store)->second);
    if (rtstore->klass == nullptr)
      rtstore->klass = typeinfo_to_schema.find(store->pointer_type())->second;
  }

  return rt;
}


} }
