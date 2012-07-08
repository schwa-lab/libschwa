/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace io {

    class WriteBuffer {
    public:
      static const size_t DEFAULT_BLOCK_SIZE;
      static const size_t DEFAULT_POOL_SIZE;
      static const size_t DEFAULT_NBLOCKS;
      static const size_t DEFAULT_NPOOLS;

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

      void next_block(void);
      void next_pool(void);
      void require_pool(const size_t nbytes);

    public:
      explicit WriteBuffer(size_t block_size=DEFAULT_BLOCK_SIZE, size_t nblocks=DEFAULT_NBLOCKS, size_t pool_size=DEFAULT_POOL_SIZE, size_t npools=DEFAULT_NPOOLS);
      ~WriteBuffer(void);

      inline size_t size(void) const { return _size; }

      void put(const char c);
      void write(const char *const data, const size_t nbytes);
      void write_zerocopy(const char *const data, const size_t nbytes);

      void copy_to(std::ostream &out);
      void copy_from(const WriteBuffer &o);

      // debugging
      inline size_t nblocks(void) const { return _nblocks; }
      inline size_t block_size(void) const { return _block_size; }
      inline size_t block_upto(void) const { return _current_block; }
      inline size_t npools(void) const { return _npools; }
      inline size_t pool_size(void) const { return _pool_size; }
      inline size_t pool_upto(void) const { return _current_pool; }

      std::ostream &dump(std::ostream &out) const;
    };

  }
}
