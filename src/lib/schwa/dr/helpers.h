/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_HELPERS_H_
#define SCHWA_DR_HELPERS_H_

#include <string>
#include <vector>

#include <schwa/_base.h>

namespace schwa {
  namespace dr {

    /**
     * Base template definition used to make \ref DR_REVERSE_SLICES work. The types used in the
     * template arguments here are what are provided by decltype in \ref DR_REVERSE_SLICES, but they
     * are unpacked by the two template specialisations below to actually do useful things.
     **/
    template <typename T1, T1 fn1, typename T2, T2 fn2, typename T3, T3 fn3, typename T4, T4 fn4>
    class ReverseSlices;

    /**
     * Template specialisation to make \ref DR_REVERSE_SLICES work in the mutually exclusive case. That
     * is, when you want to reverse your slices to a single pointer of the pointed from type.
     **/
    template <typename SOURCE, typename TARGET, typename DOC,
               Store<SOURCE> DOC::*source_store,
               Store<TARGET> DOC::*target_store,
               Slice<TARGET *> SOURCE::*slice_attr,
               SOURCE *TARGET::*pointer_attr>
    class ReverseSlices<
        Store<SOURCE> DOC::*, source_store,
        Store<TARGET> DOC::*, target_store,
        Slice<TARGET *> SOURCE::*, slice_attr,
        SOURCE *TARGET::*, pointer_attr> {
    public:
      static_assert(std::is_base_of<Doc, DOC>::value, "DOC must be a subclass of Doc");
      static_assert(std::is_base_of<Ann, SOURCE>::value, "Store<SOURCE> type SOURCE must be a subclass of Ann");
      static_assert(std::is_base_of<Ann, TARGET>::value, "Store<TARGET> type TARGET must be a subclass of Ann");

      void
      operator ()(DOC &doc) const {
        // Initialise the target pointers to all be NULL.
        for (TARGET &target : doc.*target_store)
          target.*pointer_attr = nullptr;

        // For each of the source objects which have valid target slices, populate the back-pointer.
        for (SOURCE &source : doc.*source_store) {
          Slice<TARGET *> &slice = source.*slice_attr;
          if (slice.start == nullptr || slice.stop == nullptr)
            continue;
          for (TARGET &target : slice)
            target.*pointer_attr = &source;
        }
      }
    };

    /**
     * Template specialisation to make \ref DR_REVERSE_SLICES work in the non-mutually exclusive case.
     * That is, when you want to reverse your slices to a collection of pointers instead of just a
     * single pointer.
     **/
    template <typename SOURCE, typename TARGET, typename DOC,
               Store<SOURCE> DOC::*source_store,
               Store<TARGET> DOC::*target_store,
               Slice<TARGET *> SOURCE::*slice_attr,
               std::vector<SOURCE *> TARGET::*pointers_attr>
    class ReverseSlices<
        Store<SOURCE> DOC::*, source_store,
        Store<TARGET> DOC::*, target_store,
        Slice<TARGET *> SOURCE::*, slice_attr,
        std::vector<SOURCE *> TARGET::*, pointers_attr> {
    public:
      static_assert(std::is_base_of<Doc, DOC>::value, "DOC must be a subclass of Doc");
      static_assert(std::is_base_of<Ann, SOURCE>::value, "Store<SOURCE> type SOURCE must be a subclass of Ann");
      static_assert(std::is_base_of<Ann, TARGET>::value, "Store<TARGET> type TARGET must be a subclass of Ann");

      void
      operator ()(DOC &doc) const {
        // Initialise the target pointers vector to be empty.
        for (TARGET &target : doc.*target_store)
          (target.*pointers_attr).clear();

        // For each of the source objects which have valid target slices, populate the back-pointers.
        for (SOURCE &source : doc.*source_store) {
          Slice<TARGET *> &slice = source.*slice_attr;
          if (slice.start == nullptr || slice.stop == nullptr)
            continue;
          for (TARGET &target : slice)
            (target.*pointers_attr).push_back(&source);
        }
      }
    };

    /**
     * @def DR_REVERSE_SLICES(source_store, target_store, slice_attr, pointer_attr)
     *
     * A macro that wraps the type inference used in the template magic of ReverseSlices.
     * \code{.cpp}
     * DR_REVERSE_SLICES(&Doc::sents, &Doc::tokens, &Sent::span, &Token::sent)(doc);
     * \endcode
     *
     * This decorator works for the non-mutually exclusive case as well, as long as your definition
     * of the pointer container is a std::vector.
     * \code{.cpp}
     * DR_REVERSE_SLICES(&Doc::sents, &Doc::tokens, &Sent::span, &Token::sents)(doc);
     * \endcode
     *
     * The functor returned by this macro can be stored and reused instead of just created and used
     * once.
     * \code{.cpp}
     * auto x = DR_REVERSE_SLICES(&Doc::sents, &Doc::tokens, &Sent::span, &Token::sent);
     * ...
     * for (Doc &doc : docs)
     *   x(doc);
     * \endcode
     **/
    #define DR_REVERSE_SLICES(source_store, target_store, slice_attr, pointer_attr) schwa::dr::ReverseSlices<decltype(source_store), source_store, decltype(target_store), target_store, decltype(slice_attr), slice_attr, decltype(pointer_attr), pointer_attr>()

  }
}

#endif  // SCHWA_DR_HELPERS_H_
