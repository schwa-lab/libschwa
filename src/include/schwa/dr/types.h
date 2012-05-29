/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/typeof/typeof.hpp>

namespace schwa {
  namespace dr {

    // ========================================================================
    // Constants
    // ========================================================================
    typedef uint8_t loadmode_t;
    static const loadmode_t LOAD_RW = 1;
    static const loadmode_t LOAD_RO = 2;
    static const loadmode_t LOAD_LAZY = 3;


    // ========================================================================
    // FieldDef and StoreDef
    // ========================================================================
    class BaseDef {
    public:
      const std::string name;
      const std::string help;
      const loadmode_t mode;
      std::string serial;

    protected:
      BaseDef(const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : name(name), help(help), mode(mode), serial(serial) {
        if (!(mode == LOAD_RW || mode == LOAD_RO))
          throw ValueException("Invalid `mode' value: must be either LOAD_RW or LOAD_RO");
      }

    public:
      virtual ~BaseDef(void) { }

      virtual bool is_pointer(void) const { return false; }
      virtual bool is_store(void) const { return false; }
      virtual bool is_slice(void) const { return false; }
      virtual const TypeInfo &pointer_type(void) const { return *static_cast<const TypeInfo *>(nullptr); }
    };


    template <typename T, T fn>
    class FieldDef;

    template <typename R, typename T, R T::*member_obj_ptr>
    class FieldDef<R T::*, member_obj_ptr> : public BaseDef {
    public:
      static_assert(FieldTraits<R>::is_dr_ptr_type == false, "DR_FIELD must be used with POD fields only. Use DR_FIELD2 for schwa::dr field types instead.");
      static_assert(FieldTraits<R>::is_pod_ptr == false, "Fields cannot be POD pointers. Use schwa::dr::Pointer<T> instead.");

      FieldDef(BaseSchema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseDef(name, help, mode, serial) {
        schema.add(this);
      }
      virtual ~FieldDef(void) { }

      bool is_slice(void) const { return FieldTraits<R>::is_slice; }
    };


    template <typename T1, T1 fn1, typename T2, T2 fn2>
    class FieldDefWithStore;

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    class FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr> : public BaseDef {
    public:
      static_assert(FieldTraits<R>::is_dr_ptr_type == true, "DR_FIELD2 must be used with schwa::dr field types only");
      static_assert(boost::is_same<typename FieldTraits<R>::pointer_type, S>::value, "Field (type T) and storage field (Store<T>) must have the same type (T)");
      static_assert(boost::is_base_of<Annotation, S>::value, "Store<T> type T must be a subclass of Annotation");

    private:
      const TypeInfo _pointer_type;

    public:
      FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseDef(name, help, mode, serial), _pointer_type(TypeInfo::create<S>()) {
        schema.add(this);
      }
      virtual ~FieldDefWithStore(void) { }

      bool is_pointer(void) const { return true; }
      bool is_slice(void) const { return FieldTraits<R>::is_slice; }

      const TypeInfo &pointer_type(void) const { return _pointer_type; }
    };


    template <typename T, T fn>
    class StoreDef;

    template <typename S, typename T, Store<S> T::*member_obj_ptr>
    class StoreDef<Store<S> T::*, member_obj_ptr> : public BaseDef {
    public:
      static_assert(boost::is_base_of<Annotation, S>::value, "Store<T> type T must be a subclass of Annotation");
      typedef S store_type;

    private:
      const TypeInfo _pointer_type;

    public:
      StoreDef(BaseDocumentSchema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseDef(name, help, mode, serial), _pointer_type(TypeInfo::create<S>()) {
        schema.add(this);
      }
      virtual ~StoreDef(void) { }

      bool is_store(void) const { return true; }

      const TypeInfo &pointer_type(void) const { return _pointer_type; }
    };


    #define DR_FIELD(member_obj_ptr) schwa::dr::FieldDef<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr>
    #define DR_POINTER(member_obj_ptr, store_obj_ptr) schwa::dr::FieldDefWithStore<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr, BOOST_TYPEOF(store_obj_ptr), store_obj_ptr>
    #define DR_STORE(member_obj_ptr) schwa::dr::StoreDef<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr>

  }
}
