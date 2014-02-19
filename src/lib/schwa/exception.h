/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_EXCEPTION_H_
#define SCHWA_EXCEPTION_H_

#include <exception>
#include <iosfwd>
#include <string>

#include <schwa/_base.h>

namespace schwa {

  /**
   * Base class for libschwa exceptions. Useful when combined with \ref print_exception or for
   * catching all libschwa specific exceptions.
   **/
  class Exception : public std::exception {
  protected:
    std::string _msg;

  public:
    explicit Exception(const std::string &msg) : _msg(msg) { }
    Exception(const Exception &o) : std::exception(o), _msg(o._msg) { }
    virtual ~Exception(void) throw() { }

    virtual const char* what(void) const throw() override { return _msg.c_str(); }

    inline const std::string &msg(void) const { return _msg; }
  };


  /**
   * Specific exception for describing I/O errors. They are documented with with an error message,
   * optional URI (file path), and optional line number within the file in question that the error
   * occurred on.
   **/
  class IOException : public Exception {
  protected:
    const std::string _uri;
    const int _linenum;

  public:
    explicit IOException(const std::string &msg, int linenum=-1);
    IOException(int errno_, const std::string &uri, int linenum=-1);
    IOException(const std::string &msg, const std::string &uri, int linenum=-1);
    IOException(const IOException &other);
    virtual ~IOException(void) throw() { }

    inline const std::string &uri(void) const { return _uri; }
    inline int linenum(void) const { return _linenum; }
  };


  /**
   * Specific exception to indicate that the wrong kind of value was provided.
   **/
  class ValueException : public Exception {
  public:
    explicit ValueException(const std::string &msg) : Exception(msg) { }
    ValueException(const ValueException &o) : Exception(o) { }
    virtual ~ValueException(void) throw() { }
  };


  /**
   * \ref Exception pretty printing helper.
   **/
  class print_exception {
  public:
    const std::string name;
    const Exception &e;

    explicit print_exception(const Exception &e);
    print_exception(const std::string &name, const Exception &e);

    std::ostream &dump(std::ostream &out) const;

  private:
    DISALLOW_COPY_AND_ASSIGN(print_exception);
  };

  std::ostream &operator <<(std::ostream &out, const print_exception &obj);

}

#endif  // SCHWA_EXCEPTION_H_
