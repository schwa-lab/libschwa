/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_EXTRACT_H_
#define SCHWA_LEARN_EXTRACT_H_

#include <functional>
#include <sstream>
#include <string>

#include <schwa/_base.h>
#include <schwa/dr/fields.h>
#include <schwa/learn/features.h>
#include <schwa/unicode.h>


namespace schwa {
  namespace learn {

    extern const std::string SENTINEL;


    template <typename T>
    class SentinelOffsets {
    public:
      using callback_type = std::function<std::string(const T &)>;

    private:
      T *_start;
      T *_stop;
      callback_type _callback;

    public:
      template <typename F>
      explicit SentinelOffsets(F callback) : _start(nullptr), _stop(nullptr), _callback(callback) { }

      template <typename F>
      SentinelOffsets(const dr::Slice<T *> &slice, F callback) : _start(slice.start), _stop(slice.stop), _callback(callback) { }

      template <typename F>
      SentinelOffsets(T *start, T *stop, F callback) : _start(start), _stop(stop), _callback(callback) { }

      inline void set_slice(const dr::Slice<T *> &slice) { set_slice(slice.start, slice.stop); }
      inline void
      set_slice(T *start, T *stop) {
        _start = start;
        _stop = stop;
      }

      inline std::string
      operator ()(const size_t i, const ptrdiff_t delta) const {
        T *const ptr = (_start + i) + delta;
        if (ptr < _start || ptr >= _stop)
          return SENTINEL;
        else
          return _callback(*ptr);
      }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(SentinelOffsets);
    };


    template <typename T, typename R=std::string>
    using contextual_callback = std::function<R(const SentinelOffsets<T> &, size_t, ptrdiff_t)>;

    template <typename T> inline contextual_callback<T> create_unigram_callback(void);
    template <typename T> inline contextual_callback<T> create_bigram_callback(void);
    template <typename T> inline contextual_callback<T> create_trigram_callback(void);


    class Windower {
    private:
      const int8_t _delta_left;
      const int8_t _delta_right;
      std::string _name;

    public:
      Windower(const std::string &name, int8_t delta_left, int8_t delta_right);

      template <typename T, typename R, typename TRANSFORM, typename VALUE>
      void operator ()(const SentinelOffsets<T> &offsets, contextual_callback<T, R> callback, Features<TRANSFORM, VALUE> &features, size_t i) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Windower);
    };


    template <typename T>
    class Windower2 {
    private:
      const int8_t _delta0;
      const int8_t _delta1;
      SentinelOffsets<T> &_offsets;
      std::string _prefix;
      mutable std::ostringstream _attr;

    public:
      Windower2(const std::string &name, int8_t delta0, int8_t delta1, SentinelOffsets<T> &offsets);

      template <typename TRANSFORM, typename VALUE>
      void operator ()(Features<TRANSFORM, VALUE> &features, size_t i) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Windower2);
    };


    template <typename T>
    class Windower3 {
    private:
      const int8_t _delta0;
      const int8_t _delta1;
      const int8_t _delta2;
      SentinelOffsets<T> &_offsets;
      std::string _prefix;
      mutable std::ostringstream _attr;

    public:
      Windower3(const std::string &name, int8_t delta0, int8_t delta1, int8_t delta2, SentinelOffsets<T> &offsets);

      template <typename TRANSFORM, typename VALUE>
      void operator ()(Features<TRANSFORM, VALUE> &features, size_t i) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Windower3);
    };


    template <typename TRANSFORM, typename VALUE>
    void
    add_affix_features(Features<TRANSFORM, VALUE> &features, size_t nprefix, size_t nsuffix, const std::string &utf8);

    template <typename TRANSFORM, typename VALUE>
    void
    add_affix_features(Features<TRANSFORM, VALUE> &features, size_t nprefix, size_t nsuffix, const UnicodeString &s);
  }
}

#include <schwa/learn/extract_impl.h>

#endif  // SCHWA_LEARN_EXTRACT_H_
