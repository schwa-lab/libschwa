/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/type_traits.hpp>

namespace schwa {
  namespace dr {

    class BaseAnnotationSchema;
    class BaseFieldDef;
    class BaseStoreDef;

    template <typename T, T fn>
    class FieldDef;
    template <typename T1, T1 fn1, typename T2, T2 fn2>
    class FieldDefWithStore;
    template <typename T, T fn>
    class StoreDef;

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
        //static void write(std::ostream &out, const T &val);
      };

      template <typename T>
      struct WireTraitsPrimative {
        static constexpr inline bool should_write(const T &) { return true; }
        static inline void write(std::ostream &out, const T &val) { mp::write(out, val); }
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
        static inline bool should_write(const std::string &val) { return !val.empty(); }
        static inline void write(std::ostream &out, const std::string &val) { mp::write(out, val); }
      };

      template <typename T>
      struct WireTraits<Pointer<T>> {
        static inline bool should_write(const Pointer<T> &val) { return val.ptr != nullptr; }

        static inline void
        write(std::ostream &out, const Pointer<T> &val, const T &front) {
          mp::write_uint(out, val.ptr - &front);
        }
      };

      template <typename T, bool IS_POINTER>
      struct WireTraitsSliceTraits {
        static inline bool should_write(const Slice<T> &val) { return val.start != T() && val.stop != T(); }

        static inline void
        write(std::ostream &out, const Slice<T> &val) {
          mp::write_array_header(out, 2);
          mp::write<T>(out, val.start);
          mp::write<T>(out, val.stop);
        }
      };

      template <typename T>
      struct WireTraitsSliceTraits<T, true> {
        static inline bool should_write(const Slice<T> &val) { return val.start != nullptr && val.stop != nullptr; }

        static inline void
        write(std::ostream &out, const Slice<T> &val, const typename FieldTraits<Slice<T>>::pointer_type &front) {
          mp::write_array_header(out, 2);
          mp::write_uint(out, val.start - &front);
          mp::write_uint(out, val.stop - &front);
        }
      };

      template <typename T>
      struct WireTraits<Slice<T>> : public WireTraitsSliceTraits<T, FieldTraits<Slice<T>>::is_dr_ptr_type> { };


      template <typename R, typename T, R T::*field_ptr>
      inline bool
      write_field(std::ostream &out, const unsigned int key, const void *const _ann, const void *const _doc) {
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


      template <typename R, typename T, typename S, typename D, R T::*field_ptr, Store<S> D::*store_ptr>
      inline bool
      write_field(std::ostream &out, const unsigned int key, const void *const _ann, const void *const _doc) {
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


    // ========================================================================
    // BaseSchema definitions
    // ========================================================================
    class BaseSchema {
    public:
      typedef std::vector<BaseFieldDef *> field_container;
      typedef std::vector<bool (*)(std::ostream &out, const unsigned int, const void *, const void *)> writers_container;

      const std::string name;
      const std::string help;
      std::string serial;
      const TypeInfo type;

    protected:
      field_container _fields;
      writers_container _writers;

      BaseSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : name(name), help(help), serial(serial), type(type) { }

    public:
      virtual ~BaseSchema(void) { }

      template <typename T, T fn>
      inline void add(FieldDef<T, fn> *const field) {
        typedef FieldDef<T, fn> F;
        _fields.push_back(field);
        _writers.push_back(&wire::write_field<typename F::value_type, typename F::annotation_type, fn>);
      }

      template <typename T1, T1 fn1, typename T2, T2 fn2>
      inline void add(FieldDefWithStore<T1, fn1, T2, fn2> *const field) {
        typedef FieldDefWithStore<T1, fn1, T2, fn2> F;
        _fields.push_back(field);
        _writers.push_back(&wire::write_field<typename F::value_type, typename F::annotation_type, typename F::store_type, typename F::doc_type, fn1, fn2>);
      }

      inline const field_container &fields(void) const { return _fields; }
      inline const writers_container &writers(void) const { return _writers; }

      virtual std::ostream &dump(std::ostream &out) const;
    };


    inline std::ostream &
    operator <<(std::ostream &out, const BaseSchema &s) {
      return s.dump(out);
    }


    // ========================================================================
    // Base classes
    // ========================================================================
    class Annotation {
    protected:
      Annotation(void) { }
      Annotation(const Annotation &) { }
      Annotation(const Annotation &&) { }
      Annotation &operator =(const Annotation &) { return *this; }
    };


    class Document {
    protected:
      Document(void) { }
      Document(const Document &) = delete;
    };


    class BaseAnnotationSchema : public BaseSchema {
    protected:
      BaseAnnotationSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : BaseSchema(name, help, serial, type) { }

    public:
      virtual ~BaseAnnotationSchema(void) { }

      virtual std::ostream &dump(std::ostream &out) const;
    };


    class BaseDocumentSchema : public BaseSchema {
    public:
      typedef std::vector<BaseAnnotationSchema *> schema_container;
      typedef std::vector<BaseStoreDef *> store_container;

    protected:
      schema_container _schemas;
      store_container _stores;

      BaseDocumentSchema(const std::string &name, const std::string &help, const std::string &serial, const TypeInfo &type) : BaseSchema(name, help, serial, type) { }

    public:
      virtual ~BaseDocumentSchema(void) {
        for (auto &s : _schemas)
          delete s;
      }

      template <typename T, T fn>
      inline void add(StoreDef<T, fn> *const store) {
        typedef typename StoreDef<T, fn>::store_type::Schema S;
        static_assert(boost::is_base_of<BaseAnnotationSchema, S>::value, "T::Schema for the Store<T> must be a subclass of BaseAnnotationSchema");

        _stores.push_back(store);

        // check to see if we have not yet seen this type
        const TypeInfo &t = store->pointer_type();
        for (auto &s : _schemas)
          if (s->type == t)
            return;
        S *const schema = new S();
        assert(schema != nullptr);
        _schemas.push_back(schema);
      }

      template <typename T>
      inline typename T::Schema &
      types(void) const {
        static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");
        const TypeInfo type = TypeInfo::create<T>();
        for (auto &it : _schemas)
          if (it->type == type)
            return *static_cast<typename T::Schema *>(it);
        assert(!"Type was not found!");
        return *static_cast<typename T::Schema *>(nullptr);
      }

      inline const schema_container &schemas(void) const { return _schemas; }
      inline const store_container &stores(void) const { return _stores; }

      virtual std::ostream &dump(std::ostream &out) const;
    };


    // ========================================================================
    // Templated base schemas
    // ========================================================================
    template <typename T>
    class AnnotationSchema : public BaseAnnotationSchema {
    public:
      static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");

      AnnotationSchema(const std::string &name, const std::string &help, const std::string &serial) : BaseAnnotationSchema(name, help, serial, TypeInfo::create<T>()) { }
      virtual ~AnnotationSchema(void) { }
    };


    template <typename D>
    class DocumentSchema : public BaseDocumentSchema {
    public:
      static_assert(boost::is_base_of<Document, D>::value, "D must be a subclass of Document");

      DocumentSchema(const std::string &name, const std::string &help) : BaseDocumentSchema(name, help, "", TypeInfo::create<D>()) { }
      virtual ~DocumentSchema(void) { }
    };

  }
}
