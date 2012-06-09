/* -*- Mode: C++; indent-tabs-mode: nil -*- */

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

      virtual void _set(const std::string &value);
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


    template <typename T>
    inline void
    set_value(const std::string &str, T &value, const std::string &name) {
      std::stringstream ss(str);
      if (!(ss >> value))
        throw ConfigException("Error setting value", name, str);
    }

    template <> inline void Op<std::string>::_set(const std::string &value) { _value = value; }
    template <> inline void Op<int8_t>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<int16_t>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<int32_t>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<int64_t>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<uint8_t>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<uint16_t>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<uint32_t>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<uint64_t>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<long>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<unsigned long>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<float>::_set(const std::string &value) { set_value(value, _value, _name); }
    template <> inline void Op<double>::_set(const std::string &value) { set_value(value, _value, _name); }

  }
}
