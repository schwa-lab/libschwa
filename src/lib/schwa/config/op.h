/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_OP_H_
#define SCHWA_CONFIG_OP_H_

#include <initializer_list>
#include <iosfwd>
#include <set>
#include <string>

#include <schwa/_base.h>
#include <schwa/config/base.h>
#include <schwa/io/logging_enums.h>

namespace schwa {
  namespace config {

    class OpGroup;


    class Option : public ConfigNode {
    protected:
      const bool _has_default;  //!< Whether or not this option has a default value that can be used.
      bool _was_mentioned;  //!< Whether or not this option was mentioned by name when parsing config options.
      bool _was_assigned;  //!< Whether or not this option was assigned a value when parsing config options.

      Option(OpGroup &group, const std::string &name, const std::string &desc, bool has_default);

      virtual void _assign(const std::string &value) = 0;
      virtual bool _validate(const Main &main) = 0;

    public:
      virtual ~Option(void) { }

      virtual ConfigNode *find(const std::string &key) override;

      virtual bool accepts_assignment(void) const override;
      virtual bool accepts_mention(void) const override;

      void assign(const std::string &value) override;
      void mention(void) override;
      bool validate(const Main &main) override;

      virtual void set_default(void) = 0;

      inline bool was_assigned(void) const { return _was_assigned; }
      inline bool was_mentioned(void) const { return _was_mentioned; }

    private:
      DISALLOW_COPY_AND_ASSIGN(Option);
    };


    template <typename T>
    class Op : public Option {
    protected:
      const T _default;
      T _value;

      virtual void _assign(const std::string &value) override;
      virtual bool _validate(const Main &main) override;

    public:
      Op(OpGroup &group, const std::string &name, const std::string &desc) : Option(group, name, desc, false) { }
      Op(OpGroup &group, const std::string &name, const std::string &desc, const T &default_) :
          Option(group, name, desc, true),
          _default(default_)
        { }
      virtual ~Op(void) { }

      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const override;
      virtual void set_default(void) override;

      inline const T &operator ()(void) const { return _value; }

    private:
      DISALLOW_COPY_AND_ASSIGN(Op);
    };


    template <typename T>
    class ChoicesOp : public Op<T> {
    protected:
      std::set<T> _options;

      virtual bool _validate(const Main &main) override;

    public:
      ChoicesOp(OpGroup &group, const std::string &name, const std::string &desc, std::initializer_list<T> options) :
          Op<T>(group, name, desc),
          _options(options)
        { }
      ChoicesOp(OpGroup &group, const std::string &name, const std::string &desc, std::initializer_list<T> options, const T &default_) :
          Op<T>(group, name, desc, default_),
          _options(options)
        { }
      virtual ~ChoicesOp(void) { }

      virtual void help(std::ostream &out, const std::string &prefix, unsigned int) const override;

    private:
      DISALLOW_COPY_AND_ASSIGN(ChoicesOp);
    };


    class IStreamOp : public Op<std::string> {
    public:
      static constexpr const char *const STDIN_STRING = "<stdin>";

    protected:
      std::istream *_in;
      bool _is_stdin;

      virtual bool _validate(const Main &main) override;

    public:
      IStreamOp(OpGroup &group, const std::string &name, const std::string &desc) :
          Op<std::string>(group, name, desc, STDIN_STRING),
          _in(nullptr),
          _is_stdin(false)
        { }
      virtual ~IStreamOp(void);

      inline std::istream &file(void) const { return *_in; }

    private:
      DISALLOW_COPY_AND_ASSIGN(IStreamOp);
    };


    class OStreamOp : public Op<std::string> {
    public:
      static constexpr const char *const STDOUT_STRING = "<stdout>";
      static constexpr const char *const STDERR_STRING = "<stderr>";

    protected:
      std::ostream *_out;
      bool _is_std;

      virtual bool _validate(const Main &main) override;

    public:
      OStreamOp(OpGroup &group, const std::string &name, const std::string &desc) : OStreamOp(group, name, desc, STDOUT_STRING) { }
      OStreamOp(OpGroup &group, const std::string &name, const std::string &desc, const std::string &default_) :
          Op<std::string>(group, name, desc, default_),
          _out(nullptr),
          _is_std(false)
        { }
      virtual ~OStreamOp(void);

      inline std::ostream &file(void) const { return *_out; }

    private:
      DISALLOW_COPY_AND_ASSIGN(OStreamOp);
    };


    class LogLevelOp : public ChoicesOp<std::string> {
    protected:
      schwa::io::LogLevel _level;

      virtual bool _validate(const Main &main) override;

    public:
      LogLevelOp(OpGroup &group, const std::string &name, const std::string &desc, const std::string &default_);
      virtual ~LogLevelOp(void);

      inline schwa::io::LogLevel operator ()(void) const { return _level; }

    private:
      DISALLOW_COPY_AND_ASSIGN(LogLevelOp);
    };


    class CommandOption : public Option {
    protected:
      virtual void _assign(const std::string &value) override;

    public:
      CommandOption(OpGroup &group, const std::string &name, const std::string &desc) : Option(group, name, desc, false) { }
      virtual ~CommandOption(void) { }

      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const override;

      virtual bool accepts_assignment(void) const override;
      virtual void set_default(void) override;
    };


    class HelpOption : public CommandOption {
    protected:
      virtual bool _validate(const Main &main) override;

    public:
      HelpOption(OpGroup &group, const std::string &name="help", const std::string &desc="Displays the help text") : CommandOption(group, name, desc) { }
      virtual ~HelpOption(void) { }

    private:
      DISALLOW_COPY_AND_ASSIGN(HelpOption);
    };


    class VersionOption : public CommandOption {
    protected:
      virtual bool _validate(const Main &main) override;

    public:
      VersionOption(OpGroup &group, const std::string &name="version", const std::string &desc="Displays the version") : CommandOption(group, name, desc) { }
      virtual ~VersionOption(void) { }

    private:
      DISALLOW_COPY_AND_ASSIGN(VersionOption);
    };
  }
}

#include <schwa/config/op_impl.h>

#endif  // SCHWA_CONFIG_OP_H_
