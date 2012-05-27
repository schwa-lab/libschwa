/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class BaseDef;


    class Schema {
    public:
      typedef std::vector<BaseDef *> container_type;

    protected:
      container_type _defs;
      const std::string _name;
      const std::string _help;
      std::string _serial;
      const bool _is_doc;

      Schema(const std::string &name, const std::string &help, const std::string &serial, const bool is_doc) : _name(name), _help(help), _serial(serial), _is_doc(is_doc) { }

    public:
      virtual ~Schema(void) { }

      inline void add(BaseDef *def) { _defs.push_back(def); }

      inline const std::string &name(void) const { return _name; }
      inline const std::string &help(void) const { return _help; }
      inline std::string serial(void) const { return _serial; }

      inline container_type::const_iterator begin(void) const { return _defs.begin(); }
      inline container_type::const_iterator end(void) const { return _defs.end(); }

      inline void
      set_serial(const std::string &serial) {
        if (_is_doc)
          throw ValueException("Cannot set the serial value for a Document schema");
        _serial = serial;
      }
    };


    class TypeSchema : public Schema {
    public:
      TypeSchema(const std::string &name, const std::string &help, const std::string &serial) : Schema(name, help, serial, false) { }
      virtual ~TypeSchema(void) { }
    };


    class DocSchema : public Schema {
    public:
      DocSchema(const std::string &name, const std::string &help) : Schema(name, help, "", true) { }
      virtual ~DocSchema(void) { }
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
      Document(void) = default;
      Document(const Document &) = delete;
    };



  }
}
