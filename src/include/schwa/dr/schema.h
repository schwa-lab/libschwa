/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_SCHEMA_H_
#define SCHWA_DR_SCHEMA_H_

#include <cassert>
#include <iosfwd>
#include <string>
#include <type_traits>
#include <vector>

#include <schwa/_base.h>
#include <schwa/dr/type_info.h>

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

    class RTManager;

    class Reader;
    class Writer;


    // ========================================================================
    // BaseSchema
    // ========================================================================
    class BaseSchema {
    public:
      typedef std::vector<BaseFieldDef *> field_container;

      const std::string name;
      const std::string help;
      std::string serial;
      const TypeInfo type;

    protected:
      field_container _fields;

      BaseSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type);

    public:
      virtual ~BaseSchema(void) { }

      inline void add(BaseFieldDef *const field) { _fields.push_back(field); }
      inline const field_container &fields(void) const { return _fields; }

      virtual std::ostream &dump(std::ostream &out) const;

    private:
      DISALLOW_COPY_AND_ASSIGN(BaseSchema);
    };

    std::ostream &operator <<(std::ostream &out, const BaseSchema &s);


    // ========================================================================
    // Base classes
    // ========================================================================
    class Lazy {
    protected:
      friend class Reader;
      friend class Writer;

      const char *_lazy;
      uint32_t _lazy_nelem;
      uint32_t _lazy_nbytes;

      Lazy(void);
      Lazy(const char *lazy, uint32_t nelem, uint32_t nbytes);
      Lazy(const Lazy &o);
      Lazy(const Lazy &&o);

      Lazy &operator =(const Lazy &o);

    public:
      ~Lazy(void) { }

      inline const char *lazy_data(void) const { return _lazy; }
      inline uint32_t lazy_nelem(void) const { return _lazy_nelem; }
      inline uint32_t lazy_nbytes(void) const { return _lazy_nbytes; }
    };


    class Ann : public Lazy {
    public:
      explicit Ann(void) : Lazy() { }
      Ann(const char *lazy, const uint32_t nelem, const uint32_t nbytes) : Lazy(lazy, nelem, nbytes) { }
      Ann(const Ann &o) : Lazy(o) { }
      Ann(const Ann &&o) : Lazy(o) { }
      ~Ann(void) { }

      Ann &operator =(const Ann &) { return *this; }

      template <typename T> class Schema;
    };


    class Doc : public Lazy {
    private:
      friend class Reader;
      friend class Writer;

      RTManager *_rt;

    protected:
      Doc(void);

    public:
      ~Doc(void);

      RTManager *rt(void) { return _rt; }
      const RTManager *rt(void) const { return _rt; }

      template <typename T> class Schema;

    private:
      DISALLOW_COPY_AND_ASSIGN(Doc);
    };


    class BaseAnnSchema : public BaseSchema {
    protected:
      BaseAnnSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type);

    public:
      virtual ~BaseAnnSchema(void) { }

      virtual std::ostream &dump(std::ostream &out) const override;
    };


    class BaseDocSchema : public BaseSchema {
    public:
      typedef std::vector<BaseAnnSchema *> schema_container;
      typedef std::vector<BaseStoreDef *> store_container;

    protected:
      schema_container _schemas;
      store_container _stores;

      BaseDocSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type);

    public:
      virtual ~BaseDocSchema(void);

      template <typename T, T fn>
      inline void
      add(StoreDef<T, fn> *const store) {
        typedef typename StoreDef<T, fn>::store_type::Schema S;
        static_assert(std::is_base_of<BaseAnnSchema, S>::value, "T::Schema for the Store<T> must be a subclass of BaseAnnSchema");

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
        static_assert(std::is_base_of<Ann, T>::value, "T must be a subclass of Ann");
        const TypeInfo type = TypeInfo::create<T>();
        for (auto &it : _schemas)
          if (it->type == type)
            return *static_cast<typename T::Schema *>(it);
        assert(!"Type was not found!");
        return *static_cast<typename T::Schema *>(nullptr);
      }

      inline const schema_container &schemas(void) const { return _schemas; }
      inline const store_container &stores(void) const { return _stores; }

      virtual std::ostream &dump(std::ostream &out) const override;
    };


    // ========================================================================
    // Templated base schemas
    // ========================================================================
    template <typename T>
    class Ann::Schema : public BaseAnnSchema {
    public:
      static_assert(std::is_base_of<Ann, T>::value, "T must be a subclass of Ann");

      Schema(const std::string &name, const std::string &help) : Schema(name, help, name) { }
      Schema(const std::string &name, const std::string &help, const std::string &serial) : BaseAnnSchema(name, help, serial, TypeInfo::create<T>()) { }
      virtual ~Schema(void) { }
    };


    template <typename D>
    class Doc::Schema : public BaseDocSchema {
    public:
      static_assert(std::is_base_of<Doc, D>::value, "D must be a subclass of Doc");

      Schema(const std::string &name, const std::string &help) : BaseDocSchema(name, help, "", TypeInfo::create<D>()) { }
      virtual ~Schema(void) { }
    };

  }
}

#endif  // SCHWA_DR_SCHEMA_H_
