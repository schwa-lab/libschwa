/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_BASE_H_
#define SCHWA_CONFIG_BASE_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>

namespace schwa {
  namespace config {

    class OptionBase {
    protected:
      const std::string _name;
      const std::string _desc;

      OptionBase(const std::string &name, const std::string &desc);

    public:
      virtual ~OptionBase(void) { }

      virtual OptionBase *find(const std::string &orig_key, const std::string &key) = 0;
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const = 0;
      virtual void set(const std::string &value) = 0;
      virtual void validate(void) = 0;

    private:
      DISALLOW_COPY_AND_ASSIGN(OptionBase);
    };

  }
}

#endif  // SCHWA_CONFIG_BASE_H_
