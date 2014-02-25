/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_POOL_H_
#define SCHWA_POOL_H_

#include <cstdlib>
#include <new>
#include <vector>

#include <schwa/_base.h>

namespace schwa {

  // Objects allocated within a Pool WILL NOT have their destructors called when
  // the Pool is destroyed. As such, it is only safe to allocate within the Pool
  // objects that don't need destructors.
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

    inline void *
    alloc(size_t size) {
      void *ptr = _current->alloc(size);
      if (ptr == nullptr) {
        _current = Block::create(size > _block_size ? size : _block_size);
        _blocks.push_back(_current);
        ptr = _current->alloc(size);
      }
      return ptr;
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

}

#endif  // SCHWA_POOL_H_
