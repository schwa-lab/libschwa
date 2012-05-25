/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <boost/type_traits/is_base_of.hpp>

namespace schwa {
  namespace dr {

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
      static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");
      T *ptr;

      Pointer(T *ptr=nullptr) : ptr(ptr) { }
    };


    template <typename T>
    class Pointers {
    public:
      static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");
      std::vector<T *> items;
    };


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

  }
}
