/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config/serialisation.h>

#include <fstream>
#include <iostream>

#include <schwa/config/main.h>


namespace schwa {
namespace config {

OpSaveConfig::OpSaveConfig(Group &group, const std::string &name, const std::string &desc) : Op<std::string>(group, name, desc, Flags::OPTIONAL) { }

void
OpSaveConfig::serialise_config(const Main &main) const {
  // Only save if the option was actually invoked.
  if (!was_mentioned())
    return;

  // Open the file for writing.
  std::ofstream out(_value);
  if (!out)
    throw IOException("Could not open file for writing", _value);
  main.serialise(out);
}


}  // namespace config
}  // namespace schwa
