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
#include <schwa/msgpack.h>
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

      static constexpr const size_t DEFAULT_POOL_BLOCK_SIZE = 4 * 1024 * 1024;
      static constexpr const uint64_t TYPE_ID_HASH_MIXER = 982451653ULL;

    private:
      class SparseEntry {
      public:
        Label label;
        mapped_type entry;
        SparseEntry *next;

        SparseEntry(void) : label(0), next(nullptr) { }
        explicit SparseEntry(const Label label) : label(label), next(nullptr) { }
        ~SparseEntry(void) { }

        inline Label size(Label count=0) const { return next == nullptr ? count + 1 : next->size(count + 1); }

        inline void
        deserialise(std::istream &in) {
          const uint32_t nitems = msgpack::read_array_size(in);
          assert(nitems == 2); (void)nitems;
          label = msgpack::read_uint(in);
          entry.deserialise(in);
        }

        inline void
        serialise(std::ostream &out) const {
          msgpack::write_array_size(out, 2);
          msgpack::write_uint(out, label);
          entry.serialise(out);
        }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(SparseEntry);
      };


      class Chain {
      public:
        static constexpr const uint64_t HASH_MASK = ~static_cast<uint64_t>(1);
        static constexpr const uint64_t IS_DENSE_MASK = static_cast<uint64_t>(1);

      private:
        uint64_t _value;  //<! Top 63 bits are for the hash. The bottom bit is for `is_dense`.
        union Entries {
          pointer dense;
          SparseEntry *sparse;
          void *typeless;
          Entries(void *ptr) : typeless(ptr) { }
        } _entries;
        Chain *_next;

        SparseEntry &
        _create(const Label label, SparseEntry *const prev, Pool &pool) {
          // Construct the object.
          SparseEntry *const se = pool.alloc<SparseEntry *>(sizeof(SparseEntry));
          new (se) SparseEntry(label);

          // Insert in the appropriate place in the linked list to keep it ordered by label.
          if (prev == nullptr) {
            se->next = _entries.sparse;
            _entries.sparse = se;
          }
          else {
            se->next = prev->next;
            prev->next = se;
          }

          return *se;
        }

      public:
        Chain(void) : _value(0), _entries(nullptr), _next(nullptr) { }
        Chain(const uint64_t hash, const bool is_dense, const Label label, const Label nlabels, Pool &pool, size_t &size) :
            _value(Chain::build_value(hash, is_dense)),
            _entries(is_dense ? pool.alloc(nlabels*sizeof(mapped_type)) : pool.alloc(sizeof(SparseEntry))),
            _next(nullptr)
          {
          if (SCHWA_UNLIKELY(_entries.typeless == nullptr))
            throw std::bad_alloc();

          if (is_dense) {
            size += nlabels;
            for (Label l = 0; l != nlabels; ++l)
              new (_entries.dense + l) mapped_type();
          }
          else {
            size += 1;
            new (_entries.sparse) SparseEntry(label);
          }
        }
        ~Chain(void) { }

        inline pointer get_dense(void) const { return _entries.dense; }
        inline SparseEntry *get_sparse(void) const { return _entries.sparse; }
        inline void *get_typeless(void) const { return _entries.typeless; }
        inline bool is_dense(void) const { return _value & IS_DENSE_MASK; }
        inline uint64_t masked_hash(void) const { return _value & HASH_MASK; }
        inline Chain *next(void) const { return _next; }
        inline size_t size(size_t count=0) const { return _next == nullptr ? count + 1 : _next->size(count + 1); }
        inline uint64_t value(void) const { return _value; }

        inline void set_next(Chain *next) { _next = next; }

        reference
        get(const Label label, Pool &pool, bool &created) {
          // Dense is preallocated. Simple array index.
          if (is_dense())
            return _entries.dense[label];

          // Locate or create within the ordered linked list.
          SparseEntry *prev = nullptr;
          for (SparseEntry *se = _entries.sparse; se != nullptr; se = se->next) {
            if (label == se->label)
              return se->entry;
            else if (label > se->label)
              break;
            prev = se;
          }
          created = true;
          return _create(label, prev, pool).entry;
        }

        reference
        get(const Label label) {
          // Dense is preallocated. Simple array index.
          if (is_dense())
            return _entries.dense[label];

          // Locate within the ordered linked list.
          for (SparseEntry *se = _entries.sparse; se != nullptr; se = se->next) {
            if (label == se->label)
              return se->entry;
            else if (label > se->label)
              break;
          }
          throw std::out_of_range("SparseEntry with that label value does not exist");
        }

        const_reference
        get(const Label label) const {
          // Dense is preallocated. Simple array index.
          if (is_dense())
            return _entries.dense[label];

          // Locate within the ordered linked list.
          for (SparseEntry *se = _entries.sparse; se != nullptr; se = se->next) {
            if (label == se->label)
              return se->entry;
            else if (label > se->label)
              break;
          }
          throw std::out_of_range("SparseEntry with that label value does not exist");
        }

        // Element lookup
        pointer
        find_dense(const Label label) {
          return _entries.dense + label;
        }

        SparseEntry *
        find_sparse(const Label label) const {
          for (SparseEntry *se = _entries.sparse; se != nullptr; se = se->next) {
            if (label == se->label)
              return se;
            else if (label > se->label)
              break;
          }
          return nullptr;
        }

        void
        deserialise(std::istream &in, const Label nlabels, Pool &pool, size_t &size) {
          const uint32_t nitems = msgpack::read_array_size(in);
          assert(nitems == 2); (void)nitems;
          _value = msgpack::read_uint(in);
          if (is_dense()) {
            const uint32_t nitems = msgpack::read_array_size(in);
            assert(nitems == nlabels); (void)nitems;
            _entries = pool.alloc<pointer>(nlabels*sizeof(mapped_type));
            for (Label l = 0; l != nlabels; ++l, ++size) {
              new (_entries.dense + l) mapped_type();
              _entries.dense[l].deserialise(in);
            }
          }
          else {
            const uint32_t nitems = msgpack::read_array_size(in);
            assert(nitems != 0 && nitems < nlabels);
            SparseEntry *prev = nullptr;
            for (Label l = 0; l != nitems; ++l, ++size) {
              SparseEntry *se = pool.alloc<SparseEntry *>(sizeof(SparseEntry));
              new (se) SparseEntry();
              se->deserialise(in);
              if (prev == nullptr)
                _entries.sparse = se;
              else
                prev->next = se;
              prev = se;
            }
          }
        }

        void
        serialise(std::ostream &out, const Label nlabels) const {
          msgpack::write_array_size(out, 2);
          msgpack::write_uint(out, _value);
          if (is_dense()) {
            msgpack::write_array_size(out, nlabels);
            for (Label l = 0; l != nlabels; ++l)
              _entries.dense[l].serialise(out);
          }
          else {
            const Label size = _entries.sparse->size();
            msgpack::write_array_size(out, size);
            for (const SparseEntry *se = _entries.sparse; se != nullptr; se = se->next)
              se->serialise(out);
          }
        }

        template <typename FN>
        void
        for_each_label(const Label label_begin, const Label label_end, FN &fn) const {
          if (is_dense()) {
            for (Label l = label_begin; l != label_end; ++l)
              fn(l, _entries.dense[l]);
          }
          else {
            for (const SparseEntry *se = _entries.sparse; se != nullptr; se = se->next) {
              if (se->label >= label_begin)
                fn(se->label, se->entry);
              else if (se->label >= label_end)
                break;
            }
          }
        }

        static uint64_t build_value(uint64_t hash, bool is_dense) { return (hash & HASH_MASK) | static_cast<uint64_t>(is_dense); }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(Chain);
      };
      static_assert(sizeof(Chain) == 24, "Unexpected sizeof(Chain)");


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
        mutable Chain *_item;
        mutable void *_entry;
        mutable bool _initialised;
        mutable bool _ended;

        void
        _increment(void) const {
          // Don't do anything if it's at the end.
          if (SCHWA_UNLIKELY(_ended))
            return;

          if (SCHWA_LIKELY(_initialised)) {
            // Do we still have entries left within the current Chain?
            if (_item->is_dense()) {
              const pointer e = static_cast<pointer>(_entry) + 1;
              if (e != _item->get_dense() + _ht->_nlabels) {
                _entry = e;
                return;
              }
            }
            else {
              SparseEntry *se = static_cast<SparseEntry *>(_entry)->next;
              if (se != nullptr) {
                _entry = static_cast<void *>(se);
                return;
              }
            }
            _entry = nullptr;

            // Do we still have items left in the current Chain?
            _item = _item->next();
            if (_item != nullptr) {
              _entry = _item->get_typeless();
              return;
            }

            // Start at the next index in the table.
            ++_table_index;
          }
          else {
            // We're now initialised.
            _initialised = true;
          }

          // Find the next non-empty chain.
          for ( ; _table_index != TABLE_SIZE; ++_table_index)
            if (_ht->_table[_table_index] != nullptr)
              break;

          // If the table is empty, become and end iterator.
          if (_table_index == TABLE_SIZE) {
            _ht = nullptr;
            _table_index = 0;
            _ended = true;
          }
          else {
            _item = _ht->_table[_table_index];
            _entry = _item->get_typeless();
          }
        }

      public:
        Iterator(void) : _ht(nullptr), _table_index(0), _item(nullptr), _entry(nullptr), _initialised(true), _ended(true) { }
        explicit Iterator(TABLE &ht) : _ht(&ht), _table_index(0), _item(nullptr), _entry(nullptr), _initialised(false), _ended(false) { }
        Iterator(TABLE &ht, size_t table_index, Chain *item, void *entry) : _ht(&ht), _table_index(table_index), _item(item), _entry(entry), _initialised(true), _ended(false) { }
        Iterator(const Iterator &o) : _ht(o._ht), _table_index(o._table_index), _item(o._item), _entry(o._entry), _initialised(o._initialised), _ended(o._ended) { }
        Iterator(const Iterator &&o) : _ht(o._ht), _table_index(o._table_index), _item(o._item), _entry(o._entry), _initialised(o._initialised), _ended(o._ended) { }
        ~Iterator(void) { }

        inline bool
        operator ==(const Iterator &o) const {
          if (SCHWA_UNLIKELY(!_initialised))
            _increment();
          if (SCHWA_UNLIKELY(!o._initialised))
            o._increment();
          return _ended == o._ended && _ht == o._ht && _table_index == o._table_index && _item == o._item && _entry == o._entry && _initialised == o._initialised;
        }
        inline bool operator !=(const Iterator &o) const { return !(*this == o); }

        inline reference
        operator *(void) const {
          if (SCHWA_UNLIKELY(!_initialised))
            _increment();
          return _item->is_dense() ? *static_cast<pointer>(_entry) : static_cast<SparseEntry *>(_entry)->entry;
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
          return out << "[Iterator::operator <<|" << &it << "] " << it._ht << " " << it._table_index << " " << it._item << " " << it._entry << " " << it._initialised << " " << it._ended << "]";
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
        const uint64_t value = Chain::build_value(hash, type.is_dense());
        const size_t index = hash & TABLE_INDEX_MASK;

        for (Chain *chain = _table[index]; chain != nullptr; chain = chain->next()) {
          if (chain->value() == value) {
            void *ptr = chain->is_dense() ? static_cast<void *>(chain->find_dense(label)) : static_cast<void *>(chain->find_sparse(label));
            return ptr == nullptr ? end() : iterator(*this, index, chain, ptr);
          }
        }
        return end();
      }

      template <typename CP, typename HASHER>
      inline const_iterator
      _find(const FeatureType &type, const CP &cp, const Label label, const HASHER &hasher) const {
        const uint64_t hash = _hash(type, cp, hasher);
        const uint64_t value = Chain::build_value(hash, type.is_dense());
        const size_t index = hash & TABLE_INDEX_MASK;

        for (Chain *chain = _table[index]; chain != nullptr; chain = chain->next()) {
          if (chain->value() == value) {
            void *ptr = chain->is_dense() ? static_cast<void *>(chain->find_dense(label)) : static_cast<void *>(chain->find_sparse(label));
            return ptr == nullptr ? end() : const_iterator(*this, index, chain, ptr);
          }
        }
        return end();
      }

      template <typename CP, typename HASHER>
      inline reference
      _get(const FeatureType &type, const CP &cp, const Label label, const HASHER &hasher) {
        const uint64_t hash = _hash(type, cp, hasher);
        const uint64_t value = Chain::build_value(hash, type.is_dense());
        const size_t index = hash & TABLE_INDEX_MASK;

        Chain *prev = nullptr, *chain;
        for (chain = _table[index]; chain != nullptr; chain = chain->next()) {
          if (chain->value() == value) {
            bool created = false;
            reference r = chain->get(label, _pool, created);
            if (created)
              ++_size;
            return r;
          }
          prev = chain;
        }

        // Create the new chain item and insert it appropriately.
        chain = _pool.alloc<Chain *>(sizeof(Chain));
        new (chain) Chain(hash, type.is_dense(), label, _nlabels, _pool, _size);
        if (prev == nullptr)
          _table[index] = chain;
        else
          prev->set_next(chain);

        return chain->get(label);
      }

      template <typename CP, typename HASHER>
      inline const_reference
      _get(const FeatureType &type, const CP &cp, const Label label, const HASHER &hasher) const {
        const uint64_t hash = _hash(type, cp, hasher);
        const uint64_t value = Chain::build_value(hash, type.is_dense());
        const size_t index = hash & TABLE_INDEX_MASK;

        for (Chain *chain = _table[index]; chain != nullptr; chain = chain->next())
          if (chain->value() == value)
            return chain->get(label);
        throw std::out_of_range("Entry does not exist in the table");
      }

      template <typename CP, typename HASHER, typename FN>
      inline void
      _for_each_label(const FeatureType &type, const CP &cp, const HASHER &hasher, const Label label_begin, const Label label_end, FN &fn) const {
        const uint64_t hash = _hash(type, cp, hasher);
        const uint64_t value = Chain::build_value(hash, type.is_dense());
        const size_t index = hash & TABLE_INDEX_MASK;

        for (const Chain *chain = _table[index]; chain != nullptr; chain = chain->next()) {
          if (chain->value() == value) {
            chain->for_each_label(label_begin, label_end, fn);
            return;
          }
        }
      }

    private:
      Pool _pool;
      Chain **const _table;
      size_type _size;
      mutable third_party::xxhash::XXH64_stateSpace_t _xxhash_state;  //<! State object to maintain partial xxhash state.
      const uint16_t _nlabels;

    public:
      explicit FeatureHashtable(uint16_t nlabels, size_t pool_block_size=DEFAULT_POOL_BLOCK_SIZE) : _pool(pool_block_size), _table(new Chain *[TABLE_SIZE]), _size(0), _nlabels(nlabels) {
        std::memset(_table, 0, TABLE_SIZE*sizeof(Chain *));
      }
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

      // Iteration through each label for a particular contextual predicate.
      template <typename CP, typename FN, typename HASHER=schwa::Hasher64<CP>>
      inline void
      for_each_label(const FeatureType &type, const CP &contextual_predicate, const Label label_begin, const Label label_end, FN &fn, const HASHER &hasher=HASHER()) const {
        static_assert(sizeof(typename HASHER::result_type) == 8, "64-bit hash function required");
        _for_each_label(type, contextual_predicate, hasher, label_begin, label_end, fn);
      }


      void
      deserialise(std::istream &in) {
        if (!empty())
          throw std::logic_error("Cannot deserialise a non-empty table");

        const uint16_t nlabels = msgpack::read_uint16(in);
        if (nlabels != _nlabels)
          throw std::runtime_error("nlabels on the table != nlabels on the model");

        const uint32_t nkeys = msgpack::read_map_size(in);
        for (size_t n = 0; n != nkeys; ++n) {
          const size_t index = msgpack::read_uint(in);
          Chain *chain = _table[index] = _pool.alloc<Chain *>(sizeof(Chain));
          new (chain) Chain();
          chain->deserialise(in, _nlabels, _pool, _size);
        }
      }

      void
      serialise(std::ostream &out) const {
        size_t nkeys = 0;
        for (size_t index = 0; index != TABLE_SIZE; ++index)
          if (_table[index] != nullptr)
            ++nkeys;
        msgpack::write_uint16(out, _nlabels);
        msgpack::write_map_size(out, nkeys);
        for (size_t index = 0; index != TABLE_SIZE; ++index) {
          if (_table[index] != nullptr) {
            msgpack::write_uint(out, index);
            _table[index]->serialise(out, _nlabels);
          }
        }
      }

      std::ostream &
      pprint(std::ostream &out) const {
        out << "<FeatureHashtable size=" << _size << ">\n";
        for (size_t index = 0; index != TABLE_SIZE; ++index) {
          const Chain *chain = _table[index];
          if (chain == nullptr)
            continue;
          out << index << " => (" << chain->size() << ")";
          for ( ; chain != nullptr; chain = chain->next())
            out << " [" << chain << " masked_hash=0x" << std::hex << chain->masked_hash() << std::dec << " is_dense=" << chain->is_dense() << "]";
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
