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

      Group(const std::string &name, const std::string &desc, Flags flags=Flags::NONE) : ConfigNode(name, desc, flags) { }

      virtual void _help(std::ostream &out, unsigned int depth) const override;
      virtual void _help_self(std::ostream &out, unsigned int depth) const override;

      virtual void _pre_add(ConfigNode &child);
      virtual void _post_add(ConfigNode &child);

      friend class Main;  // So that Group can access _help.

    public:
      Group(Group &group, const std::string &name, const std::string &desc, Flags flags=Flags::NONE);
      virtual ~Group(void) { }

      void add(Group &child);
      void add(Option &child);

      virtual ConfigNode *find(char short_name) override;
      virtual ConfigNode *find(const std::string &key) override;

      virtual bool accepts_assignment(void) const override;
      virtual bool accepts_mention(void) const override;
      virtual bool requires_assignment(void) const override;

      virtual void assign(const std::string &value) override;
      virtual void mention(void) override;
      virtual void serialise(std::ostream &out) const override;
      virtual bool terminate_main(void) const override;
      virtual bool validate(const Main &main) override;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Group);
    };

  }
}

#endif  // SCHWA_CONFIG_GROUP_H_
