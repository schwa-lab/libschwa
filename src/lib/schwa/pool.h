/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_POOL_H_
#define SCHWA_POOL_H_

#include <cstdlib>
#include <iostream>
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

    public:
      inline void *
      aligned_alloc(size_t nbytes, const size_t alignment) {
        size_t ptr = reinterpret_cast<size_t>(data() + _nbytes_used);
        if (ptr % alignment != 0) {
          nbytes += alignment - (ptr % alignment);
          ptr += alignment - (ptr % alignment);
        }
        const size_t upto = _nbytes_used + nbytes;
        if (upto > _nbytes_allocd)
          return nullptr;
        _nbytes_used = upto;
        return reinterpret_cast<void *>(ptr);
      }

      inline void *
      alloc(const size_t nbytes) {
        const size_t upto = _nbytes_used + nbytes;
        if (upto > _nbytes_allocd)
          return nullptr;
        void *ptr = data() + _nbytes_used;
        _nbytes_used = upto;
        return ptr;
      }

      inline bool
      can_aligned_alloc(size_t nbytes, const size_t alignment) const {
        const size_t ptr = reinterpret_cast<size_t>(data());
        if (ptr % alignment != 0)
          nbytes += alignment - (ptr % alignment);
        return _nbytes_used + nbytes <= _nbytes_allocd;
      }

      inline bool
      can_alloc(const size_t nbytes) const {
        return _nbytes_used + nbytes <= _nbytes_allocd;
      }

      inline char *
      data(void) {
        return reinterpret_cast<char *>(this) - _nbytes_allocd;
      }

      inline const char *
      data(void) const {
        return reinterpret_cast<const char *>(this) - _nbytes_allocd;
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

      static Block *
      create(const size_t nbytes) {
        void *ptr;
        const int ret = ::posix_memalign(&ptr, 32, nbytes + sizeof(Block));  // FIXME this needs to be dependant on HAVE_POSIX_MEMALIGN.
        if (SCHWA_UNLIKELY(ret != 0))
          throw std::bad_alloc();
        return new (reinterpret_cast<void *>(reinterpret_cast<char *>(ptr) + nbytes)) Block(nbytes);
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
        std::free(b->data());
      for (Block *b : _unused_blocks)
        std::free(b->data());
    }

    template <typename T=void *>
    inline T
    aligned_alloc(const size_t nbytes, const size_t alignment) {
      void *ptr = (_current == nullptr) ? nullptr : _current->aligned_alloc(nbytes, alignment);
      if (ptr == nullptr) {
        // Reuse an existing unused block or allocate a new block.
        _current = nullptr;
        for (auto it = _unused_blocks.begin(); it != _unused_blocks.end(); ++it) {
          if ((*it)->can_aligned_alloc(nbytes, alignment)) {
            _current = *it;
            _unused_blocks.erase(it);
            break;
          }
        }
        if (_current == nullptr)
          _current = Block::create(nbytes > _block_size ? nbytes : _block_size);
        _used_blocks.push_back(_current);

        ptr = _current->aligned_alloc(nbytes, alignment);
      }
      return static_cast<T>(ptr);
    }

    template <typename T=void *>
    inline T
    alloc(const size_t nbytes) {
      void *ptr = (_current == nullptr) ? nullptr : _current->alloc(nbytes);
      if (ptr == nullptr) {
        // Reuse an existing unused block or allocate a new block.
        _current = nullptr;
        for (auto it = _unused_blocks.begin(); it != _unused_blocks.end(); ++it) {
          if ((*it)->can_alloc(nbytes)) {
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

}  // namespace schwa


/**
 * std::new operator for use with a Pool.
 **/
inline void *
operator new(size_t nbytes, ::schwa::Pool &pool) {
  return pool.alloc(nbytes);
}

#endif  // SCHWA_POOL_H_
