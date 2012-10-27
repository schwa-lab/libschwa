/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_FIELD_DEFS_H_
#define SCHWA_DR_FIELD_DEFS_H_

#include <string>
#include <type_traits>

#include <schwa/_base.h>
#include <schwa/dr/fields.h>
#include <schwa/dr/schema.h>
#include <schwa/dr/type_info.h>

namespace schwa {
  namespace io {
    class ArrayReader;
    class WriteBuffer;
  }

  namespace dr {

    class IStore;
    class RTSchema;

    // ========================================================================
    // Constants
    // ========================================================================
    enum class FieldMode : uint8_t {
      READ_WRITE = 1,
      READ_ONLY = 2,
      DELETE = 3
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
      const bool is_pointer : 1;
      const bool is_self_pointer : 1;
      const bool is_slice : 1;
      const bool is_collection : 1;

    protected:
      BaseFieldDef(const std::string &name, const std::string &help, FieldMode mode, const std::string &serial, bool is_pointer, bool is_self_pointer, bool is_slice, bool is_collection);

    public:
      virtual ~BaseFieldDef(void) { }

      const TypeInfo &pointer_type(void) const override;
      ptrdiff_t store_offset(const Doc *) const override;

      virtual void read_field(io::ArrayReader &in, Ann &_ann, IStore &_store, Doc &_doc) const = 0;
      virtual void read_field(io::ArrayReader &in, Doc &_doc) const = 0;
      virtual bool write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &_store, const Doc &_doc) const = 0;
      virtual bool write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) const = 0;
    };


    class BaseStoreDef : public BaseDef {
    protected:
      BaseStoreDef(const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);

    public:
      virtual ~BaseStoreDef(void) { }

      virtual IStore &istore(const Doc &_doc) const = 0;
      virtual void resize(Doc &doc, const size_t size) const = 0;
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

      void read_field(io::ArrayReader &in, Ann &_ann, IStore &_store, Doc &_doc) const override;
      void read_field(io::ArrayReader &in, Doc &_doc) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &_store, const Doc &_doc) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) const override;

    private:
      DISALLOW_COPY_AND_ASSIGN(FieldDef);
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
      static_assert(std::is_same<typename FieldTraits<R>::value_type, S>::value, "Field (type T) and storage field (Store<T>) must have the same type (T)");
      static_assert(std::is_base_of<Ann, S>::value, "Store<T> type T must be a subclass of Ann");
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

      void read_field(io::ArrayReader &in, Ann &_ann, IStore &_store, Doc &_doc) const override;
      void read_field(io::ArrayReader &in, Doc &_doc) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &_store, const Doc &_doc) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) const override;

    private:
      DISALLOW_COPY_AND_ASSIGN(FieldDefWithStore);
    };

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, BlockStore<S> D::*store_ptr>
    class FieldDefWithStore<R T::*, field_ptr, BlockStore<S> D::*, store_ptr> : public BaseFieldDef {
    public:
      static_assert(FieldTraits<R>::is_dr_ptr_type == true, "DR_POINTER must be used with schwa::dr field types only");
      static_assert(std::is_same<typename FieldTraits<R>::value_type, S>::value, "Field (type T) and storage field (BlockStore<T>) must have the same type (T)");
      static_assert(std::is_base_of<Ann, S>::value, "BlockStore<T> type T must be a subclass of Ann");
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

      void read_field(io::ArrayReader &in, Ann &_ann, IStore &_store, Doc &_doc) const override;
      void read_field(io::ArrayReader &in, Doc &_doc) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &_store, const Doc &_doc) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) const override;

    private:
      DISALLOW_COPY_AND_ASSIGN(FieldDefWithStore);
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
      static_assert(std::is_same<typename FieldTraits<R>::value_type, T>::value, "DR_SELF must be used on recursive pointers only");
      typedef R value_type;
      typedef T annotation_type;

    private:
      const TypeInfo _pointer_type;

    public:
      FieldDefWithSelfStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode) : FieldDefWithSelfStore(schema, name, help, mode, name) { }
      FieldDefWithSelfStore(BaseSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);
      virtual ~FieldDefWithSelfStore(void) { }

      const TypeInfo &pointer_type(void) const override;

      void read_field(io::ArrayReader &in, Ann &_ann, IStore &_store, Doc &_doc) const override;
      void read_field(io::ArrayReader &in, Doc &_doc) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const Ann &_ann, const IStore &_store, const Doc &_doc) const override;
      bool write_field(io::WriteBuffer &out, const uint32_t key, const Doc &_doc) const override;

    private:
      DISALLOW_COPY_AND_ASSIGN(FieldDefWithSelfStore);
    };


    // ========================================================================
    // StoreDef (DR_STORE)
    // ========================================================================
    template <typename T, T fn>
    class StoreDef;

    template <typename S, typename T, Store<S> T::*store_ptr>
    class StoreDef<Store<S> T::*, store_ptr> : public BaseStoreDef {
    public:
      static_assert(std::is_base_of<Ann, S>::value, "Store<T> type T must be a subclass of Ann");
      typedef S store_type;

    private:
      const TypeInfo _pointer_type;

    public:
      StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode) : StoreDef(schema, name, help, mode, name) { }
      StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);
      virtual ~StoreDef(void) { }

      const TypeInfo &pointer_type(void) const override;
      ptrdiff_t store_offset(const Doc *doc) const override;

      IStore &istore(const Doc &_doc) const override;
      void resize(Doc &doc, const size_t size) const override;

    private:
      DISALLOW_COPY_AND_ASSIGN(StoreDef);
    };

    template <typename S, typename T, BlockStore<S> T::*store_ptr>
    class StoreDef<BlockStore<S> T::*, store_ptr> : public BaseStoreDef {
    public:
      static_assert(std::is_base_of<Ann, S>::value, "Store<T> type T must be a subclass of Ann");
      typedef S store_type;

    private:
      const TypeInfo _pointer_type;

    public:
      StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode) : StoreDef(schema, name, help, mode, name) { }
      StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const FieldMode mode, const std::string &serial);
      virtual ~StoreDef(void) { }

      const TypeInfo &pointer_type(void) const override;
      ptrdiff_t store_offset(const Doc *doc) const override;

      IStore &istore(const Doc &_doc) const override;
      void resize(Doc &doc, const size_t size) const override;

    private:
      DISALLOW_COPY_AND_ASSIGN(StoreDef);
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

#endif
