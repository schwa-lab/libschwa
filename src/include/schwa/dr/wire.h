/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    template <typename T>
    class Pointer;
    template <typename T>
    class Pointers;
    template <typename T>
    class Slice;
    template <typename T>
    class Store;
    template <typename T>
    struct FieldTraits;


    namespace wire {
      namespace mp = schwa::msgpack;

      template <typename T>
      struct WireTraits {
        //static bool should_write(const T &val);
        //template <typename OUT> static void write(OUT &out, const T &val);
        //template <typename IN> static void read(IN &in, T &val);
      };


      template <typename T>
      struct WireTraitsPrimative {
        static constexpr inline bool
        should_write(const T &) {
          return true;
        }

        template <typename OUT>
        static inline void
        write(OUT &out, const T &val) {
          mp::write(out, val);
        }

        template <typename IN>
        static inline void
        read(IN &in, T &val) {
          mp::read(in, val);
        }
      };


      template <> struct WireTraits<int8_t> : public WireTraitsPrimative<int8_t> { };
      template <> struct WireTraits<int16_t> : public WireTraitsPrimative<int16_t> { };
      template <> struct WireTraits<int32_t> : public WireTraitsPrimative<int32_t> { };
      template <> struct WireTraits<int64_t> : public WireTraitsPrimative<int64_t> { };
      template <> struct WireTraits<uint8_t> : public WireTraitsPrimative<uint8_t> { };
      template <> struct WireTraits<uint16_t> : public WireTraitsPrimative<uint16_t> { };
      template <> struct WireTraits<uint32_t> : public WireTraitsPrimative<uint32_t> { };
      template <> struct WireTraits<uint64_t> : public WireTraitsPrimative<uint64_t> { };
      template <> struct WireTraits<float> : public WireTraitsPrimative<float> { };
      template <> struct WireTraits<double> : public WireTraitsPrimative<double> { };
      template <> struct WireTraits<bool> : public WireTraitsPrimative<bool> { };


      template <>
      struct WireTraits<std::string> {
        static inline bool
        should_write(const std::string &val) {
          return !val.empty();
        }

        template <typename OUT>
        static inline void
        write(OUT &out, const std::string &val) {
          mp::write(out, val);
        }

        template <typename IN>
        static inline void
        read(IN &in, std::string &val) {
          mp::read(in, val);
        }
      };


      template <typename T>
      struct WireTraits<Pointer<T>> {
        static inline bool should_write(const Pointer<T> &val) { return val.ptr != nullptr; }

        template <typename OUT>
        static inline void
        write(OUT &out, const Pointer<T> &val, const T &front) {
          mp::write_uint(out, val.ptr - &front);
        }

        template <typename IN>
        static inline void
        read(IN &in, Pointer<T> &val, T &front) {
          const size_t offset = mp::read_uint(in);
          val.ptr = &front + offset;
        }
      };


      template <typename T>
      struct WireTraits<Pointers<T>> {
        static inline bool should_write(const Pointers<T> &val) { return val.items.size() != 0; }

        template <typename OUT>
        static inline void
        write(OUT &out, const Pointers<T> &val, const T &front) {
          mp::write_array_size(out, val.items.size());
          for (auto &it : val.items)
            mp::write_uint(out, it - &front);
        }

        template <typename IN>
        static inline void
        read(IN &in, Pointers<T> &val, T &front) {
          const size_t nitems = mp::read_uint(in);
          for (size_t i = 0; i != nitems; ++i) {
            const size_t offset = mp::read_uint(in);
            val.items.push_back(&front + offset);
          }
        }
      };


      template <typename T, bool IS_POINTER>
      struct WireTraitsSliceTraits {
        static inline bool should_write(const Slice<T> &val) { return !(val.start == T() && val.stop == T()); }

        template <typename OUT>
        static inline void
        write(OUT &out, const Slice<T> &val) {
          mp::write_array_size(out, 2);
          mp::write(out, val.start);
          mp::write(out, val.stop - val.start);
        }

        template <typename IN>
        static inline void
        read(IN &in, Slice<T> &val) {
          const size_t nitems = mp::read_array_size(in);
          assert(nitems == 2);
          mp::read<IN>(in, val.start);
          mp::read<IN>(in, val.stop);
          val.stop += val.start;
        }
      };


      template <typename T>
      struct WireTraitsSliceTraits<T, true> {
        static inline bool should_write(const Slice<T> &val) { return !(val.start == nullptr && val.stop == nullptr); }

        template <typename OUT>
        static inline void
        write(OUT &out, const Slice<T> &val, const typename FieldTraits<Slice<T>>::value_type &front) {
          const size_t a = val.start - &front;
          const size_t b = val.stop - &front;
          mp::write_array_size(out, 2);
          mp::write_uint(out, a);
          mp::write_uint(out, b - a);
        }

        template <typename IN>
        static inline void
        read(IN &in, Slice<T> &val, typename FieldTraits<Slice<T>>::value_type &front) {
          const size_t nitems = mp::read_array_size(in);
          assert(nitems == 2);
          const size_t offset = mp::read_uint(in);
          const size_t delta = mp::read_uint(in);
          val.start = &front + offset;
          val.stop = &front + (offset + delta);
        }
      };


      template <typename T>
      struct WireTraits<Slice<T>> : public WireTraitsSliceTraits<T, FieldTraits<Slice<T>>::is_dr_ptr_type> { };


      template <typename IN, typename R, typename T, R T::*field_ptr>
      inline void
      read_field(IN &in, void *const _ann, void *const _doc) {
        static_cast<void>(_doc);
        T &ann = *static_cast<T *>(_ann);
        R &val = ann.*field_ptr;
        WireTraits<R>::read(in, val);
      }


      template <typename OUT, typename R, typename T, R T::*field_ptr>
      inline bool
      write_field(OUT &out, const uint32_t key, const void *const _ann, const void *const _doc) {
        static_cast<void>(_doc);
        const T &ann = *static_cast<const T *>(_ann);
        const R &val = ann.*field_ptr;
        if (WireTraits<R>::should_write(val)) {
          mp::write_uint(out, key);
          WireTraits<R>::write(out, val);
          return true;
        }
        return false;
      }


      template <typename IN, typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
      inline void
      read_field(IN &in, void *const _ann, void *const _doc) {
        D &doc = *static_cast<D *>(_doc);
        T &ann = *static_cast<T *>(_ann);
        R &val = ann.*field_ptr;
        WireTraits<R>::read(in, val, (doc.*store_ptr).front());
      }


      template <typename OUT, typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
      inline bool
      write_field(OUT &out, const uint32_t key, const void *const _ann, const void *const _doc) {
        const D &doc = *static_cast<const D *>(_doc);
        const T &ann = *static_cast<const T *>(_ann);
        const R &val = ann.*field_ptr;
        if (WireTraits<R>::should_write(val)) {
          mp::write_uint(out, key);
          WireTraits<R>::write(out, val, (doc.*store_ptr).front());
          return true;
        }
        return false;
      }

    }
  }
}
