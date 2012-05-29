/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/type_traits.hpp>

namespace schwa {
  namespace dr {

    class BaseAnnotationSchema;
    class BaseDef;

    template <typename T, T fn>
    class StoreDef;


    // ========================================================================
    // BaseSchema definitions
    // ========================================================================
    class BaseSchema {
    public:
      typedef std::vector<BaseDef *> field_container;

      const std::string name;
      const std::string help;
      std::string serial;
      const TypeInfo type;
      const bool is_document_schema;

    protected:
      field_container _defs;

      BaseSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type, const bool is_document_schema) : name(name), help(help), serial(serial), type(type), is_document_schema(is_document_schema) { }

    public:
      virtual ~BaseSchema(void) { }

      inline void add(BaseDef *def) { _defs.push_back(def); }

      inline field_container::const_iterator begin(void) const { return _defs.begin(); }
      inline field_container::const_iterator end(void) const { return _defs.end(); }
    };


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
      BaseAnnotationSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : BaseSchema(name, help, serial, type, false) { }

    public:
      virtual ~BaseAnnotationSchema(void) { }
    };


    class BaseDocumentSchema : public BaseSchema {
    public:
      typedef std::vector<BaseAnnotationSchema *> schema_container;

    protected:
      schema_container _schemas;

      BaseDocumentSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : BaseSchema(name, help, serial, type, true) { }

    public:
      virtual ~BaseDocumentSchema(void) {
        for (auto &s : _schemas)
          delete s;
      }

      template <typename T, T fn>
      inline void add(StoreDef<T, fn> *def) {
        typedef typename StoreDef<T, fn>::store_type::Schema S;
        static_assert(boost::is_base_of<BaseAnnotationSchema, S>::value, "T::Schema for the Store<T> must be a subclass of BaseAnnotationSchema");

        _defs.push_back(def);

        // check to see if we have not yet seen this type
        const TypeInfo &t = def->pointer_type();
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
