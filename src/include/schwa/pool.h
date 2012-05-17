/* -*- Mode: C++; indent-tabs-mode: nil -*- */
/**
 * schwa::MemoryArena and schwa::Pool
 * very fast, efficient bulk memory allocation used in
 * hash tables and other data structures with a large number
 * of small objects
 */

namespace schwa {

  // Wraps a large chunk of memory which is determined
  // when the chunk is created.  Chunks of memory are not
  // reallocated, instead another MemoryArena needs to be
  // created.
  class MemoryArena {
  protected:
    const size_t _SIZE;
    char *const _begin;
    char *const _end;

    char *_current;
    char *_mark;

  public:
    MemoryArena(size_t size) : _SIZE(size), _begin(new char[_SIZE]), _end(_begin + _SIZE), _current(_begin), _mark(_begin) { }
    ~MemoryArena(void) { delete [] _begin; }

    char *
    alloc(const size_t size) {
      if (size + _current > _end)
        return 0;
      char *result = _current;
      _current += size;
      return result;
    }

    void mark(void) { _mark = _current; }
    void undo(void) { _current = _mark; }
    void clear(void) { _mark = _current = _begin; }

    size_t size(void) const { return _SIZE; }
    size_t used(void) const { return _current - _begin; }
    size_t unused(void) const { return _end - _current; }
  };

  typedef std::vector<MemoryArena *> MemoryArenas;


  // very fast memory allocation for bulk access
  // however, you cannot free individual objects
  // only free all objects simultaneously by resetting
  // the pool.  This functionality is often what is required
  // for classes such as hash tables which create lots of
  // rather small objects which all need to be destroyed at
  // the same time.
  class Pool{
  private:
    const unsigned long _MINSIZE;

    MemoryArenas _used;
    MemoryArenas _unused;
    MemoryArena *_current;

  public:
    Pool(size_t minsize) : _MINSIZE(minsize), _current(new MemoryArena(_MINSIZE)) { }

    ~Pool(void) {
      delete _current;
      for (MemoryArenas::iterator it(_used.begin()); it != _used.end(); ++it)
        delete *it;
      for (MemoryArenas::iterator it(_unused.begin()); it != _unused.end(); ++it)
        delete *it;
    }

    // allocate size bytes from the Pool
    // get a new MemoryArena if we don't have size amount of
    // space in the current MemoryArena.
    char *
    alloc(const size_t size) {
      char *buf = _current->alloc(size);
      if (buf)
        return buf;

      if (size < _MINSIZE) {
        _used.push_back(_current);
        if (!_unused.empty()) {
          _current = _unused.back();
          _unused.pop_back();
        }
        else
          _current = new MemoryArena(_MINSIZE);
        buf = _current->alloc(size);
      }
      else {
        MemoryArena *tmp = new MemoryArena(size);
        buf = tmp->alloc(size);
        _used.push_back(tmp);
      }
      return buf;
    }

    // allocate space and copy
    void *
    dup(const void *mem, size_t size) {
      void *buf = alloc(size);
      std::memmove(buf, mem, size);
      return buf;
    }

    // allocate space and copy, putting a '\0' on the end
    const char *
    strdup(const char *const str, size_t len) {
      char *buf = alloc(len + 1);
      std::memmove(buf, str, len);
      buf[len] = '\0';
      return buf;
    }

    const char *
    strdup(const char *const str) {
      return strdup(str, strlen(str));
    }

    // free all of the objects in the Pool
    // makes no attempt to call any destructors for these
    // objects, this needs to have been managed by the client code
    void clear(void) {
      _current->clear();

      _unused.insert(_unused.end(), _used.begin(), _used.end());
      for (MemoryArenas::iterator it(_used.begin()); it != _used.end(); ++it)
        (*it)->clear();
      _used.resize(0);
    }

    // support for rolling-back the last allocation (and only the last)
    void mark(void) { _current->mark(); }
    void undo(void) { _current->undo(); }

    // some Pool statistics
    size_t narenas(void) const { return _used.size(); }
    size_t minsize(void) const { return _MINSIZE; }

    size_t
    size(void) const {
      size_t nbytes = _current->size();
      for (MemoryArenas::const_iterator it(_used.begin()); it != _used.end(); ++it)
        nbytes += (*it)->size();
      for (MemoryArenas::const_iterator it(_unused.begin()); it != _unused.end(); ++it)
        nbytes += (*it)->size();
      return nbytes;
    }

    size_t
    used(void) const {
      size_t nbytes = _current->used();
      for (MemoryArenas::const_iterator it(_used.begin()); it != _used.end(); ++it)
        nbytes += (*it)->used();
      return nbytes;
    }

    void
    stats(std::ostream &out) const {
      size_t u = used();
      size_t b = size();
      out << "number of arenas " << narenas() << '\n';
      out << "minimum arena size " << _MINSIZE << '\n';
      out << "total memory  " << b << " bytes\n";
      out << "used memory   " << u << " bytes\n";
      out << "unused memory " << b - u << " bytes\n";
      out << "utilisation   " << std::setprecision(5) << u/(float)b*100.0 << "%\n";
    }
  };


  inline std::ostream &
  operator <<(std::ostream &out, const Pool &pool) {
    pool.stats(out);
    return out;
  }

}
