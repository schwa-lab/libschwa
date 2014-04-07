/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_POOL_H_
#define SCHWA_POOL_H_

#include <cstdlib>
#include <new>
#include <vector>

#include <schwa/_base.h>

namespace schwa {

  /**
   * Objects allocated within a Pool WILL NOT have their destructors called when
   * the Pool is destroyed. As such, it is only safe to allocate within the Pool
   * objects that don't need destructors.
   **/
  class Pool {
  private:
    class Block {
    private:
      const size_t _size;
      size_t _upto;

      explicit Block(size_t size) : _size(size), _upto(0) { }

    public:
      inline char *data(void) { return reinterpret_cast<char *>(this + 1); }
      inline size_t size(void) const { return _size; }
      inline size_t upto(void) const { return _upto; }

      inline void *
      alloc(size_t size) {
        size_t upto = _upto + size;
        if (upto > _size)
          return nullptr;
        void *ptr = data() + _upto;
        _upto = upto;
        return ptr;
      }

      inline void operator delete(void *ptr) {
        std::free(ptr);
      }

      static Block *
      create(size_t size) {
        void *ptr = std::malloc(sizeof(Block) + size);
        if (ptr == nullptr)
          throw std::bad_alloc();
        return new (ptr) Block(size);
      }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Block);
    };

    const size_t _block_size;
    Block *_current;
    std::vector<Block *> _blocks;

  public:
    explicit Pool(size_t block_size) : _block_size(block_size), _current(Block::create(block_size)) {
      _blocks.push_back(_current);
    }

    ~Pool(void) {
      for (Block *b : _blocks)
        delete b;
    }

    template <typename T=void *>
    inline T
    alloc(size_t size) {
      void *ptr = _current->alloc(size);
      if (ptr == nullptr) {
        _current = Block::create(size > _block_size ? size : _block_size);
        _blocks.push_back(_current);
        ptr = _current->alloc(size);
      }
      return static_cast<T>(ptr);
    }

    inline size_t nblocks(void) const { return _blocks.size(); }
    inline size_t allocd(void) const {
      size_t count = 0;
      for (Block *b : _blocks)
        count += b->size();
      return count;
    }
    inline size_t used(void) const {
      size_t count = 0;
      for (Block *b : _blocks)
        count += b->upto();
      return count;
    }

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
    using difference_type = std::ptrdiff_t;
    using pointer = T *;
    using reference = T &;
    using size_type = std::size_t;
    using value_type = T;
    template <class U> struct rebind { using other = PoolAllocator<U>; };

    Pool &pool;

    explicit PoolAllocator(schwa::Pool &pool) noexcept : pool(pool) { }
    template <typename U> PoolAllocator(const PoolAllocator<U> &o) : pool(o.pool) { }

    inline T *
    allocate(const std::size_t n) {
      return pool.alloc<T *>(n * sizeof(T));
    }

    template <class U, class... Args>
    inline void
    construct (U* p, Args&&... args) {
      new (p) U(args...);
    }

    inline void
    deallocate(T *, std::size_t) { }

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
operator new(std::size_t nbytes, ::schwa::Pool &pool) {
  return pool.alloc(nbytes);
}


#endif  // SCHWA_POOL_H_
