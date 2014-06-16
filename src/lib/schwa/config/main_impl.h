/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_MAIN_IMPL_H_
#define SCHWA_CONFIG_MAIN_IMPL_H_

#include <schwa/exception.h>
#include <schwa/io/logging.h>

namespace schwa {
  namespace config {

    template <typename LOGGER>
    void
    Main::main(int argc, char **argv) {
      // Store a copy of the full commandline arguments for debugging/reproducability purposes.
      _cmdline_args.clear();
      for (int i = 0; i != argc; ++i)
        _cmdline_args.push_back(argv[i]);

      // Process the command line arguments.
      if (!_main())
        throw SystemExit(1);

      // Configure logging.
      io::default_logger = new LOGGER(_op_log->value().c_str(), _op_log_level->level());
    }

  }
}

#endif  // SCHWA_CONFIG_MAIN_IMPL_H_
