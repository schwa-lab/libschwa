/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class BaseDef;


    class Schema {
    protected:
      std::vector<BaseDef *> _defs;

      Schema(void) { }
      virtual ~Schema(void) { }

    public:
      inline void add(BaseDef *def) { _defs.push_back(def); }
    };


    class TypeSchema : public Schema {
    protected:
      const std::string _name;
      const std::string _help;
      std::string _serial;

    public:
      TypeSchema(const std::string &name, const std::string &help, const std::string &serial) : Schema(), _name(name), _help(help), _serial(serial) { }
      virtual ~TypeSchema(void) { }

      inline const std::string &field_name(void) const { return _name; }
      inline const std::string &help(void) const { return _help; }
      inline std::string serial(void) const { return _serial; }

      inline void set_serial(const std::string &serial) { _serial = serial; }
    };


    class DocSchema : public Schema {
    public:
      DocSchema(void) : Schema() { }
      virtual ~DocSchema(void) { }
    };

  }
}
