// Copyright 2005-2009 The RE2 Authors.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

// Modified from Google perftools's tcmalloc_unittest.cc.

#ifndef SCHWA_THIRD_PARTY_RE2_UTIL_RANDOM_H__
#define SCHWA_THIRD_PARTY_RE2_UTIL_RANDOM_H__

#include <schwa/third-party/re2/util/util.h>

namespace schwa { namespace third_party { namespace re2 {

// ACM minimal standard random number generator.  (re-entrant.)
class ACMRandom {
 public:
  ACMRandom(int32 seed) : seed_(seed) {}
  int32 Next();
  int32 Uniform(int32);

  void Reset(int32 seed) { seed_ = seed; }

 private:
  int32 seed_;
};

} } }  // namespace

#endif  // SCHWA_THIRD_PARTY_RE2_UTIL_RANDOM_H__
