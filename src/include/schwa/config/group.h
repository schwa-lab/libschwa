/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_GROUP_H_
#define SCHWA_CONFIG_GROUP_H_

#include <iosfwd>
#include <string>
#include <vector>

#include <schwa/_base.h>
#include <schwa/config/base.h>

namespace schwa {
  namespace config {

    class OpGroup : public OptionBase {
    protected:
      std::vector<OptionBase *> _children;

      OpGroup(const std::string &name, const std::string &desc) : OptionBase(name, desc) { }
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const override;

    public:
      OpGroup(OpGroup &group, const std::string &name, const std::string &desc);
      virtual ~OpGroup(void) { }

      inline void add(OptionBase *const child) { _children.push_back(child); }
      inline void help(std::ostream &out) const { help(out, "--", 0); }

      virtual OptionBase *find(const std::string &orig_key, const std::string &key) override;
      virtual void set(const std::string &value) override;
      virtual void validate(void) override;

    private:
      DISALLOW_COPY_AND_ASSIGN(OpGroup);
    };

  }
}

#endif  // SCHWA_CONFIG_GROUP_H_
