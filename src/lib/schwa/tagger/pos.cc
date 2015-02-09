/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/tagger/pos.h>

#include <schwa/io/logging.h>


namespace schwa {
namespace tagger {

POSExtractor::POSExtractor(unsigned int lex_cutoff) :
    _offsets_token_norm_raw(&_get_token_norm_raw),
    _w_im2_i("w", -2, 0, _offsets_token_norm_raw),
    _w_im1_i("w", -1, 0, _offsets_token_norm_raw),
    _w_i_ip1("w",  0, 1, _offsets_token_norm_raw),
    _w_i_ip2("w",  0, 2, _offsets_token_norm_raw),
    _w_im1_i_ip1("w", -1, 0, 1, _offsets_token_norm_raw),
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
