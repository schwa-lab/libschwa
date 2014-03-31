/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_BASE_H_
#define SCHWA_CONFIG_BASE_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>
#include <schwa/utils/enums.h>

namespace schwa {
  namespace config {

    class Group;
    class Main;

    enum class Flags : uint8_t {
      NONE = 0,
      OPTIONAL = 1 << 0,
    };

    inline bool operator &(const Flags a, const Flags b) { return to_underlying(a) & to_underlying(b); }


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
      char _short_name;  //!< The short single-letter name for the option. '\0' if not defined.
      Flags _flags;  //!< The bitmask of the flags set for this config node.

      ConfigNode(const std::string &name, const std::string &desc, Flags flags=Flags::NONE);
      ConfigNode(const std::string &name, char short_name, const std::string &desc, Flags flags=Flags::NONE);

      virtual void _help(std::ostream &out, unsigned int depth) const = 0;
      virtual void _help_self(std::ostream &out, unsigned int depth) const = 0;

      friend class Group;  // So that Group can access _help.
      friend class Main;  // So that Group can access _help.

    public:
      virtual ~ConfigNode(void) { }

      virtual ConfigNode *find(char short_name);
      virtual ConfigNode *find(const std::string &key);

      virtual bool accepts_assignment(void) const = 0;
      virtual bool accepts_mention(void) const = 0;
      virtual bool requires_assignment(void) const = 0;

      virtual void assign(const std::string &value) = 0;
      virtual void mention(void) = 0;
      virtual void serialise(std::ostream &out) const = 0;
      virtual bool validate(const Main &main) = 0;

      inline const std::string &desc(void) const { return _desc; }
      inline const std::string &full_name(void) const { return _full_name; }
      inline const std::string &name(void) const { return _name; }

      inline bool optional(void) const { return _flags & Flags::OPTIONAL; }

      void set_prefix(const std::string &prefix);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(ConfigNode);
    };

  }
}

#endif  // SCHWA_CONFIG_BASE_H_
