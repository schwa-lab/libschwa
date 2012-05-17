/* -*- Mode: C++; indent-tabs-mode: nil -*- */
/**
 * schwa::hashtable::TINY -- schwa::hashtable::MASSIVE
 * constant sizes for hash table template parameters
 * these parameters are either the size of the string and entry memory pools
 * (well in fact the size of each arena that the memory pool creates)
 * and also the number of buckets in the hashtable
 */

namespace schwa {
  namespace hashtable {

    static const unsigned long BABY    = 1 << 10;
    static const unsigned long TINY    = 1 << 16;
    static const unsigned long SMALL   = 1 << 18;
    static const unsigned long MEDIUM  = 1 << 20;
    static const unsigned long LARGE   = 1 << 22;
    static const unsigned long MASSIVE = 1 << 24;

  }
}
