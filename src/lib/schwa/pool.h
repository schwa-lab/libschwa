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
      const size_t _nbytes_allocd;
      size_t _nbytes_used;

      explicit Block(size_t nbytes_allocd) : _nbytes_allocd(nbytes_allocd), _nbytes_used(0) { }

      inline char *
      data(void) {
        return reinterpret_cast<char *>(this + 1);
      }

    public:
      inline void *
      alloc(const size_t nbytes) {
        const size_t upto = _nbytes_used + nbytes;
        if (upto > _nbytes_allocd)
          return nullptr;
        void *ptr = data() + _nbytes_used;
        _nbytes_used = upto;
        return ptr;
      }

      inline void
      drain(void) {
        _nbytes_used = 0;
      }

      inline size_t
      nbytes_allocd(void) const {
        return _nbytes_allocd;
      }

      inline size_t
      nbytes_used(void) const {
        return _nbytes_used;
      }

      inline void
      operator delete(void *ptr) {
        std::free(ptr);
      }

      static Block *
      create(const size_t nbytes) {
        void *ptr = std::malloc(sizeof(Block) + nbytes);
        if (SCHWA_UNLIKELY(ptr == nullptr))
          throw std::bad_alloc();
        return new (ptr) Block(nbytes);
      }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(Block);
    };

    const size_t _block_size;
    Block *_current;
    std::vector<Block *> _used_blocks;
    std::vector<Block *> _unused_blocks;

  public:
    explicit Pool(size_t block_size) : _block_size(block_size), _current(nullptr) { }
    ~Pool(void) {
      for (Block *b : _used_blocks)
        delete b;
      for (Block *b : _unused_blocks)
        delete b;
    }

    template <typename T=void *>
    inline T
    alloc(const size_t nbytes) {
      void *ptr = (_current == nullptr) ? nullptr : _current->alloc(nbytes);
      if (ptr == nullptr) {
        // Reuse an existing unused block or allocate a new block.
        _current = nullptr;
        for (auto it = _unused_blocks.begin(); it != _unused_blocks.end(); ++it) {
          if ((*it)->nbytes_allocd() >= nbytes) {
            _current = *it;
            _unused_blocks.erase(it);
            break;
          }
        }
        if (_current == nullptr)
          _current = Block::create(nbytes > _block_size ? nbytes : _block_size);
        _used_blocks.push_back(_current);

        ptr = _current->alloc(nbytes);
      }
      return static_cast<T>(ptr);
    }

    void
    drain(void) {
      for (Block *b : _used_blocks) {
        b->drain();
        _unused_blocks.push_back(b);
      }
      _used_blocks.clear();
      _current = nullptr;
    }

    inline size_t
    nblocks(void) const {
      return _used_blocks.size() + _unused_blocks.size();
    }

    inline size_t
    nblocks_unused(void) const {
      return _unused_blocks.size();
    }

    inline size_t
    nblocks_used(void) const {
      return _used_blocks.size();
    }

    size_t
    nbytes_allocd(void) const {
      size_t nbytes = 0;
      for (Block *b : _used_blocks)
        nbytes += b->nbytes_allocd();
      for (Block *b : _unused_blocks)
        nbytes += b->nbytes_allocd();
      return nbytes;
    }

    size_t
    nbytes_used(void) const {
      size_t nbytes = 0;
      for (Block *b : _used_blocks)
        nbytes += b->nbytes_used();
      return nbytes;
    }

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(Pool);
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
    construct (U* p, Args&&... args) {
      new (p) U(args...);
    }

    inline void
    deallocate(pointer, size_type) {
      // Do nothing.
    }

    template <typename U>
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
