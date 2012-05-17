/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace config {

    class Cfg;

    class Option : public Node {
    public:
      static const std::string REQUIRED;

    protected:
      void die_invalid(const std::string &val) const;

      virtual bool is_valid(void) const = 0;

    public:
      Option(const std::string &name, const std::string &desc, Flags flags);
      Option(Cfg &cfg, const std::string &name, const std::string &desc, Flags flags);
      virtual ~Option(void) { }

      virtual bool has(const std::string &) const { return false; }
      virtual void check(void);

      virtual void reg(Node &, Flags=0) { die("cannot register a child Node of an Option"); }
      virtual Node &get(const std::string &) { die("an Option cannot have a child Node"); return *this; }

      virtual bool has_children(void) const { return false; }

      virtual void write_config(std::ostream &out, std::string prefix = "", bool root = true) const;
      virtual void write_preface(std::ostream &out, std::string prefix = "", bool root = true) const;
      virtual void write_help(std::ostream &out, std::string prefix = "", bool full = false) const;

      virtual void writeln_preface(std::ostream &out, std::string prefix = "", bool root = true) const;
    };


    template <typename T>
    class Op : public Option {
    public:
      T value;
      T DEFAULT_VALUE;

      Op(const std::string &name, const std::string &desc) : Option(name, desc, SHOW_ALL) { }
      Op(const std::string &name, const std::string &desc, T default_value) : Option(name, desc, SHOW_ALL | HAS_DEFAULT | IS_DEFINED), value(default_value), DEFAULT_VALUE(default_value) { }
      Op(Cfg &cfg, const std::string &name, const std::string &desc) : Option(cfg, name, desc, SHOW_ALL) { }
      Op(Cfg &cfg, const std::string &name, const std::string &desc, T default_value) : Option(cfg, name, desc, SHOW_ALL | HAS_DEFAULT | IS_DEFINED), value(default_value), DEFAULT_VALUE(default_value) { }
      Op(Cfg &cfg, Flags flags, const std::string &name, const std::string &desc) : Option(cfg, name, desc, flags) { }
      Op(Cfg &cfg, Flags flags, const std::string &name, const std::string &desc, T default_value) : Option(cfg, name, desc, flags | HAS_DEFAULT | IS_DEFINED), value(default_value), DEFAULT_VALUE(default_value) { }
      virtual ~Op(void) { }

      bool is_default(void) const { return DEFAULT_VALUE == value; }
      T get_default(void) const { return DEFAULT_VALUE; }

      virtual void
      set_default(T val) {
        flags |= HAS_DEFAULT | IS_DEFINED;
        DEFAULT_VALUE = value = val;
      }

      virtual T get_value(void) const { return value; }
      T operator ()(void) const { return get_value(); }

      virtual void
      set_value(T val) {
        flags |= IS_DEFINED | HAS_CHANGED;
        value = val;
      }

      virtual void
      set(const std::string &value_str) {
        T temp;

        std::istringstream is(value_str);
        is >> temp;
        if (is.fail() || !is.eof())
          die_invalid(value_str);
        set_value(temp);
      }

      virtual bool needs_arg(void) const { return true; }
      virtual bool is_valid(void) const { return is_optional() || is_defined(); }

      virtual void
      write_config(std::ostream &out, std::string prefix="", bool root=true) const {
        Option::write_config(out, prefix, root);
        out << " = " << std::boolalpha << value << std::noboolalpha;
      }

      virtual void
      write_desc(std::ostream &out, bool full) const {
        Option::write_desc(out, full);
        if (has_default())
          out << " (def = " << std::boolalpha << DEFAULT_VALUE << std::noboolalpha << ")";
        else if (!is_optional())
          out << REQUIRED;
      }
    };


    template <>
    inline bool
    Op<bool>::needs_arg(void) const {
      return false;
    }

    template <>
    inline void
    Op<bool>::set(const std::string &value_str) {
      std::string s = value_str;
      lower(s);
      if (s == "true" || s == "yes" || s == "1")
        set_value(true);
      else if (s == "false" || s == "no" || s == "0")
        set_value(false);
      else
        die_invalid(value_str);
    }

    // to convert 0x?? into text: \n,\t,...
    template <>
    inline void
    Op<std::string>::write_config(std::ostream &out, std::string prefix, bool root) const {
      Option::write_config(out, prefix, root);
      out << " = \"" << bin2asc(value) << "\"";
    }

    // to convert 0x?? into text: \n,\t,...
    template<>
    inline void
    Op<std::string>::write_desc(std::ostream &out, bool full) const {
      Option::write_desc(out, full);
      if (has_default())
        out << " (def = \"" << bin2asc(DEFAULT_VALUE) << "\")";
      else if (!is_optional())
        out << REQUIRED;
    }

    // to convert escape sequences into 0x??
    template<>
    inline void
    Op<std::string>::set(const std::string &value_str) {
      try {
        set_value(asc2bin(value_str));
      }
      catch(schwa::Exception e) {
        die(e.what());
      }
    }

    template <>
    inline void
    Op<char>::write_config(std::ostream &out, std::string prefix, bool root) const {
      Option::write_config(out, prefix, root);
      out << " = '" << char2esc_str(value) << '\'';
    }

    template <>
    inline void
    Op<char>::write_desc(std::ostream &out, bool full) const {
      Option::write_desc(out, full);
      if (has_default())
        out << " (def = '" << char2esc_str(DEFAULT_VALUE) << "')";
      else if (!is_optional())
        out << REQUIRED;
    }

    template <>
    inline void
    Op<char>::set(const std::string &value_str) {
      size_t len = value_str.size();
      if (len == 1)
        return set_value(value_str[0]);
      if (len == 2 && value_str[0] == '\\') {
        if (value_str[1] == 's')
          return set_value(' ');
        else
          return set_value(esc2char(value_str[1]));
      }
      die("option argument must be a single character or escape sequence (argument '" + value_str + "')");
    }


    template <typename T>
    class Restricted : public Op<T> {
    public:
      typedef void (* Validator)(const T &);

    protected:
      const Validator _validator;

      void
      precheck(const T &val) {
        try {
          (*_validator)(val);
        }
        catch(schwa::Exception &e) {
          throw schwa::ConfigError(e.msg, this->NAME);
        }
      }

    public:
      Restricted(const std::string &name, const std::string &desc, const Validator v) : Op<T>(name, desc), _validator(v) { }
      Restricted(const std::string &name, const std::string &desc, const Validator v, T default_value) : Op<T>(name, desc, default_value), _validator(v) { }
      Restricted(Cfg &cfg, const std::string &name, const std::string &desc, const Validator v) : Op<T>(cfg, name, desc), _validator(v) { }
      Restricted(Cfg &cfg, const std::string &name, const std::string &desc, const Validator v, T default_value) : Op<T>(cfg, name, desc, default_value), _validator(v) { }
      Restricted(Cfg &cfg, Flags flags, const std::string &name, const std::string &desc, const Validator v) : Op<T>(cfg, flags, name, desc), _validator(v) { }
      Restricted(Cfg &cfg, Flags flags, const std::string &name, const std::string &desc, const Validator v, T default_value) : Op<T>(cfg, flags, name, desc, default_value), _validator(v) { }

      virtual void
      set_default(T val) {
        precheck(val);
        Op<T>::set_default(val);
      }

      virtual void
      set_value(T val) {
        precheck(val);
        Op<T>::set_value(val);
      }
    };


    class Alias : public Node {
    protected:
      Node &_node;
      const std::string _other;

    public:
      Alias(Node &node, const std::string &name, const std::string &other);
      Alias(Cfg &cfg, Node &node, const std::string &name, const std::string &other);
      Alias(Cfg &cfg, Flags flags, Node &node, const std::string &name, const std::string &other);
      virtual ~Alias(void) { }

      virtual void reg(Node &node, Flags flags = 0) { _node.reg(node, flags); }
      virtual bool has(const std::string &name) const { return _node.has(name); }
      virtual Node &get(const std::string &name) { return _node.get(name); }
      virtual void set(const std::string &value_str) { _node.set(value_str); }
      virtual void check(void);

      virtual bool needs_arg(void) const { return _node.needs_arg(); }
      virtual bool has_children(void) const { return false; }

      virtual void write_config(std::ostream &, std::string="", bool=true) const { }
      virtual void write_preface(std::ostream &, std::string="", bool=true) const { }
      virtual void write_help(std::ostream &out, std::string prefix="", bool full=false) const;
      virtual void write_desc(std::ostream &out, bool full) const;
    };


    class OpVersion : public Option {
    public:
      OpVersion(void) : Option("version", "the version number", HIDE_CONFIG | HAS_CHANGED) { }
      OpVersion(Cfg &cfg) : Option(cfg, "version", "the version number", HIDE_CONFIG | HAS_CHANGED) { }

      void set(const std::string &val);

      bool is_valid(void) const { return true; }
      bool needs_arg(void) const { return false; }

      void write_config(std::ostream &out, std::string prefix="", bool root=true) const;
    };


    class OpLicence : public Option {
    public:
      OpLicence(void) : Option("licence", "the C&C tools licence", HIDE_CONFIG | HIDE_PREFACE) { }
      OpLicence(Cfg &cfg) : Option(cfg, "licence", "the C&C tools licence", HIDE_CONFIG | HIDE_PREFACE) { }

      void set(const std::string &val);

      bool is_valid(void) const { return true; }
      bool needs_arg(void) const { return false; }
    };


    class OpHelp : public Option {
    protected:
      Cfg *_cfg;

    public:
      OpHelp(const std::string &name = "help", const std::string &desc = "show the short help message") : Option(name, desc, HIDE_CONFIG | HIDE_PREFACE), _cfg(0) { }
      OpHelp(Cfg &cfg, const std::string &name = "help", const std::string &desc = "show the short help message") : Option(cfg, name, desc, HIDE_CONFIG | HIDE_PREFACE), _cfg(&cfg) { }

      virtual void set(const std::string &val);
      void usage(int exitcode, bool full) const;

      bool is_valid(void) const { return true; }
      bool needs_arg(void) const { return false; }
    };


    class OpMoreHelp : public OpHelp {
    public:
      OpMoreHelp(const std::string &name = "more", const std::string &desc = "show the long help message") : OpHelp(name, desc) { }
      OpMoreHelp(Cfg &cfg, const std::string &name = "more", const std::string &desc = "show the long help message") : OpHelp(cfg, name, desc) { }

      virtual void set(const std::string &val);
    };


    class OpCfg : public Option {
    protected:
      Cfg *_cfg;

    public:
      OpCfg(void) : Option("config", "load a configuration file", HIDE_CONFIG | HIDE_PREFACE), _cfg(0) { }
      OpCfg(Cfg &cfg) : Option(cfg, "config", "load a configuration file", HIDE_CONFIG | HIDE_PREFACE), _cfg(&cfg) { }

      void set(const std::string &val);

      bool needs_arg(void) const { return true; }
      bool is_valid(void) const { return true; }
    };


    class OpPath : public Op<std::string> {
    protected:
      const OpPath *BASE;

    public:
      OpPath(const std::string &name, const std::string &desc, const OpPath *base=0) : Op<std::string>(name, desc), BASE(base) { }
      OpPath(const std::string &name, const std::string &desc, std::string default_value, const OpPath *base=0) : Op<std::string>(name, desc, default_value), BASE(base) {}
      OpPath(Cfg &cfg, const std::string &name, const std::string &desc, const OpPath *base=0) : Op<std::string>(cfg, name, desc), BASE(base) { }
      OpPath(Cfg &cfg, const std::string &name, const std::string &desc, std::string default_value, const OpPath *base=0) : Op<std::string>(cfg, name, desc, default_value), BASE(base) { }
      OpPath(Cfg &cfg, Flags flags, const std::string &name, const std::string &desc, const OpPath *base=0) : Op<std::string>(cfg, flags, name, desc), BASE(base) { }
      OpPath(Cfg &cfg, Flags flags, const std::string &name, const std::string &desc, std::string default_value, const OpPath *base=0) : Op<std::string>(cfg, flags, name, desc, default_value), BASE(base) { }

      std::string get_value(void) const;
    };


    class OpDummy : public Option {
    public:
      OpDummy(void) : Option("dummy", "dummy option", HIDE_ALL) { }

      bool is_valid(void) const { return true; }
      bool needs_arg(void) const { return false; }

      void reg(Node &, Flags=0) { /* do nothing */ }
      bool has(const std::string &) const { return true; }
      Node &get(const std::string &name) {
        std::cerr << "ignoring request for " << name << std::endl;
        return *this;
      }
      void set(const std::string &) { /* do nothing */ }
      void check(void) { /* do nothing */ }
    };

  }
}
