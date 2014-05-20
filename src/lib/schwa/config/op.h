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

    class Group;


    class Option : public ConfigNode {
    protected:
      const bool _has_default;  //!< Whether or not this option has a default value that can be used.

      Option(Group &group, const std::string &name, const std::string &desc, Flags flags, bool has_default);
      Option(Group &group, const std::string &name, char short_name, const std::string &desc, Flags flags, bool has_default);

      virtual void _assign(const std::string &value) = 0;
      virtual void _mention(void) { }
      virtual bool _validate(const Main &main) = 0;

    public:
      virtual ~Option(void) { }

      virtual bool accepts_assignment(void) const override;
      virtual bool accepts_mention(void) const override;
      virtual bool requires_assignment(void) const override;

      bool validate(const Main &main) override;

      virtual void set_default(void) = 0;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Option);
    };


    template <typename T>
    class Op : public Option {
    protected:
      const T _default;
      T _value;

      virtual void _assign(const std::string &value) override;
      virtual bool _validate(const Main &main) override;

      virtual void _help(std::ostream &out, unsigned int depth) const override;
      virtual void _help_self(std::ostream &out, unsigned int depth) const override;

    public:
      Op(Group &group, const std::string &name, const std::string &desc, Flags flags=Flags::NONE) : Option(group, name, desc, flags, false), _default(T()) { }
      Op(Group &group, const std::string &name, char short_name, const std::string &desc, Flags flags=Flags::NONE) : Option(group, name, short_name, desc, flags, false), _default(T()) { }
      Op(Group &group, const std::string &name, const std::string &desc, const T &default_, Flags flags=Flags::NONE) : Option(group, name, desc, flags, true), _default(default_) { }
      Op(Group &group, const std::string &name, char short_name, const std::string &desc, const T &default_, Flags flags=Flags::NONE) : Option(group, name, short_name, desc, flags, true), _default(default_) { }
      virtual ~Op(void) { }

      virtual bool requires_assignment(void) const override;

      virtual void serialise(std::ostream &out) const override;
      virtual void set_default(void) override;

      inline const T &value(void) const { return _value; }
      inline const T &operator ()(void) const { return _value; }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Op);
    };


    template <typename T>
    class OpChoices : public Op<T> {
    protected:
      std::set<T> _options;

      virtual void _help_self(std::ostream &out, unsigned int) const override;

      virtual bool _validate(const Main &main) override;

    public:
      OpChoices(Group &group, const std::string &name, const std::string &desc, std::initializer_list<T> options) : Op<T>(group, name, desc), _options(options) { }
      OpChoices(Group &group, const std::string &name, char short_name, const std::string &desc, std::initializer_list<T> options) : Op<T>(group, name, short_name, desc), _options(options) { }
      OpChoices(Group &group, const std::string &name, const std::string &desc, std::initializer_list<T> options, const T &default_) : Op<T>(group, name, desc, default_), _options(options) { }
      OpChoices(Group &group, const std::string &name, char short_name, const std::string &desc, std::initializer_list<T> options, const T &default_) : Op<T>(group, name, short_name, desc, default_), _options(options) { }
      virtual ~OpChoices(void) { }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(OpChoices);
    };


    class OpIStream : public Op<std::string> {
    public:
      static constexpr const char *const STDIN_STRING = "<stdin>";

    protected:
      std::istream *_in;
      bool _is_stdin;

      virtual bool _validate(const Main &main) override;

    public:
      OpIStream(Group &group, const std::string &name, const std::string &desc, Flags flags=Flags::NONE);
      OpIStream(Group &group, const std::string &name, char short_name, const std::string &desc, Flags flags=Flags::NONE);
      OpIStream(Group &group, const std::string &name, const std::string &desc, const std::string &default_, Flags flags=Flags::NONE);
      OpIStream(Group &group, const std::string &name, char short_name, const std::string &desc, const std::string &default_, Flags flags=Flags::NONE);
      virtual ~OpIStream(void);

      inline std::istream &file(void) const { return *_in; }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(OpIStream);
    };


    class OpOStream : public Op<std::string> {
    public:
      static constexpr const char *const STDOUT_STRING = "<stdout>";
      static constexpr const char *const STDERR_STRING = "<stderr>";

    protected:
      std::ostream *_out;
      bool _is_std;

      virtual bool _validate(const Main &main) override;

    public:
      OpOStream(Group &group, const std::string &name, const std::string &desc, Flags flags=Flags::NONE);
      OpOStream(Group &group, const std::string &name, char short_name, const std::string &desc, Flags flags=Flags::NONE);
      OpOStream(Group &group, const std::string &name, const std::string &desc, const std::string &default_, Flags flags=Flags::NONE);
      OpOStream(Group &group, const std::string &name, char short_name, const std::string &desc, const std::string &default_, Flags flags=Flags::NONE);
      virtual ~OpOStream(void);

      inline std::ostream &file(void) const { return *_out; }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(OpOStream);
    };


    class OpLogLevel : public OpChoices<std::string> {
    protected:
      schwa::io::LogLevel _level;

      virtual bool _validate(const Main &main) override;

    public:
      OpLogLevel(Group &group, const std::string &name, const std::string &desc, const std::string &default_);
      OpLogLevel(Group &group, const std::string &name, char short_name, const std::string &desc, const std::string &default_);
      virtual ~OpLogLevel(void);

      inline schwa::io::LogLevel level(void) const { return _level; }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(OpLogLevel);
    };


    class OpSequenceTagFormat : public OpChoices<std::string> {
    protected:
      SequenceTagFormat _format;

      virtual bool _validate(const Main &main) override;

    public:
      OpSequenceTagFormat(Group &group, const std::string &name, const std::string &desc, const std::string &default_);
      OpSequenceTagFormat(Group &group, const std::string &name, char short_name, const std::string &desc, const std::string &default_);
      virtual ~OpSequenceTagFormat(void);

      inline SequenceTagFormat level(void) const { return _format; }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(OpSequenceTagFormat);
    };


    class CommandOption : public Option {
    protected:
      virtual void _assign(const std::string &value) override;

      virtual void _help(std::ostream &out, unsigned int depth) const override;
      virtual void _help_self(std::ostream &out, unsigned int depth) const override;

    public:
      CommandOption(Group &group, const std::string &name, const std::string &desc, Flags flags=Flags::NONE) : Option(group, name, desc, flags, false) { }
      CommandOption(Group &group, const std::string &name, char short_name, const std::string &desc, Flags flags=Flags::NONE) : Option(group, name, short_name, desc, flags, false) { }
      virtual ~CommandOption(void) { }

      virtual bool accepts_assignment(void) const override;
      virtual void serialise(std::ostream &out) const override;
      virtual void set_default(void) override;
    };


    class OpHelp : public CommandOption {
    protected:
      virtual bool _validate(const Main &main) override;

    public:
      OpHelp(Group &group, const std::string &name="help", char short_name='h', const std::string &desc="Displays the help text") : CommandOption(group, name, short_name, desc) { }
      virtual ~OpHelp(void) { }

      virtual bool terminate_main(void) const override;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(OpHelp);
    };


    class OpVersion : public CommandOption {
    protected:
      virtual bool _validate(const Main &main) override;

    public:
      OpVersion(Group &group, const std::string &name="version", const std::string &desc="Displays the version") : CommandOption(group, name, desc) { }
      virtual ~OpVersion(void) { }

      virtual bool terminate_main(void) const override;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(OpVersion);
    };
  }
}

#include <schwa/config/op_impl.h>

#endif  // SCHWA_CONFIG_OP_H_
