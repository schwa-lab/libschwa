/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TAGGER_POS_H_
#define SCHWA_TAGGER_POS_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/config.h>
#include <schwa/canonical-schema.h>
#include <schwa/learn.h>


namespace schwa {
  namespace tagger {

    class POSModelParams : public learn::ModelParams {
    public:
      static const unsigned int DEFAULT_RARE_TOKEN_CUTOFF;

      config::Op<std::string> lexicon_path;
      config::Op<unsigned int> rare_token_cutoff;

      POSModelParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags=config::Flags::NONE);
      virtual ~POSModelParams(void);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(POSModelParams);
    };


    class POSInputModel : public learn::InputModel {
    protected:
      std::string _lexicon_path;
      unsigned int _rare_token_cutoff;
      learn::Lexicon _lexicon;

    public:
      POSInputModel(const std::string &path, POSModelParams &params);
      virtual ~POSInputModel(void);

      learn::Lexicon &lexicon(void) { return _lexicon; }
      unsigned int rare_token_cutoff(void) const { return _rare_token_cutoff; }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(POSInputModel);
    };


    class POSOutputModel : public learn::OutputModel {
    protected:
      const std::string _lexicon_path;
      const unsigned int _rare_token_cutoff;
      learn::Lexicon _lexicon;

    public:
      POSOutputModel(const std::string &path, const POSModelParams &params, const config::Main &main_config);
      virtual ~POSOutputModel(void);

      learn::Lexicon &lexicon(void) { return _lexicon; }
      unsigned int rare_token_cutoff(void) const { return _rare_token_cutoff; }

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(POSOutputModel);
    };


    class POSExtractor {
    private:
      const bool _is_train;
      const unsigned int _rare_token_cutoff;
      learn::Lexicon &_lexicon;
      learn::SentinelOffsets<canonical_schema::Token> _offsets_token_norm_raw;

    public:
      explicit POSExtractor(POSInputModel &model);
      explicit POSExtractor(POSOutputModel &model);

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
