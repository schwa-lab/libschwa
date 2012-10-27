/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_EXCEPTION_H_
#define SCHWA_EXCEPTION_H_

#include <exception>
#include <iosfwd>
#include <string>

#include <schwa/_base.h>

namespace schwa {

  // general errors with an error message
  class Exception : public std::exception {
  public:
    const std::string msg;

    Exception(const std::string &msg) : msg(msg) { }
    Exception(const Exception &other) : std::exception(other), msg(other.msg) { }
    virtual ~Exception(void) throw() { }

    virtual const char* what(void) const throw() override { return msg.c_str(); }
  };


  // I/O errors with an error message, filename, and line number
  // normally these occur when files are missing or file reading
  // when the text does not match the expected input format
  class IOException : public Exception {
  public:
    const std::string uri;
    const int line;

    IOException(const std::string &msg, int line=0);
    IOException(const std::string &msg, const std::string &uri, int line=0);
    IOException(const IOException &other);
    virtual ~IOException(void) throw() { }
  };


  // general errors with an error message
  class ValueException : public Exception {
  public:
    ValueException(const std::string &msg) : Exception(msg) { }
    ValueException(const ValueException &other) : Exception(other) { }
    virtual ~ValueException(void) throw() { }
  };


  // exception pretty printing functor
  class print_exception {
  public:
    const std::string name;
    const Exception &e;

    print_exception(const Exception &e);
    print_exception(const std::string &name, const Exception &e);

    std::ostream &dump(std::ostream &out) const;

  private:
    DISALLOW_COPY_AND_ASSIGN(print_exception);
  };

  std::ostream &operator <<(std::ostream &out, const print_exception &obj);

}

#endif  // SCHWA_EXCEPTION_H_
