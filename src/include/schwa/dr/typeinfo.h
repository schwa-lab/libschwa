/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    // ========================================================================
    // Runtime type information
    // ========================================================================
    class TypeInfo {
    public:
      const std::string name;

    protected:
      TypeInfo(const std::string &typeid_name) : name(port::demangle_typeid(typeid_name.c_str())) { }

    public:
      ~TypeInfo(void) { }

      inline bool operator ==(const TypeInfo &o) const { return name == o.name; }
      inline bool operator !=(const TypeInfo &o) const { return name != o.name; }
      inline bool operator <(const TypeInfo &o) const { return name < o.name; }

      template <typename T>
      static TypeInfo create(void) { return TypeInfo(typeid(T).name()); }
    };


    inline std::ostream &
    operator <<(std::ostream &out, const TypeInfo &type) {
      return out << type.name;
    }

  }
}
