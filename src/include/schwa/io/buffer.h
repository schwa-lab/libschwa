/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace io {

    class WriteBuffer {
    public:
      static constexpr size_t DEFAULT_BLOCK_SIZE = 64;
      static constexpr size_t DEFAULT_POOL_SIZE = 4096;
      static constexpr size_t DEFAULT_NBLOCKS = 8;

      struct BlockInfo {
        const size_t capacity;
        size_t size;

        BlockInfo(size_t capacity, size_t size=0) : capacity(capacity), size(size) { }
      };

    private:
      const size_t _pool_size;
      char **_pools;
      size_t _npools;
      size_t _current_pool;
      size_t _pool_used;

      const size_t _block_size;
      char **_blocks;
      BlockInfo *_block_info;
      size_t _current_block;
      size_t _nblocks;

      size_t _size;

      size_t _lb_capacity;
      size_t _lb_size;
      char * _lb_upto;

      inline void
      next_block(void) {
        ++_current_block;
        if (_current_block == _nblocks) {
          _nblocks *= 2;
          _blocks = static_cast<char **>(std::realloc(_blocks, _nblocks * sizeof(char *)));
          assert(_blocks != nullptr);
          _block_info = static_cast<BlockInfo *>(std::realloc(_block_info, _nblocks * sizeof(BlockInfo)));
          assert(_block_info != nullptr);
        }
      }

      inline void
      next_pool(void) {
        ++_current_pool;
        if (_current_pool == _npools) {
          _npools *= 2;
          _pools = static_cast<char **>(std::realloc(_pools, _npools * sizeof(char *)));
          assert(_pools != nullptr);
        }
      }

    public:
      explicit WriteBuffer(size_t block_size=DEFAULT_BLOCK_SIZE, size_t nblocks=DEFAULT_NBLOCKS, size_t pool_size=DEFAULT_POOL_SIZE) :
          _pool_size(pool_size),
          _pools(static_cast<char **>(std::malloc(nblocks * sizeof(char *)))),
          _npools(nblocks),
          _current_pool(0),
          _pool_used(0),
          _block_size(block_size),
          _blocks(static_cast<char **>(std::malloc(nblocks * sizeof(char *)))),
          _block_info(static_cast<BlockInfo *>(std::malloc(nblocks * sizeof(BlockInfo)))),
          _current_block(0),
          _nblocks(nblocks),
          _size(0) {
        assert(_pools != nullptr);
        assert(_blocks != nullptr);
        assert(_block_info != nullptr);

        char *p = static_cast<char *>(std::malloc(pool_size));
        assert(p != nullptr);
        _pools[_current_pool] = p;

        _blocks[_current_block] = &(_pools[_current_pool][_pool_used]);
        _pool_used += _block_size;
        new (_block_info + _current_block) BlockInfo(block_size);
        _lb_capacity = block_size;
        _lb_size = 0;
        _lb_upto = _blocks[_current_block];
      }

      ~WriteBuffer(void) {
        for (size_t i = 0; i <= _current_pool; ++i)
          std::free(_pools[i]);
        std::free(_pools);
        std::free(_blocks);
        std::free(_block_info);
      }

      inline size_t size(void) const { return _size; }

      void
      put(const char c) {
        if (_lb_size == _lb_capacity) {
          _block_info[_current_block].size = _lb_size;
          next_block();

          if (_pool_used == _pool_size) {
            next_pool();

            char *p = static_cast<char *>(std::malloc(_pool_size));
            assert(p != nullptr);
            _pools[_current_pool] = p;
            _pool_used = 0;
          }

          _blocks[_current_block] = &(_pools[_current_pool][_pool_used]);
          _pool_used += _block_size;
          new (_block_info + _current_block) BlockInfo(_block_size);

          _lb_capacity = _block_size;
          _lb_size = 0;
          _lb_upto = _blocks[_current_block];
        }

        *_lb_upto = c;
        _lb_upto += 1;
        _lb_size += 1;
        _size += 1;
      }

      void
      write(const char *const data, const size_t nbytes) {
        _block_info[_current_block].size = _lb_size;
        next_block();

        _blocks[_current_block] = const_cast<char *>(data);
        new (_block_info + _current_block) BlockInfo(nbytes, nbytes);

        _lb_capacity = nbytes;
        _lb_size = nbytes;
        _lb_upto = const_cast<char *>(data);
        _size += nbytes;
      }
    };

  }
}
