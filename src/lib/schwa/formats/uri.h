/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_FORMATS_URI_H_
#define SCHWA_FORMATS_URI_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>


namespace schwa {
  namespace formats {

    class URI {
    private:
      std::string _scheme;
      std::string _userinfo;
      std::string _host;
      std::string _path;
      std::string _query;
      std::string _fragment;
      uint16_t _port;

    public:
      URI(void);
      URI(const URI &o);
      URI(const URI &&o);
      URI &operator =(const URI &o);

      std::string &fragment(void) { return _fragment; }
      std::string &host(void) { return _host; }
      std::string &path(void) { return _path; }
      uint16_t &port(void) { return _port; }
      std::string &query(void) { return _query; }
      std::string &scheme(void) { return _scheme; }
      std::string &userinfo(void) { return _userinfo; }

      const std::string &fragment(void) const { return _fragment; }
      const std::string &host(void) const { return _host; }
      const std::string &path(void) const { return _path; }
      const uint16_t &port(void) const { return _port; }
      const std::string &query(void) const { return _query; }
      const std::string &scheme(void) const { return _scheme; }
      const std::string &userinfo(void) const { return _userinfo; }

      void clear(void);
      bool empty(void) const;

      std::string str(void) const;
      std::ostream &str(std::ostream &out) const;

      bool operator ==(const URI &o) const;
      bool operator !=(const URI &o) const { return !(*this == o); }
    };


    class URILexer {
    public:
      bool matches(const uint8_t *input, size_t nbytes) const;
      bool matches(const uint8_t *input, size_t nbytes, URI &uri) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(URILexer);
    };

  }  // namespace formats
}  // namespace schwa

#endif  // SCHWA_FORMATS_URI_H_
