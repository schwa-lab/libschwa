/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace containers {

    template <typename T>
    class BlockVector {
    public:
      class Block {
      public:
        typedef const T *const_iterator;
        typedef const T *const_pointer;
        typedef const T &const_reference;
        typedef T *iterator;
        typedef T *pointer;
        typedef T &reference;
        typedef size_t size_type;
        typedef T value_type;

      private:
        const size_type _capacity;
        size_type _size;
        Block *_next;

      public:
        explicit Block(const size_type capacity);
        ~Block(void) { }

        inline reference operator [](const size_type i) { return get(i); }
        inline const_reference operator [](const size_type i) const { return get(i); }

        inline reference get(const size_type i);
        inline const_reference get(const size_type i) const;

        inline const_iterator begin(void) const;
        inline const_iterator end(void) const;
        inline iterator begin(void);
        inline iterator end(void);

        inline const_reference front(void) const { return *begin(); }
        inline const_reference back(void) const { return *(end() - 1); }
        inline reference front(void) { return *begin(); }
        inline reference back(void) { return *(end() - 1); }

        inline size_type capacity(void) const { return _capacity; }
        inline size_type size(void) const { return _size; }

        inline bool contains(const_reference obj) const { return begin() <= &obj && &obj < end(); }
        inline bool empty(void) const { return _size == 0; }
        inline bool full(void) const { return _size == _capacity; }

        reference create(void);
        reference create(const_reference other);

        inline Block *next(void) const { return _next; }
        inline void set_next(Block *const next) { _next = next; }
      };


    class Iterator : public std::iterator<std::forward_iterator_tag, T> {
    public:
      typedef typename std::iterator<std::forward_iterator_tag, T>::difference_type difference_type;
      typedef typename std::iterator<std::forward_iterator_tag, T>::iterator_category iterator_category;
      typedef typename std::iterator<std::forward_iterator_tag, T>::pointer pointer;
      typedef typename std::iterator<std::forward_iterator_tag, T>::reference reference;
      typedef typename std::iterator<std::forward_iterator_tag, T>::value_type value_type;

    private:
      Block *_block;
      typename Block::iterator _it;
      typename Block::iterator _end;

      void increment_it(void);

    public:
      explicit Iterator(Block *block=nullptr);
      Iterator(const Iterator &o);
      ~Iterator(void) { }

      Iterator &operator =(const Iterator &o);

      inline bool operator ==(const Iterator &o) const { return _block == o._block && _it == o._it; }
      inline bool operator !=(const Iterator &o) const { return _block != o._block || _it != o._it; }

      inline reference operator *(void) { return *_it; }
      inline pointer operator ->(void) { return _it; }

      Iterator &operator ++(void);
      Iterator operator ++(int);

      inline std::ostream &dump(std::ostream &out) const;

      friend inline std::ostream &operator <<(std::ostream &out, const Iterator &it) { return it.dump(out); }
    };

    typedef const T *const_pointer;
    typedef const T &const_reference;
    typedef Iterator iterator;
    typedef T *pointer;
    typedef T &reference;
    typedef size_t size_type;
    typedef T value_type;

    private:
      Block *_first;
      Block *_last;

    public:
      BlockVector(void);
      ~BlockVector(void);

      inline reference operator [](size_type i) { return get(i); }
      inline const_reference operator [](size_type i) const { return get(i); }

      inline reference get(size_type i);
      inline const_reference get(size_type i) const;

      size_type index_of(const_reference obj) const;

      size_type nblocks(void) const;
      size_type size(void) const;

      inline iterator begin(void) { return iterator(_first); }
      inline iterator end(void) { return iterator(); }

      inline const_reference front(void) const { return _first->front(); }
      inline const_reference back(void) const { return _last->back(); }
      inline reference front(void) { return _first->front(); }
      inline reference back(void) { return _last->back(); }

      inline const Block &first_block(void) const { return *_first; }
      inline const Block &last_block(void) const { return *_last; }
      inline Block &first_block(void) { return *_first; }
      inline Block &last_block(void) { return *_last; }

      inline void push_back(const_reference obj);

      Block &reserve(size_type nelem);
    };

  }
}

#include <schwa/containers/block_vector_impl.h>
