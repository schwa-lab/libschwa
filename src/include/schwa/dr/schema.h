/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <typeinfo>

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

      inline bool operator ==(const TypeInfo &o) { return name == o.name; }
      inline bool operator !=(const TypeInfo &o) { return name != o.name; }

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

    protected:
      container_type _defs;
      const std::string _name;
      const std::string _help;
      std::string _serial;
      const TypeInfo _type;
      const bool _is_doc;

      Schema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type, const bool is_doc) : _name(name), _help(help), _serial(serial), _type(type), _is_doc(is_doc) { }

    public:
      virtual ~Schema(void) { }

      inline void add(BaseDef *def) { _defs.push_back(def); }

      inline const std::string &name(void) const { return _name; }
      inline const std::string &help(void) const { return _help; }
      inline std::string serial(void) const { return _serial; }
      inline const TypeInfo &type(void) const { return _type; }

      inline container_type::const_iterator begin(void) const { return _defs.begin(); }
      inline container_type::const_iterator end(void) const { return _defs.end(); }

      virtual bool is_document(void) const { return false; }

      inline void
      set_serial(const std::string &serial) {
        if (_is_doc)
          throw ValueException("Cannot set the serial value for a Document schema");
        _serial = serial;
      }
    };


    class TypeSchema : public Schema {
    public:
      TypeSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : Schema(name, help, serial, type, false) { }
      virtual ~TypeSchema(void) { }
    };


    class DocSchema : public Schema {
    public:
      DocSchema(const std::string &name, const std::string &help, const TypeInfo &type) : Schema(name, help, "", type, true) { }
      virtual ~DocSchema(void) { }

      bool is_document(void) const { return true; }
    };


    // ========================================================================
    // Base classes
    // ========================================================================
    class Annotation {
    public:
      typedef TypeSchema Schema;

    protected:
      size_t _dr_index;

      Annotation(void) : _dr_index(0) { }
      Annotation(const Annotation &) = delete;

    public:
      inline void set_dr_index(size_t dr_index) { _dr_index = dr_index; }
    };


    class Document {
    public:
      typedef DocSchema Schema;

    protected:
      Document(void) { }
      Document(const Document &) = delete;
    };



  }
}
