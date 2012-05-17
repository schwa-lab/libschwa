/* -*- Mode: C++; indent-tabs-mode: nil -*- */

namespace schwa {
  namespace hashtable {

    template <class E>
    class AlphaCmp {
    public:
      inline bool operator ()(const E *const e1, const E *const e2) {
        return strcmp(e1->str, e2->str) < 0;
      }
    };

    template <class E>
    class ValueCmp {
    public:
      inline bool operator ()(const E *const e1, const E *const e2) {
        return e1->value < e2->value;
      }
    };

    template <class E>
    class RevValueCmp {
    public:
      inline bool operator ()(const E *const e1, const E *const e2) {
        return e1->value > e2->value;
      }
    };

    template <class E>
    class IndexCmp {
    public:
      inline bool operator ()(const E *const e1, const E *const e2) {
        return e1->index < e2->index;
      }
    };


    template <class Entry, class Key, unsigned long NBUCKETS, unsigned long SPOOL>
    class Ordered : public Base<Entry, Key, NBUCKETS, SPOOL> {
    public:
      typedef Base<Entry, Key, NBUCKETS, SPOOL> Super;

      typedef std::vector<Entry *> Entries;
      typedef typename Entries::iterator iterator;
      typedef typename Entries::const_iterator const_iterator;

    protected:
      Entries _entries;

    public:
      Ordered(std::string name, Pool *pool=0) : Super(name, pool) { }
      virtual ~Ordered(void) { }

      using Super::insert;

      virtual Entry *
      insert(Key key, const Hash hash, const unsigned long bucket) {
        Entry *entry = Super::insert(key, hash, bucket);
        _entries.push_back(entry);
        return entry;
      }

      virtual void
      clear(void) {
        Super::clear();
        _entries.resize(0);
      }

      virtual void
      renumber(void) {
        for (unsigned long i = 0; i != _entries.size(); ++i)
          _entries[i]->index = i;
      }

      void
      compact(void) {
        iterator new_end = std::remove(_entries.begin(), _entries.end(), reinterpret_cast<Entry *>(0));
        _entries.erase(new_end, _entries.end());
      }

      void
      compress(void) {
        iterator new_end = std::remove(_entries.begin(), _entries.end(), reinterpret_cast<Entry *>(0));
        _entries.erase(new_end, _entries.end());
        renumber();
      }

      template <class Comparator>
      void sort(Comparator cmp) { std::sort(_entries.begin(), _entries.end(), cmp); }

      void sort_by_alpha(void) { sort(AlphaCmp<Entry>()); renumber(); }
      void sort_by_value(void) { sort(ValueCmp<Entry>()); renumber(); };
      void sort_by_rev_value(void) { sort(RevValueCmp<Entry>()); renumber(); };

      void
      save(std::ostream &out) const {
        for (const_iterator i = _entries.begin(); i != _entries.end(); ++i)
          (*i)->save(out) << '\n';
      }
    };

  }
}
