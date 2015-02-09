/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tagger/pos.h>

#include <schwa/io/logging.h>


namespace schwa {
namespace tagger {

POSExtractor::POSExtractor(unsigned int lex_cutoff) :
    _unigram_token_callback(learn::create_unigram_callback<canonical_schema::Token>()),
    _bigram_token_callback(learn::create_bigram_callback<canonical_schema::Token>()),
    _trigram_token_callback(learn::create_trigram_callback<canonical_schema::Token>()),
    _offsets_token_norm_raw(&_get_token_norm_raw),
    _lex_token("tokens"),
    _lex_cutoff(lex_cutoff)
  { }


void
POSExtractor::phase1_begin(void) {
  LOG(INFO) << "POSExtractor phase1_begin" << std::endl;
}

void
POSExtractor::phase1_end(void) {
  // Cull the rare tokens from the lexicon.
  _lex_token.cull(_lex_cutoff);
  LOG(INFO) << "POSExtractor phase1_end" << std::endl;
}


void
POSExtractor::phase1_extract(canonical_schema::Token &token, size_t) {
  // Add the current token to the lexicon.
  _lex_token.add(_get_token_norm_raw(token));
}


void
POSExtractor::phase2_begin(void) {
  LOG(INFO) << "POSExtractor phase2_begin" << std::endl;
}


void
POSExtractor::phase2_bos(canonical_schema::Sentence &sentence) {
  _offsets_token_norm_raw.set_slice(sentence.span);
}


void
POSExtractor::phase2_end(void) {
  LOG(INFO) << "POSExtractor phase2_end" << std::endl;
}

}  // namespace tagger
}  // namespace schwa
