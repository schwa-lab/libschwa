/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace config {

    class ConfigException : public Exception {
    public:
      const std::string msg;
      const std::string name;
      const std::string value;

      explicit ConfigException(const std::string &msg) : Exception(msg), msg(msg) { }
      ConfigException(const std::string &msg, const std::string &name) : Exception(msg), msg(msg), name(name) { }
      ConfigException(const std::string &msg, const std::string &name, const std::string &value) : Exception(msg), msg(msg), name(name), value(value) { }
      ConfigException(const ConfigException &o) : Exception(o), msg(o.msg), name(o.name), value(o.value) { }
      virtual ~ConfigException(void) throw() { }

      virtual const char *what(void) const throw();
    };


    class OptionBase {
    protected:
      const std::string _name;
      const std::string _desc;

      OptionBase(const std::string &name, const std::string &desc);
      OptionBase(const OptionBase &) = delete;
      OptionBase &operator =(const OptionBase &) = delete;

    public:
      virtual ~OptionBase(void) { }

      virtual OptionBase *find(const std::string &orig_key, const std::string key) = 0;
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const = 0;
      virtual void set(const std::string &value) = 0;
      virtual void validate(void) = 0;
    };

  }
}
