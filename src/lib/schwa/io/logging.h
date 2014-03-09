/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_LOGGING_H_
#define SCHWA_IO_LOGGING_H_

#include <mutex>
#include <ostream>
#include <sstream>

#include <schwa/_base.h>
#include <schwa/io/logging_enums.h>

namespace schwa {
  namespace io {

    /**
     * Pretty format the log information into a buffer provided by `buf`. `buf_len` provides the
     * size of the provided buffer, and should be at least 80 characters. The number of characters
     * written to `buf` is returned.
     **/
    size_t pretty_log_header(LogLevel level, const char *file, unsigned int linenum, char *buf, size_t buf_len);


    /**
     * Base logging class, defining the API used by the \ref LOG(level) and \ref LOG2 macros. Logging should
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
      protected:
        std::ostream &_out;
        LogLevel _threshold;
        LogLevel _level;

      public:
        Streambuf(std::ostream &out, LogLevel threshold);
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


    class ThreadsafeBasicLogger : public Logger {
    public:
      class Streambuf : public BasicLogger::Streambuf {
      protected:
        std::mutex _lock;

      public:
        Streambuf(std::ostream &ostream, LogLevel threshold);
        virtual ~Streambuf(void);

        inline void lock(void) { _lock.lock(); }

      protected:
        virtual int sync(void) override;
      };

    protected:
      Streambuf _streambuf;

    public:
      explicit ThreadsafeBasicLogger(std::ostream &out, LogLevel threshold=LogLevel::INFO);
      virtual ~ThreadsafeBasicLogger(void);

      inline LogLevel threshold(void) const override { return _streambuf.threshold(); }
      inline void threshold(const LogLevel threshold) override { _streambuf.threshold(threshold); }

      virtual Logger &operator ()(LogLevel level, const char *file, unsigned int linenum) override;
    };


    class ThreadsafePrettyLogger : public ThreadsafeBasicLogger {
    protected:
      char _buf[80];

    public:
      explicit ThreadsafePrettyLogger(std::ostream &out, LogLevel threshold=LogLevel::INFO);
      virtual ~ThreadsafePrettyLogger(void);

      virtual Logger &operator ()(LogLevel level, const char *file, unsigned int linenum) override;
    };


    /**
     * Pointer to the default logger instance.
     **/
    extern Logger *default_logger;


    /**
     * Main macro used for logging. The argument is one of the (unqualified) `LogLevel` enum values.
     * This then returns the default logger instance configured to log at the appropriate level.
     * E.g. LOG(INFO) << "hello, world!" << std::endl;
     * @see LogLevel
     * @see default_logger
     */
    #define LOG(level) (*::schwa::io::default_logger)(::schwa::io::LogLevel::level, __FILE__, __LINE__)
    #define LOG2(level, logger)                logger(::schwa::io::LogLevel::level, __FILE__, __LINE__)

  }
}

#endif  // SCHWA_IO_LOGGING_H_
