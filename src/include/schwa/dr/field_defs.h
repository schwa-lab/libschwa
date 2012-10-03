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
      BaseDef(const std::string &name, const std::string &help, FieldMode mode, const std::string &serial);

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
      BaseFieldDef(const std::string &name, const std::string &help, FieldMode mode, const std::string &serial, bool is_pointer, bool is_self_pointer, bool is_slice);

    public:
      virtual ~BaseFieldDef(void) { }

      const TypeInfo &pointer_type(void) const override;
      ptrdiff_t store_offset(const Doc *) const override;

      virtual void read_field(io::ArrayReader &in, void *const _ann, void *const _store, void *const _doc) const = 0;
      virtual bool write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const _store, const void *const _doc) const = 0;
    };


    class BaseStoreDef : public BaseDef {
    protected:
      BaseStoreDef(const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);

    public:
      virtual ~BaseStoreDef(void) { }

      virtual void resize(Doc &doc, const size_t size) const = 0;

      virtual size_t size(const Doc &doc) const = 0;

      virtual char *store_begin(const Doc &_doc) const = 0;
      virtual size_t store_object_size(void) const = 0;
    };


    // ========================================================================
    // FieldDef (DR_FIELD)
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
      FieldDef(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);
      virtual ~FieldDef(void) { }

      void read_field(io::ArrayReader &in, void *const _ann, void *const, void *const) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const, const void *const) const override;
    };


    // ========================================================================
    // FieldDefWithStore (DR_POINTER)
    // ========================================================================
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
      FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);
      virtual ~FieldDefWithStore(void) { }

      const TypeInfo &pointer_type(void) const override;
      ptrdiff_t store_offset(const Doc *doc) const override;

      void read_field(io::ArrayReader &in, void *const _ann, void *const, void *const _doc) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const, const void *const _doc) const override;
    };

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, GrowableStore<S> D::*store_ptr>
    class FieldDefWithStore<R T::*, field_ptr, GrowableStore<S> D::*, store_ptr> : public BaseFieldDef {
    public:
      static_assert(FieldTraits<R>::is_dr_ptr_type == true, "DR_POINTER must be used with schwa::dr field types only");
      static_assert(boost::is_same<typename FieldTraits<R>::value_type, S>::value, "Field (type T) and storage field (GrowableStore<T>) must have the same type (T)");
      static_assert(boost::is_base_of<Ann, S>::value, "GrowableStore<T> type T must be a subclass of Ann");
      typedef R value_type;
      typedef T annotation_type;
      typedef S store_type;
      typedef D doc_type;

    private:
      const TypeInfo _pointer_type;

    public:
      FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode) : FieldDefWithStore(schema, name, help, mode, name) { }
      FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);
      virtual ~FieldDefWithStore(void) { }

      const TypeInfo &pointer_type(void) const override;
      ptrdiff_t store_offset(const Doc *doc) const override;

      void read_field(io::ArrayReader &in, void *const _ann, void *const, void *const _doc) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const, const void *const _doc) const override;
    };


    // ========================================================================
    // FieldDefWithSelfStore (DR_SELF)
    // ========================================================================
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
      FieldDefWithSelfStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);
      virtual ~FieldDefWithSelfStore(void) { }

      const TypeInfo &pointer_type(void) const override;

      void read_field(io::ArrayReader &in, void *const _ann, void *const _store, void *const) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const void *const _ann, const void *const _store, const void *const) const override;
    };


    // ========================================================================
    // StoreDef (DR_STORE)
    // ========================================================================
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
      StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);
      virtual ~StoreDef(void) { }

      const TypeInfo &pointer_type(void) const override;

      char *store_begin(const Doc &_doc) const override;
      size_t store_object_size(void) const override;
      ptrdiff_t store_offset(const Doc *doc) const override;

      void resize(Doc &doc, const size_t size) const override;
      size_t size(const Doc &doc) const override;
    };

    template <typename S, typename T, GrowableStore<S> T::*store_ptr>
    class StoreDef<GrowableStore<S> T::*, store_ptr> : public BaseStoreDef {
    public:
      static_assert(boost::is_base_of<Ann, S>::value, "Store<T> type T must be a subclass of Ann");
      typedef S store_type;

    private:
      const TypeInfo _pointer_type;

    public:
      StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode) : StoreDef(schema, name, help, mode, name) { }
      StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);
      virtual ~StoreDef(void) { }

      const TypeInfo &pointer_type(void) const override;

      char *store_begin(const Doc &_doc) const override;
      size_t store_object_size(void) const override;
      ptrdiff_t store_offset(const Doc *doc) const override;

      void resize(Doc &doc, const size_t size) const override;
      size_t size(const Doc &doc) const override;
    };


    // ========================================================================
    // Macros
    // ========================================================================
    #define DR_FIELD(member_obj_ptr)                  schwa::dr::FieldDef<decltype(member_obj_ptr), member_obj_ptr>
    #define DR_POINTER(member_obj_ptr, store_obj_ptr) schwa::dr::FieldDefWithStore<decltype(member_obj_ptr), member_obj_ptr, decltype(store_obj_ptr), store_obj_ptr>
    #define DR_SELF(member_obj_ptr)                   schwa::dr::FieldDefWithSelfStore<decltype(member_obj_ptr), member_obj_ptr>
    #define DR_STORE(member_obj_ptr)                  schwa::dr::StoreDef<decltype(member_obj_ptr), member_obj_ptr>

  }
}

#include <schwa/dr/field_defs_impl.h>
