/* -*- Mode: C++; indent-tabs-mode: nil -*- */
/** @file */
#ifndef SCHWA_CONFIG_SERIALISATION_H_
#define SCHWA_CONFIG_SERIALISATION_H_

#include <schwa/config/op.h>

namespace schwa {
  namespace config {

    class Main;


    class OpSaveConfig : public Op<std::string> {
    public:
      OpSaveConfig(Group &group, const std::string &name, const std::string &desc);
      virtual ~OpSaveConfig(void) { }

      void serialise_config(const Main &main) const;
    };


  }
}

#endif  // SCHWA_CONFIG_SERIALISATION_H_
