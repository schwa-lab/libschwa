/* -*- Mode: C++; indent-tabs-mode: nil -*- */
/**
 * schwa::Hash
 * hash value type and operator overloaded support methods
 * allows new hash functions to be implemented in a uniform
 * manner with the same basic shift and add approach to
 * implementing the hash function.
 * in practice this methods works quite well
 */

namespace schwa {

  class Hash {
  private:
    // value to set hash values to if initially undefined
    const static unsigned long _INIT = 0;

    // shift value to multiply by with each additional
    // component of the hash function
    // this number is normally selected to be a smallish prime
    // 31 and 37 seem to work well
    const static unsigned long _INC = 31;

    // hash value currently stored in an unsigned long
    unsigned long _hash;

  public:
    // make all the constructors explicit so there are no nasty
    // surprises with automatic casts
    explicit Hash(unsigned long hash): _hash(hash) { };
    explicit Hash(unsigned short hash): _hash(hash) { };
    explicit Hash(unsigned char hash): _hash(hash) { };
    explicit Hash(char c): _hash(_INIT) { *this += c; };
    explicit Hash(const char *str): _hash(_INIT) { *this += str; };
    explicit Hash(const std::string &str): _hash(_INIT) { *this += str; };

    Hash(const Hash &other): _hash(other._hash) { };

    // += operators which do the shift and add automatically
    Hash &
    operator +=(const char c) {
      _hash = _hash*_INC + static_cast<unsigned char>(c);
      return *this;
    }

    Hash &
    operator +=(const char *str) {
      for( ; *str; ++str)
        *this += *str;
      return *this;
    };

    Hash &
    operator +=(const std::string &str) {
      for(const char *s = str.c_str(); *s; ++s)
        *this += *s;
      return *this;
    };

    Hash &
    operator +=(const Hash &hash) {
      _hash *= _INC;
      _hash += hash._hash;
      return *this;
    }

    Hash &
    operator +=(unsigned char value) {
      _hash *= _INC;
      _hash += value;
      return *this;
    }

    Hash &
    operator +=(unsigned short value) {
      _hash *= _INC;
      _hash += value;
      return *this;
    }

    Hash &
    operator +=(unsigned long value) {
      _hash *= _INC;
      _hash += value;
      return *this;
    }

    Hash &
    operator +=(int value) {
      return *this += static_cast<unsigned long>(value);
    };

    // for greater manual control over hash function shifting
    // and adding, there are the separated out add and shift (multiply) steps
    Hash &operator |=(unsigned long value) { _hash += value; return *this; };
    Hash &operator *=(unsigned long value) { _hash *= value; return *this; };

    // get a unsigned long back for indexing by applying mod
    unsigned long operator %(const size_t buckets) const { return _hash % buckets; };

    // get the unsigned long internal value
    unsigned long value(void) const { return _hash; };
  };

  inline bool operator ==(const Hash &a, const Hash &b) { return a.value() == b.value(); }
  inline bool operator !=(const Hash &a, const Hash &b) { return a.value() != b.value(); }
}
