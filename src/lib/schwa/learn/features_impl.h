/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_FEATURES_IMPL_H_
#define SCHWA_LEARN_FEATURES_IMPL_H_

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <new>
#include <stdexcept>
#include <sstream>
#include <string>
#include <type_traits>

#include <schwa/_base.h>
#include <schwa/hash.h>
#include <schwa/io/logging.h>
#include <schwa/pool.h>


namespace schwa {
  namespace learn {

    template <typename ENTRY, size_t INDEX_BITS=24>
    class FeatureHashtable {
    public:
      static_assert(std::is_default_constructible<ENTRY>::value, "ENTRY type must be default constructable.");
      static_assert(std::has_virtual_destructor<ENTRY>::value == false, "ENTRY type cannot be virtual.");
      static_assert(INDEX_BITS > 0, "INDEX_BITS must be non-zero");
      static_assert(INDEX_BITS <= 64, "INDEX_BITS cannot exceed 64 since the hash function is 64-bit.");

      using difference_type = ptrdiff_t;
      using mapped_type = ENTRY;
      using size_type = size_t;

      using reference = typename std::add_lvalue_reference<mapped_type>::type;
      using const_reference = typename std::add_lvalue_reference<typename std::add_const<mapped_type>::type>::type;
      using pointer = typename std::add_pointer<mapped_type>::type;
      using const_pointer = typename std::add_pointer<typename std::add_const<mapped_type>::type>::type;

      static constexpr const size_t TABLE_INDEX_BITS = INDEX_BITS;  //!< Number of bits from the hash to use as the index into the table.
      static constexpr const uint64_t TABLE_INDEX_MASK = (static_cast<uint64_t>(1) << TABLE_INDEX_BITS) - static_cast<uint64_t>(1);
      static constexpr const size_t TABLE_SIZE = static_cast<size_t>(1) << TABLE_INDEX_BITS;

      static constexpr const uint64_t TYPE_ID_HASH_MIXER = 982451653ULL;

    private:
      class SparseEntry {
      private:
        Label _label;
        pointer _entry;

      public:
        SparseEntry(Label label, Pool &pool) : _label(label), _entry(pool.alloc<pointer>(sizeof(mapped_type))) {
          new (_entry) mapped_type();
        }
        ~SparseEntry(void) {
          _entry->~mapped_type();
        }

        inline reference entry(void) const { return *_entry; }
        inline Label label(void) const { return _label; }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(SparseEntry);
      };


      class ChainItem {
      public:
        static constexpr const uint16_t BLOCK_ALLOC_ITEMS_SPARSE = 4;

      private:
        const uint64_t _hash;
        const uint8_t _ft_id;
        const bool _is_dense;
        uint16_t _nallocd;
        uint16_t _nused;
        union Entries {
          pointer dense;
          SparseEntry *sparse;
          void *typeless;
          Entries(void *ptr) : typeless(ptr) { }
        } _entries;

        SparseEntry &
        _create(const Label label, const uint16_t index, Pool &pool) {
          // Allocate more space if needed, doubling the number of entries allocated each time.
          if (SCHWA_UNLIKELY(_nused == _nallocd)) {
            void *const data = std::malloc(2*_nallocd*sizeof(SparseEntry));
            if (SCHWA_UNLIKELY(data == nullptr))
              throw std::bad_alloc();
            std::memcpy(data, _entries.typeless, _nallocd*sizeof(SparseEntry));
            std::free(_entries.typeless);
            _entries.typeless = data;
            _nallocd *= 2;
          }

          // Move objects down if needed.
          if (index != _nused)
            std::memmove(_entries.sparse + index + 1, _entries.sparse + index, (_nused - index)*sizeof(SparseEntry));

          // Construct and return the object.
          ++_nused;
          new (_entries.sparse + index) SparseEntry(label, pool);
          return _entries.sparse[index];
        }

      public:
        ChainItem(const uint64_t hash, const uint8_t ft_id, const bool is_dense, const Label nlabels, Pool &pool) :
            _hash(hash),
            _ft_id(ft_id),
            _is_dense(is_dense),
            _nallocd(is_dense ? nlabels : BLOCK_ALLOC_ITEMS_SPARSE),
            _nused(is_dense ? nlabels : 0),
            _entries(is_dense ? pool.alloc<pointer>(_nallocd*sizeof(mapped_type)) : std::malloc(_nallocd*sizeof(SparseEntry)))
          {
          if (SCHWA_UNLIKELY(_entries.typeless == nullptr))
            throw std::bad_alloc();
          // Construct dense objects.
          if (_is_dense) {
            for (uint16_t i = 0; i != _nused; ++i)
              new (_entries.dense + i) mapped_type();
          }
        }

        ~ChainItem(void) {
          // Call the destructors.
          for (uint16_t i = 0; i != _nused; ++i) {
            if (_is_dense)
              _entries.dense[i].~mapped_type();
            else
              _entries.sparse[i].~SparseEntry();
          }
          // Don't free up dense objects as they were allocated in the pool.
          if (!_is_dense)
            std::free(_entries.typeless);
        }

        // Capacity
        inline uint16_t allocd(void) const { return _nallocd; }
        inline bool empty(void) const { return _nused == 0; }
        inline uint8_t ft_id(void) const { return _ft_id; }
        inline uint64_t hash(void) const { return _hash; }
        inline bool is_dense(void) const { return _is_dense; }
        inline uint16_t size(void) const { return _nused; }

        // Element access
        inline reference operator [](const uint16_t index) { return _is_dense ? _entries.dense[index] : _entries.sparse[index].entry(); }
        inline const_reference operator [](const uint16_t index) const { return _is_dense ? _entries.dense[index] : _entries.sparse[index].entry(); }

        reference
        get(const Label label, Pool &pool, bool &created) {
          // Dense is preallocated. Simple array index.
          if (_is_dense)
            return _entries.dense[label];

          // Locate or create within the ordered array.
          uint16_t i;
          for (i = 0; i != _nused; ++i) {
            SparseEntry &e = _entries.sparse[i];
            if (label == e.label())
              return e.entry();
            else if (label > e.label())
              break;
          }
          created = true;
          return _create(label, i, pool).entry();
        }

        const_reference
        get(const Label label) const {
          // Dense is preallocated. Simple array index.
          if (_is_dense)
            return _entries.dense[label];

          // Locate within the ordered array.
          uint16_t i;
          for (i = 0; i != _nused; ++i) {
            const SparseEntry &e = _entries.sparse[i];
            if (label == e.label())
              return e.entry();
            else if (label > e.label())
              break;
          }
          throw std::out_of_range("SparseEntry with that label value does not exist");
        }

        // Element lookup
        inline ssize_t
        index(const Label label) const {
          // Dense is preallocated. Simple array index.
          if (_is_dense)
            return label;

          // Locate within the ordered array.
          for (uint16_t i = 0; i != _nused; ++i) {
            SparseEntry &e = _entries.sparse[i];
            if (label == e.label())
              return i;
            else if (label > e.label())
              break;
          }
          return -1;
        }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(ChainItem);
      };
      static_assert(sizeof(ChainItem) == 24, "Unexpected sizeof(ChainItem)");


      class Chain {
      public:
        static constexpr const uint32_t BLOCK_ALLOC_ITEMS = 4096/sizeof(ChainItem);

      private:
        uint32_t _nallocd;
        uint32_t _nused;
        ChainItem *_items;

        ChainItem &
        _create(const uint64_t hash, const uint8_t ft_id, const bool is_dense, const Label nlabels, Pool &pool) {
          // Allocate more space if needed.
          if (SCHWA_UNLIKELY(_nused == _nallocd)) {
            void *const data = std::malloc((_nallocd + BLOCK_ALLOC_ITEMS)*sizeof(ChainItem));
            if (SCHWA_UNLIKELY(data == nullptr))
              throw std::bad_alloc();
            std::memcpy(data, _items, _nallocd*sizeof(ChainItem));
            std::free(_items);
            _items = reinterpret_cast<ChainItem *>(data);
            _nallocd += BLOCK_ALLOC_ITEMS;
          }

          // Construct and return the object.
          new (_items + _nused) ChainItem(hash, ft_id, is_dense, nlabels, pool);
          return _items[_nused++];
        }

      public:
        Chain(void) : _nallocd(0), _nused(0), _items(nullptr) { }

        ~Chain(void) {
          for (uint32_t i = 0; i != _nused; ++i)
            _items[i].~ChainItem();
          std::free(_items);
        }

        // Capacity
        inline uint32_t allocd(void) const { return _nallocd; }
        inline bool empty(void) const { return _nused == 0; }
        inline uint32_t size(void) const { return _nused; }

        // Element access
        inline ChainItem &operator [](const uint32_t index) { return _items[index]; }
        inline const ChainItem &operator [](const uint32_t index) const { return _items[index]; }

        inline ChainItem &
        get(const uint64_t hash, const uint8_t ft_id, const bool is_dense, const uint16_t nlabels, Pool &pool, bool &created) {
          for (uint32_t i = 0; i != _nused; ++i)
            if (_items[i].hash() == hash)
              return _items[i];
          created = true;
          return _create(hash, ft_id, is_dense, nlabels, pool);
        }

        inline const ChainItem &
        get(const uint64_t hash) const {
          for (uint32_t i = 0; i != _nused; ++i)
            if (_items[i].hash() == hash)
              return _items[i];
          throw std::out_of_range("ChainItem with that hash value does not exist");
        }

        // Element lookup
        inline ssize_t
        index(const uint64_t hash) const {
          for (uint32_t i = 0; i != _nused; ++i)
            if (_items[i].hash() == hash)
              return i;
          return -1;
        }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(Chain);
      };
      static_assert(sizeof(Chain) == 16, "Unexpected sizeof(Chain)");


      template <typename TABLE>
      class Iterator : public std::iterator<std::forward_iterator_tag, typename TABLE::mapped_type> {
      public:
        using super_type = std::iterator<std::forward_iterator_tag, typename TABLE::mapped_type>;
        using difference_type = typename super_type::difference_type;
        using iterator_category = typename super_type::iterator_category;
        using pointer = typename super_type::pointer;
        using reference = typename super_type::reference;
        using value_type = typename super_type::value_type;

      private:
        mutable TABLE *_ht;
        mutable size_t _table_index;
        mutable uint32_t _item_index;
        mutable uint16_t _entry_index;
        mutable bool _initialised;
        mutable bool _ended;

        void
        _increment(void) const {
          // Don't do anything if it's at the end.
          if (SCHWA_UNLIKELY(_ended))
            return;

          if (SCHWA_LIKELY(_initialised)) {
            // Do we still have entries left in the current ChainItem?
            ++_entry_index;
            if (_entry_index != _ht->_table[_table_index][_item_index].size())
              return;
            _entry_index = 0;

            // Do we still have items left in the current Chain?
            ++_item_index;
            if (_item_index != _ht->_table[_table_index].size())
              return;
            _item_index = 0;

            // Start at the next index in the table.
            ++_table_index;
          }
          else {
            // We're now initialised.
            _initialised = true;
          }

          // Find the next non-empty chain.
          for ( ; _table_index != TABLE_SIZE; ++_table_index)
            if (!_ht->_table[_table_index].empty())
              break;

          // If the table is empty, become and end iterator.
          if (_table_index == TABLE_SIZE) {
            _ht = nullptr;
            _table_index = 0;
            _ended = true;
          }
        }

      public:
        Iterator(void) : _ht(nullptr), _table_index(0), _item_index(0), _entry_index(0), _initialised(true), _ended(true) { }
        explicit Iterator(TABLE &ht) : _ht(&ht), _table_index(0), _item_index(0), _entry_index(0), _initialised(false), _ended(false) { }
        Iterator(TABLE &ht, size_t table_index, uint32_t item_index, uint16_t entry_index) : _ht(&ht), _table_index(table_index), _item_index(item_index), _entry_index(entry_index), _initialised(true), _ended(false) { }
        Iterator(const Iterator &o) : _ht(o._ht), _table_index(o._table_index), _item_index(o._item_index), _entry_index(o._entry_index), _initialised(o._initialised), _ended(o._ended) { }
        Iterator(const Iterator &&o) : _ht(o._ht), _table_index(o._table_index), _item_index(o._item_index), _entry_index(o._entry_index), _initialised(o._initialised), _ended(o._ended) { }
        ~Iterator(void) { }

        inline bool
        operator ==(const Iterator &o) const {
          if (SCHWA_UNLIKELY(!_initialised))
            _increment();
          if (SCHWA_UNLIKELY(!o._initialised))
            o._increment();
          return _ended == o._ended && _ht == o._ht && _table_index == o._table_index && _item_index == o._item_index && _entry_index == o._entry_index;
        }
        inline bool operator !=(const Iterator &o) const { return !(*this == o); }

        inline reference
        operator *(void) const {
          if (SCHWA_UNLIKELY(!_initialised))
            _increment();
          return _ht->_table[_table_index][_item_index][_entry_index];
        }
        inline pointer operator ->(void) const { return &(**this); }

        inline Iterator &
        operator ++(void) {
          if (SCHWA_UNLIKELY(!_initialised))
            _increment();
          _increment();
          return *this;
        }
        inline Iterator operator ++(int) { Iterator it(*this); ++(*this); return it; }

        Iterator &
        operator =(const Iterator &o) {
          std::memcpy(this, &o, sizeof(Iterator));
          return *this;
        }

        friend inline std::ostream &
        operator <<(std::ostream &out, const Iterator &it) {
          if (SCHWA_UNLIKELY(!it._initialised))
            it._increment();
          return out << "[Iterator::operator <<|" << &it << "] " << it._ht << " " << it._table_index << " " << it._item_index << " " << it._entry_index << " " << it._initialised << " " << it._ended << "]";
        }
      };

    public:
      using iterator = Iterator<FeatureHashtable>;
      using const_iterator = Iterator<const FeatureHashtable>;
      friend class Iterator<FeatureHashtable>;
      friend class Iterator<const FeatureHashtable>;

    private:
      /**
       * Hashes together the feature type identifier and the contextual predicate using xxhash's
       * state-based hasher to form a combined digest. Returns the resultant hash.
       **/
      template <typename CP, typename HASHER>
      inline uint64_t
      _hash(const FeatureType &type, const CP &cp, const HASHER &hasher) const {
        third_party::xxhash::XXH64_resetState(&_xxhash_state, TYPE_ID_HASH_MIXER*static_cast<uint64_t>(type.id()));
        hasher(cp, &_xxhash_state);
        return third_party::xxhash::XXH64_intermediateDigest(&_xxhash_state);
      }

      template <typename CP, typename HASHER>
      inline iterator
      _find(const FeatureType &type, const CP &cp, const Label label, const HASHER &hasher) {
        const uint64_t hash = _hash(type, cp, hasher);
        const size_t table_index = hash & TABLE_INDEX_MASK;
        const Chain &chain = _table[table_index];
        if (chain.empty())
          return end();
        const ssize_t item_index = chain.index(hash);
        if (item_index == -1)
          return end();
        const ssize_t entry_index = chain[item_index].index(label);
        if (entry_index == -1)
          return end();
        return iterator(*this, table_index, static_cast<uint32_t>(item_index), static_cast<uint16_t>(entry_index));
      }

      template <typename CP, typename HASHER>
      inline const_iterator
      _find(const FeatureType &type, const CP &cp, const Label label, const HASHER &hasher) const {
        const uint64_t hash = _hash(type, cp, hasher);
        const size_t table_index = hash & TABLE_INDEX_MASK;
        const Chain &chain = _table[table_index];
        if (chain.empty())
          return end();
        const ssize_t item_index = chain.index(hash);
        if (item_index == -1)
          return end();
        const ssize_t entry_index = chain[item_index].index(label);
        if (entry_index == -1)
          return end();
        return const_iterator(*this, table_index, static_cast<uint32_t>(item_index), static_cast<uint16_t>(entry_index));
      }

      template <typename CP, typename HASHER>
      inline reference
      _get(const FeatureType &type, const CP &cp, const Label label, const HASHER &hasher) {
        const uint64_t hash = _hash(type, cp, hasher);
        const size_t table_index = hash & TABLE_INDEX_MASK;
        const bool is_dense = type.storage() == FeatureStorage::DENSE;
        bool created = false;
        reference entry = _table[table_index].get(hash, type.id(), is_dense, _nlabels, _pool, created).get(label, _pool, created);
        if (created)
          ++_size;
        return entry;
      }

      template <typename CP, typename HASHER>
      inline const_reference
      _get(const FeatureType &type, const CP &cp, const Label label, const HASHER &hasher) const {
        const uint64_t hash = _hash(type, cp, hasher);
        const size_t table_index = hash & TABLE_INDEX_MASK;
        return _table[table_index].get(hash).get(label);
      }

    private:
      Pool _pool;
      Chain *const _table;
      size_type _size;
      mutable third_party::xxhash::XXH64_stateSpace_t _xxhash_state;  //<! State object to maintain partial xxhash state.
      const uint16_t _nlabels;

    public:
      explicit FeatureHashtable(uint16_t nlabels, size_t pool_block_size=4*4096) : _pool(pool_block_size), _table(new Chain[TABLE_SIZE]), _size(0), _nlabels(nlabels) { }

      ~FeatureHashtable(void) {
        delete [] _table;
      }

      // Capacity
      inline bool empty(void) const { return _size == 0; }
      inline size_type size(void) const { return _size; }

      // Iterators
      inline iterator begin(void) { return iterator(*this); }
      inline iterator end(void) { return iterator(); }
      inline const_iterator begin(void) const { return const_iterator(*this); }
      inline const_iterator end(void) const { return const_iterator(); }
      inline const_iterator cbegin(void) const { return const_iterator(*this); }
      inline const_iterator cend(void) const { return const_iterator(); }

      // Hash policy
      inline float load_factor(void) const noexcept { return _size / static_cast<float>(TABLE_SIZE); }
      inline uint16_t nlabels(void) const { return _nlabels; }

      // Element lookup
      template <typename CP, typename HASHER=schwa::Hasher64<CP>>
      inline iterator
      find(const FeatureType &type, const CP &contextual_predicate, const Label label, const HASHER &hasher=HASHER()) {
        static_assert(sizeof(typename HASHER::result_type) == 8, "64-bit hash function required");
        return _find(type, contextual_predicate, label, hasher);
      }

      template <typename CP, typename HASHER=schwa::Hasher64<CP>>
      inline const_iterator
      find(const FeatureType &type, const CP &contextual_predicate, const Label label, const HASHER &hasher=HASHER()) const {
        static_assert(sizeof(typename HASHER::result_type) == 8, "64-bit hash function required");
        return _find(type, contextual_predicate, label, hasher);
      }

      // Element access
      template <typename CP, typename HASHER=schwa::Hasher64<CP>>
      inline reference
      get(const FeatureType &type, const CP &contextual_predicate, const Label label, const HASHER &hasher=HASHER()) {
        static_assert(sizeof(typename HASHER::result_type) == 8, "64-bit hash function required");
        return _get(type, contextual_predicate, label, hasher);
      }

      template <typename CP, typename HASHER=schwa::Hasher64<CP>>
      inline const_reference
      get(const FeatureType &type, const CP &contextual_predicate, const Label label, const HASHER &hasher=HASHER()) const {
        static_assert(sizeof(typename HASHER::result_type) == 8, "64-bit hash function required");
        return _get(type, contextual_predicate, label, hasher);
      }

      template <typename CP, typename HASHER=schwa::Hasher64<CP>>
      inline reference
      operator ()(const FeatureType &type, const CP &contextual_predicate, const Label label, const HASHER &hasher=HASHER()) {
        static_assert(sizeof(typename HASHER::result_type) == 8, "64-bit hash function required");
        return _get(type, contextual_predicate, label, hasher);
      }

      template <typename CP, typename HASHER=schwa::Hasher64<CP>>
      inline const_reference
      operator ()(const FeatureType &type, const CP &contextual_predicate, const Label label, const HASHER &hasher=HASHER()) const {
        static_assert(sizeof(typename HASHER::result_type) == 8, "64-bit hash function required");
        return _get(type, contextual_predicate, label, hasher);
      }


      std::ostream &
      pprint(std::ostream &out) const {
        out << "<FeatureHashtable size=" << _size << ">\n";
        for (size_t table_index = 0; table_index != TABLE_SIZE; ++table_index) {
          const Chain &chain = _table[table_index];
          if (chain.empty())
            continue;
          out << table_index << " => (" << chain.size() << ")";
          for (uint32_t item_index = 0; item_index != chain.size(); ++item_index) {
            const ChainItem &item = chain[item_index];
            out << " [" << &item << " hash=0x" << std::hex << item.hash() << std::dec << " is_dense=" << item.is_dense() << " nused=" << item.size() << " nallocd=" << item.allocd() << " ft_id=" << static_cast<unsigned int>(item.ft_id()) << "]";
          }
          out << "\n";
        }
        return out << "</FeatureHashtable>" << std::endl;
    }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(FeatureHashtable);
    };


    template <class TRANSFORM, typename VALUE>
    inline void
    Features<TRANSFORM, VALUE>::dump_crfsuite(std::ostream &out) const {
      for (auto &pair : _values) {
        out << '\t';

        std::stringstream key;
        key << pair.first;
        dump_crfsuite_feature(key.str(), out);

        out << ":" << pair.second;
      }
    }


    template <class TRANSFORM>
    inline void
    Instance<TRANSFORM>::dump_crfsuite(std::ostream &out) const {
      out << klass;
      features.dump_crfsuite(out);
      out << std::endl;
    }

  }
}

#endif  // SCHWA_LEARN_FEATURES_IMPL_H_
