/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    template <typename R, typename T, R T::*field_ptr>
    FieldDef<R T::*, field_ptr>::FieldDef(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) :
      BaseFieldDef(name, help, mode, serial, false, false, FieldTraits<R>::is_slice)
      {
        schema.add(this);
    }


    template <typename R, typename T, R T::*field_ptr, bool HAS_DOC_BASE>
    struct FieldDefDispatch {
      static inline void read_field(io::ArrayReader &, Doc &) { assert(!"should never be called"); std::abort(); }
      static inline bool write_field(io::WriteBuffer &, const uint32_t, const Doc &) { assert(!"should never be called"); std::abort(); return false; }

      static inline void
      read_field(io::ArrayReader &in, Ann &_ann) {
        T &ann = static_cast<T &>(_ann);
        R &val = ann.*field_ptr;
        wire::WireTraits<R>::read(in, val);
      }

      static inline bool
      write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann) {
        const T &ann = static_cast<const T &>(_ann);
        const R &val = ann.*field_ptr;
        if (wire::WireTraits<R>::should_write(val)) {
          msgpack::write_uint(out, key);
          wire::WireTraits<R>::write(out, val);
          return true;
        }
        return false;
      }
    };

    template <typename R, typename T, R T::*field_ptr>
    struct FieldDefDispatch<R, T, field_ptr, true> {
      static inline void read_field(io::ArrayReader &, Ann &) { assert(!"should never be called"); std::abort(); }
      static inline bool write_field(io::WriteBuffer &, const uint32_t, const Ann &) { assert(!"should never be called"); std::abort(); return false; }

      static inline void
      read_field(io::ArrayReader &in, Doc &_doc) {
        T &doc = static_cast<T &>(_doc);
        R &val = doc.*field_ptr;
        wire::WireTraits<R>::read(in, val);
      }

      static inline bool
      write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) {
        const T &ann = static_cast<const T &>(_doc);
        const R &val = ann.*field_ptr;
        if (wire::WireTraits<R>::should_write(val)) {
          msgpack::write_uint(out, key);
          wire::WireTraits<R>::write(out, val);
          return true;
        }
        return false;
      }
    };

    template <typename R, typename T, R T::*field_ptr>
    void
    FieldDef<R T::*, field_ptr>::read_field(io::ArrayReader &in, Ann &_ann, IStore &, Doc &) const {
      FieldDefDispatch<R, T, field_ptr, boost::is_base_of<Doc, T>::value>::read_field(in, _ann);
    }

    template <typename R, typename T, R T::*field_ptr>
    bool
    FieldDef<R T::*, field_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &, const Doc &) const {
      return FieldDefDispatch<R, T, field_ptr, boost::is_base_of<Doc, T>::value>::write_field(out, key, _ann);
    }

    template <typename R, typename T, R T::*field_ptr>
    void
    FieldDef<R T::*, field_ptr>::read_field(io::ArrayReader &in, Doc &_doc) const {
      FieldDefDispatch<R, T, field_ptr, boost::is_base_of<Doc, T>::value>::read_field(in, _doc);
    }

    template <typename R, typename T, R T::*field_ptr>
    bool
    FieldDef<R T::*, field_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) const {
      return FieldDefDispatch<R, T, field_ptr, boost::is_base_of<Doc, T>::value>::write_field(out, key, _doc);
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


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr, bool HAS_DOC_BASE>
    struct FieldDefWithStoreDispatch {
      static inline void read_field(io::ArrayReader &, Doc &) { assert(!"should be impossible to invoke this"); std::abort(); }
      static inline bool write_field(io::WriteBuffer &, uint32_t, const Doc &) { assert(!"should be impossible to invoke this"); std::abort(); return false; }

      static inline void
      read_field(io::ArrayReader &in, Ann &_ann, Doc &_doc) {
        D &doc = static_cast<D &>(_doc);
        T &ann = static_cast<T &>(_ann);
        R &val = ann.*field_ptr;
        wire::WireTraits<R>::read(in, val, doc.*store_ptr);
      }

      static inline bool
      write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const Doc &_doc) {
        const T &ann = static_cast<const T &>(_ann);
        const D &doc = static_cast<const D &>(_doc);
        const R &val = ann.*field_ptr;
        if (wire::WireTraits<R>::should_write(val)) {
          msgpack::write_uint(out, key);
          wire::WireTraits<R>::write(out, val, doc.*store_ptr);
          return true;
        }
        return false;
      }
    };

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    struct FieldDefWithStoreDispatch<R, T, S, D, field_ptr, store_ptr, true> {
      static inline void read_field(io::ArrayReader &, Ann &, Doc &) { assert(!"should be impossible to invoke this"); std::abort(); }
      static inline bool write_field(io::WriteBuffer &, uint32_t, const Ann &, const Doc &) { assert(!"should be impossible to invoke this"); std::abort(); return false; }

      static inline void
      read_field(io::ArrayReader &in, Doc &_doc) {
        D &doc = static_cast<D &>(_doc);
        T &ann = static_cast<T &>(_doc);
        R &val = ann.*field_ptr;
        wire::WireTraits<R>::read(in, val, doc.*store_ptr);
      }

      static inline bool
      write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) {
        const T &ann = static_cast<const T &>(_doc);
        const D &doc = static_cast<const D &>(_doc);
        const R &val = ann.*field_ptr;
        if (wire::WireTraits<R>::should_write(val)) {
          msgpack::write_uint(out, key);
          wire::WireTraits<R>::write(out, val, doc.*store_ptr);
          return true;
        }
        return false;
      }
    };

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    void
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::read_field(io::ArrayReader &in, Ann &_ann, IStore &, Doc &_doc) const {
      FieldDefWithStoreDispatch<R, T, S, D, field_ptr, store_ptr, boost::is_base_of<Doc, T>::value>::read_field(in, _ann, _doc);
    }

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    bool
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &, const Doc &_doc) const {
      return FieldDefWithStoreDispatch<R, T, S, D, field_ptr, store_ptr, boost::is_base_of<Doc, T>::value>::write_field(out, key, _ann, _doc);
    }

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    void
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::read_field(io::ArrayReader &in, Doc &_doc) const {
      FieldDefWithStoreDispatch<R, T, S, D, field_ptr, store_ptr, boost::is_base_of<Doc, T>::value>::read_field(in, _doc);
    }

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    bool
    FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) const {
      return FieldDefWithStoreDispatch<R, T, S, D, field_ptr, store_ptr, boost::is_base_of<Doc, T>::value>::write_field(out, key, _doc);
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


    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr, bool HAS_DOC_BASE>
    struct FieldDefWithBlockStoreDispatch {
      static inline void read_field(io::ArrayReader &, Doc &) { assert(!"should be impossible to invoke this"); std::abort(); }
      static inline bool write_field(io::WriteBuffer &, uint32_t, const Doc &) { assert(!"should be impossible to invoke this"); std::abort(); return false; }

      static inline void
      read_field(io::ArrayReader &in, Ann &_ann, Doc &_doc) {
        D &doc = static_cast<D &>(_doc);
        T &ann = static_cast<T &>(_ann);
        R &val = ann.*field_ptr;
        wire::WireTraits<R>::read(in, val, doc.*store_ptr);
      }

      static inline bool
      write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const Doc &_doc) {
        const T &ann = static_cast<const T &>(_ann);
        const D &doc = static_cast<const D &>(_doc);
        const R &val = ann.*field_ptr;
        if (wire::WireTraits<R>::should_write(val)) {
          msgpack::write_uint(out, key);
          wire::WireTraits<R>::write(out, val, doc.*store_ptr);
          return true;
        }
        return false;
      }
    };

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    struct FieldDefWithBlockStoreDispatch<R, T, S, D, field_ptr, store_ptr, true> {
      static inline void read_field(io::ArrayReader &, Ann &, Doc &) { assert(!"should be impossible to invoke this"); std::abort(); }
      static inline bool write_field(io::WriteBuffer &, uint32_t, const Ann &, const Doc &) { assert(!"should be impossible to invoke this"); std::abort(); return false; }

      static inline void
      read_field(io::ArrayReader &in, Doc &_doc) {
        D &doc = static_cast<D &>(_doc);
        T &ann = static_cast<T &>(_doc);
        R &val = ann.*field_ptr;
        wire::WireTraits<R>::read(in, val, doc.*store_ptr);
      }

      static inline bool
      write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) {
        const T &ann = static_cast<const T &>(_doc);
        const D &doc = static_cast<const D &>(_doc);
        const R &val = ann.*field_ptr;
        if (wire::WireTraits<R>::should_write(val)) {
          msgpack::write_uint(out, key);
          wire::WireTraits<R>::write(out, val, doc.*store_ptr);
          return true;
        }
        return false;
      }
    };

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    void
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::read_field(io::ArrayReader &in, Ann &_ann, IStore &, Doc &_doc) const {
      FieldDefWithBlockStoreDispatch<R, T, S, D, field_ptr, store_ptr, boost::is_base_of<Doc, T>::value>::read_field(in, _ann, _doc);
    }

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    bool
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &, const Doc &_doc) const {
      return FieldDefWithBlockStoreDispatch<R, T, S, D, field_ptr, store_ptr, boost::is_base_of<Doc, T>::value>::write_field(out, key, _ann, _doc);
    }

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    void
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::read_field(io::ArrayReader &in, Doc &_doc) const {
      FieldDefWithBlockStoreDispatch<R, T, S, D, field_ptr, store_ptr, boost::is_base_of<Doc, T>::value>::read_field(in, _doc);
    }

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    bool
    FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) const {
      return FieldDefWithBlockStoreDispatch<R, T, S, D, field_ptr, store_ptr, boost::is_base_of<Doc, T>::value>::write_field(out, key, _doc);
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
    FieldDefWithSelfStore<R T::*, field_ptr>::read_field(io::ArrayReader &in, Ann &_ann, IStore &store, Doc &) const {
      T &ann = static_cast<T &>(_ann);
      R &val = ann.*field_ptr;
      wire::WireTraits<R>::read(in, val, store);
    }


    template <typename R, typename T, R T::*field_ptr>
    bool
    FieldDefWithSelfStore<R T::*, field_ptr>::write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &store, const Doc &) const {
      const T &ann = static_cast<const T &>(_ann);
      const R &val = ann.*field_ptr;
      if (wire::WireTraits<R>::should_write(val)) {
        msgpack::write_uint(out, key);
        wire::WireTraits<R>::write(out, val, store);
        return true;
      }
      return false;
    }


    template <typename R, typename T, R T::*field_ptr>
    void
    FieldDefWithSelfStore<R T::*, field_ptr>::read_field(io::ArrayReader &, Doc &) const {
      assert(!"should be impossible to invoke this");
      std::abort();
    }


    template <typename R, typename T, R T::*field_ptr>
    bool
    FieldDefWithSelfStore<R T::*, field_ptr>::write_field(io::WriteBuffer &, const uint32_t, const Doc &) const {
      assert(!"should be impossible to invoke this");
      std::abort();
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
    IStore &
    StoreDef<Store<S> T::*, store_ptr>::istore(const Doc &_doc) const {
      const T &doc = static_cast<const T &>(_doc);
      return const_cast<Store<S> &>(doc.*store_ptr);
    }


    template <typename S, typename T, Store<S> T::*store_ptr>
    inline void
    StoreDef<Store<S> T::*, store_ptr>::resize(Doc &doc, const size_t size) const {
      (static_cast<T &>(doc).*store_ptr).resize(size);
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
    IStore &
    StoreDef<BlockStore<S> T::*, store_ptr>::istore(const Doc &_doc) const {
      const BlockStore<S> &store = static_cast<const T &>(_doc).*store_ptr;
      return const_cast<BlockStore<S> &>(store);
    }


    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    inline void
    StoreDef<BlockStore<S> T::*, store_ptr>::resize(Doc &doc, const size_t size) const {
      BlockStore<S> &store = static_cast<T &>(doc).*store_ptr;
      assert(store.size() == 0);
      auto &block = store.reserve(size);
      for (size_t i = 0; i != size; ++i)
        block.create();
    }
  }
}
