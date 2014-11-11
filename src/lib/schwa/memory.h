/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MEMORY_H_
#define SCHWA_MEMORY_H_

#include <cstdlib>
#include <memory>
#include <new>

#include <schwa/_base.h>
#include <schwa/pool.h>


namespace schwa {

  /**
   * A std::allocator conforming class which is backed by a system-dependant memory-aligning
   * memory allocation function. On POSIX systems, this is ::posix_memalign.
   **/
  template <typename T>
  class AlignedAllocator {
  public:
    static constexpr const size_t DEFAULT_ALIGNMENT = 32;

    using const_pointer = const T *;
    using const_reference = const T &;
    using difference_type = ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using size_type = size_t;
    using value_type = T;

    template <typename U> struct rebind { using other = AlignedAllocator<U>; };

  private:
    const size_t _alignment;

    template <typename U> friend class AlignedAllocator;

  public:
    AlignedAllocator(size_t alignment=DEFAULT_ALIGNMENT) noexcept : _alignment(alignment) { }
    template <typename U> AlignedAllocator(const AlignedAllocator<U> &o) : _alignment(o._alignment) { }

    pointer address(reference x) const noexcept { return &x; }
    const_pointer address(const_reference x) const noexcept { return &x; }
    inline size_t alignment(void) const { return _alignment; }

    inline pointer
    allocate(size_type n) {
      void *ptr;
      const int ret = ::posix_memalign(&ptr, _alignment, n*sizeof(T));  // FIXME this needs to be dependant on HAVE_POSIX_MEMALIGN.
      if (SCHWA_UNLIKELY(ret != 0))
        throw std::bad_alloc();
      return reinterpret_cast<pointer>(ptr);
    }

    template <typename U, typename... Args>
    inline void
    construct (U* ptr, Args&&... args) {
      new (ptr) U(args...);
    }

    inline void
    deallocate(pointer ptr, size_type) {
      std::free(ptr);
    }

    template <typename U>
    inline void
    destroy (U* ptr) {
      ptr->~U();
    }
  };


  /**
   * A std::allocator conforming class for use with a Pool. The \p deallocate method does not free
   * up any resources since it is backed by a Pool.
   **/
  template <typename T>
  class PoolAllocator {
  public:
    using const_pointer = const T *;
    using const_reference = const T &;
    using difference_type = ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using size_type = size_t;
    using value_type = T;
    template <typename U> struct rebind { using other = PoolAllocator<U>; };

  private:
    Pool &_pool;

    template <typename U> friend class PoolAllocator;

  public:
    explicit PoolAllocator(schwa::Pool &pool) noexcept : _pool(pool) { }
    template <typename U> PoolAllocator(const PoolAllocator<U> &o) : _pool(o._pool) { }

    pointer address(reference x) const noexcept { return &x; }
    const_pointer address(const_reference x) const noexcept { return &x; }
    Pool &pool(void) const { return _pool; }

    inline pointer
    allocate(size_type n) {
      return _pool.alloc<T *>(n*sizeof(T));
    }

    template <typename U, typename... Args>
    inline void
    construct (U *p, Args&&... args) {
      new (p) U(args...);
    }

    inline void
    deallocate(pointer, size_type) {
      // Do nothing.
    }

    template <typename U>
    inline void
    destroy (U *p) {
      p->~U();
    }
  };


  template <typename T>
  class AlignedPoolAllocator {
  public:
    static constexpr const size_t DEFAULT_ALIGNMENT = 32;

    using const_pointer = const T *;
    using const_reference = const T &;
    using difference_type = ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using size_type = size_t;
    using value_type = T;
    template <typename U> struct rebind { using other = AlignedPoolAllocator<U>; };

  private:
    Pool &_pool;
    const size_t _default_alignment;

    template <typename U> friend class AlignedPoolAllocator;

  public:
    explicit AlignedPoolAllocator(schwa::Pool &pool, size_t default_alignment=DEFAULT_ALIGNMENT) noexcept : _pool(pool), _default_alignment(default_alignment) { }
    template <typename U> AlignedPoolAllocator(const AlignedPoolAllocator<U> &o) : _pool(o._pool), _default_alignment(o._default_alignment) { }

    pointer address(reference x) const noexcept { return &x; }
    const_pointer address(const_reference x) const noexcept { return &x; }
    Pool &pool(void) const { return _pool; }

    inline pointer
    allocate(const size_type nitems) {
      return _pool.aligned_alloc<T *>(nitems*sizeof(T), _default_alignment);
    }

    inline pointer
    allocate(const size_type nitems, const size_t alignment) {
      return _pool.aligned_alloc<T *>(nitems*sizeof(T), alignment);
    }

    template <typename U, typename... Args>
    inline void
    construct (U *p, Args&&... args) {
      new (p) U(args...);
    }

    inline void
    deallocate(pointer, size_type) {
      // Do nothing.
    }

    template <typename U>
    inline void
    destroy (U *p) {
      p->~U();
    }
  };

}  // namespace schwa

#endif  // SCHWA_MEMORY_H_
