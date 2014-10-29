/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_MEMORY_H_
#define SCHWA_MEMORY_H_

#include <cstdlib>
#include <memory>
#include <new>

#include <schwa/_base.h>


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

}  // namespace schwa

#endif  // SCHWA_MEMORY_H_
