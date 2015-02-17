/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TAGGER_POS_H_
#define SCHWA_TAGGER_POS_H_

#include <string>

#include <schwa/_base.h>
#include <schwa/config.h>
#include <schwa/canonical-schema.h>
#include <schwa/io/logging.h>
#include <schwa/learn.h>
#include <schwa/lex/brown-clusters.h>


namespace schwa {
  namespace tagger {
    namespace pos {

      class ModelParams : public learn::ModelParams {
      public:
        static const unsigned int DEFAULT_RARE_TOKEN_CUTOFF;

        config::Op<unsigned int> feature_hashing;
        config::Op<std::string> lexicon_path;
        config::Op<unsigned int> rare_token_cutoff;
        config::Op<std::string> brown_clusters_path;

        ModelParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags=config::Flags::NONE);
        virtual ~ModelParams(void);

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(ModelParams);
      };


      class InputModel : public learn::InputModel {
      protected:
        Pool _pool;
        StringPool _string_pool;
        std::string _lexicon_path;
        unsigned int _rare_token_cutoff;
        learn::Lexicon _lexicon;
        lex::BrownClusters _brown_clusters;

      public:
        InputModel(const std::string &path, ModelParams &params);
        virtual ~InputModel(void);

        learn::Lexicon &lexicon(void) { return _lexicon; }
        lex::BrownClusters &brown_clusters(void) { return _brown_clusters; }
        unsigned int rare_token_cutoff(void) const { return _rare_token_cutoff; }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(InputModel);
      };


      class OutputModel : public learn::OutputModel {
      protected:
        Pool _pool;
        StringPool _string_pool;
        const std::string _lexicon_path;
        const unsigned int _rare_token_cutoff;
        learn::Lexicon _lexicon;
        lex::BrownClusters _brown_clusters;

      public:
        OutputModel(const std::string &path, const ModelParams &params, const config::Main &main_config);
        virtual ~OutputModel(void);

        learn::Lexicon &lexicon(void) { return _lexicon; }
        lex::BrownClusters &brown_clusters(void) { return _brown_clusters; }
        unsigned int rare_token_cutoff(void) const { return _rare_token_cutoff; }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(OutputModel);
      };


      class Extractor {
      private:
        const bool _is_train;
        const unsigned int _rare_token_cutoff;
        learn::Lexicon &_lexicon;
        const lex::BrownClusters &_brown_clusters;
        const uint8_t *_brown_cluster_path;
        unsigned int *const _brown_cluster_path_lengths;
        char *const _brown_cluster_feature;
        io::Logger &_logger;
        learn::SentinelOffsets<canonical_schema::Token> _offsets_token_norm_raw;

      public:
        explicit Extractor(InputModel &model);
        explicit Extractor(OutputModel &model);
        ~Extractor(void);

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

        SCHWA_DISALLOW_COPY_AND_ASSIGN(Extractor);
      };


    }
  }
}

#include <schwa/tagger/pos_impl.h>

#endif  // SCHWA_TAGGER_POS_H_
