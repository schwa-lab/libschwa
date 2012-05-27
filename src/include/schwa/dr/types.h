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
    protected:
      const std::string _name;
      const std::string _help;
      const loadmode_t _mode;
      std::string _serial;

      BaseDef(Schema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : _name(name), _help(help), _mode(mode), _serial(serial) {
        if (!(mode == LOAD_RW || mode == LOAD_RO))
          throw ValueException("Invalid `mode' value: must be either LOAD_RW or LOAD_RO");
        schema.add(this);
      }

    public:
      virtual ~BaseDef(void) { }
    };


    template <typename T, T fn>
    class FieldDef;

    template <typename R, typename T, R T::*member_obj_ptr>
    class FieldDef<R T::*, member_obj_ptr> : public BaseDef {
    public:
      static_assert(FieldTraits<R>::is_dr_ptr_type == false, "DR_FIELD must be used with POD fields only. Use DR_FIELD2 for schwa::dr field types instead.");
      static_assert(FieldTraits<R>::is_pod_ptr == false, "Fields cannot be POD pointers. Use schwa::dr::Pointer<T> instead.");

      FieldDef(Schema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseDef(schema, name, help, mode, serial) { }
      virtual ~FieldDef(void) { }
    };


    template <typename T1, T1 fn1, typename T2, T2 fn2>
    class FieldDefWithStore;

    template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
    class FieldDefWithStore<R T::*, field_ptr, Store<S> D::*, store_ptr> : public BaseDef {
    public:
      static_assert(FieldTraits<R>::is_dr_ptr_type == true, "DR_FIELD2 must be used with schwa::dr field types only");
      static_assert(boost::is_same<typename FieldTraits<R>::pointer_type, S>::value, "Field (type T) and storage field (Store<T>) must have the same type (T)");
      static_assert(boost::is_base_of<Annotation, S>::value, "Store<T> type T must be a subclass of Annotation");

      FieldDefWithStore(Schema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseDef(schema, name, help, mode, serial) { }
      virtual ~FieldDefWithStore(void) { }
    };


    template <typename T, T fn>
    class StoreDef;

    template <typename S, typename T, Store<S> T::*member_obj_ptr>
    class StoreDef<Store<S> T::*, member_obj_ptr> : public BaseDef {
    public:
      static_assert(boost::is_base_of<Annotation, S>::value, "Store<T> type T must be a subclass of Annotation");

      StoreDef(Schema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseDef(schema, name, help, mode, serial) { }
      virtual ~StoreDef(void) { }
    };


    #define DR_FIELD(member_obj_ptr) schwa::dr::FieldDef<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr>
    #define DR_FIELD2(member_obj_ptr, store_obj_ptr) schwa::dr::FieldDefWithStore<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr, BOOST_TYPEOF(store_obj_ptr), store_obj_ptr>
    #define DR_STORE(member_obj_ptr) schwa::dr::StoreDef<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr>


  }
}
