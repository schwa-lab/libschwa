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
    // Base classes for field and store definitions
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

      virtual const TypeInfo &pointer_type(void) const { return *static_cast<const TypeInfo *>(nullptr); }
    };


    class BaseFieldDef : public BaseDef {
    public:
      const bool is_pointer;
      const bool is_slice;

    protected:
      BaseFieldDef(const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial, const bool is_pointer, const bool is_slice) : BaseDef(name, help, mode, serial), is_pointer(is_pointer), is_slice(is_slice) { }

    public:
      virtual ~BaseFieldDef(void) { }

      virtual ptrdiff_t store_offset(const Doc &) const { assert(!"this should never be invoked"); return -1; }
    };


    class BaseStoreDef : public BaseDef {
    protected:
      BaseStoreDef(const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseDef(name, help, mode, serial) { }

    public:
      virtual ~BaseStoreDef(void) { }

      virtual void resize(Doc &doc, const size_t size) const = 0;

      virtual size_t size(const Doc &doc) const = 0;
      virtual ptrdiff_t store_offset(const Doc &) const = 0;
      virtual void write(std::ostream &out, const Doc &_doc, const BaseSchema &schema, void (*writer)(std::ostream &, const Doc &, const BaseSchema &, const void *const)) const = 0;

      virtual char *read_begin(Doc &_doc) const = 0;
      virtual size_t read_size(void) const = 0;
    };


    // ========================================================================
    // Templated FieldDef and StoreDef
    // ========================================================================
    template <typename T, T fn>
    class FieldDef;

    template <typename R, typename T, R T::*member_obj_ptr>
    class FieldDef<R T::*, member_obj_ptr> : public BaseFieldDef {
    public:
      static_assert(FieldTraits<R>::is_dr_ptr_type == false, "DR_FIELD must be used with POD fields only. Use DR_POINTER for schwa::dr field types instead.");
      static_assert(FieldTraits<R>::is_pod_ptr == false, "Fields cannot be POD pointers. Use schwa::dr::Pointer<T> instead.");
      typedef R value_type;
      typedef T annotation_type;

      FieldDef(BaseSchema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseFieldDef(name, help, mode, serial, false, FieldTraits<R>::is_slice) {
        schema.add(this);
      }
      virtual ~FieldDef(void) { }
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
      FieldDefWithStore(BaseSchema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseFieldDef(name, help, mode, serial, true, FieldTraits<R>::is_slice), _pointer_type(TypeInfo::create<S>()) {
        schema.add(this);
      }
      virtual ~FieldDefWithStore(void) { }

      const TypeInfo &pointer_type(void) const { return _pointer_type; }
      ptrdiff_t store_offset(const Doc &doc) const { return reinterpret_cast<const char *>(&(static_cast<const D &>(doc).*store_ptr)) - reinterpret_cast<const char *>(&doc); }
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
      StoreDef(BaseDocSchema &schema, const std::string &name, const std::string &help, const loadmode_t mode, const std::string &serial) : BaseStoreDef(name, help, mode, serial), _pointer_type(TypeInfo::create<S>()) {
        schema.add(this);
      }
      virtual ~StoreDef(void) { }

      void resize(Doc &doc, const size_t size) const { (static_cast<T &>(doc).*store_ptr).resize(size); }

      const TypeInfo &pointer_type(void) const { return _pointer_type; }
      size_t size(const Doc &doc) const { return (static_cast<const T &>(doc).*store_ptr).size(); }
      ptrdiff_t store_offset(const Doc &doc) const { return reinterpret_cast<const char *>(&(static_cast<const T &>(doc).*store_ptr)) - reinterpret_cast<const char *>(&doc); }

      char *
      read_begin(Doc &_doc) const {
        T &doc = static_cast<T &>(_doc);
        Store<S> &store = doc.*store_ptr;
        return reinterpret_cast<char *>(&store[0]);
      }

      inline size_t read_size(void) const { return sizeof(S); }

      void
      write(std::ostream &out, const Doc &_doc, const BaseSchema &schema, void (*writer)(std::ostream &, const Doc &, const BaseSchema &, const void *const)) const {
        namespace mp = schwa::msgpack;
        const T &doc = static_cast<const T &>(_doc);
        const Store<S> &store = doc.*store_ptr;

        // <instances> ::= [ <instance> ]
        mp::write_array_size(out, store.size());
        for (auto &obj : store)
          writer(out, _doc, schema, &obj);
      }
    };


    #define DR_FIELD(member_obj_ptr) schwa::dr::FieldDef<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr>
    #define DR_POINTER(member_obj_ptr, store_obj_ptr) schwa::dr::FieldDefWithStore<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr, BOOST_TYPEOF(store_obj_ptr), store_obj_ptr>
    #define DR_STORE(member_obj_ptr) schwa::dr::StoreDef<BOOST_TYPEOF(member_obj_ptr), member_obj_ptr>

  }
}
