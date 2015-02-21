/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_PHASES_IMPL_H_
#define SCHWA_LEARN_PHASES_IMPL_H_


namespace schwa {
namespace learn {

// ========================================================================
// ExtractionPhaseRunnerDispatcher
// ========================================================================
/**
 * Helper class for compile-time dispatch to the appropriate phase method on the provided extractor
 * object. This is used by ExtractionPhaseRunner.
 **/
template <unsigned int PHASE>
class ExtractionPhaseRunnerDispatcher;


template <>
class ExtractionPhaseRunnerDispatcher<1> {
public:
  template <typename EXTRACTOR>
  static inline void call_extractor_bod(EXTRACTOR &extractor, canonical_schema::Doc &doc) { extractor.phase1_bod(doc); }
  template <typename EXTRACTOR>
  static inline void call_extractor_bos(EXTRACTOR &extractor, canonical_schema::Sentence &sentence) { extractor.phase1_bos(sentence); }
  template <typename EXTRACTOR, typename... ARGS>
  static inline void call_extractor_extract(EXTRACTOR &extractor, canonical_schema::Sentence &sentence, canonical_schema::Token &token, ARGS&&... args) { extractor.phase1_extract(sentence, token, args...); }
  template <typename EXTRACTOR>
  static inline void call_extractor_eos(EXTRACTOR &extractor, canonical_schema::Sentence &sentence) { extractor.phase1_eos(sentence); }
  template <typename EXTRACTOR>
  static inline void call_extractor_eod(EXTRACTOR &extractor, canonical_schema::Doc &doc) { extractor.phase1_eod(doc); }
};

template <>
class ExtractionPhaseRunnerDispatcher<2> {
public:
  template <typename EXTRACTOR>
  static inline void call_extractor_bod(EXTRACTOR &extractor, canonical_schema::Doc &doc) { extractor.phase2_bod(doc); }
  template <typename EXTRACTOR>
  static inline void call_extractor_bos(EXTRACTOR &extractor, canonical_schema::Sentence &sentence) { extractor.phase2_bos(sentence); }
  template <typename EXTRACTOR, typename... ARGS>
  static inline void call_extractor_extract(EXTRACTOR &extractor, canonical_schema::Sentence &sentence, canonical_schema::Token &token, ARGS&&... args) { extractor.phase2_extract(sentence, token, args...); }
  template <typename EXTRACTOR>
  static inline void call_extractor_eos(EXTRACTOR &extractor, canonical_schema::Sentence &sentence) { extractor.phase2_eos(sentence); }
  template <typename EXTRACTOR>
  static inline void call_extractor_eod(EXTRACTOR &extractor, canonical_schema::Doc &doc) { extractor.phase2_eod(doc); }
};

template <>
class ExtractionPhaseRunnerDispatcher<3> {
public:
  template <typename EXTRACTOR>
  static inline void call_extractor_bod(EXTRACTOR &extractor, canonical_schema::Doc &doc) { extractor.phase3_bod(doc); }
  template <typename EXTRACTOR>
  static inline void call_extractor_bos(EXTRACTOR &extractor, canonical_schema::Sentence &sentence) { extractor.phase3_bos(sentence); }
  template <typename EXTRACTOR, typename... ARGS>
  static inline void call_extractor_extract(EXTRACTOR &extractor, canonical_schema::Sentence &sentence, canonical_schema::Token &token, ARGS&&... args) { extractor.phase3_extract(sentence, token, args...); }
  template <typename EXTRACTOR>
  static inline void call_extractor_eos(EXTRACTOR &extractor, canonical_schema::Sentence &sentence) { extractor.phase3_eos(sentence); }
  template <typename EXTRACTOR>
  static inline void call_extractor_eod(EXTRACTOR &extractor, canonical_schema::Doc &doc) { extractor.phase3_eod(doc); }
};


// ========================================================================
// ExtractionPhaseRunner
// ========================================================================
template <typename SENTENCE_EXTRACTOR> template <typename... ARGS>
inline void
ExtractionPhaseRunner<SENTENCE_EXTRACTOR>::extract_with_blocks(canonical_schema::Doc &doc, ARGS&&... args) {
  // Paragraphs.
  for (canonical_schema::Block &block : doc.blocks) {
    if (block.paragraph == nullptr)
      continue;
    for (canonical_schema::Sentence &sentence : block.paragraph->span)
      _extractor.extract_sentence(sentence, args...);
  }

  // Headings.
  for (canonical_schema::Block &block : doc.blocks) {
    if (block.heading == nullptr)
      continue;
    _extractor.extract_sentence(*block.heading->sentence, args...);
  }
}


template <typename SENTENCE_EXTRACTOR> template <typename... ARGS>
inline void
ExtractionPhaseRunner<SENTENCE_EXTRACTOR>::extract_without_blocks(canonical_schema::Doc &doc, ARGS&&... args) {
  for (canonical_schema::Sentence &sentence : doc.sentences)
    _extractor.extract_sentence(sentence, args...);
}


template <typename SENTENCE_EXTRACTOR> template <typename... ARGS>
inline void
ExtractionPhaseRunner<SENTENCE_EXTRACTOR>::extract(canonical_schema::Doc &doc, ARGS&&... args) {
  ExtractionPhaseRunnerDispatcher<PHASE>::call_extractor_bod(_extractor.extractor(), doc);
  if (doc.blocks.empty())
    extract_without_blocks(doc, args...);
  else
    extract_with_blocks(doc, args...);
  ExtractionPhaseRunnerDispatcher<PHASE>::call_extractor_eod(_extractor.extractor(), doc);
}


// ========================================================================
// BasicSenteceExtractor
// ========================================================================
template <typename EXTRACTOR, unsigned int _PHASE> template <typename... ARGS>
inline void
BasicSenteceExtractor<EXTRACTOR, _PHASE>::extract_sentence(canonical_schema::Sentence &sentence, ARGS&&... args) {
  ExtractionPhaseRunnerDispatcher<PHASE>::call_extractor_bos(_extractor, sentence);
  for (canonical_schema::Token &token : sentence.span)
    ExtractionPhaseRunnerDispatcher<PHASE>::call_extractor_extract(_extractor, sentence, token, args...);
  ExtractionPhaseRunnerDispatcher<PHASE>::call_extractor_eos(_extractor, sentence);
}

}  // namespace learn
}  // namespace schwa

#endif  // SCHWA_LEARN_PHASES_IMPL_H_
