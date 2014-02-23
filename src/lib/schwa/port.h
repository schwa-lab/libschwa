/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_PORT_H_
#define SCHWA_PORT_H_

#include <string>

#include <schwa/_base.h>

#if defined(__APPLE__)
  #include <machine/byte_order.h>
#elif defined(__linux)
  #include <endian.h>
#else
  #error Do not know how to deal with endianness conversion on your platform
#endif

namespace schwa {
  namespace port {

    extern const char *BOLD;
    extern const char *DARK_GREY;
    extern const char *RED;
    extern const char *OFF;

    /**
     * Attempts to return a more meaningful version of a typeid name.
     **/
    std::string demangle_typeid(const char *const typeid_name);

    /**
     * Returns the absolute path to the currently running executable.
     * See http://stackoverflow.com/a/1024937 for the system-dependant ways to obtain this.
     **/
    std::string abspath_to_argv0(void);

    /**
     * Returns the directory of the provided \p path.
     **/
    std::string path_dirname(const std::string &path);

    /**
     * Returns paths \p a and \p b joined together.
     **/
    std::string path_join(const std::string &a, const std::string &b);

#if defined(__APPLE__)
    inline uint16_t h_to_be16(const uint16_t x) { return OSSwapHostToBigConstInt16(x); }
    inline uint32_t h_to_be32(const uint32_t x) { return OSSwapHostToBigConstInt32(x); }
    inline uint64_t h_to_be64(const uint64_t x) { return OSSwapHostToBigConstInt64(x); }
    inline uint16_t be16_to_h(const uint16_t x) { return OSSwapBigToHostConstInt16(x); }
    inline uint32_t be32_to_h(const uint32_t x) { return OSSwapBigToHostConstInt32(x); }
    inline uint64_t be64_to_h(const uint64_t x) { return OSSwapBigToHostConstInt64(x); }
#elif defined(__linux)
    inline uint16_t h_to_be16(const uint16_t x) { return htobe16(x); }
    inline uint32_t h_to_be32(const uint32_t x) { return htobe32(x); }
    inline uint64_t h_to_be64(const uint64_t x) { return htobe64(x); }
    inline uint16_t be16_to_h(const uint16_t x) { return be16toh(x); }
    inline uint32_t be32_to_h(const uint32_t x) { return be32toh(x); }
    inline uint64_t be64_to_h(const uint64_t x) { return be64toh(x); }
#else
  #error Do not know how to deal with endianness conversion on your platform
#endif
  }
}

#endif  // SCHWA_PORT_H_
