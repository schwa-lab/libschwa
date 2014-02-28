/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_EXCEPTION_H_
#define SCHWA_CONFIG_EXCEPTION_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/exception.h>

namespace schwa {
  namespace config {

    /**
     * Exception subclass to indicate an error in the configuration framework. This includes both
     * the invalid setting up of the configuration framework itself and the invalid parsing of
     * configuration values.
     **/
    class ConfigException : public Exception {
    public:
      explicit ConfigException(const std::string &msg) : Exception(msg) { }
      ConfigException(const ConfigException &other) : Exception(other) { }
      virtual ~ConfigException(void) throw() { }
    };

  }
}

#endif  // SCHWA_CONFIG_EXCEPTION_H_
