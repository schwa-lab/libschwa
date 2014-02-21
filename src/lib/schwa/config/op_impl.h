/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_CONFIG_OP_IMPL_H_
#define SCHWA_CONFIG_OP_IMPL_H_

#include <sstream>
#include <string>
#include <vector>

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
    Op<T>::_assign(const std::string &value) {
      std::istringstream ss(value);
      if (!(ss >> _value)) {
        std::ostringstream ss;
        ss << "Error setting value for \"" << _name << "\": \"" << value << "\"";
        throw ConfigException(ss.str());
      }
    }


    template <typename T>
    inline void
    Op<T>::serialise(std::ostream &out) const {
      out << _value;
    }


    template <>
    inline void
    Op<bool>::_assign(const std::string &value) {
      if (value == "true" || value == "1")
        _value = true;
      else if (value == "false" || value == "0")
        _value = false;
      else {
        std::ostringstream ss;
        ss << "Error setting value for \"" << _name << "\": \"" << value << "\"";
        throw ConfigException(ss.str());
      }
    }


    template <>
    inline void
    Op<bool>::serialise(std::ostream &out) const {
      out << std::boolalpha << _value;
    }


    template <>
    inline void
    Op<std::string>::_assign(const std::string &value) {
      _value = value;
    }


    template <typename T>
    bool
    Op<T>::_validate(const Main &) {
      return true;
    }


    template <typename T>
    void
    Op<T>::_help_self(std::ostream &out, const unsigned int depth) const {
      for (unsigned int i = 0; i != depth; ++i)
        out << "  ";
      out << port::BOLD << "--" << _full_name << port::OFF << ": " << _desc;
    }


    template <typename T>
    void
    Op<T>::_help(std::ostream &out, const unsigned int depth) const {
      Op<T>::_help_self(out, depth);

      std::vector<std::string> extra;
      if (optional())
        extra.push_back("optional");
      if (_has_default) {
        std::ostringstream ss;
        ss << "default: " << std::boolalpha << _default;
        extra.push_back(ss.str());
      }
      if (!extra.empty()) {
        out << " (";
        for (decltype(extra)::size_type i = 0; i != extra.size(); ++i) {
          if (i != 0)
            out << ", ";
          out << extra[i];
        }
        out << ")";
      }
      out << std::endl;
    }


    template <typename T>
    void
    Op<T>::set_default(void) {
      _value = _default;
    }


    // ========================================================================
    // OpChoices<T>
    // ========================================================================
    template <typename T>
    bool
    OpChoices<T>::_validate(const Main &) {
      if (_options.find(Op<T>::_value) == _options.end()) {
        std::ostringstream ss;
        ss << "Invalid value for \"" << Op<T>::_name << "\": \"" << Op<T>::_value << "\" is not a valid choice.";
        throw ConfigException(ss.str());
      }
      return true;
    }


    template <typename T>
    void
    OpChoices<T>::_help_self(std::ostream &out, const unsigned int depth) const {
      for (unsigned int i = 0; i != depth; ++i)
        out << "  ";
      out << port::BOLD << "--" << Op<T>::_full_name << port::OFF << ": " << Op<T>::_desc;
      out << " {";
      bool first = true;
      for (const auto &it : _options) {
        if (!first)
          out << ",";
        out << it;
        first = false;
      }
      out << "}";
    }

  }
}

#endif
