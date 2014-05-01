/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/io/logging.h>

#include <cassert>
#include <cstring>
#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <thread>

#include <sys/time.h>

#include <schwa/io/utils.h>


namespace schwa {
namespace io {

const char *
loglevel_name(const LogLevel level) {
  switch (level) {
  case LogLevel::CRITICAL: return LogLevelTraits<LogLevel::CRITICAL>::name; break;
  case LogLevel::ERROR: return LogLevelTraits<LogLevel::ERROR>::name; break;
  case LogLevel::WARNING: return LogLevelTraits<LogLevel::WARNING>::name; break;
  case LogLevel::INFO: return LogLevelTraits<LogLevel::INFO>::name; break;
  case LogLevel::DEBUG: return LogLevelTraits<LogLevel::DEBUG>::name; break;
  default: return nullptr; break;
  }
}


unsigned int
loglevel_value(const LogLevel level) {
  switch (level) {
  case LogLevel::CRITICAL: return LogLevelTraits<LogLevel::CRITICAL>::value; break;
  case LogLevel::ERROR: return LogLevelTraits<LogLevel::ERROR>::value; break;
  case LogLevel::WARNING: return LogLevelTraits<LogLevel::WARNING>::value; break;
  case LogLevel::INFO: return LogLevelTraits<LogLevel::INFO>::value; break;
  case LogLevel::DEBUG: return LogLevelTraits<LogLevel::DEBUG>::value; break;
  default: return 0; break;
  }
}


std::ostream &
operator <<(std::ostream &out, const LogLevel level) {
  return out << loglevel_name(level);
}


size_t
pretty_log_header(const LogLevel level, const char *file, const unsigned int linenum, char *const buf, const size_t buf_len) {
  char now_buf[20];
  time_t now;
  timeval now_tv;
  gettimeofday(&now_tv, nullptr);
  now = now_tv.tv_sec;

  std::strftime(now_buf, sizeof(now_buf), "%FT%T", std::localtime(&now));  // YYYY-MM-DDTHH:mm:ss (19)
  const size_t len = std::strlen(file);
  if (len > 35) {
    file += len - 35;
  }
  return std::snprintf(buf, buf_len, "[%19s.%06zd %-8s %35s:%-4u] ", now_buf, now_tv.tv_usec, loglevel_name(level), file, linenum);
}


size_t
pretty_log_header_threaded(const LogLevel level, const char *file, const unsigned int linenum, char *const buf, const size_t buf_len) {
  char now_buf[20];
  time_t now;
  timeval now_tv;
  gettimeofday(&now_tv, nullptr);
  now = now_tv.tv_sec;

  std::strftime(now_buf, sizeof(now_buf), "%FT%T", std::localtime(&now));  // YYYY-MM-DDTHH:mm:ss (19)

  std::ostringstream thread_id_ss;
  thread_id_ss << std::this_thread::get_id();
  std::string thread_id = thread_id_ss.str();
  if (thread_id.size() > 8) {
    thread_id.resize(8);
  }

  const size_t len = std::strlen(file);
  if (len > 25) {
    file += len - 25;
  }
  return std::snprintf(buf, buf_len, "[%19s.%06zd %-8s %25s:%-4u][%-8s] ", now_buf, now_tv.tv_usec, loglevel_name(level), file, linenum, thread_id.c_str());
}


BasicLogger::Streambuf::Streambuf(std::ostream &out, LogLevel threshold) :
    _out(&out),
    _opened_out(false),
    _threshold(threshold),
    _level(LogLevel::INFO)
  { }

BasicLogger::Streambuf::Streambuf(const char *const filename, LogLevel threshold) :
    _out(safe_open_ofstream(filename)),
    _opened_out(true),
    _threshold(threshold),
    _level(LogLevel::INFO)
  { }

BasicLogger::Streambuf::~Streambuf(void) {
  if (_opened_out)
    delete _out;
}

int
BasicLogger::Streambuf::sync(void) {
  if (_level >= _threshold)
    (*_out) << str() << std::flush;
  str("");
  return 0;
}


BasicLogger::BasicLogger(std::ostream &out, LogLevel threshold) : Logger(&_streambuf), _streambuf(out, threshold) { }

BasicLogger::BasicLogger(const char *filename, LogLevel threshold) : Logger(&_streambuf), _streambuf(filename, threshold) { }

BasicLogger::~BasicLogger(void) { }

Logger &
BasicLogger::operator ()(const LogLevel level, const char *, unsigned int) {
  _streambuf.level(level);
  *this << "[" << level << "]";
  return *this;
}


PrettyLogger::PrettyLogger(std::ostream &out, LogLevel threshold) : BasicLogger(out, threshold) { }

PrettyLogger::PrettyLogger(const char *filename, LogLevel threshold) : BasicLogger(filename, threshold) { }

PrettyLogger::~PrettyLogger(void) { }

Logger &
PrettyLogger::operator ()(const LogLevel level, const char *const file, const unsigned int linenum) {
  _streambuf.level(level);
  const size_t used = pretty_log_header(level, file, linenum, _buf, sizeof(_buf));
  assert(used == sizeof(_buf) - 1);
  *this << _buf;
  return *this;
}


ThreadsafeBasicLogger::Streambuf::Streambuf(std::ostream &out, LogLevel threshold) : BasicLogger::Streambuf(out, threshold) { }

ThreadsafeBasicLogger::Streambuf::Streambuf(const char *filename, LogLevel threshold) : BasicLogger::Streambuf(filename, threshold) { }

ThreadsafeBasicLogger::Streambuf::~Streambuf(void) { }

int
ThreadsafeBasicLogger::Streambuf::sync(void) {
  const int ret = BasicLogger::Streambuf::sync();
  _lock.unlock();
  return ret;
}


ThreadsafeBasicLogger::ThreadsafeBasicLogger(std::ostream &out, LogLevel threshold) : Logger(&_streambuf), _streambuf(out, threshold) { }

ThreadsafeBasicLogger::ThreadsafeBasicLogger(const char *filename, LogLevel threshold) : Logger(&_streambuf), _streambuf(filename, threshold) { }

ThreadsafeBasicLogger::~ThreadsafeBasicLogger(void) { }

Logger &
ThreadsafeBasicLogger::operator ()(const LogLevel level, const char *, unsigned int) {
  _streambuf.lock();
  _streambuf.level(level);
  *this << "[" << level << "]";
  return *this;
}


ThreadsafePrettyLogger::ThreadsafePrettyLogger(std::ostream &out, LogLevel threshold) : ThreadsafeBasicLogger(out, threshold) { }

ThreadsafePrettyLogger::ThreadsafePrettyLogger(const char *filename, LogLevel threshold) : ThreadsafeBasicLogger(filename, threshold) { }

ThreadsafePrettyLogger::~ThreadsafePrettyLogger(void) { }

Logger &
ThreadsafePrettyLogger::operator ()(const LogLevel level, const char *const file, const unsigned int linenum) {
  _streambuf.lock();
  _streambuf.level(level);
  const size_t used = pretty_log_header_threaded(level, file, linenum, _buf, sizeof(_buf));
  assert(used == sizeof(_buf) - 1);
  *this << _buf;
  return *this;
}


static BasicLogger _default_default_logger(std::clog);
Logger *default_logger = &_default_default_logger;


}  // namespace io
}  // namespace schwa
