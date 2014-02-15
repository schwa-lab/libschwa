/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_LOGGING_H_
#define SCHWA_IO_LOGGING_H_

#include <schwa/_base.h>

#include <iostream>
#include <sstream>

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
      };

    LOGLEVELTRAITS(CRITICAL);
    LOGLEVELTRAITS(ERROR);
    LOGLEVELTRAITS(WARNING);
    LOGLEVELTRAITS(INFO);
    LOGLEVELTRAITS(DEBUG);
#undef LOGLEVELTRAITS

    const char *loglevel_name(LogLevel level);
    unsigned int loglevel_value(LogLevel level);

    inline std::ostream &
    operator <<(std::ostream &out, const LogLevel level) {
      return out << loglevel_name(level);
    }


    /**
     * Pretty format the log information into a buffer provided by `buf`. `buf_len` provides the
     * size of the provided buffer, and should be at least 80 characters. The number of characters
     * written to `buf` is returned.
     **/
    size_t pretty_log_header(LogLevel level, const char *file, unsigned int linenum, char *buf, size_t buf_len);


    /**
     * Base logging class, defining the API used by the LOG and LOG2 macros. Logging should
     * normally be instantiated by those macros.
     **/
    class Logger : public std::ostream {
    public:
      explicit Logger(std::streambuf *buf) : std::ostream(buf) { }
      virtual ~Logger(void) { }

      virtual LogLevel threshold(void) const = 0;
      virtual void threshold(const LogLevel threshold)  = 0;

      virtual Logger &operator ()(LogLevel level, const char *file, unsigned int linenum) = 0;
    };


    /**
     * Basic logger implementation that prefixes the log messages with the log level.
     **/
    class BasicLogger : public Logger {
    public:
      class Streambuf : public std::stringbuf {
      private:
        std::ostream &_ostream;
        LogLevel _threshold;
        LogLevel _level;

      public:
        Streambuf(std::ostream &ostream, LogLevel threshold);
        virtual ~Streambuf(void);

        inline LogLevel level(void) const { return _level; }
        inline void level(const LogLevel level) { _level = level; }

        inline LogLevel threshold(void) const { return _threshold; }
        inline void threshold(const LogLevel threshold) { _threshold = threshold; }

      protected:
        virtual int sync(void) override;
      };

    protected:
      Streambuf _streambuf;

    public:
      explicit BasicLogger(std::ostream &out, LogLevel threshold=LogLevel::INFO);
      virtual ~BasicLogger(void);

      inline LogLevel threshold(void) const override { return _streambuf.threshold(); }
      inline void threshold(const LogLevel threshold) override { _streambuf.threshold(threshold); }

      virtual Logger &operator ()(LogLevel level, const char *file, unsigned int linenum) override;
    };


    /**
     * Pretty logger implementation which prefixes the log messages with a timestamp, log level,
     * file name and line number information. Uses `pretty_log_header` to generate this prefix.
     **/
    class PrettyLogger : public BasicLogger {
    protected:
      char _buf[80];

    public:
      explicit PrettyLogger(std::ostream &out, LogLevel threshold=LogLevel::INFO);
      virtual ~PrettyLogger(void);

      virtual Logger &operator ()(LogLevel level, const char *file, unsigned int linenum) override;
    };


    /**
     * Pointer to the default logger instance.
     **/
    extern Logger *default_logger;


    #define LOG(level) (*::schwa::io::default_logger)(::schwa::io::LogLevel::level, __FILE__, __LINE__)
    #define LOG2(level, log)                      log(::schwa::io::LogLevel::level, __FILE__, __LINE__)

  }
}

#endif  // SCHWA_IO_LOGGING_H_
