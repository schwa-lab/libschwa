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
      T start;
      T stop;

      Slice(void) { }
      Slice(const T &start, const T &stop) : start(start), stop(stop) { }
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
    class Store {
    public:
      static_assert(boost::is_base_of<Annotation, T>::value, "T must be a subclass of Annotation");
      typedef std::vector<T> container_type;
      typedef typename container_type::const_reference const_reference;
      typedef typename container_type::const_iterator const_iterator;
      typedef typename container_type::const_pointer const_pointer;
      typedef typename container_type::const_reverse_iterator const_reverse_iterator;
      typedef typename container_type::difference_type difference_type;
      typedef typename container_type::iterator iterator;
      typedef typename container_type::pointer pointer;
      typedef typename container_type::reference reference;
      typedef typename container_type::reverse_iterator reverse_iterator;
      typedef typename container_type::size_type size_type;
      typedef typename container_type::value_type value_type;

    protected:
      container_type _items;

    public:
      Store(void) { }
      Store(const size_type size) : _items(size) { }
      Store(const Store &&o) : _items(o._items) { }
      ~Store(void) { }

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
