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

      OpOStream *_log;
      OpLogLevel *_log_level;
      OpLoadConfig *_load_config;
      OpSaveConfig *_save_config;

      std::vector<std::string> _cmdline_args;

      virtual void _post_add(ConfigNode &child) override;

      // Used to parse and accept argv. Returns whether or not to keep going.
      bool _main(void);

    public:
      Main(const std::string &name, const std::string &desc);
      virtual ~Main(void);

      virtual ConfigNode *find(const std::string &key) override;

      virtual void serialise(std::ostream &out) const override;
      void serialise_cmdline_args(std::ostream &out) const;

      void help(std::ostream &out) const;

      template <typename LOGGER>
      void main(int argc, char **argv);

    private:
      DISALLOW_COPY_AND_ASSIGN(Main);
    };

  }
}

#include <schwa/config/main_impl.h>

#endif  // SCHWA_CONFIG_MAIN_H_
