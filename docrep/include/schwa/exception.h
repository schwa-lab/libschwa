namespace schwa {

  // general errors with an error message
  class Exception : public std::exception {
  public:
    const std::string msg;

    Exception(const std::string &msg) : msg(msg){ }
    Exception(const Exception &other) : std::exception(other), msg(other.msg) { }
    virtual ~Exception(void) throw() { }

    virtual const char* what(void) const throw() { return msg.c_str(); }
  };


  // I/O errors with an error message, filename, and line number
  // normally these occur when files are missing or file reading
  // when the text does not match the expected input format
  class IOException : public Exception {
  public:
    const std::string uri;
    const int line;

    IOException(const std::string &msg) : Exception(msg), uri(), line(0) { }
    IOException(const std::string &msg, const std::string &uri, int line=0) : Exception(msg), uri(uri), line(line) { }
    IOException(const IOException &other) : Exception(other), uri(other.uri), line(other.line) { }
    virtual ~IOException(void) throw() { }
  };


  // Configuration system errors (illegal values etc)
  class ConfigError : public IOException {
  public:
    const std::string option;

    ConfigError(const std::string &msg, const std::string &option) throw() : IOException(msg), option(option) { }
    ConfigError(const std::string &msg, const std::string &option, const std::string &uri, int line=0) : IOException(msg, uri, line), option(option) { }
    ConfigError(const std::string &msg, const std::string &uri, int line) : IOException(msg, uri, line), option() { }
    ConfigError(const ConfigError &other, const std::string &option) : IOException(other), option(option) { }

    virtual ~ConfigError(void) throw() { }
  };

}
