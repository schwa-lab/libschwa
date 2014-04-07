/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_ISTORE_H_
#define SCHWA_DR_ISTORE_H_

#include <iterator>

#include <schwa/_base.h>

namespace schwa {
  namespace dr {

    class Ann;


    class IStore {
    public:
      class inner_typeless_iterator : public std::iterator<std::forward_iterator_tag, Ann> {
      public:
        virtual ~inner_typeless_iterator(void) { }
        virtual bool operator ==(const inner_typeless_iterator &o) const = 0;
        virtual bool operator !=(const inner_typeless_iterator &o) const = 0;
        virtual reference operator *(void) = 0;
        virtual pointer operator ->(void) = 0;
        virtual inner_typeless_iterator &operator ++(void) = 0;
      };

      class typeless_iterator : public std::iterator<std::forward_iterator_tag, Ann> {
      public:
        using difference_type = typename std::iterator<std::forward_iterator_tag, Ann>::difference_type;
        using iterator_category = typename std::iterator<std::forward_iterator_tag, Ann>::iterator_category;
        using pointer = typename std::iterator<std::forward_iterator_tag, Ann>::pointer;
        using reference = typename std::iterator<std::forward_iterator_tag, Ann>::reference;
        using value_type = typename std::iterator<std::forward_iterator_tag, Ann>::value_type;

      private:
        inner_typeless_iterator *const _it;

      public:
        explicit typeless_iterator(inner_typeless_iterator *it) : _it(it) { }
        ~typeless_iterator(void) { delete _it; }

        inline bool operator ==(const typeless_iterator &o) const { return *_it == *o._it; }
        inline bool operator !=(const typeless_iterator &o) const { return *_it != *o._it; }

        inline reference operator *(void) { return (*_it).operator*(); }
        inline pointer operator ->(void) { return (*_it).operator->(); }

        inline typeless_iterator &operator ++(void) { (*_it).operator++(); return *this; }
      };

    protected:
      IStore(void) { }

    public:
      virtual ~IStore(void) { }

      virtual typeless_iterator typeless_begin(void) = 0;
      virtual typeless_iterator typeless_end(void) = 0;

      virtual const Ann &at_index(size_t index) const = 0;
      virtual Ann &at_index(size_t index) = 0;
      virtual size_t index_of(const Ann &obj) const = 0;
      virtual size_t nelem(void) const = 0;
    };

  }
}

#endif  // SCHWA_DR_ISTORE_H_
