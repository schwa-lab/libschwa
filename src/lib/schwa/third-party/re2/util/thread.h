// Copyright 2009 The RE2 Authors.  All Rights Reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#ifndef SCHWA_THIRD_PARTY_RE2_UTIL_THREAD_H__
#define SCHWA_THIRD_PARTY_RE2_UTIL_THREAD_H__

#include <pthread.h>

namespace schwa { namespace third_party { namespace re2 {

class Thread {
 public:
  Thread();
  virtual ~Thread();
  void Start();
  void Join();
  void SetJoinable(bool);
  virtual void Run() = 0;
 
 private:
  pthread_t pid_;
  bool running_;
  bool joinable_;
};

} } }  // namespace

#endif  // SCHWA_THIRD_PARTY_RE2_UTIL_THREAD_H__
