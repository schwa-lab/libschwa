/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_TYPE_INFO_H_
#define SCHWA_DR_TYPE_INFO_H_

#include <string>
#include <typeinfo>

#include <schwa/_base.h>

namespace schwa {
  namespace dr {

    // Runtime type information for a docrep type.
    class TypeInfo {
    public:
      const std::string name;

    protected:
      TypeInfo(const std::string &typeid_name);

    public:
      TypeInfo(const TypeInfo &other);
      ~TypeInfo(void) { }

      inline bool operator ==(const TypeInfo &o) const { return name == o.name; }
      inline bool operator !=(const TypeInfo &o) const { return name != o.name; }
      inline bool operator <(const TypeInfo &o) const { return name < o.name; }

      template <typename T>
      static TypeInfo create(void) {
        return TypeInfo(typeid(T).name());
      }
    };

    std::ostream &operator <<(std::ostream &out, const TypeInfo &type);

  }
}

#endif  // SCHWA_DR_TYPE_INFO_H_
