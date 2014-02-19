/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_GROUP_H_
#define SCHWA_CONFIG_GROUP_H_

#include <iosfwd>
#include <string>
#include <vector>

#include <schwa/_base.h>
#include <schwa/config/op.h>

namespace schwa {
  namespace config {

    class Group : public ConfigNode {
    protected:
      std::vector<Option *> _options;
      std::vector<Group *> _groups;

      Group(const std::string &name, const std::string &desc) : ConfigNode(name, desc) { }

      void _add_check(ConfigNode &child);

    public:
      Group(Group &group, const std::string &name, const std::string &desc);
      virtual ~Group(void) { }

      void add(Option &child);
      void add(Group &child);

      void help(std::ostream &out) const;
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const override;

      virtual ConfigNode *find(const std::string &key) override;

      virtual bool accepts_assignment(void) const override;
      virtual bool accepts_mention(void) const override;

      virtual void assign(const std::string &value) override;
      virtual void mention(void) override;
      virtual bool validate(const Main &main) override;

    private:
      DISALLOW_COPY_AND_ASSIGN(Group);
    };

  }
}

#endif  // SCHWA_CONFIG_GROUP_H_
