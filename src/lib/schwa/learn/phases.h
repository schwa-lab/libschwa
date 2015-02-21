/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_PHASES_H_
#define SCHWA_LEARN_PHASES_H_

#include <schwa/_base.h>
#include <schwa/canonical-schema.h>


namespace schwa {
  namespace learn {

    template <typename SENTENCE_EXTRACTOR>
    class ExtractionPhaseRunner {
    public:
      static constexpr const unsigned int PHASE = SENTENCE_EXTRACTOR::PHASE;
      using sentence_extractor_type = SENTENCE_EXTRACTOR;
      static_assert(PHASE >= 1 && PHASE <= 3, "PHASE template parameter must be in the range 1 <= PHASE <= 3");

    protected:
      sentence_extractor_type &_extractor;

    public:
      explicit ExtractionPhaseRunner(sentence_extractor_type &extractor) : _extractor(extractor) { }

      template <typename... ARGS>
      void extract_with_blocks(canonical_schema::Doc &doc, ARGS&&... args);
      template <typename... ARGS>
      void extract_without_blocks(canonical_schema::Doc &doc, ARGS&&... args);
      template <typename... ARGS>
      void extract(canonical_schema::Doc &doc, ARGS&&... args);
    };


    /**
     * A sentence extractor implementation for use with ExtractionPhaseRunner which does the
     * simplest thing possible -- iterate through each of the tokens in the sentence and call the
     * appropriate extract method on the wrapped extractor object.
     **/
    template <typename EXTRACTOR, unsigned int _PHASE>
    class BasicSenteceExtractor {
    public:
      static constexpr const unsigned int PHASE = _PHASE;
      using extractor_type = EXTRACTOR;

    protected:
      extractor_type &_extractor;

    public:
      explicit BasicSenteceExtractor(extractor_type &extractor) : _extractor(extractor) { }

      inline extractor_type &extractor(void) { return _extractor; }

      template <typename... ARGS>
      void extract_sentence(canonical_schema::Sentence &sentence, ARGS&&... args);
    };

  }
}

#include <schwa/learn/phases_impl.h>

#endif  // SCHWA_LEARN_PHASES_H_
