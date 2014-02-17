/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_EXCEPTION_H_
#define SCHWA_CONFIG_EXCEPTION_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>

namespace schwa {
  namespace config {

    class ConfigException : public Exception {
    public:
      const std::string msg;
      const std::string name;
      const std::string value;

    private:
      const std::string _what;

    public:
      explicit ConfigException(const std::string &msg);
      ConfigException(const std::string &msg, const std::string &name);
      ConfigException(const std::string &msg, const std::string &name, const std::string &value);
      ConfigException(const ConfigException &o);
      virtual ~ConfigException(void) throw() { }

      virtual const char *what(void) const throw() override;
    };

  }
}

#endif  // SCHWA_CONFIG_EXCEPTION_H_
