/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_IO_UTILS_H_
#define SCHWA_IO_UTILS_H_

#include <ios>
#include <iosfwd>
#include <string>

#include <schwa/_base.h>


namespace schwa {
  namespace io {

    /**
     * Opens a std::istream to the provided path. If the file fails to open for reading, am
     * IO::Exception is thrown. Provides access to the human-readable filename for the istream as
     * well as whether or not it's a standard input stream and a tty.
     **/
    class InputStream {
    protected:
      std::string _path;
      std::istream *_stream;
      ssize_t _nbytes;  //!< The number of bytes in the file, or -1 if stdin.

    public:
      explicit InputStream(const char *path, const std::ios_base::openmode mode=std::ios_base::in);
      explicit InputStream(const std::string &path, const std::ios_base::openmode mode=std::ios_base::in) : InputStream(path.c_str(), mode) { }
      ~InputStream(void);

      inline ssize_t nbytes(void) const { return _nbytes; }
      inline const std::string &path(void) const { return _path; }

      inline operator std::istream &() { return *_stream; }
      inline std::istream &operator *(void) { return *_stream; }

      bool is_stdin(void) const;
      bool is_tty(void) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(InputStream);
    };


    /**
     * Opens a std::ostream to the provided path. If the file fails to open for writing, am
     * IO::Exception is thrown. Provides access to the human-readable filename for the ostream as
     * well as whether or not it's a standard output stream and a tty.
     **/
    class OutputStream {
    protected:
      std::string _path;
      std::ostream *_stream;

    public:
      explicit OutputStream(const char *path, const std::ios_base::openmode mode=std::ios_base::out);
      explicit OutputStream(const std::string &path, const std::ios_base::openmode mode=std::ios_base::out) : OutputStream(path.c_str(), mode) { }
      ~OutputStream(void);

      inline const std::string &path(void) const { return _path; }
      inline operator std::ostream &() { return *_stream; }
      inline std::ostream &operator *(void) { return *_stream; }

      bool is_stdout(void) const;
      bool is_stderr(void) const;
      bool is_tty(void) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(OutputStream);
    };

  }
}

#endif  // SCHWA_IO_UTILS_H_
