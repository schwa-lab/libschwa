/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace containers {

    // ========================================================================
    // BlockVector<T>::Block
    // ========================================================================
    template <typename T>
    BlockVector<T>::Block::Block(const size_t capacity) : _capacity(capacity), _size(0), _next(nullptr) { }


    template <typename T>
    inline const T &
    BlockVector<T>::Block::get(const size_t i) const {
      return reinterpret_cast<const T *>(this + 1)[i];
    }

    template <typename T>
    inline T &
    BlockVector<T>::Block::get(const size_t i) {
      return const_cast<T &>(static_cast<const Block *>(this)->get(i));
    }


    template <typename T>
    inline typename BlockVector<T>::Block::const_iterator
    BlockVector<T>::Block::begin(void) const {
      return reinterpret_cast<const T *>(this + 1);
    }

    template <typename T>
    inline typename BlockVector<T>::Block::iterator
    BlockVector<T>::Block::begin(void) {
      return const_cast<typename BlockVector<T>::Block::iterator>(static_cast<const Block *>(this)->begin());
    }


    template <typename T>
    inline typename BlockVector<T>::Block::const_iterator
    BlockVector<T>::Block::end(void) const {
      return reinterpret_cast<const T *>(this + 1) + _size;
    }

    template <typename T>
    inline typename BlockVector<T>::Block::iterator
    BlockVector<T>::Block::end(void) {
      return const_cast<typename BlockVector<T>::Block::iterator>(static_cast<const Block *>(this)->end());
    }


    template <typename T>
    T &
    BlockVector<T>::Block::create(void) {
      T &obj = get(_size++);
      new (&obj) T();
      return obj;
    }


    template <typename T>
    T &
    BlockVector<T>::Block::create(const T &other) {
      T &obj = get(_size++);
      new (&obj) T(other);
      return obj;
    }


    // ========================================================================
    // BlockVector<T>
    // ========================================================================
    template <typename T>
    BlockVector<T>::BlockVector(void) : _first(nullptr), _last(nullptr) { }


    template <typename T>
    BlockVector<T>::~BlockVector(void) {
      Block *next = nullptr;
      for (Block *b = _first; b; ) {
        next = b->next();
        ::free(b);
        b = next;
      }
    }


    template <typename T>
    inline T &
    BlockVector<T>::get(size_t i) {
      return const_cast<T &>(static_cast<const BlockVector<T> *>(this)->get(i));
    }


    template <typename T>
    const T &
    BlockVector<T>::get(size_t i) const {
      for (const Block *b = _first; b; b = b->next()) {
        if (i < b->size())
          return b->get(i);
        i -= b->size();
      }
      assert(!"offset too large");
      return *static_cast<T *>(nullptr);
    }


    template <typename T>
    size_t
    BlockVector<T>::size(void) const {
      size_t n = 0;
      for (const Block *b = _first; b; b = b->next())
        n += b->size();
      return n;
    }


    template <typename T>
    typename BlockVector<T>::Block &
    BlockVector<T>::reserve(const size_t nelem) {
      void *const data = ::malloc(sizeof(Block) + nelem*sizeof(T));
      assert(data != nullptr);
      Block *block = new (data) Block(nelem);
      if (_last)
        _last->set_next(block);
      else
        _first = block;
      _last = block;
      return *block;
    }

  }
}
