/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace hashtable {

    template <class E, class K, unsigned long NBUCKETS, unsigned long SPOOL>
    class Base {
    public:
      typedef E Entry;
      typedef K Key;

      const std::string name;
      size_t size;

    protected:
      Pool *const pool_;
      const bool shared_;

      const static unsigned long NBUCKETS_ = NBUCKETS;
      const static unsigned long SPOOL_ = SPOOL;

      Entry *buckets_[NBUCKETS];

    public:
      Base(const std::string &name, Pool *pool=0) : name(name), size(0), pool_(pool ? pool : new Pool(SPOOL)), shared_(pool != 0) {
        std::memset(buckets_, 0, sizeof(buckets_));
      }

      virtual ~Base(void) {
        if (!shared_)
          delete pool_;
      }

      Pool *pool(void) { return pool_; }

      virtual Entry *
      insert(Key key, const Hash hash, const unsigned long bucket) {
        Entry *entry = Entry::create(pool_, key, size, hash, buckets_[bucket]);
        buckets_[bucket] = entry;
        ++size;
        return entry;
      }

      virtual void
      clear(void) {
        if (size) {
          size = 0;
          std::memset(buckets_, 0, sizeof(buckets_));
          pool_->clear();
        }
      }

      Entry *
      insert(Key key) {
        const Hash hash(key);
        return insert(key, hash, hash % NBUCKETS);
      }

      Entry *
      add(Key key) {
        const Hash hash(key);
        unsigned long bucket = hash % NBUCKETS;
        Entry *entry = buckets_[bucket]->find(key, hash);
        if (entry)
          return entry;
        return insert(key, hash, bucket);
      }

      Entry *
      find(Key key) const {
        const Hash hash(key);
        return buckets_[hash % NBUCKETS]->find(key, hash);
      }

      Entry *
      find(const char c) const {
        return buckets_[Hash(c) % NBUCKETS]->find(c);
      }

      void
      printstats(std::ostream &os) const {
        size_t maxchain = 0;
        size_t nbins = 0;
        size_t nbytes = 0;

        for (unsigned long i = 0; i < NBUCKETS; i++) {
          if (buckets_[i]) {
            unsigned long temp = buckets_[i]->nchained();
            if (maxchain < temp)
              maxchain = temp;
            nbins++;
          }
        }

        os << "number of entries " << size << '\n';
        os << "number of bins used " << nbins << " (of " << NBUCKETS << ")\n";
        os << "used bins/nbins " << nbins/static_cast<float>(NBUCKETS) << '\n';
        os << "maximum chain length " << maxchain << '\n';
        os << "average chain length " << size/static_cast<float>(nbins) << '\n';

        nbytes = size * sizeof(Entry);
        os << "      entry objs " << nbytes << " bytes\n";
        nbytes += sizeof(buckets_);
        os << "      bin []     " << sizeof(buckets_) << " bytes\n";
        os << "total            " << nbytes << " bytes\n";
      }
    };

  }
}
