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


    class OpBase : public OptionBase {
    protected:
      const bool _has_default;
      bool _is_set;

      OpBase(OpGroup &group, const std::string &name, const std::string &desc, bool has_default);

      virtual void _set(const std::string &value) = 0;
      virtual void _validate(void) = 0;

    public:
      virtual ~OpBase(void) { }

      virtual OptionBase *find(const std::string &orig_key, const std::string &key) override;
      virtual void set(const std::string &value) override;
      virtual void set_default(void) = 0;
      virtual void validate(void) override;

    private:
      DISALLOW_COPY_AND_ASSIGN(OpBase);
    };


    template <typename T>
    class Op : public OpBase {
    protected:
      const T _default;
      T _value;

      virtual void _set(const std::string &value) override;
      virtual void _validate(void) override;

    public:
      Op(OpGroup &group, const std::string &name, const std::string &desc) : OpBase(group, name, desc, false) { }
      Op(OpGroup &group, const std::string &name, const std::string &desc, const T &default_) :
          OpBase(group, name, desc, true),
          _default(default_)
        { }
      virtual ~Op(void) { }

      virtual void help(std::ostream &out, const std::string &prefix, unsigned int) const override;
      virtual void set_default(void) override;

      inline const T &operator ()(void) const { return _value; }

    private:
      DISALLOW_COPY_AND_ASSIGN(Op);
    };


    template <typename T>
    class EnumOp : public Op<T> {
    protected:
      std::set<T> _options;

      virtual void _validate(void) override;

    public:
      EnumOp(OpGroup &group, const std::string &name, const std::string &desc, std::initializer_list<T> options) :
          Op<T>(group, name, desc),
          _options(options)
        { }
      EnumOp(OpGroup &group, const std::string &name, const std::string &desc, std::initializer_list<T> options, const T &default_) :
          Op<T>(group, name, desc, default_),
          _options(options)
        { }
      virtual ~EnumOp(void) { }

      virtual void help(std::ostream &out, const std::string &prefix, unsigned int) const override;

    private:
      DISALLOW_COPY_AND_ASSIGN(EnumOp);
    };


    class IStreamOp : public Op<std::string> {
    public:
      static constexpr const char *const STDIN_STRING = "<stdin>";

    protected:
      std::istream *_in;
      bool _is_stdin;

      virtual void _validate(void) override;

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

      virtual void _validate(void) override;

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


    class LogLevelOp : public EnumOp<std::string> {
    protected:
      schwa::io::LogLevel _level;

      virtual void _validate(void);

    public:
      LogLevelOp(OpGroup &group, const std::string &name, const std::string &desc, const std::string &default_);
      virtual ~LogLevelOp(void);

      inline schwa::io::LogLevel operator ()(void) const { return _level; }

    private:
      DISALLOW_COPY_AND_ASSIGN(LogLevelOp);
    };

  }
}

#include <schwa/config/op_impl.h>

#endif  // SCHWA_CONFIG_OP_H_
