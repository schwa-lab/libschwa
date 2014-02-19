/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_MAIN_H_
#define SCHWA_CONFIG_MAIN_H_

#include <iostream>
#include <string>
#include <vector>

#include <schwa/_base.h>
#include <schwa/config/group.h>

namespace schwa {
  namespace config {

    class Main : public Group {
    protected:
      std::vector<ConfigNode *> _owned;
      OpOStream *_log;
      OpLogLevel *_log_level;

    public:
      Main(const std::string &name, const std::string &desc);
      virtual ~Main(void);

      virtual ConfigNode *find(const std::string &key) override;

      // Used to parse and accept argv. Returns whether or not to keep going.
      bool process(int argc, char **argv);

      template <typename LOGGER> void main(int argc, char **argv);

    private:
      DISALLOW_COPY_AND_ASSIGN(Main);
    };

  }
}

#include <schwa/config/main_impl.h>

#endif  // SCHWA_CONFIG_MAIN_H_
