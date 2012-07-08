/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace io {

    class WriteBuffer : public std::streambuf {
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

    protected:
      virtual int
      overflow(const int c=EOF) override {
        std::cout << "WriteBuffer::overflow(0x" << std::hex << c << std::dec << ")" << std::endl;
        if (c == EOF)
          return c;

        // create a new block
        _blocks[_current_block].size = pptr() - pbase();
        next_block();

        // move to the next pool if the current pool is full
        if (_pool_used == _pool_size) {
          next_pool();

          char *p = static_cast<char *>(std::malloc(_pool_size));
          assert(p != nullptr);
          _pools[_current_pool] = p;
          _pool_used = 0;
        }

        // initialise the new block with memory from a pool
        char *const write_buf = &(_pools[_current_pool][_pool_used]);
        new (_blocks + _current_block) Block(_block_size, write_buf);
        _pool_used += _block_size;

        // tell std::streambuf where the new write buffer is and write the overflow character
        setp(write_buf, write_buf + _block_size);
        *write_buf = static_cast<char>(c);
        pbump(1);

        return c;
      }

      virtual std::streamsize
      xsputn(const char *const data, const std::streamsize nbytes) override {
        std::cout << "WriteBuffer::xsputn(" << nbytes << ")" << std::endl;
        // create a new block
        _blocks[_current_block].size = pptr() - pbase();
        next_block();

        // initialise the new block with the provided data
        char *const write_buf = const_cast<char *>(data);
        new (_blocks + _current_block) Block(nbytes, write_buf, nbytes);

        // tell std::streambuf where the new write buffer is
        setp(write_buf, write_buf + nbytes);
        pbump(nbytes);

        return nbytes;
      }


    public:
      explicit WriteBuffer(size_t block_size=DEFAULT_BLOCK_SIZE, size_t nblocks=DEFAULT_NBLOCKS, size_t pool_size=DEFAULT_POOL_SIZE, size_t npools=DEFAULT_NPOOLS) :
          std::streambuf(),
          _pool_size(pool_size),
          _pools(static_cast<char **>(std::malloc(npools * sizeof(char *)))),
          _npools(npools),
          _current_pool(0),
          _pool_used(0),
          _block_size(block_size),
          _blocks(static_cast<Block *>(std::malloc(nblocks * sizeof(Block)))),
          _current_block(0),
          _nblocks(nblocks) {
        assert(_pools != nullptr);
        assert(_blocks != nullptr);

        char *p = static_cast<char *>(std::malloc(pool_size));
        assert(p != nullptr);
        _pools[_current_pool] = p;

        char *const write_buf = &(_pools[_current_pool][_pool_used]);
        new (_blocks + _current_block) Block(block_size, write_buf);
        _pool_used += _block_size;

        // tell std::streambuf where the write buffer is
        setp(write_buf, write_buf + _block_size);
      }

      virtual ~WriteBuffer(void) {
        for (size_t i = 0; i <= _current_pool; ++i)
          std::free(_pools[i]);
        std::free(_pools);
        std::free(_blocks);
      }

      size_t
      size(void) const {
        size_t size = 0;
        for (size_t i = 0; i <= _current_block; ++i)
          size += _blocks[i].size;
        return size;
      }
    };

  }
}
