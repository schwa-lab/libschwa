/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace config {

    class OptBase {
    protected:
      const std::string _name;
      const std::string _desc;

      OptBase(const std::string &name, const std::string &desc) : _name(name), _desc(desc) { }
      OptBase(const OptBase &) = delete;
      OptBase &operator =(const OptBase &) = delete;

    public:
      virtual ~OptBase(void) { }

      virtual void help(std::ostream &out) const = 0;
      virtual void validate(void) = 0;
    };


    class OptionBase : public OptBase {
    protected:
      const bool _has_default;
      bool _is_set;

      OptionBase(const std::string &name, const std::string &desc, const bool has_default) : OptBase(name, desc), _has_default(has_default) { }

      virtual void _set(const std::string &value) = 0;

    public:
      virtual ~OptionBase(void) { }

      void
      set(const std::string &value) {
        _set(value);
        _is_set = true;
      }

      virtual void set_default(void) = 0;
    };


    template <typename T>
    class Option;

    template <>
    class Option<std::string> : public OptionBase {
    protected:
      const std::string _default;

    public:
      Option(const std::string &name, const std::string &desc) : OptionBase(name, desc, false) { }
      Option(const std::string &name, const std::string &desc, const std::string &default_) : OptionBase(name, desc, true), _default(default_) { }
      virtual ~Option(void) { }
    };

    template <>
    class Option<> : public OptionBase {
    protected:
      const std::string _default;

    public:
      Option(const std::string &name, const std::string &desc) : OptionBase(name, desc, false) { }
      Option(const std::string &name, const std::string &desc, const std::string &default_) : OptionBase(name, desc, true), _default(default_) { }
      virtual ~Option(void) { }
    };


  }
}
