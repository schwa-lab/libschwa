/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/serialisation.h>

#include <fstream>
#include <iostream>

#include <schwa/config/main.h>
#include <schwa/io/streams.h>

namespace io = schwa::io;


namespace schwa {
namespace config {

// ============================================================================
// OpLoadConfig
// ============================================================================
OpLoadConfig::OpLoadConfig(Group &group, const std::string &name, const std::string &desc) : Op<std::string>(group, name, desc, Flags::OPTIONAL) { }

OpLoadConfig::OpLoadConfig(Group &group, const std::string &name, const char short_name, const std::string &desc) : Op<std::string>(group, name, short_name, desc, Flags::OPTIONAL) { }


// ============================================================================
// OpSaveConfig
// ============================================================================
OpSaveConfig::OpSaveConfig(Group &group, const std::string &name, const std::string &desc) : Op<std::string>(group, name, desc, Flags::OPTIONAL) { }

OpSaveConfig::OpSaveConfig(Group &group, const std::string &name, const char short_name, const std::string &desc) : Op<std::string>(group, name, short_name, desc, Flags::OPTIONAL) { }

void
OpSaveConfig::save_config(const Main &main) const {
  // Only save if the option was actually invoked.
  if (!was_mentioned())
    return;

  // Open the file for writing.
  io::OutputStream out(_value);
  main.serialise(*out);
}

}  // namespace config
}  // namespace schwa
