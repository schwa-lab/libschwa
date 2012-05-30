/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/type_traits.hpp>

namespace schwa {
  namespace dr {

    class BaseAnnotationSchema;
    class BaseFieldDef;
    class BaseStoreDef;

    template <typename T, T fn>
    class StoreDef;


    // ========================================================================
    // BaseSchema definitions
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

      BaseSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : name(name), help(help), serial(serial), type(type) { }

    public:
      virtual ~BaseSchema(void) { }

      inline void add(BaseFieldDef *const field) { _fields.push_back(field); }

      inline const field_container &fields(void) const { return _fields; }

      virtual std::ostream &dump(std::ostream &out) const;
    };


    inline std::ostream &
    operator <<(std::ostream &out, const BaseSchema &s) {
      return s.dump(out);
    }


    // ========================================================================
    // Base classes
    // ========================================================================
    class Annotation {
    protected:
      Annotation(void) { }
      Annotation(const Annotation &) { }
      Annotation(const Annotation &&) { }
      Annotation &operator =(const Annotation &) { return *this; }
    };


    class Document {
    protected:
      Document(void) { }
      Document(const Document &) = delete;
    };


    class BaseAnnotationSchema : public BaseSchema {
    protected:
      BaseAnnotationSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : BaseSchema(name, help, serial, type) { }

    public:
      virtual ~BaseAnnotationSchema(void) { }

      virtual std::ostream &dump(std::ostream &out) const;
    };


    class BaseDocumentSchema : public BaseSchema {
    public:
      typedef std::vector<BaseAnnotationSchema *> schema_container;
      typedef std::vector<BaseStoreDef *> store_container;

    protected:
      schema_container _schemas;
      store_container _stores;

      BaseDocumentSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : BaseSchema(name, help, serial, type) { }

    public:
      virtual ~BaseDocumentSchema(void) {
        for (auto &s : _schemas)
          delete s;
      }

      template <typename T, T fn>
      inline void add(StoreDef<T, fn> *const store) {
        typedef typename StoreDef<T, fn>::store_type::Schema S;
        static_assert(boost::is_base_of<BaseAnnotationSchema, S>::value, "T::Schema for the Store<T> must be a subclass of BaseAnnotationSchema");

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
        static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");
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
    class AnnotationSchema : public BaseAnnotationSchema {
    public:
      static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");

      AnnotationSchema(const std::string &name, const std::string &help, const std::string &serial) : BaseAnnotationSchema(name, help, serial, TypeInfo::create<T>()) { }
      virtual ~AnnotationSchema(void) { }
    };


    template <typename D>
    class DocumentSchema : public BaseDocumentSchema {
    public:
      static_assert(boost::is_base_of<Document, D>::value, "D must be a subclass of Document");

      DocumentSchema(const std::string &name, const std::string &help) : BaseDocumentSchema(name, help, "", TypeInfo::create<D>()) { }
      virtual ~DocumentSchema(void) { }
    };

  }
}
