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

      enum FieldType : uint8_t {
        NAME = 0,
        POINTER_TO = 1,
        IS_SLICE = 2,
        IS_SELF_POINTER = 3,
        IS_COLLECTION = 4
      };


      template <typename T, bool>
      struct _WireTraits {
        //static bool should_write(const T &val);
        //template <typename OUT> static void write(OUT &out, const T &val);
        //template <typename IN> static void read(IN &in, T &val);
      };


      template <typename T>
      struct _WireTraits<T, true> {
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


      template <typename T>
      struct WireTraits : _WireTraits<T, std::is_arithmetic<T>::value> { };


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
        static inline bool
        should_write(const Pointer<T> &val) {
          return val.ptr != nullptr;
        }

        template <typename OUT>
        static inline void
        write(OUT &out, const Pointer<T> &val, const IStore &istore) {
          mp::write_uint(out, istore.index_of(*val.ptr));
        }

        template <typename IN>
        static inline void
        read(IN &in, Pointer<T> &val, IStore &istore) {
          const size_t offset = mp::read_uint(in);
          val.ptr = &static_cast<T &>(istore.at_index(offset));
        }
      };


      template <typename T>
      struct WireTraits<Pointers<T>> {
        static inline bool
        should_write(const Pointers<T> &val) {
          return val.items.size() != 0;
        }

        template <typename OUT>
        static inline void
        write(OUT &out, const Pointers<T> &val, const IStore &istore) {
          mp::write_array_size(out, val.items.size());
          for (auto &it : val.items)
            mp::write_uint(out, istore.index_of(*it));
        }

        template <typename IN>
        static inline void
        read(IN &in, Pointers<T> &val, IStore &istore) {
          const size_t nitems = mp::read_uint(in);
          for (size_t i = 0; i != nitems; ++i) {
            const size_t offset = mp::read_uint(in);
            val.items.push_back(&static_cast<T &>(istore.at_index(offset)));
          }
        }
      };


      template <typename T, bool IS_POINTER>
      struct WireTraitsSliceTraits {
        static inline bool
        should_write(const Slice<T> &val) {
          return !(val.start == T() && val.stop == T());
        }

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
        static inline bool
        should_write(const Slice<T> &val) {
          return !(val.start == nullptr && val.stop == nullptr);
        }

        template <typename OUT>
        static inline void
        write(OUT &out, const Slice<T> &val, const IStore &istore) {
          const size_t a = istore.index_of(*val.start);
          const size_t b = istore.index_of(*val.stop);
          mp::write_array_size(out, 2);
          mp::write_uint(out, a);
          mp::write_uint(out, b - a);
        }

        template <typename IN>
        static inline void
        read(IN &in, Slice<T> &val, IStore &istore) {
          const size_t nitems = mp::read_array_size(in);
          assert(nitems == 2);
          const size_t offset = mp::read_uint(in);
          const size_t delta = mp::read_uint(in);
          val.start = &static_cast<typename FieldTraits<Slice<T>>::value_type &>(istore.at_index(offset));
          val.stop = &static_cast<typename FieldTraits<Slice<T>>::value_type &>(istore.at_index(offset + delta));
        }
      };


      template <typename T>
      struct WireTraits<Slice<T>> : WireTraitsSliceTraits<T, FieldTraits<Slice<T>>::is_dr_ptr_type> { };
    }
  }
}
