/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/type_traits.hpp>

namespace schwa {
  namespace dr {

    // ========================================================================
    // Field types
    // ========================================================================
    template <typename T>
    class Slice {
    public:
      T begin;
      T end;

      Slice(void) { }
      Slice(const T &begin, const T &end) : begin(begin), end(end) { }
    };


    template <typename T>
    class Pointer {
    public:
      typedef T pointer_type;

      T *ptr;

      Pointer(T *ptr=nullptr) : ptr(ptr) { }
      ~Pointer(void) { }
    };


    template <typename T>
    class Pointers {
    public:
      typedef T pointer_type;

      std::vector<T *> items;
    };


    // ========================================================================
    // Storage types
    // ========================================================================
    template <typename T>
    class MemoryBlock {
    private:
      char *const _data;
      const size_t _capacity;
      size_t _size;

    public:
      MemoryBlock(const size_t capacity) : _data(new char[capacity * sizeof(T)]), _capacity(capacity), _size(0) {
        assert(_data != nullptr);
      }
      ~MemoryBlock(void) {
        delete [] _data;
      }

      inline size_t capacity(void) const { return _capacity; }
      inline size_t size(void) const { return _size; }

      inline char *
      alloc(void) {
        if (_size == _capacity)
          return nullptr;
        char *const ptr = &_data[_size * sizeof(T)];
        ++_size;
        return ptr;
      }
    };


    template <typename T, size_t GROW_SIZE=32>
    class Store {
    public:
      static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");
      static_assert(GROW_SIZE > 0, "GROW_SIZE must be positive");
      typedef T value_type;

    protected:
      std::vector<MemoryBlock<T> *> _blocks;
      MemoryBlock<T> *_block;
      size_t _size;

    public:
      Store(void) : _block(nullptr), _size(0) { }
      Store(const Store &) = delete;
      ~Store(void) {
        delete _block;
        for (auto &block : _blocks)
          delete block;
      }

      inline size_t size(void) const { return _size; }
      inline size_t nblocks(void) const { return (_block != nullptr) + _blocks.size(); }

      T &
      create(void) {
        if (_block == nullptr)
          _block = new MemoryBlock<T>(GROW_SIZE);

        char *bytes = _block->alloc();
        if (bytes == nullptr) {
          _blocks.push_back(_block);
          _block = new MemoryBlock<T>(GROW_SIZE);
          bytes = _block->alloc();
        }

        T *const ptr = new (bytes) T();
        ptr->set_dr_index(_size++);
        return *ptr;
      }
    };


    template <typename T>
    class Singleton {
    public:
      static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");
      T *ptr;

      Singleton(T *ptr=nullptr) : ptr(ptr) { }
    };


    // ========================================================================
    // FieldTraits
    // ========================================================================
    template <typename T>
    struct FieldTraits {
      static constexpr const bool is_dr_ptr_type = false;
      static constexpr const bool is_pod_ptr = boost::is_pointer<T>::value;
      static constexpr const bool is_slice = false;
    };

    template <typename T>
    struct FieldTraits<Pointer<T>> {
      typedef T pointer_type;
      static constexpr const bool is_dr_ptr_type = true;
      static constexpr const bool is_slice = false;
    };

    template <typename T>
    struct FieldTraits<Pointers<T>> {
      typedef T pointer_type;
      static constexpr const bool is_dr_ptr_type = true;
      static constexpr const bool is_slice = false;
    };

    template <typename T, bool>
    struct SliceFieldTraits {
      static constexpr const bool is_dr_ptr_type = false;
      static constexpr const bool is_pod_ptr = false;
    };

    template <typename T>
    struct SliceFieldTraits<T, true> {
      static constexpr const bool is_dr_ptr_type = true;
      typedef typename boost::remove_pointer<T>::type pointer_type;
    };

    template <typename T>
    struct FieldTraits<Slice<T>> : public SliceFieldTraits<T, boost::is_pointer<T>::value> {
      static constexpr const bool is_slice = true;
    };


  }
}
