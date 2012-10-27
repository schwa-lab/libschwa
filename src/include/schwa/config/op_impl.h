/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_OP_IMPL_H_
#define SCHWA_CONFIG_OP_IMPL_H_

#include <sstream>

#include <schwa/config/exception.h>
#include <schwa/config/op.h>
#include <schwa/port.h>

namespace schwa {
  namespace config {

    // ========================================================================
    // Op<T>
    // ========================================================================
    template <typename T>
    void
    Op<T>::_set(const std::string &value) {
      std::stringstream ss(value);
      if (!(ss >> _value))
        throw ConfigException("Error setting value", _name, value);
    }

    template <>
    inline void
    Op<std::string>::_set(const std::string &value) {
      _value = value;
    }

    template <typename T>
    void
    Op<T>::_validate(void) { }

    template <typename T>
    void
    Op<T>::help(std::ostream &out, const std::string &prefix, unsigned int) const {
      out << "  " << port::BOLD << prefix << _name << port::OFF << ": " << _desc;
      if (_has_default)
        out << " (default: " << _default << ")";
      out << std::endl;
    }

    template <typename T>
    void
    Op<T>::set_default(void) {
      _value = _default;
    }


    // ========================================================================
    // EnumOp<T>
    // ========================================================================
    template <typename T>
    void
    EnumOp<T>::_validate(void) {
      if (_options.find(Op<T>::_value) == _options.end())
        throw ConfigException("Invalid enum value", Op<T>::_name, Op<T>::_value);
    }

    template <typename T>
    void
    EnumOp<T>::help(std::ostream &out, const std::string &prefix, unsigned int) const {
      out << "  " << port::BOLD << prefix << Op<T>::_name << port::OFF << ": " << Op<T>::_desc;
      out << " {";
      bool first = true;
      for (const auto &it : _options) {
        if (!first)
          out << ",";
        out << it;
        first = false;
      }
      out << "}";
      if (Op<T>::_has_default)
        out << " (default: " << Op<T>::_default << ")";
      out << std::endl;
    }

  }
}

#endif
