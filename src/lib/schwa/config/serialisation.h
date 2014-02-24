/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_SERIALISATION_H_
#define SCHWA_CONFIG_SERIALISATION_H_

#include <string>

#include <schwa/config/op.h>

namespace schwa {
  namespace config {

    class Main;


    /**
     * Configuration option for loading serialised config options from a file. The value for this
     * option is the path to the configuration file. Configuration files can be generated using
     * \ref OpSaveConfig instances.
     **/
    class OpLoadConfig : public Op<std::string> {
    public:
      OpLoadConfig(Group &group, const std::string &name="load-config", const std::string &desc="The file to load config from");
      OpLoadConfig(Group &group, const std::string &name, char short_name, const std::string &desc);
      virtual ~OpLoadConfig(void) { }

      template <typename C>
      void load_config(C &container) const;
    };


    /**
     * Configuration option for saving the current set of configuration options to a file. The
     * value for this option is the path to save the configuration file out to. Saved configuration
     * files can be read by \ref OpLoadConfig instances.
     **/
    class OpSaveConfig : public Op<std::string> {
    public:
      OpSaveConfig(Group &group, const std::string &name="save-config", const std::string &desc="The file to save the config to");
      OpSaveConfig(Group &group, const std::string &name, char short_name, const std::string &desc);
      virtual ~OpSaveConfig(void) { }

      void save_config(const Main &main) const;
    };

  }
}

#include <schwa/config/serialisation_impl.h>

#endif  // SCHWA_CONFIG_SERIALISATION_H_
