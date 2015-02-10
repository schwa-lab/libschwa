/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TAGGER_POS_H_
#define SCHWA_TAGGER_POS_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/canonical-schema.h>
#include <schwa/learn.h>


namespace schwa {
  namespace tagger {

    class POSExtractor {
    public:
      static constexpr const unsigned int DEFAULT_LEX_CUTOFF = 5;

    private:
      learn::SentinelOffsets<canonical_schema::Token> _offsets_token_norm_raw;
      //learn::Windower2<canonical_schema::Token> _w_im2_i;
      //learn::Windower2<canonical_schema::Token> _w_im1_i;
      //learn::Windower2<canonical_schema::Token> _w_i_ip1;
      //learn::Windower2<canonical_schema::Token> _w_i_ip2;
      //learn::Windower3<canonical_schema::Token> _w_im1_i_ip1;
      learn::Lexicon _lex_token;
      unsigned int _lex_cutoff;

    public:
      explicit POSExtractor(unsigned int lex_cutoff=DEFAULT_LEX_CUTOFF);

      void phase1_begin(void);
      void phase1_bod(canonical_schema::Doc &) { }
      void phase1_bos(canonical_schema::Sentence &) { }
      void phase1_end(void);
      void phase1_eod(canonical_schema::Doc &) { };
      void phase1_eos(canonical_schema::Sentence &) { };
      void phase1_extract(canonical_schema::Token &token, size_t i);

      void phase2_begin(void);
      void phase2_bod(canonical_schema::Doc &) { }
      void phase2_bos(canonical_schema::Sentence &sentence);
      void phase2_end(void);
      void phase2_eod(canonical_schema::Doc &) { }
      void phase2_eos(canonical_schema::Sentence &) { }
      void phase2_update_history(canonical_schema::Token &, size_t, const std::string &) { }

      std::string get_label(canonical_schema::Token &token) { return token.pos; }
      void set_label(canonical_schema::Token &token, const std::string &label) { token.pos = label; }

      template <typename TRANSFORM, typename VALUE>
      void phase2_extract(canonical_schema::Token &token, size_t i, learn::Features<TRANSFORM, VALUE> &features);

    private:
      static inline const std::string &
      _get_token_norm_raw(const canonical_schema::Token &t) {
        return t.norm.empty() ? t.raw : t.norm;
      }

      SCHWA_DISALLOW_COPY_AND_ASSIGN(POSExtractor);
    };


  }
}

#include <schwa/tagger/pos_impl.h>

#endif  // SCHWA_TAGGER_POS_H_
