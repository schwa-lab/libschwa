/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_EXTRACT_H_
#define SCHWA_LEARN_EXTRACT_H_

#include <functional>
#include <string>

#include <schwa/_base.h>
#include <schwa/dr/fields.h>
#include <schwa/learn/features.h>


namespace schwa {
  namespace learn {

    const std::string SENTINEL = "__SENTINEL__";


    template <typename T>
    class SentinelOffsets {
    public:
      using callback_type = std::function<std::string(T *)>;

    private:
      T *const _start;
      T *const _stop;
      callback_type _callback;

    public:
      template <typename F>
      SentinelOffsets(const dr::Slice<T *> &slice, F callback) : _start(slice.start), _stop(slice.stop), _callback(callback) { }
      template <typename F>
      SentinelOffsets(T *start, T *stop, F callback) : _start(start), _stop(stop), _callback(callback) { }

      inline std::string
      operator ()(const size_t i, const ptrdiff_t delta) const {
        T *const ptr = (_start + i) + delta;
        if (ptr < _start || ptr >= _stop)
          return SENTINEL;
        else
          return _callback(ptr);
      }
    };


    template <typename T, typename R, class TRANSFORM=NoTransform>
    void
    window(const std::string &name, size_t i, ptrdiff_t dl, ptrdiff_t dr, const SentinelOffsets<T> &offsets, Features<TRANSFORM> &features, std::function<R(const SentinelOffsets<T> &, size_t, ptrdiff_t)> callback);

  }
}

#include <schwa/learn/extract_impl.h>

#endif  // SCHWA_LEARN_EXTRACT_H_
