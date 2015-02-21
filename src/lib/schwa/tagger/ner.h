/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_TAGGER_NER_H_
#define SCHWA_TAGGER_NER_H_

#include <list>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <schwa/_base.h>
#include <schwa/config.h>
#include <schwa/canonical-schema.h>
#include <schwa/io/logging.h>
#include <schwa/learn.h>
#include <schwa/lex/brown-clusters.h>
#include <schwa/lex/gazetteer.h>
#include <schwa/lex/word-embeddings.h>
#include <schwa/third-party/re2/re2.h>


namespace schwa {
  namespace tagger {
    namespace ner {

      class ModelParams : public learn::ModelParams {
      public:
        config::OpSequenceTagEncoding tag_encoding;
        config::Op<unsigned int> feature_hashing;
        config::Op<std::string> brown_clusters_path;
        config::Op<std::string> gazetteer_path;
        config::Op<std::string> word_embeddings_path;
        config::Op<double> word_embeddings_sigma;

        ModelParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags=config::Flags::NONE);
        virtual ~ModelParams(void);

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(ModelParams);
      };


      class InputModel : public learn::InputModel {
      protected:
        Pool _pool;
        StringPool _string_pool;
        lex::BrownClusters _brown_clusters;
        lex::Gazetteer _gazetteer;
        lex::WordEmbeddings _word_embeddings;
        SequenceTagEncoding _tag_encoding;

      public:
        InputModel(const std::string &path, ModelParams &params);
        virtual ~InputModel(void);

        const lex::BrownClusters &brown_clusters(void) const { return _brown_clusters; }
        const lex::Gazetteer &gazetteer(void) const { return _gazetteer; }
        SequenceTagEncoding tag_encoding(void) const { return _tag_encoding; }
        const lex::WordEmbeddings &word_embeddings(void) const { return _word_embeddings; }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(InputModel);
      };


      class OutputModel : public learn::OutputModel {
      protected:
        Pool _pool;
        StringPool _string_pool;
        lex::BrownClusters _brown_clusters;
        lex::Gazetteer _gazetteer;
        lex::WordEmbeddings _word_embeddings;
        SequenceTagEncoding _tag_encoding;

      public:
        OutputModel(const std::string &path, const ModelParams &params, const config::Main &main_config);
        virtual ~OutputModel(void);

        const lex::BrownClusters &brown_clusters(void) const { return _brown_clusters; }
        const lex::Gazetteer &gazetteer(void) const { return _gazetteer; }
        SequenceTagEncoding tag_encoding(void) const { return _tag_encoding; }
        const lex::WordEmbeddings &word_embeddings(void) const { return _word_embeddings; }

      private:
        SCHWA_DISALLOW_COPY_AND_ASSIGN(OutputModel);
      };


      /**
       * Preprocesses a document before use in any of the NER tools. This performs a bunch of
       * things such as attempting to truecase all-caps sentences, and performing some
       * normalisation on token surface forms to provide some generalisation to the NER model, such
       * as normalising all numbers and ordinals to a common value.
       **/
      void preprocess_doc(canonical_schema::Doc &doc, const lex::BrownClusters &brown_clusters);


      class Extractor {
      public:
        static const third_party::re2::RE2 RE_ACRONYM;
        static const third_party::re2::RE2 RE_ORDINAL;
        static const third_party::re2::RE2 RE_ROMAN_NUMERAL;
        static const third_party::re2::RE2 RE_PERSON_INITIAL_1;
        static const third_party::re2::RE2 RE_PERSON_INITIAL_2;

      private:
        const bool _is_train;
        const bool _is_second_stage;
        const bool _is_threaded;
        const SequenceTagEncoding _tag_encoding;
        const lex::BrownClusters &_brown_clusters;
        const uint8_t *_brown_cluster_path;
        unsigned int *const _brown_cluster_path_lengths;
        char *const _brown_cluster_feature;
        const lex::Gazetteer &_gazetteer;
        std::vector<uint8_t> _gazetteer_match;
        const lex::WordEmbeddings &_word_embeddings;
        io::Logger &_logger;
        learn::SentinelOffsets<canonical_schema::Token> _offsets_token_ne_normalised;
        learn::SentinelOffsets<canonical_schema::Token> _offsets_token_norm_raw;
        std::unordered_map<std::string, std::list<std::tuple<std::string, unsigned int>>> _token_tag_counts;

        void _check_regular_expressions(void) const;

      public:
        Extractor(InputModel &model, bool is_second_stage, bool is_threaded);
        Extractor(OutputModel &model, bool is_second_stage, bool is_threaded);
        ~Extractor(void);

        void phase1_begin(void) { }
        void phase1_bod(canonical_schema::Doc &) { }
        void phase1_bos(canonical_schema::Sentence &) { }
        void phase1_end(void) { }
        void phase1_eod(canonical_schema::Doc &) { };
        void phase1_eos(canonical_schema::Sentence &) { };
        void phase1_extract(canonical_schema::Sentence &, canonical_schema::Token &) { }

        static void prepare_doc(canonical_schema::Doc &doc, bool is_second_stage, bool is_train, SequenceTagEncoding tag_encoding);

        void phase2_bod(canonical_schema::Doc &doc);
        void phase2_bos(canonical_schema::Sentence &sentence);
        void phase2_extract(canonical_schema::Sentence &sentence, canonical_schema::Token &token);
        void phase2_eos(canonical_schema::Sentence &) { }
        void phase2_eod(canonical_schema::Doc &) { }

        void phase3_bod(canonical_schema::Doc &) { }
        void phase3_bos(canonical_schema::Sentence &sentence);
        template <typename TRANSFORM, typename VALUE>
        void phase3_extract(canonical_schema::Sentence &sentence, canonical_schema::Token &token, learn::Features<TRANSFORM, VALUE> &features);
        void phase3_update_history(canonical_schema::Sentence &sentence, canonical_schema::Token &token, const std::string &label_string);
        void phase3_eos(canonical_schema::Sentence &) { }
        void phase3_eod(canonical_schema::Doc &doc);

        std::string get_label(canonical_schema::Token &token) { return token.ne_label; }
        void set_label(canonical_schema::Token &token, const std::string &label) { token.ne_label = label; }

      private:
        static inline const std::string &
        _get_token_norm_raw(const canonical_schema::Token &t) {
          return t.get_norm_raw();
        }

        static inline const std::string &
        _get_token_ne_normalised(const canonical_schema::Token &t) {
          return t.ne_normalised;
        }

        SCHWA_DISALLOW_COPY_AND_ASSIGN(Extractor);
      };

    }
  }
}

#include <schwa/tagger/ner_impl.h>

#endif  // SCHWA_TAGGER_NER_H_
