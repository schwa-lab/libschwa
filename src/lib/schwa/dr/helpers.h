/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DR_HELPERS_H_
#define SCHWA_DR_HELPERS_H_

#include <cassert>
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
    #define DR_REVERSE_SLICES(source_store, target_store, slice_attr, pointer_attr) \
      ::schwa::dr::ReverseSlices<\
        decltype(source_store), source_store, \
        decltype(target_store), target_store, \
        decltype(slice_attr), slice_attr, \
        decltype(pointer_attr), pointer_attr \
      >()


    template <typename T1, T1 fn1, typename T2, T2 fn2, typename T3, T3 fn3, typename T4, T4 fn4, typename T5, T5 fn5, typename T6, T6 fn6, typename T7, T7 fn7, typename T8, T8 fn8>
    class SequenceTagger;

    template <typename SPAN, typename TARGET, typename SENTENCE, typename DOC,
               Store<SPAN> DOC::*span_store,
               Store<SENTENCE> DOC::*sentence_store,
               Store<TARGET> DOC::*target_store,
               Slice<TARGET *> SPAN::*span_slice_attr,
               Slice<TARGET *> SENTENCE::*sentence_slice_attr,
               SPAN *TARGET::*pointer_attr,
               std::string SPAN::*klass_attr,
               std::string TARGET::*tag_attr>
    class SequenceTagger<
        Store<SPAN> DOC::*, span_store,
        Store<SENTENCE> DOC::*, sentence_store,
        Store<TARGET> DOC::*, target_store,
        Slice<TARGET *> SPAN::*, span_slice_attr,
        Slice<TARGET *> SENTENCE::*, sentence_slice_attr,
        SPAN *TARGET::*, pointer_attr,
        std::string SPAN::*, klass_attr,
        std::string TARGET::*, tag_attr> {
    public:
      static_assert(std::is_base_of<Doc, DOC>::value, "DOC must be a subclass of Doc");
      static_assert(std::is_base_of<Ann, SENTENCE>::value, "Store<SENTENCE> type SENTENCE must be a subclass of Ann");
      static_assert(std::is_base_of<Ann, SPAN>::value, "Store<SPAN> type SPAN must be a subclass of Ann");
      static_assert(std::is_base_of<Ann, TARGET>::value, "Store<TARGET> type TARGET must be a subclass of Ann");

      void
      do_iob1(DOC &doc) const {
        // For each valid sentence...
        for (SENTENCE &sentence : doc.*sentence_store) {
          Slice<TARGET *> &slice = sentence.*sentence_slice_attr;
          if (slice.start == nullptr || slice.stop == nullptr)
            continue;

          // For each TARGET covered by the sentence...
          SPAN *prev_span = nullptr;
          std::string prev_klass, klass, prefix;
          for (TARGET &target : slice) {
            SPAN *span = target.*pointer_attr;
            if (span == nullptr) {
              klass = "O";
              prefix = "";
            }
            else {
              klass = span->*klass_attr;
              prefix = (span != prev_span && prev_span != nullptr && prev_klass == klass) ? "B-" : "I-";
            }
            target.*tag_attr = prefix + klass;
            prev_span = span;
            prev_klass = klass;
          }
        }
      }

      void
      do_iob2(DOC &doc) const {
        // Initialise the sequence tags to be "O".
        for (TARGET &target : doc.*target_store)
          target.*tag_attr = "O";

        // For each of the span objects which have valid slices...
        std::string klass;
        for (SPAN &span : doc.*span_store) {
          Slice<TARGET *> &slice = span.*span_slice_attr;
          if (slice.start == nullptr || slice.stop == nullptr)
            continue;

          // Work out the tag prefix and populate the tag_attr on the TARGETs covered by the span.
          klass = span.*klass_attr;
          for (TARGET &target : slice)
            target.*tag_attr = ((&target == slice.start) ? "B-" : "I-") + klass;
        }
      }

      void
      do_bmewo(DOC &doc) const {
        // Initialise the sequence tags to be "O".
        for (TARGET &target : doc.*target_store)
          target.*tag_attr = "O";

        // For each of the span objects which have valid slices...
        std::string prefix, klass;
        for (SPAN &span : doc.*span_store) {
          Slice<TARGET *> &slice = span.*span_slice_attr;
          if (slice.start == nullptr || slice.stop == nullptr)
            continue;

          // Work out the tag prefix and populate the tag_attr on the TARGETs covered by the span.
          klass = span.*klass_attr;
          for (TARGET &target : slice) {
            if (slice.stop - slice.start == 1)
              prefix = "W-";
            else if (&target == slice.start)
              prefix = "B-";
            else if (&target == slice.stop - 1)
              prefix = "E-";
            else
              prefix = "M-";
            target.*tag_attr = prefix + klass;
          }
        }
      }

      void
      operator ()(DOC &doc, const SequenceTagEncoding encoding) const {
        switch (encoding) {
        case SequenceTagEncoding::IOB1: do_iob1(doc); break;
        case SequenceTagEncoding::IOB2: do_iob2(doc); break;
        case SequenceTagEncoding::BMEWO: do_bmewo(doc); break;
        }
      }
    };

    #define DR_SEQUENCE_TAGGER(span_store, sentence_store, target_store, span_slice_attr, sentence_slice_attr, pointer_attr, klass_attr, tag_attr) \
      ::schwa::dr::SequenceTagger< \
        decltype(span_store), span_store, \
        decltype(sentence_store), sentence_store, \
        decltype(target_store), target_store, \
        decltype(span_slice_attr), span_slice_attr, \
        decltype(sentence_slice_attr), sentence_slice_attr, \
        decltype(pointer_attr), pointer_attr, \
        decltype(klass_attr), klass_attr, \
        decltype(tag_attr), tag_attr \
      >()


    template <typename T1, T1 fn1, typename T2, T2 fn2, typename T3, T3 fn3, typename T4, T4 fn4, typename T5, T5 fn5, typename T6, T6 fn6>
    class SequenceUntagger;

    template <typename SPAN, typename TARGET, typename SENTENCE, typename DOC,
               Store<SPAN> DOC::*span_store,
               Store<SENTENCE> DOC::*sentence_store,
               Slice<TARGET *> SPAN::*span_slice_attr,
               Slice<TARGET *> SENTENCE::*sentence_slice_attr,
               std::string SPAN::*klass_attr,
               std::string TARGET::*tag_attr>
    class SequenceUntagger<
        Store<SPAN> DOC::*, span_store,
        Store<SENTENCE> DOC::*, sentence_store,
        Slice<TARGET *> SPAN::*, span_slice_attr,
        Slice<TARGET *> SENTENCE::*, sentence_slice_attr,
        std::string SPAN::*, klass_attr,
        std::string TARGET::*, tag_attr> {
    public:
      static_assert(std::is_base_of<Doc, DOC>::value, "DOC must be a subclass of Doc");
      static_assert(std::is_base_of<Ann, SENTENCE>::value, "Store<SENTENCE> type SENTENCE must be a subclass of Ann");
      static_assert(std::is_base_of<Ann, SPAN>::value, "Store<SPAN> type SPAN must be a subclass of Ann");
      static_assert(std::is_base_of<Ann, TARGET>::value, "Store<TARGET> type TARGET must be a subclass of Ann");

      void
      untag(DOC &doc) const {
        // For each valid sentence...
        for (SENTENCE &sentence : doc.*sentence_store) {
          Slice<TARGET *> &slice = sentence.*sentence_slice_attr;
          if (slice.start == nullptr || slice.stop == nullptr)
            continue;

          // For each TARGET covered by the sentence...
          for (TARGET *start = slice.start; start != slice.stop; ++start) {
            std::string klass = start->*tag_attr;
            if (klass.empty() || klass == "O")
              continue;

            klass = klass.substr(2);
            char prev_prefix = klass[0];

            TARGET *stop;
            for (stop = start + 1; stop != slice.stop; ++stop) {
              if (prev_prefix == 'W' || prev_prefix == 'E')
                break;
              const std::string &klass2 = stop->*tag_attr;
              if (klass2.empty() || klass2 == "O" || klass2[0] == 'B' || klass2[0] == 'W' || klass2.substr(2) != klass)
                break;
              prev_prefix = klass2[0];
            }

            (doc.*span_store).create(1);
            SPAN &span = (doc.*span_store).back();
            (span.*span_slice_attr).start = start;
            (span.*span_slice_attr).stop = stop;
            span.*klass_attr = klass;

            start = stop - 1;
          }  // for each target
        }  // for each sentence
      }

      inline void operator ()(DOC &doc) const { return untag(doc); }
    };

    #define DR_SEQUENCE_UNTAGGER(span_store, sentence_store, span_slice_attr, sentence_slice_attr, klass_attr, tag_attr) \
      ::schwa::dr::SequenceUntagger< \
        decltype(span_store), span_store, \
        decltype(sentence_store), sentence_store, \
        decltype(span_slice_attr), span_slice_attr, \
        decltype(sentence_slice_attr), sentence_slice_attr, \
        decltype(klass_attr), klass_attr, \
        decltype(tag_attr), tag_attr \
      >()
  }
}

#endif  // SCHWA_DR_HELPERS_H_
