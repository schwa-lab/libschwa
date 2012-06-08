/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace config {

    // general errors with an error message
    class ConfigException : public Exception {
    public:
      const std::string msg;
      const std::string name;

      ConfigException(const std::string &msg, const std::string &name) : Exception(msg), msg(msg), name(name) { }
      ConfigException(const ConfigException &o) : Exception(o), msg(o.msg), name(o.name) { }
      virtual ~ConfigException(void) throw() { }

      virtual const char*
      what(void) const throw() {
        std::stringstream ss;
        ss << msg << ": " << name;
        return ss.str().c_str();
      }
    };


    class OptionBase {
    protected:
      const std::string _name;
      const std::string _desc;

      OptionBase(const std::string &name, const std::string &desc) : _name(name), _desc(desc) {
        for (int i = 0; i < name.size() - 1; ++i)
          if (name[i] == '-' && name[i + 1] == '-')
            throw ConfigException("Option names cannot contain \"--\"", _name);
      }
      OptionBase(const OptionBase &) = delete;
      OptionBase &operator =(const OptionBase &) = delete;

    public:
      virtual ~OptionBase(void) { }

      virtual void help(std::ostream &out) const = 0;
      virtual void validate(void) = 0;
    };


    class OpGroup : public OptionBase {
    protected:
      std::vector<OptionBase *> _children;

    public:
      OpGroup(const std::string &name, const std::string &desc) : OptionBase(name, desc) { }
      virtual ~OpGroup(void) { }

      inline void add(OptionBase *const child) { _children.push_back(child); }

      virtual void
      help(std::ostream &out) const {
        out << port::BOLD << _name << port::OFF << ": " << _desc << std::endl;
        for (auto &child : _children) {
          out << "  ";
          child->help(out);
        }
      }

      virtual void
      validate(void) {
        for (auto &child : _children)
          child->validate();
      }
    };


    class OpBase : public OptionBase {
    protected:
      const bool _has_default;
      bool _is_set;

      OpBase(OpGroup &group, const std::string &name, const std::string &desc, const bool has_default) : OptionBase(name, desc), _has_default(has_default) {
        group.add(this);
      }

      virtual void _set(const std::string &value) = 0;
      virtual void _validate(void) = 0;

    public:
      virtual ~OpBase(void) { }

      void
      set(const std::string &value) {
        _set(value);
        _is_set = true;
      }

      virtual void set_default(void) = 0;

      virtual void
      validate(void) {
        if (!_is_set) {
          if (!_has_default)
            throw ConfigException("Unset configuration option", _name);
          set_default();
        }
        _validate();
      }
    };


    template <typename T>
    class Op : public OpBase {
    protected:
      const T _default;
      T _value;

      virtual void _set(const std::string &value);
      virtual void _validate(void);

    public:
      Op(OpGroup &group, const std::string &name, const std::string &desc) : OpBase(group, name, desc, false) { }
      Op(OpGroup &group, const std::string &name, const std::string &desc, const T &default_) : OpBase(group, name, desc, true), _default(default_) { }
      virtual ~Op(void) { }

      virtual void
      help(std::ostream &out) const {
        out << port::BOLD << _name << port::OFF << ": " << _desc;
        if (_has_default)
          out << " (" << _default << ")";
        out << std::endl;
      }

      virtual void
      set_default(void) {
        _value = _default;
      }
    };


    // ------------------------------------------------------------------------
    template <>
    void
    Op<std::string>::_set(const std::string &value) {
      _value = value;
    }

    template <>
    void
    Op<std::string>::_validate(void) { }

  }
}
