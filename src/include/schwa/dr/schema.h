/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <typeinfo>

#include <boost/type_traits.hpp>

namespace schwa {
  namespace dr {

    class BaseAnnotationSchema;
    class BaseDef;

    template <typename T, T fn>
    class StoreDef;

    // ========================================================================
    // Runtime type information
    // ========================================================================
    class TypeInfo {
    public:
      const std::string name;

    protected:
      TypeInfo(const std::string &typeid_name) : name(port::demangle_typeid(typeid_name.c_str())) { }

    public:
      ~TypeInfo(void) { }

      inline bool operator ==(const TypeInfo &o) const { return name == o.name; }
      inline bool operator !=(const TypeInfo &o) const { return name != o.name; }
      inline bool operator <(const TypeInfo &o) const { return name < o.name; }

      template <typename T>
      static TypeInfo create(void) { return TypeInfo(typeid(T).name()); }
    };

    inline std::ostream &
    operator <<(std::ostream &out, const TypeInfo &type ) {
      return out << type.name;
    }


    // ========================================================================
    // Schema definitions
    // ========================================================================
    class Schema {
    public:
      typedef std::vector<BaseDef *> field_container;
      typedef std::vector<BaseAnnotationSchema *> schema_container;

      const std::string name;
      const std::string help;
      std::string serial;
      const TypeInfo type;
      const bool is_document_schema;

    protected:
      field_container _defs;
      schema_container _schemas;
      bool _finalised;

      Schema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type, const bool is_document_schema) : name(name), help(help), serial(serial), type(type), is_document_schema(is_document_schema), _finalised(false) { }

    public:
      virtual ~Schema(void);

      inline void add(BaseDef *def) { _defs.push_back(def); }

      template <typename T, T fn>
      inline void add(StoreDef<T, fn> *def);

      inline field_container::const_iterator begin(void) const { return _defs.begin(); }
      inline field_container::const_iterator end(void) const { return _defs.end(); }
    };


    // ========================================================================
    // Base classes
    // ========================================================================
    class Annotation {
    protected:
      size_t _dr_index;

      Annotation(void) : _dr_index(0) { }
      Annotation(const Annotation &) = delete;

    public:
      inline void set_dr_index(size_t dr_index) { _dr_index = dr_index; }
    };


    class Document {
    protected:
      Document(void) { }
      Document(const Document &) = delete;
    };


    class BaseAnnotationSchema : public Schema {
    protected:
      BaseAnnotationSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : Schema(name, help, serial, type, false) { }

    public:
      virtual ~BaseAnnotationSchema(void) { }
    };


    class BaseDocumentSchema : public Schema {
    protected:
      BaseDocumentSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : Schema(name, help, serial, type, true) { }

    public:
      virtual ~BaseDocumentSchema(void) { }

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


    // ========================================================================
    // Out-of-line declaration of Schema
    // ========================================================================
    template <typename T, T fn>
    inline void
    Schema::add(StoreDef<T, fn> *def) {
      typedef typename StoreDef<T, fn>::store_type::Schema S;
      static_assert(boost::is_base_of<BaseAnnotationSchema, S>::value, "T::Schema for the Store<T> must be a subclass of BaseAnnotationSchema");

      _defs.push_back(def);

      _finalised = false;
      S *schema = new S();
      assert(schema != nullptr);
      _schemas.push_back(schema);
    }

  }
}
