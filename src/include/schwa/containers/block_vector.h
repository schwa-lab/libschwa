/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace containers {

    template <typename T>
    class BlockVector {
    public:
      class Block {
      public:
        typedef T *iterator;
        typedef const T *const_iterator;

      private:
        const size_t _capacity;
        size_t _size;
        Block *_next;

      public:
        explicit Block(const size_t capacity);
        ~Block(void) { }

        inline T &operator [](const size_t i) { return get(i); }
        inline const T &operator [](const size_t i) const { return get(i); }

        inline T &get(const size_t i);
        inline const T &get(const size_t i) const;

        inline const_iterator begin(void) const;
        inline const_iterator end(void) const;
        inline iterator begin(void);
        inline iterator end(void);

        inline const T &first(void) const { return *begin(); }
        inline const T &last(void) const { return *(end() - 1); }
        inline T &first(void) { return *begin(); }
        inline T &last(void) { return *(end() - 1); }

        inline size_t capacity(void) const { return _capacity; }
        inline size_t size(void) const { return _size; }

        inline bool contains(const T &obj) const { return begin() <= &obj && &obj < end(); }
        inline bool empty(void) const { return _size == 0; }
        inline bool full(void) const { return _size == _capacity; }

        T &create(void);
        T &create(const T &other);

        inline Block *next(void) const { return _next; }
        inline void set_next(Block *const next) { _next = next; }
      };

    private:
      Block *_first;
      Block *_last;

    public:
      BlockVector(void);
      ~BlockVector(void);

      inline T &operator [](size_t i) { return get(i); }
      inline const T &operator [](size_t i) const { return get(i); }

      inline T &get(size_t i);
      const T &get(size_t i) const;

      size_t size(void) const;

      Block &reserve(const size_t nelem);
    };

  }
}

#include <schwa/containers/block_vector_impl.h>
