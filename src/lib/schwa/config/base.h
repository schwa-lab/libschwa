/* -*- Mode: C++; indent-tabs-mode: nil -*- */
/** @file */
#ifndef SCHWA_CONFIG_BASE_H_
#define SCHWA_CONFIG_BASE_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>

namespace schwa {
  namespace config {

    class Group;
    class Main;

    /**
     * Base class for all config nodes. All nodes have a name and some associated help text which
     * is displayed upon a top-level call to --help.
     **/
    class ConfigNode {
    public:
      static constexpr const char *const SEPARATOR = "--";

    protected:
      std::string _name;  //!< Name of the config option.
      std::string _desc;  //!< Help text for the config option.
      std::string _full_name;  //!< The full name of the option, accounting for nesting.

      ConfigNode(const std::string &name, const std::string &desc);

      virtual void _help(std::ostream &out, unsigned int depth) const = 0;
      virtual void _help_self(std::ostream &out, unsigned int depth) const = 0;

      friend class Group;  // So that Group can access _help.

    public:
      virtual ~ConfigNode(void) { }

      virtual ConfigNode *find(const std::string &key) = 0;

      virtual bool accepts_assignment(void) const = 0;
      virtual bool accepts_mention(void) const = 0;

      virtual void assign(const std::string &value) = 0;
      virtual void mention(void) = 0;
      virtual bool validate(const Main &main) = 0;

      inline const std::string &desc(void) const { return _desc; }
      inline const std::string &full_name(void) const { return _full_name; }
      inline const std::string &name(void) const { return _name; }

      void set_prefix(const std::string &prefix);

    private:
      DISALLOW_COPY_AND_ASSIGN(ConfigNode);
    };

  }
}

#endif  // SCHWA_CONFIG_BASE_H_
