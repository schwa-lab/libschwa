/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_MAIN_H_
#define SCHWA_CONFIG_MAIN_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>
#include <schwa/config/group.h>

namespace schwa {
  namespace config {

    class OpMain : public OpGroup {
    protected:
      virtual void help(std::ostream &out, const std::string &prefix, unsigned int depth) const override;

    public:
      OpMain(const std::string &name, const std::string &desc) : OpGroup(name, desc) { }
      virtual ~OpMain(void) { }

      virtual OptionBase *find(const std::string &orig_key, const std::string &key) override;
      using OpGroup::help;

      // Used to parse and accept argv. Calls ::exit(1) upon error. Logs to std::cerr.
      void main(int argc, char **argv);

      // Used to parse and accept argv. Return success. Logs to help_ostream.
      bool process(int argc, char **argv, std::ostream &help_ostream);

    private:
      DISALLOW_COPY_AND_ASSIGN(OpMain);
    };

  }
}

#endif  // SCHWA_CONFIG_MAIN_H_
