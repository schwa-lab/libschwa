/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace containers {

    // ========================================================================
    // BlockVector<T>::Block
    // ========================================================================
    template <typename T>
    BlockVector<T>::Block::Block(const size_t capacity) :
      _capacity(capacity),
      _size(0),
      _next(nullptr)
      { }


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
    // BlockVector<T>::Iterator
    // ========================================================================
    template <typename T>
    BlockVector<T>::Iterator::Iterator(typename BlockVector<T>::Block *block) :
      _block(block),
      _it(_block ? _block->begin() : typename Block::iterator()),
      _end(_block ? _block->end() : typename Block::iterator())
      {
        increment_it();
    }


    template <typename T>
    BlockVector<T>::Iterator::Iterator(const Iterator &o) :
      _block(o._block),
      _it(o._it),
      _end(o._end)
      { }


    template <typename T>
    typename BlockVector<T>::Iterator &
    BlockVector<T>::Iterator::operator =(const Iterator &o) {
      _block = o._block;
      _it = o._it;
      _end = o._end;
      return *this;
    }


    template <typename T>
    void
    BlockVector<T>::Iterator::increment_it(void) {
      while (_block && _it == _end) {
        _block = _block->next();
        if (_block == nullptr) {
          _it = _end = typename Block::iterator();
          break;
        }
        else {
          _it = _block->begin();
          _end = _block->end();
        }
      }
    }


    template <typename T>
    typename BlockVector<T>::Iterator &
    BlockVector<T>::Iterator::operator ++(void) {
      ++_it;
      increment_it();
      return *this;
    }


    template <typename T>
    typename BlockVector<T>::Iterator
    BlockVector<T>::Iterator::operator ++(int) {
      Iterator tmp(*this);
      operator++();
      return tmp;
    }


    template <typename T>
    inline std::ostream &
    BlockVector<T>::Iterator::dump(std::ostream &out) const {
      return out << "[block=" << _block << " it=" << _it << " end=" << _end << "]";
    }


    // ========================================================================
    // BlockVector<T>
    // ========================================================================
    template <typename T>
    BlockVector<T>::BlockVector(void) :
      _first(nullptr),
      _last(nullptr)
      { }


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
    inline typename BlockVector<T>::reference
    BlockVector<T>::get(size_type i) {
      return const_cast<T &>(static_cast<const BlockVector<T> *>(this)->get(i));
    }


    template <typename T>
    typename BlockVector<T>::const_reference
    BlockVector<T>::get(size_type i) const {
      for (const Block *b = _first; b; b = b->next()) {
        if (i < b->size())
          return b->get(i);
        i -= b->size();
      }
      assert(!"offset too large");
      return *static_cast<T *>(nullptr);
    }


    template <typename T>
    typename BlockVector<T>::size_type
    BlockVector<T>::index_of(const_reference obj) const {
      const_pointer const ptr = &obj;
      size_type n = 0;
      for (const Block *b = _first; b; b = b->next()) {
        if (b->begin() <= ptr && ptr < b->end())
          return n + (ptr - b->begin());
        n += b->size();
      }
      assert(!"object not found");
      return static_cast<size_type>(-1);
    }


    template <typename T>
    typename BlockVector<T>::size_type
    BlockVector<T>::nblocks(void) const {
      size_type n = 0;
      for (const Block *b = _first; b; b = b->next())
        ++n;
      return n;
    }


    template <typename T>
    typename BlockVector<T>::size_type
    BlockVector<T>::size(void) const {
      size_type n = 0;
      for (const Block *b = _first; b; b = b->next())
        n += b->size();
      return n;
    }


    template <typename T>
    void
    BlockVector<T>::push_back(const_reference obj) {
      assert(_last != nullptr);
      assert(!_last->full());
      _last->create(obj);
    }


    template <typename T>
    typename BlockVector<T>::Block &
    BlockVector<T>::reserve(const size_type nelem) {
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
