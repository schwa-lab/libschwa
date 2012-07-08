/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace io {

    class WriteBuffer {
    public:
      static constexpr size_t DEFAULT_BLOCK_SIZE = 64;
      static constexpr size_t DEFAULT_POOL_SIZE = 4096;
      static constexpr size_t DEFAULT_NBLOCKS = 8;
      static constexpr size_t DEFAULT_NPOOLS = 8;

      struct Block {
        const size_t capacity;
        char *const data;
        size_t size;

        Block(size_t capacity, char *data, size_t size=0) : capacity(capacity), data(data), size(size) { }
      };

    private:
      const size_t _pool_size;
      char **_pools;
      size_t _npools;
      size_t _current_pool;
      size_t _pool_used;

      const size_t _block_size;
      Block *_blocks;
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
          _blocks = static_cast<Block *>(std::realloc(_blocks, _nblocks * sizeof(Block)));
          assert(_blocks != nullptr);
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

      inline void
      require_pool(const size_t nbytes) {
        if (_pool_used + nbytes > _pool_size) {
          next_pool();

          char *p = static_cast<char *>(std::malloc(std::max(_pool_size, nbytes)));
          assert(p != nullptr);
          _pools[_current_pool] = p;
          _pool_used = 0;
        }
      }

    public:
      explicit WriteBuffer(size_t block_size=DEFAULT_BLOCK_SIZE, size_t nblocks=DEFAULT_NBLOCKS, size_t pool_size=DEFAULT_POOL_SIZE, size_t npools=DEFAULT_NPOOLS) :
          _pool_size(pool_size),
          _pools(static_cast<char **>(std::malloc(npools * sizeof(char *)))),
          _npools(npools),
          _current_pool(0),
          _pool_used(0),
          _block_size(block_size),
          _blocks(static_cast<Block *>(std::malloc(nblocks * sizeof(Block)))),
          _current_block(0),
          _nblocks(nblocks),
          _size(0) {
        assert(_pools != nullptr);
        assert(_blocks != nullptr);

        char *p = static_cast<char *>(std::malloc(pool_size));
        assert(p != nullptr);
        _pools[_current_pool] = p;

        char *const write_buf = &(_pools[_current_pool][_pool_used]);
        new (_blocks + _current_block) Block(block_size, write_buf);
        _pool_used += _block_size;

        _lb_capacity = block_size;
        _lb_size = 0;
        _lb_upto = write_buf;
      }

      ~WriteBuffer(void) {
        for (size_t i = 0; i <= _current_pool; ++i)
          std::free(_pools[i]);
        std::free(_pools);
        std::free(_blocks);
      }

      inline size_t size(void) const { return _size; }

      void
      put(const char c) {
        if (_lb_size == _lb_capacity) {
          _blocks[_current_block].size = _lb_size;
          next_block();
          require_pool(_block_size);

          char *const write_buf = &(_pools[_current_pool][_pool_used]);
          new (_blocks + _current_block) Block(_block_size, write_buf);
          _pool_used += _block_size;

          _lb_capacity = _block_size;
          _lb_size = 0;
          _lb_upto = write_buf;
        }

        *_lb_upto = c;
        _lb_upto += 1;
        _lb_size += 1;
        _size += 1;
      }

      WriteBuffer &
      write(const char *const data, const size_t nbytes) {
        if (_lb_size + nbytes > _lb_capacity) {
          _blocks[_current_block].size = _lb_size;
          next_block();
          require_pool(nbytes);

          char *const write_buf = &(_pools[_current_pool][_pool_used]);
          new (_blocks + _current_block) Block(nbytes, write_buf);
          _pool_used += nbytes;

          _lb_capacity = nbytes;
          _lb_size = 0;
          _lb_upto = write_buf;
        }

        std::memcpy(_lb_upto, data, nbytes);
        _lb_upto += nbytes;
        _lb_size += nbytes;
        _size += nbytes;

        return *this;
      }

      WriteBuffer &
      write_zerocopy(const char *const data, const size_t nbytes) {
        std::cout << "write_zerocopy" << std::endl;
        _blocks[_current_block].size = _lb_size;
        next_block();

        char *const write_buf = const_cast<char *>(data);
        new (_blocks + _current_block) Block(nbytes, write_buf, nbytes);

        _lb_capacity = nbytes;
        _lb_size = nbytes;
        _lb_upto = write_buf + nbytes;
        _size += nbytes;

        return *this;
      }

      void
      debug(std::ostream &out) const {
        _blocks[_current_block].size = _lb_size;
        out << "npools=" << _npools << " current=" << _current_pool << std::endl;
        out << "nblocks=" << _nblocks << " current=" << _current_block << std::endl;
        for (size_t i = 0; i <= _current_block; ++i) {
          out << i << "] size=" << _blocks[i].size << " cap=" << _blocks[i].capacity << " data=";
          out << std::hex;
          for (size_t s = 0; s != _blocks[i].size; ++s)
            out << static_cast<unsigned int>(_blocks[i].data[s]) << " ";
          out << std::dec << std::endl;
        }
      }
    };

  }
}
