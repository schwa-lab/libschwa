/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_LOGGING_ENUMS_H_
#define SCHWA_IO_LOGGING_ENUMS_H_

#include <iosfwd>

namespace schwa {
  namespace io {

    /**
     * Log levels and values copied from the Python logging library.
     * @see http://docs.python.org/3/library/logging.html#levels
     */
    enum class LogLevel {
      CRITICAL = 50,
      ERROR = 40,
      WARNING = 30,
      INFO = 20,
      DEBUG = 10,
    };

    template <LogLevel L>
    struct LogLevelTraits { };

#define LOGLEVELTRAITS(level) \
      template <> \
      struct LogLevelTraits<::schwa::io::LogLevel::level> { \
        static constexpr const char *const name = #level; \
        static constexpr unsigned int value = static_cast<unsigned int>(::schwa::io::LogLevel::level); \
      }

    LOGLEVELTRAITS(CRITICAL);
    LOGLEVELTRAITS(ERROR);
    LOGLEVELTRAITS(WARNING);
    LOGLEVELTRAITS(INFO);
    LOGLEVELTRAITS(DEBUG);
#undef LOGLEVELTRAITS

    const char *loglevel_name(LogLevel level);
    unsigned int loglevel_value(LogLevel level);

    std::ostream &operator <<(std::ostream &out, LogLevel level);

  }
}

#endif  // SCHWA_IO_LOGGING_ENUMS_H_
