/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <initializer_list>

namespace schwa {
  namespace config {

    class OpBase : public OptionBase {
    protected:
      const bool _has_default;
      bool _is_set;

      OpBase(OpGroup &group, const std::string &name, const std::string &desc, const bool has_default);

      virtual void _set(const std::string &value) = 0;
      virtual void _validate(void) = 0;

    public:
      virtual ~OpBase(void) { }

      virtual OptionBase *find(const std::string &, const std::string key);
      virtual void set(const std::string &value);
      virtual void set_default(void) = 0;
      virtual void validate(void);
    };


    template <typename T>
    class Op : public OpBase {
    protected:
      const T _default;
      T _value;

      virtual void
      _set(const std::string &value) {
        std::stringstream ss(value);
        if (!(ss >> _value))
          throw ConfigException("Error setting value", _name, value);
      }
      virtual void _validate(void) { }

    public:
      Op(OpGroup &group, const std::string &name, const std::string &desc) : OpBase(group, name, desc, false) { }
      Op(OpGroup &group, const std::string &name, const std::string &desc, const T &default_) : OpBase(group, name, desc, true), _default(default_) { }
      virtual ~Op(void) { }

      virtual void
      help(std::ostream &out, const std::string &prefix, unsigned int) const {
        out << "  " << port::BOLD << prefix << _name << port::OFF << ": " << _desc;
        if (_has_default)
          out << " (" << _default << ")";
        out << std::endl;
      }

      virtual void
      set_default(void) {
        _value = _default;
      }

      inline const T &operator ()(void) const { return _value; }
    };


    template <> inline void Op<std::string>::_set(const std::string &value) { _value = value; }


    template <typename T>
    class EnumOp : public Op<T> {
    protected:
      std::set<T> _options;

      virtual void
      _validate(void) {
        if (_options.find(Op<T>::_value) == _options.end())
          throw ConfigException("Invalid enum value", Op<T>::_name, Op<T>::_value);
      }

    public:
      EnumOp(OpGroup &group, const std::string &name, const std::string &desc, std::initializer_list<T> options) : Op<T>(group, name, desc), _options(options) { }
      EnumOp(OpGroup &group, const std::string &name, const std::string &desc, std::initializer_list<T> options, const T &default_) : Op<T>(group, name, desc, default_), _options(options) { }
      virtual ~EnumOp(void) { }

      virtual void
      help(std::ostream &out, const std::string &prefix, unsigned int) const {
        out << "  " << port::BOLD << prefix << Op<T>::_name << port::OFF << ": " << Op<T>::_desc;
        out << " {";
        bool first = true;
        for (auto &it : _options) {
          if (!first)
            out << ",";
          out << it;
          first = false;
        }
        out << "}";
        if (Op<T>::_has_default)
          out << " (" << Op<T>::_default << ")";
        out << std::endl;
      }
    };


    class IStreamOp : public Op<std::string> {
    public:
      static const char *const STDIN_STRING;

    protected:
      std::istream *_in;
      bool _is_stdin;

      virtual void _validate(void);

    public:
      IStreamOp(OpGroup &group, const std::string &name, const std::string &desc) : Op<std::string>(group, name, desc, STDIN_STRING), _in(nullptr), _is_stdin(false) { }
      virtual ~IStreamOp(void);

      inline std::istream &file(void) const { return *_in; }
    };


    class OStreamOp : public Op<std::string> {
    public:
      static const char *const STDOUT_STRING;

    protected:
      std::ostream *_out;
      bool _is_stdout;

      virtual void _validate(void);

    public:
      OStreamOp(OpGroup &group, const std::string &name, const std::string &desc) : Op<std::string>(group, name, desc, STDOUT_STRING), _out(nullptr), _is_stdout(false) { }
      virtual ~OStreamOp(void);

      inline std::ostream &file(void) const { return *_out; }
    };

  }
}
