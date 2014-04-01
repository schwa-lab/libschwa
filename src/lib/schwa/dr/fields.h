/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_FIELDS_H_
#define SCHWA_DR_FIELDS_H_

#include <memory>
#include <type_traits>
#include <vector>

#include <schwa/_base.h>
#include <schwa/containers/block_vector.h>
#include <schwa/dr/istore.h>


namespace schwa {
  namespace dr {

    // ========================================================================
    // Field types
    // ========================================================================
    template <typename T>
    class Slice {
    public:
      T start;
      T stop;

      Slice(void) : start(T()), stop(T()) { }
      Slice(const T &start, const T &stop) : start(start), stop(stop) { }

      inline bool operator ==(const Slice<T> &o) const { return start == o.start && stop == o.stop; }
      inline bool operator !=(const Slice<T> &o) const { return start != o.start || stop != o.stop; }
    };


    template <typename T>
    using Pointer = T *;

    template <typename T, class Alloc=std::allocator<T *>>
    using Pointers = std::vector<T *, Alloc>;


    // ========================================================================
    // Storage types
    // ========================================================================
    template <typename T>
    class Store : public IStore {
    public:
      static_assert(std::is_base_of<Ann, T>::value, "T must be a subclass of Ann");
      using container_type = std::vector<T>;
      using const_reference = typename container_type::const_reference;
      using const_iterator = typename container_type::const_iterator;
      using const_pointer = typename container_type::const_pointer;
      using const_reverse_iterator = typename container_type::const_reverse_iterator;
      using difference_type = typename container_type::difference_type;
      using iterator = typename container_type::iterator;
      using pointer = typename container_type::pointer;
      using reference = typename container_type::reference;
      using reverse_iterator = typename container_type::reverse_iterator;
      using size_type = typename container_type::size_type;
      using value_type = typename container_type::value_type;

      class inner_typeless_iterator : public IStore::inner_typeless_iterator {
      private:
        typename Store<T>::iterator _it;

      public:
        inner_typeless_iterator(typename Store<T>::iterator it) : IStore::inner_typeless_iterator(), _it(it) { }
        virtual ~inner_typeless_iterator(void) { }
        virtual bool operator ==(const IStore::inner_typeless_iterator &o) const override { return _it == static_cast<const inner_typeless_iterator &>(o)._it; }
        virtual bool operator !=(const IStore::inner_typeless_iterator &o) const override { return _it != static_cast<const inner_typeless_iterator &>(o)._it; }
        virtual reference operator *(void) override { return _it.operator*(); }
        virtual pointer operator ->(void) override { return _it.operator->(); }
        virtual IStore::inner_typeless_iterator &operator ++(void) { ++_it; return *this; }
      };

    private:
      container_type _items;

    public:
      Store(void) { }
      Store(const size_type size) : _items(size) { }
      Store(const Store &&o) : _items(o._items) { }
      virtual ~Store(void) { }

      // extra methods
      inline void create(const size_type n, const T x=T()) { _items.insert(_items.end(), n, x); }

      // iterators
      inline iterator begin(void) { return _items.begin(); }
      inline iterator end(void) { return _items.end(); }
      inline const_iterator begin(void) const { return _items.begin(); }
      inline const_iterator end(void) const { return _items.end(); }
      inline reverse_iterator rbegin(void) { return _items.rbegin(); }
      inline reverse_iterator rend(void) { return _items.rend(); }
      inline const_reverse_iterator rbegin(void) const { return _items.rbegin(); }
      inline const_reverse_iterator rend(void) const { return _items.rend(); }

      // capacity
      inline size_type capacity(void) const { return _items.capacity(); }
      inline bool empty(void) const { return _items.empty(); }
      inline void reserve(const size_type n) { _items.reserve(n); }
      inline void resize(const size_type n, T value=T()) { _items.resize(n, value); }
      inline size_type size(void) const { return _items.size(); }

      // element access
      inline reference operator [](const size_type n) { return _items[n]; }
      inline reference at(const size_type n) { return _items.at(n); }
      inline reference back(void) { return _items.back(); }
      inline reference front(void) { return _items.front(); }
      inline const_reference operator [](const size_type n) const { return _items[n]; }
      inline const_reference at(const size_type n) const { return _items.at(n); }
      inline const_reference back(void) const { return _items.back(); }
      inline const_reference front(void) const { return _items.front(); }

      // modifiers
      inline void push_back(const T &x) { _items.push_back(x); }
      inline void pop_back(void) { _items.pop_back(); }
      inline void assign(const size_type n, const T &x) { _items.assign(n, x); }
      inline iterator insert(iterator position, const T &x) { return _items.insert(position, x); }
      inline void insert(iterator position, const size_type n, const T &x) { _items.insert(position, n, x); }
      inline iterator erase(iterator position) { return _items.erase(position); }
      inline iterator erase(iterator first, iterator last) { return _items.erase(first, last); }
      inline void swap(container_type &other) { _items.swap(other); }
      inline void clear(void) { _items.clear(); }
      template <typename InputIterator> inline void assign(InputIterator first, InputIterator last) { _items.assign(first, last); }
      template <typename InputIterator> inline void insert(iterator position, InputIterator first, InputIterator last) { _items.insert(position, first, last); }

      // IStore
      typeless_iterator typeless_begin(void) override { return typeless_iterator(new inner_typeless_iterator(begin())); }
      typeless_iterator typeless_end(void) override { return typeless_iterator(new inner_typeless_iterator(end())); }
      const Ann &at_index(size_t index) const override { return _items[index]; }
      Ann &at_index(size_t index) override { return _items[index]; }
      size_t index_of(const Ann &obj) const override { return &static_cast<const T &>(obj) - &front(); }
      size_t nelem(void) const override { return _items.size(); }
    };


    template <typename T>
    class BlockStore : public IStore {
    public:
      static_assert(std::is_base_of<Ann, T>::value, "T must be a subclass of Ann");
      using container_type = containers::BlockVector<T>;
      using block = typename container_type::Block;
      using const_pointer = typename container_type::const_pointer;
      using const_reference = typename container_type::const_reference;
      using iterator = typename container_type::iterator;
      using pointer = typename container_type::pointer;
      using reference = typename container_type::reference;
      using size_type = typename container_type::size_type;
      using value_type = typename container_type::value_type;

      class inner_typeless_iterator : public IStore::inner_typeless_iterator {
      private:
        typename BlockStore<T>::iterator _it;

      public:
        inner_typeless_iterator(typename BlockStore<T>::iterator it) : IStore::inner_typeless_iterator(), _it(it) { }
        virtual ~inner_typeless_iterator(void) { }
        virtual bool operator ==(const IStore::inner_typeless_iterator &o) const override { return _it == static_cast<const inner_typeless_iterator &>(o)._it; }
        virtual bool operator !=(const IStore::inner_typeless_iterator &o) const override { return _it != static_cast<const inner_typeless_iterator &>(o)._it; }
        virtual reference operator *(void) override { return _it.operator*(); }
        virtual pointer operator ->(void) override { return _it.operator->(); }
        virtual IStore::inner_typeless_iterator &operator ++(void) { ++_it; return *this; }
      };

    private:
      container_type _items;

    public:
      BlockStore(void) { }
      virtual ~BlockStore(void) { }

      // extra methods
      inline void
      create(const size_type n, const T x=T()) {
        block &b = _items.last_block();
        assert(b.capacity() - b.size() >= n);
        for (size_type i = 0; i != n; ++i)
          _items.push_back(x);
      }

      // iterators
      inline iterator begin(void) { return _items.begin(); }
      inline iterator end(void) { return _items.end(); }

      // capacity
      inline size_type capacity(void) const { return _items.capacity(); }
      inline size_type nblocks(void) const { return _items.nblocks(); }
      inline size_type size(void) const { return _items.size(); }
      inline bool empty(void) const { return _items.empty(); }
      inline bool full(void) const { return _items.full(); }

      // element access
      inline reference operator [](const size_type n) { return _items[n]; }
      inline reference back(void) { return _items.back(); }
      inline reference front(void) { return _items.front(); }
      inline const_reference operator [](const size_type n) const { return _items[n]; }
      inline const_reference at(const size_type n) const { return _items.at(n); }
      inline const_reference back(void) const { return _items.back(); }
      inline const_reference front(void) const { return _items.front(); }

      // modifiers
      inline block &reserve(const size_t nelem) { return _items.reserve(nelem); }

      // IStore
      typeless_iterator typeless_begin(void) override { return typeless_iterator(new inner_typeless_iterator(begin())); }
      typeless_iterator typeless_end(void) override { return typeless_iterator(new inner_typeless_iterator(end())); }
      const Ann &at_index(size_t index) const override { return _items[index]; }
      Ann &at_index(size_t index) override { return _items[index]; }
      size_t index_of(const Ann &obj) const override { return _items.index_of(static_cast<const T &>(obj)); }
      size_t nelem(void) const override { return _items.size(); }
    };


    // ========================================================================
    // FieldTraits
    // ========================================================================
    template <typename T>
    struct FieldTraits {
      static constexpr bool is_dr_ptr_type = false;
      static constexpr bool is_pod_ptr = std::is_pointer<T>::value;
      static constexpr bool is_slice = false;
      static constexpr bool is_collection = false;
    };

    template <typename T>
    struct FieldTraits<Pointer<T>> {
      static_assert(std::is_base_of<Ann, T>::value, "T must be a subclass of Ann");
      using value_type = T;
      static constexpr bool is_dr_ptr_type = true;
      static constexpr bool is_slice = false;
      static constexpr bool is_collection = false;
    };

    template <typename T>
    struct FieldTraits<Pointers<T>> {
      static_assert(std::is_base_of<Ann, T>::value, "T must be a subclass of Ann");
      using value_type = T;
      static constexpr bool is_dr_ptr_type = true;
      static constexpr bool is_slice = false;
      static constexpr bool is_collection = true;
    };

    template <typename T, bool>
    struct SliceFieldTraits {
      static constexpr bool is_dr_ptr_type = false;
      static constexpr bool is_pod_ptr = false;
      static constexpr bool is_collection = false;
    };

    template <typename T>
    struct SliceFieldTraits<T, true> {
      using value_type = typename std::remove_pointer<T>::type;
      static constexpr bool is_dr_ptr_type = true;
      static constexpr bool is_collection = false;
    };

    template <typename T>
    struct FieldTraits<Slice<T>> : public SliceFieldTraits<T, std::is_pointer<T>::value> {
      static constexpr bool is_slice = true;
      static constexpr bool is_collection = false;
    };

  }
}

#endif  // SCHWA_DR_FIELDS_H_
