/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <typeinfo>

#include <boost/type_traits.hpp>

namespace schwa {
  namespace dr {

    class BaseDef;

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
      typedef std::vector<BaseDef *> container_type;

      const std::string name;
      const std::string help;
      std::string serial;
      const TypeInfo type;
      const bool is_document_schema;

    protected:
      container_type _defs;

      Schema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type, const bool is_document_schema) : name(name), help(help), serial(serial), type(type), is_document_schema(is_document_schema) { }

    public:
      virtual ~Schema(void) { }

      inline void add(BaseDef *def) { _defs.push_back(def); }

      inline container_type::const_iterator begin(void) const { return _defs.begin(); }
      inline container_type::const_iterator end(void) const { return _defs.end(); }
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


    // ========================================================================
    // Templated base schemas
    // ========================================================================
    template <typename T>
    class AnnotationSchema : public Schema {
    public:
      static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");

      AnnotationSchema(const std::string &name, const std::string &help, const std::string &serial) : Schema(name, help, serial, TypeInfo::create<T>(), false) { }
      virtual ~AnnotationSchema(void) { }
    };


    template <typename D>
    class DocumentSchema : public Schema {
    public:
      static_assert(boost::is_base_of<Document, D>::value, "D must be a subclass of Document");

      DocumentSchema(const std::string &name, const std::string &help) : Schema(name, help, "", TypeInfo::create<D>(), true) { }
      virtual ~DocumentSchema(void) { }
    };

  }
}
