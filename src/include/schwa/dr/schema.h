/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/type_traits.hpp>

namespace schwa {
  namespace dr {

    class BaseAnnSchema;
    class BaseFieldDef;
    class BaseStoreDef;

    template <typename T, T fn>
    class FieldDef;
    template <typename T1, T1 fn1, typename T2, T2 fn2>
    class FieldDefWithStore;
    template <typename T, T fn>
    class StoreDef;

    class Reader;
    class Writer;


    // ========================================================================
    // BaseSchema definitions
    // ========================================================================
    class BaseSchema {
    public:
      typedef std::vector<BaseFieldDef *> field_container;
      typedef std::vector<void (*)(io::ArrayReader &in, void *, void *)> readers_container;
      typedef std::vector<bool (*)(io::WriteBuffer &out, const unsigned int, const void *, const void *)> writers_container;

      const std::string name;
      const std::string help;
      std::string serial;
      const TypeInfo type;

    protected:
      field_container _fields;
      readers_container _readers;
      writers_container _writers;

      BaseSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : name(name), help(help), serial(serial), type(type) { }

    public:
      virtual ~BaseSchema(void) { }

      template <typename T, T fn>
      inline void add(FieldDef<T, fn> *const field) {
        typedef FieldDef<T, fn> F;
        _fields.push_back(field);
        _readers.push_back(&wire::read_field<io::ArrayReader, typename F::value_type, typename F::annotation_type, fn>);
        _writers.push_back(&wire::write_field<io::WriteBuffer, typename F::value_type, typename F::annotation_type, fn>);
      }

      template <typename T1, T1 fn1, typename T2, T2 fn2>
      inline void add(FieldDefWithStore<T1, fn1, T2, fn2> *const field) {
        typedef FieldDefWithStore<T1, fn1, T2, fn2> F;
        _fields.push_back(field);
        _readers.push_back(&wire::read_field<io::ArrayReader, typename F::value_type, typename F::annotation_type, typename F::store_type, typename F::doc_type, fn1, fn2>);
        _writers.push_back(&wire::write_field<io::WriteBuffer, typename F::value_type, typename F::annotation_type, typename F::store_type, typename F::doc_type, fn1, fn2>);
      }

      inline const field_container &fields(void) const { return _fields; }
      inline const readers_container &readers(void) const { return _readers; }
      inline const writers_container &writers(void) const { return _writers; }

      virtual std::ostream &dump(std::ostream &out) const;
    };


    inline std::ostream &
    operator <<(std::ostream &out, const BaseSchema &s) {
      return s.dump(out);
    }


    // ========================================================================
    // Base classes
    // ========================================================================
    class Ann {
    private:
      const char *_lazy;
      uint32_t _lazy_nelem;
      uint32_t _lazy_nbytes;

      friend class Reader;
      friend class Writer;

    public:
      explicit Ann(void) : _lazy(nullptr), _lazy_nelem(0), _lazy_nbytes(0) { }
      Ann(const char *lazy, const uint32_t nelem, const uint32_t nbytes) : _lazy(lazy), _lazy_nelem(nelem), _lazy_nbytes(nbytes) { }
      Ann(const Ann &o) : _lazy(o._lazy), _lazy_nelem(o._lazy_nelem), _lazy_nbytes(o._lazy_nbytes) { }
      Ann(const Ann &&o) : _lazy(o._lazy), _lazy_nelem(o._lazy_nelem), _lazy_nbytes(o._lazy_nbytes) { }
      ~Ann(void) { }

      Ann &
      operator =(const Ann &o) {
        _lazy = o._lazy;
        _lazy_nelem = o._lazy_nelem;
        _lazy_nbytes = o._lazy_nbytes;
        return *this;
      }

      inline const char *lazy(void) const { return _lazy; }
      inline uint32_t lazy_nelem(void) const { return _lazy_nelem; }
      inline uint32_t lazy_nbytes(void) const { return _lazy_nbytes; }

      template <typename T> class Schema;
    };


    class Doc {
    private:
      std::vector<const char *> _lazy_buffers;
      const char *_lazy;
      uint32_t _lazy_nelem;
      uint32_t _lazy_nbytes;

      friend class Reader;
      friend class Writer;

    protected:
      Doc(void) { }
      Doc(const Doc &) = delete;
      Doc(const Doc &&) = delete;
      ~Doc(void) {
        for (auto &p : _lazy_buffers)
          delete [] p;
      }

    public:
      inline const char *lazy(void) const { return _lazy; }
      inline uint32_t lazy_nelem(void) const { return _lazy_nelem; }
      inline uint32_t lazy_nbytes(void) const { return _lazy_nbytes; }

      template <typename T> class Schema;
    };


    class BaseAnnSchema : public BaseSchema {
    protected:
      BaseAnnSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : BaseSchema(name, help, serial, type) { }

    public:
      virtual ~BaseAnnSchema(void) { }

      virtual std::ostream &dump(std::ostream &out) const;
    };


    class BaseDocSchema : public BaseSchema {
    public:
      typedef std::vector<BaseAnnSchema *> schema_container;
      typedef std::vector<BaseStoreDef *> store_container;

    protected:
      schema_container _schemas;
      store_container _stores;

      BaseDocSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : BaseSchema(name, help, serial, type) { }

    public:
      virtual ~BaseDocSchema(void) {
        for (auto &s : _schemas)
          delete s;
      }

      template <typename T, T fn>
      inline void add(StoreDef<T, fn> *const store) {
        typedef typename StoreDef<T, fn>::store_type::Schema S;
        static_assert(boost::is_base_of<BaseAnnSchema, S>::value, "T::Schema for the Store<T> must be a subclass of BaseAnnSchema");

        _stores.push_back(store);

        // check to see if we have not yet seen this type
        const TypeInfo &t = store->pointer_type();
        for (auto &s : _schemas)
          if (s->type == t)
            return;
        S *const schema = new S();
        assert(schema != nullptr);
        _schemas.push_back(schema);
      }

      template <typename T>
      inline typename T::Schema &
      types(void) const {
        static_assert(boost::is_base_of<Ann, T>::value, "T must be a subclass of Ann");
        const TypeInfo type = TypeInfo::create<T>();
        for (auto &it : _schemas)
          if (it->type == type)
            return *static_cast<typename T::Schema *>(it);
        assert(!"Type was not found!");
        return *static_cast<typename T::Schema *>(nullptr);
      }

      inline const schema_container &schemas(void) const { return _schemas; }
      inline const store_container &stores(void) const { return _stores; }

      virtual std::ostream &dump(std::ostream &out) const;
    };


    // ========================================================================
    // Templated base schemas
    // ========================================================================
    template <typename T>
    class Ann::Schema : public BaseAnnSchema {
    public:
      static_assert(boost::is_base_of<Ann, T>::value, "T must be a subclass of Ann");

      Schema(const std::string &name, const std::string &help, const std::string &serial) : BaseAnnSchema(name, help, serial, TypeInfo::create<T>()) { }
      virtual ~Schema(void) { }
    };


    template <typename D>
    class Doc::Schema : public BaseDocSchema {
    public:
      static_assert(boost::is_base_of<Doc, D>::value, "D must be a subclass of Doc");

      Schema(const std::string &name, const std::string &help) : BaseDocSchema(name, help, "", TypeInfo::create<D>()) { }
      virtual ~Schema(void) { }
    };

  }
}
