/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace dr {

    class Annotation {
    };


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
      BOOST_STATIC_ASSERT(boost::is_base_of<Annotation, T>::value);
      T *ptr;

      Pointer(T *ptr=0) : ptr(ptr) { }
    };


    template <typename T>
    class Pointers {
    public:
      BOOST_STATIC_ASSERT(boost::is_base_of<Annotation, T>::value);
      std::vector<T *> items;
    };


    template <typename T>
    class MemoryBlock {
    private:
      char *const _data;
      const size_t _capacity;
      size_t _size;

    public:
      MemoryBlock(const size_t capacity, MemoryBlock *next=0) : _data(new char[capacity * sizeof(T)]), _capacity(capacity), _size(0), _next(next) {
        assert(_data != 0);
      }
      ~MemoryBlock(void) {
        delete [] _data;
      }

      inline size_t capacity(void) const { return _capacity; }
      inline size_t size(void) const { return _size; }

      inline char *
      alloc(void) {
        if (_size == _capacity)
          return 0;
        char *const ptr = _data[_size * sizeof(T)];
        ++_size;
        return ptr;
      }
    };


    template <typename T>
    class Store {
    protected:
      std::vector<MemoryBlock<T> *> _blocks;
      size_t _size;

    public:
      Store(void) : _size(0) { }
      ~Store(void) {
        for (std::vector<MemoryBlock<T> *>::const_iterator it = _blocks.begin(); it != _blocks.end(); ++it)
          delete *it;
      }

    };

  }
}
