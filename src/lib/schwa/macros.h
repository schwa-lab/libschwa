/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MACROS_H_
#define SCHWA_MACROS_H_

// A macro to disallow the copy constructor and operator= functions.
// This should be used in the private: declarations for a class.
#define SCHWA_DISALLOW_COPY_AND_ASSIGN(TypeName) \
    TypeName(const TypeName &) = delete;   \
    void operator =(const TypeName &) = delete

#endif  // SCHWA_MACROS_H_
