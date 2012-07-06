/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace config {

    class OpGroup : public OptionBase {
    protected:
      std::vector<OptionBase *> _children;

      OpGroup(const std::string &name, const std::string &desc) : OptionBase(name, desc) { }
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const;

    public:
      OpGroup(OpGroup &group, const std::string &name, const std::string &desc);
      virtual ~OpGroup(void) { }

      inline void add(OptionBase *const child) { _children.push_back(child); }
      inline void help(std::ostream &out) const { help(out, "--", 0); }

      virtual OptionBase *find(const std::string &orig_key, const std::string key);
      virtual void set(const std::string &value);
      virtual void validate(void);
    };


    class OpMain : public OpGroup {
    protected:
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const;

    public:
      OpMain(const std::string &name, const std::string &desc) : OpGroup(name, desc) { }
      virtual ~OpMain(void) { }

      virtual OptionBase *find(const std::string &orig_key, const std::string key);
      using OpGroup::help;

      bool process(const int argc, const char *const argv[], std::ostream &help_ostream=std::cerr);
    };

  }
}
