/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/formats/uri.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>


namespace schwa {
namespace formats {

// ================================================================================================
// URI
// ================================================================================================
URI::URI(void) : _port(0) { }

URI::URI(const URI &o) :
    _scheme(o._scheme),
    _userinfo(o._userinfo),
    _host(o._host),
    _path(o._path),
    _query(o._query),
    _fragment(o._fragment),
    _port(o._port)
  { }

URI::URI(const URI &&o) :
    _scheme(o._scheme),
    _userinfo(o._userinfo),
    _host(o._host),
    _path(o._path),
    _query(o._query),
    _fragment(o._fragment),
    _port(o._port)
  { }

URI &
URI::operator =(const URI &o) {
  _scheme = o._scheme;
  _userinfo = o._userinfo;
  _host = o._host;
  _path = o._path;
  _query = o._query;
  _fragment = o._fragment;
  _port = o._port;
  return *this;
}


void
URI::clear(void) {
  _scheme.clear();
  _userinfo.clear();
  _host.clear();
  _path.clear();
  _query.clear();
  _fragment.clear();
  _port = 0;
}

bool
URI::empty(void) const {
  return _scheme.empty() && _userinfo.empty() && _host.empty() && _path.empty() && _query.empty() && _fragment.empty() && _port == 0;
}

bool
URI::operator ==(const URI &o) const {
  return _scheme == o._scheme && _userinfo == o._userinfo && _host == o._host && _path == o._path && _query == o._query && _fragment == o._fragment && _port == o._port;
}


std::string
URI::str(void) const {
  std::ostringstream ss;
  str(ss);
  return ss.str();
}


std::ostream &
URI::str(std::ostream &out) const {
  if (!_scheme.empty())
    out << _scheme << ":";
  if (!(_userinfo.empty() && _host.empty()))
    out << "//";
  if (!_userinfo.empty())
    out << _userinfo << "@";
  if (!_host.empty())
    out << _host;
  if (_port != 0)
    out << ":" << std::dec << _port;
  if (!_path.empty())
    out << _path;
  if (!_query.empty())
    out << "?" << _query;
  if (!_fragment.empty())
    out << "#" << _fragment;
  return out;
}

}  // namesapce formats
}  // namespace schwa
