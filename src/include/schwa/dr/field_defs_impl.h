/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    template <typename R, typename T, R T::*field_ptr>
    FieldDef<R T::*, field_ptr>::FieldDef(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseFieldDef(name, help, mode, serial, false, false, FieldTraits<R>::is_slice)
      {
        schema.add(this);
    }

    template <typename R, typename T, R T::*field_ptr>
    void
    FieldDef<R T::*, field_ptr>::read_field(io::ArrayReader &in, void *const _ann, void *const, void *const) const {
      T &ann = *static_cast<T *>(_ann);
      R &val = ann.*field_ptr;
      wire::WireTraits<R>::read(in, val);
    }


    template <typename R, typename T, R T::*field_ptr>
    bool
    FieldDef<R T::*, field_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const, const void *const) const {
      const T &ann = *static_cast<const T *>(_ann);
      const R &val = ann.*field_ptr;
      if (wire::WireTraits<R>::should_write(val)) {
        msgpack::write_uint(out, key);
        wire::WireTraits<R>::write(out, val);
        return true;
      }
      return false;
    }


    // ========================================================================
    // FieldDefWithStore (DR_POINTER)
    // ========================================================================
    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
        BaseFieldDef(name, help, mode, serial, true, false, FieldTraits<R>::is_slice),
        _pointer_type(TypeInfo::create<S>())
      {
      schema.add(this);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    const TypeInfo &
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::pointer_type(void) const {
      return _pointer_type;
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    ptrdiff_t
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::store_offset(const Doc *doc) const {
      return reinterpret_cast<const char *>(&(static_cast<const D *>(doc)->*store_ptr)) - reinterpret_cast<const char *>(doc);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    void
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::read_field(io::ArrayReader &in, void *const _ann, void *const, void *const _doc) const {
      D &doc = *static_cast<D *>(_doc);
      T &ann = *static_cast<T *>(_ann);
      R &val = ann.*field_ptr;
      wire::WireTraits<R>::read(in, val, (doc.*store_ptr).front());
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    bool
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const, const void *const _doc) const {
      const D &doc = *static_cast<const D *>(_doc);
      const T &ann = *static_cast<const T *>(_ann);
      const R &val = ann.*field_ptr;
      if (wire::WireTraits<R>::should_write(val)) {
        msgpack::write_uint(out, key);
        wire::WireTraits<R>::write(out, val, (doc.*store_ptr).front());
        return true;
      }
      return false;
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseFieldDef(name, help, mode, serial, true, false, FieldTraits<R>::is_slice),
      _pointer_type(TypeInfo::create<S>())
      {
      schema.add(this);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    const TypeInfo &
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::pointer_type(void) const {
      return _pointer_type;
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    ptrdiff_t
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::store_offset(const Doc *doc) const {
      return reinterpret_cast<const char *>(&(static_cast<const D *>(doc)->*store_ptr)) - reinterpret_cast<const char *>(doc);
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    void
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::read_field(io::ArrayReader &in, void *const _ann, void *const, void *const _doc) const {
      D &doc = *static_cast<D *>(_doc);
      T &ann = *static_cast<T *>(_ann);
      R &val = ann.*field_ptr;
      wire::WireTraits<R>::read(in, val, (doc.*store_ptr).front());
    }


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    bool
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const, const void *const _doc) const {
      const D &doc = *static_cast<const D *>(_doc);
      const T &ann = *static_cast<const T *>(_ann);
      const R &val = ann.*field_ptr;
      if (wire::WireTraits<R>::should_write(val)) {
        msgpack::write_uint(out, key);
        wire::WireTraits<R>::write(out, val, (doc.*store_ptr).front());
        return true;
      }
      return false;
    }

    // ========================================================================
    // FieldDefWithSelfStore (DR_SELF)
    // ========================================================================
    template <typename R, typename T, R T::*field_ptr>
    FieldDefWithSelfStore<R T::*, field_ptr>::FieldDefWithSelfStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseFieldDef(name, help, mode, serial, false, true, FieldTraits<R>::is_slice),
      _pointer_type(TypeInfo::create<R>())
      {
      schema.add(this);
    }


    template <typename R, typename T, R T::*field_ptr>
    const TypeInfo &
    FieldDefWithSelfStore<R T::*, field_ptr>::pointer_type(void) const {
      return _pointer_type;
    }


    template <typename R, typename T, R T::*field_ptr>
    void
    FieldDefWithSelfStore<R T::*, field_ptr>::read_field(io::ArrayReader &in, void *const _ann, void *const _store, void *const) const {
      T &ann = *static_cast<T *>(_ann);
      R &val = ann.*field_ptr;
      wire::WireTraits<R>::read(in, val, *static_cast<T *>(_store));
    }


    template <typename R, typename T, R T::*field_ptr>
    bool
    FieldDefWithSelfStore<R T::*, field_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const _store, const void *const) const {
      const T &ann = *static_cast<const T *>(_ann);
      const R &val = ann.*field_ptr;
      if (wire::WireTraits<R>::should_write(val)) {
        msgpack::write_uint(out, key);
        wire::WireTraits<R>::write(out, val, *static_cast<const T *>(_store));
        return true;
      }
      return false;
    }


    // ========================================================================
    // StoreDef (DR_STORE)
    // ========================================================================
    template <typename S, typename T, Store<S> T::*store_ptr>
    StoreDef<Store<S> T::*, store_ptr>::StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseStoreDef(name, help, mode, serial),
      _pointer_type(TypeInfo::create<S>())
      {
      schema.add(this);
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    const TypeInfo &
    StoreDef<Store<S> T::*, store_ptr>::pointer_type(void) const {
      return _pointer_type;
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    ptrdiff_t
    StoreDef<Store<S> T::*, store_ptr>::store_offset(const Doc *doc) const {
      return reinterpret_cast<const char *>(&(static_cast<const T *>(doc)->*store_ptr)) - reinterpret_cast<const char *>(doc);
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    char *
    StoreDef<Store<S> T::*, store_ptr>::store_begin(const Doc &_doc) const {
      const T &doc = static_cast<const T &>(_doc);
      const Store<S> &store = doc.*store_ptr;
      return const_cast<char *>(reinterpret_cast<const char *>(&store[0]));
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    inline size_t
    StoreDef<Store<S> T::*, store_ptr>::store_object_size(void) const {
      return sizeof(S);
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    inline void
    StoreDef<Store<S> T::*, store_ptr>::resize(Doc &doc, const size_t size) const {
      (static_cast<T &>(doc).*store_ptr).resize(size);
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    size_t
    StoreDef<Store<S> T::*, store_ptr>::size(const Doc &doc) const {
      return (static_cast<const T &>(doc).*store_ptr).size();
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    StoreDef<BlockStore<S> T::*, store_ptr>::StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseStoreDef(name, help, mode, serial),
      _pointer_type(TypeInfo::create<S>())
      {
      schema.add(this);
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    const TypeInfo &
    StoreDef<BlockStore<S> T::*, store_ptr>::pointer_type(void) const {
      return _pointer_type;
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    ptrdiff_t
    StoreDef<BlockStore<S> T::*, store_ptr>::store_offset(const Doc *doc) const {
      return reinterpret_cast<const char *>(&(static_cast<const T *>(doc)->*store_ptr)) - reinterpret_cast<const char *>(doc);
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    char *
    StoreDef<BlockStore<S> T::*, store_ptr>::store_begin(const Doc &_doc) const {
      const BlockStore<S> &store = static_cast<const T &>(_doc).*store_ptr;
      assert(store.nblocks() == 1);
      return const_cast<char *>(reinterpret_cast<const char *>(&store.front()));
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    inline size_t
    StoreDef<BlockStore<S> T::*, store_ptr>::store_object_size(void) const {
      return sizeof(S);
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    inline void
    StoreDef<BlockStore<S> T::*, store_ptr>::resize(Doc &doc, const size_t size) const {
      BlockStore<S> &store = static_cast<T &>(doc).*store_ptr;
      assert(store.size() == 0);
      store.reserve(size);
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    size_t
    StoreDef<BlockStore<S> T::*, store_ptr>::size(const Doc &doc) const {
      return (static_cast<const T &>(doc).*store_ptr).size();
    }
  }
}
