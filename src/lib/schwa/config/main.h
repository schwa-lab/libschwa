/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_MAIN_H_
#define SCHWA_CONFIG_MAIN_H_

#include <iostream>
#include <string>
#include <vector>

#include <schwa/_base.h>
#include <schwa/config/group.h>
#include <schwa/config/serialisation.h>


namespace schwa {
  namespace config {

    class Main : public Group {
    protected:
      std::vector<ConfigNode *> _owned;
      std::vector<ConfigNode *> _positional_arg_nodes;

      std::string _unclaimed_args_desc;
      std::vector<std::string> _cmdline_args;
      std::vector<std::string> _unclaimed_args;

      OpHelp *_op_help;
      OpShortHelp *_op_short_help;
      OpVersion *_op_version;
      Op<std::string> *_op_log;
      OpLogLevel *_op_log_level;
      OpLoadConfig *_op_load_config;
      OpSaveConfig *_op_save_config;

      ConfigNode *_find(const std::string &key);

      virtual void _help(std::ostream &out, unsigned int depth) const override;
      virtual void _help_self(std::ostream &out, unsigned int depth) const override;

      virtual void _post_add(ConfigNode &child) override;

      // Used to parse and accept argv. Returns whether or not to keep going.
      bool _main(void);

    public:
      Main(const std::string &name, const std::string &desc);
      virtual ~Main(void);

      using Group::find;
      virtual ConfigNode *find(const std::string &key) override;

      virtual void serialise(std::ostream &out) const override;
      void serialise_cmdline_args(std::ostream &out) const;

      void help(std::ostream &out) const;
      void help_short(std::ostream &out) const;

      inline void allow_unclaimed_args(const std::string &desc) { _unclaimed_args_desc = desc; }
      inline bool allow_unclaimed_args(void) const { return !_unclaimed_args_desc.empty(); }
      inline const std::vector<std::string> &unclaimed_args(void) const { return _unclaimed_args; }

      inline OpHelp *op_help(void) { return _op_help; }
      inline OpShortHelp *op_short_help(void) { return _op_short_help; }
      inline OpVersion *op_version(void) { return _op_version; }

      template <typename LOGGER>
      void main(int argc, char **argv);

      static bool is_short_option(const std::string &key);
      static bool is_long_option(const std::string &key);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Main);
    };

  }
}

#include <schwa/config/main_impl.h>

#endif  // SCHWA_CONFIG_MAIN_H_
