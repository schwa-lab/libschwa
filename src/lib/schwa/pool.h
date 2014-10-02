/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_POOL_H_
#define SCHWA_POOL_H_

#include <cstdlib>
#include <new>
#include <vector>

#include <schwa/_base.h>


namespace schwa {

  /**
   * Objects allocated within a Pool WILL NOT have their destructors called when the Pool is
   * destroyed. As such, it is only safe to allocate within the Pool objects that don't need
   * destructors.
   *
   * The starting address of blocks will be aligned to the \p alignment argument passed in to the
   * constructor. If an alloc() request is larger than the block size, a single once-off block
   * will be allocd large enough for the alloc() request.
   **/
  class Pool {
  public:
    static constexpr const size_t DEFAULT_ALIGNMENT = 32;  //!< The default memory alignment used for blocks.

  private:
    const size_t _block_size;  //!< The block size to alloc.
    const size_t _alignment; //!< The starting address of all allocd blocks will have this alignment.
    size_t _allocd;  //!< Number of bytes allocd in the current block.
    size_t _used;  //!< Number of bytes used in the current block.
    std::vector<void *> _blocks;  //<! The allocd blocks. The active block is the back-most element.

    inline void
    _alloc_new_block(const size_t nbytes) {
      void *ptr;
      const int ret = ::posix_memalign(&ptr, _alignment, nbytes);
      if (SCHWA_UNLIKELY(ret != 0))
        throw std::bad_alloc();
      _allocd = nbytes;
      _used = 0;
      _blocks.push_back(ptr);
    }

  public:
    explicit Pool(size_t block_size, size_t alignment=DEFAULT_ALIGNMENT) : _block_size(block_size), _alignment(alignment) {
      _alloc_new_block(_block_size);
    }
    ~Pool(void) {
      for (void *ptr : _blocks)
        std::free(ptr);
    }

    template <typename T=void *>
    inline T
    alloc(const size_t nbytes) {
      // Do we have room in the current block for the allocation request?
      if (_used + nbytes > _allocd)
        _alloc_new_block(nbytes > _block_size ? nbytes : _block_size);
      void *const ptr = reinterpret_cast<uint8_t *>(_blocks.back()) + _used;
      _used += nbytes;
      return static_cast<T>(ptr);
    }

    inline size_t nblocks(void) const { return _blocks.size(); }

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(Pool);
  };


  /**
   * A std::allocator conforming class for use with a Pool. The deallocate
   * method does not free up any resources.
   **/
  template <class T>
  struct PoolAllocator {
    using const_pointer = const T *;
    using const_reference = const T &;
    using difference_type = ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using size_type = size_t;
    using value_type = T;
    template <class U> struct rebind { using other = PoolAllocator<U>; };

    Pool &pool;

    explicit PoolAllocator(schwa::Pool &pool) noexcept : pool(pool) { }
    template <typename U> PoolAllocator(const PoolAllocator<U> &o) : pool(o.pool) { }

    inline T *
    allocate(const size_t n) {
      return pool.alloc<T *>(n * sizeof(T));
    }

    template <class U, class... Args>
    inline void
    construct (U* p, Args&&... args) {
      new (p) U(args...);
    }

    inline void
    deallocate(T *, size_t) { }

    template <class U>
    inline void
    destroy (U* p) {
      p->~U();
    }
  };

}  // namespace schwa


/**
 * std::new operator for use with a Pool.
 **/
inline void *
operator new(size_t nbytes, ::schwa::Pool &pool) {
  return pool.alloc(nbytes);
}


#endif  // SCHWA_POOL_H_
