/* -*- Mode: C++; indent-tabs-mode: nil -*- */
/** @file */
#ifndef SCHWA_CONFIG_SERIALISATION_IMPL_H_
#define SCHWA_CONFIG_SERIALISATION_IMPL_H_

#include <fstream>
#include <stack>

#include <schwa/exception.h>

namespace schwa {
  namespace config {

    template <typename C>
    inline void
    OpLoadConfig::load_config(C &container) const {
      // Open the file for reading.
      std::ifstream in(_value);
      if (!in)
        throw IOException("Could not open config file for reading", _value);

      // Keep a stack of the found values so that we can push_front onto the provided container.
      std::stack<typename C::value_type> stack;

      // For each line in the file...
      for (std::string line; std::getline(in, line); ) {
        // Skip comments and blank lines.
        if (line.empty() || line[0] == '#')
          continue;

        // Split the line into key and optional value.
        const size_t pos = line.find('=');
        if (pos == std::string::npos)
          stack.push("--" + line);
        else {
          const std::string key = line.substr(0, pos);
          const std::string value = line.substr(pos + 1);

          // Don't put ourself back in for processing.
          if (key != _full_name) {
            stack.push("--" + key);
            stack.push(value);
          }
        }
      }

      // Transfer the found args back to the container.
      for  ( ; !stack.empty(); stack.pop())
        container.push_front(stack.top());
    }

  }
}

#endif  // SCHWA_CONFIG_SERIALISATION_IMPL_H_
