/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class RTSchema;

    // ========================================================================
    // Constants
    // ========================================================================
    enum class FieldMode : uint8_t {
      READ_WRITE = 1,
      READ_ONLY = 2,
      DELETE = 3,
      STREAM_ONLY = 4
    };


    // ========================================================================
    // Base classes for field and store definitions
    // ========================================================================
    class BaseDef {
    public:
      const std::string name;
      const std::string help;
      const FieldMode mode;
      std::string serial;

    protected:
      BaseDef(const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);

    public:
      virtual ~BaseDef(void) { }

      virtual const TypeInfo &pointer_type(void) const = 0;
      virtual ptrdiff_t store_offset(const Doc *) const = 0;
    };


    class BaseFieldDef : public BaseDef {
    public:
      const bool is_pointer;
      const bool is_self_pointer;
      const bool is_slice;

    protected:
      BaseFieldDef(const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial, const bool is_pointer, const bool is_self_pointer, const bool is_slice);

    public:
      virtual ~BaseFieldDef(void) { }

      const TypeInfo &
      pointer_type(void) const override {
        assert(!"this should never be invoked");
        return *static_cast<const TypeInfo *>(nullptr);
      }

      ptrdiff_t
      store_offset(const Doc *) const override {
        assert(!"this should never be invoked");
        return -1;
      }

      virtual void read_field(io::ArrayReader &in, void *const _ann, void *const _store, void *const _doc) const = 0;
      virtual bool write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const _store, const void *const _doc) const = 0;
    };


    class BaseStoreDef : public BaseDef {
    protected:
      typedef void (*writer_type)(io::WriteBuffer &, const Doc &, const RTSchema &, const void *const);

      BaseStoreDef(const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);

    public:
      virtual ~BaseStoreDef(void) { }

      virtual void resize(Doc &doc, const size_t size) const = 0;

      virtual size_t size(const Doc &doc) const = 0;

      virtual char *store_begin(const Doc &_doc) const = 0;
      virtual size_t store_object_size(void) const = 0;
    };


    // ========================================================================
    // Templated FieldDef and StoreDef
    // ========================================================================
    template <typename T, T fn>
    class FieldDef;

    template <typename R, typename T, R T::*field_ptr>
    class FieldDef<R T::*, field_ptr> : public BaseFieldDef {
    public:
      static_assert(FieldTraits<R>::is_dr_ptr_type == false, "DR_FIELD must be used with POD fields only. Use DR_POINTER for schwa::dr field types instead.");
      static_assert(FieldTraits<R>::is_pod_ptr == false, "Fields cannot be POD pointers. Use schwa::dr::Pointer<T> instead.");
      typedef R value_type;
      typedef T annotation_type;

      FieldDef(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode) : FieldDef(schema, name, help, mode, name) { }
      FieldDef(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) : BaseFieldDef(name, help, mode, serial, false, false, FieldTraits<R>::is_slice) {
        schema.add(this);
      }
      virtual ~FieldDef(void) { }

      virtual void
      read_field(io::ArrayReader &in, void *const _ann, void *const, void *const) const override {
        T &ann = *static_cast<T *>(_ann);
        R &val = ann.*field_ptr;
        wire::WireTraits<R>::read(in, val);
      }

      virtual bool
      write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const, const void *const) const override {
        const T &ann = *static_cast<const T *>(_ann);
        const R &val = ann.*field_ptr;
        if (wire::WireTraits<R>::should_write(val)) {
          msgpack::write_uint(out, key);
          wire::WireTraits<R>::write(out, val);
          return true;
        }
        return false;
      }
    };


    template <typename T1, T1 fn1, typename T2, T2 fn2>
    class FieldDefWithStore;

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    class FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr> : public BaseFieldDef {
    public:
      static_assert(FieldTraits<R>::is_dr_ptr_type == true, "DR_POINTER must be used with schwa::dr field types only");
      static_assert(boost::is_same<typename FieldTraits<R>::value_type, S>::value, "Field (type T) and storage field (Store<T>) must have the same type (T)");
      static_assert(boost::is_base_of<Ann, S>::value, "Store<T> type T must be a subclass of Ann");
      typedef R value_type;
      typedef T annotation_type;
      typedef S store_type;
      typedef D doc_type;

    private:
      const TypeInfo _pointer_type;

    public:
      FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode) : FieldDefWithStore(schema, name, help, mode, name) { }
      FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) : BaseFieldDef(name, help, mode, serial, true, false, FieldTraits<R>::is_slice), _pointer_type(TypeInfo::create<S>()) {
        schema.add(this);
      }
      virtual ~FieldDefWithStore(void) { }

      const TypeInfo &
      pointer_type(void) const override {
        return _pointer_type;
      }

      ptrdiff_t
      store_offset(const Doc *doc) const override {
        return reinterpret_cast<const char *>(&(static_cast<const D *>(doc)->*store_ptr)) - reinterpret_cast<const char *>(doc);
      }

      virtual void
      read_field(io::ArrayReader &in, void *const _ann, void *const, void *const _doc) const override {
        D &doc = *static_cast<D *>(_doc);
        T &ann = *static_cast<T *>(_ann);
        R &val = ann.*field_ptr;
        wire::WireTraits<R>::read(in, val, (doc.*store_ptr).front());
      }

      virtual bool
      write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const, const void *const _doc) const override {
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
    };


    template <typename T1, T1 fn1>
    class FieldDefWithSelfStore;

    template <typename R, typename T, R T::*field_ptr>
    class FieldDefWithSelfStore<R T::*, field_ptr> : public BaseFieldDef {
    public:
      static_assert(FieldTraits<R>::is_dr_ptr_type == true, "DR_SELF must be used with schwa::dr field types only");
      static_assert(boost::is_same<typename FieldTraits<R>::value_type, T>::value, "DR_SELF must be used on recursive pointers only");
      typedef R value_type;
      typedef T annotation_type;

    private:
      const TypeInfo _pointer_type;

    public:
      FieldDefWithSelfStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode) : FieldDefWithSelfStore(schema, name, help, mode, name) { }
      FieldDefWithSelfStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) : BaseFieldDef(name, help, mode, serial, false, true, FieldTraits<R>::is_slice), _pointer_type(TypeInfo::create<R>()) {
        schema.add(this);
      }
      virtual ~FieldDefWithSelfStore(void) { }

      const TypeInfo &
      pointer_type(void) const override {
        return _pointer_type;
      }

      virtual void
      read_field(io::ArrayReader &in, void *const _ann, void *const _store, void *const) const override {
        T &ann = *static_cast<T *>(_ann);
        R &val = ann.*field_ptr;
        wire::WireTraits<R>::read(in, val, *static_cast<T *>(_store));
      }

      virtual bool
      write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const _store, const void *const) const override {
        const T &ann = *static_cast<const T *>(_ann);
        const R &val = ann.*field_ptr;
        if (wire::WireTraits<R>::should_write(val)) {
          msgpack::write_uint(out, key);
          wire::WireTraits<R>::write(out, val, *static_cast<const T *>(_store));
          return true;
        }
        return false;
      }
    };


    template <typename T, T fn>
    class StoreDef;

    template <typename S, typename T, Store<S> T::*store_ptr>
    class StoreDef<Store<S> T::*, store_ptr> : public BaseStoreDef {
    public:
      static_assert(boost::is_base_of<Ann, S>::value, "Store<T> type T must be a subclass of Ann");
      typedef S store_type;

    private:
      const TypeInfo _pointer_type;

    public:
      StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode) : StoreDef(schema, name, help, mode, name) { }
      StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial) : BaseStoreDef(name, help, mode, serial), _pointer_type(TypeInfo::create<S>()) {
        schema.add(this);
      }
      virtual ~StoreDef(void) { }

      const TypeInfo &
      pointer_type(void) const override {
        return _pointer_type;
      }

      char *
      store_begin(const Doc &_doc) const override {
        const T &doc = static_cast<const T &>(_doc);
        const Store<S> &store = doc.*store_ptr;
        return const_cast<char *>(reinterpret_cast<const char *>(&store[0]));
      }

      inline size_t
      store_object_size(void) const override {
        return sizeof(S);
      }

      inline void
      resize(Doc &doc, const size_t size) const override {
        (static_cast<T &>(doc).*store_ptr).resize(size);
      }

      inline size_t
      size(const Doc &doc) const override {
        return (static_cast<const T &>(doc).*store_ptr).size();
      }

      ptrdiff_t
      store_offset(const Doc *doc) const override {
        return reinterpret_cast<const char *>(&(static_cast<const T *>(doc)->*store_ptr)) - reinterpret_cast<const char *>(doc);
      }
    };


    #define DR_FIELD(member_obj_ptr)                  schwa::dr::FieldDef<decltype(member_obj_ptr), member_obj_ptr>
    #define DR_POINTER(member_obj_ptr, store_obj_ptr) schwa::dr::FieldDefWithStore<decltype(member_obj_ptr), member_obj_ptr, decltype(store_obj_ptr), store_obj_ptr>
    #define DR_SELF(member_obj_ptr)                   schwa::dr::FieldDefWithSelfStore<decltype(member_obj_ptr), member_obj_ptr>
    #define DR_STORE(member_obj_ptr)                  schwa::dr::StoreDef<decltype(member_obj_ptr), member_obj_ptr>

  }
}
