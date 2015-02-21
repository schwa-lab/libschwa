/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_CRFSUITE_H_
#define SCHWA_LEARN_CRFSUITE_H_

#include <iosfwd>
#include <string>
#include <vector>

#include <schwa/_base.h>
#include <schwa/config.h>
#include <schwa/canonical-schema.h>
#include <schwa/io/streams.h>
#include <schwa/learn/io.h>
#include <schwa/learn/model.h>
#include <schwa/learn/phases.h>
#include <schwa/third-party/crfsuite/crfsuite.h>


namespace schwa {
  namespace learn {

    // ========================================================================
    // CRFsuite API helpers.
    // ========================================================================
    void throw_crfsuite_error(const std::string &api_call, int ret);


    // ========================================================================
    // CRFsuite flat file format helper functions
    // ========================================================================
    void dump_crfsuite_data(std::ostream &out, const third_party::crfsuite::crfsuite_data_t &data);
    void dump_crfsuite_value(std::ostream &out, const char *value);


    // ========================================================================
    // CRFsuiteTrainerParams
    // ========================================================================
    class CRFsuiteTrainerParams : public config::Group {
    public:
      config::OpChoices<std::string> algorithm;
      config::Op<double> c2;
      config::Op<unsigned int> max_iterations;
      config::Op<unsigned int> num_memories;
      config::Op<double> epsilon;
      config::OpChoices<std::string> line_search;
      config::Op<unsigned int> max_line_search_iterations;
      config::Op<unsigned int> feature_min_freq;
      config::Op<bool> feature_possible_states;
      config::Op<bool> feature_possible_transitions;

      CRFsuiteTrainerParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags=config::Flags::NONE);
      virtual ~CRFsuiteTrainerParams(void);
    };


    // ========================================================================
    // CRFsuiteTrainerPhase3SentenceExtractor
    // ========================================================================
    template <typename EXTRACTOR>
    class CRFsuiteTrainerPhase3SentenceExtractor {
    public:
      static constexpr const unsigned int PHASE = 3;
      using extractor_type = EXTRACTOR;

    private:
      extractor_type &_extractor;
      third_party::crfsuite::crfsuite_data_t &_data;
      third_party::crfsuite::crfsuite_instance_t _instance;
      third_party::crfsuite::crfsuite_item_t *_item;

      void _begin_item_sequence(size_t nitems);
      void _end_item_sequence(void);

      template <typename TO_STRING, typename FEATURES>
      void _add_item(TO_STRING &to_string_helper, const FEATURES &features, const std::string &label);

    public:
      CRFsuiteTrainerPhase3SentenceExtractor(extractor_type &extractor, third_party::crfsuite::crfsuite_data_t &data);
      ~CRFsuiteTrainerPhase3SentenceExtractor(void);

      inline extractor_type &extractor(void) { return _extractor; }

      template <typename TO_STRING, typename FEATURES>
      void extract_sentence(canonical_schema::Sentence &sentence, TO_STRING &to_string_helper, FEATURES &features);
    };


    // ========================================================================
    // CRFsuiteTrainer
    // ========================================================================
    template <typename EXTRACTOR>
    class CRFsuiteTrainer {
    private:
      static int _crfsuite_logging_callback(void *self, const char *format, va_list args);

    private:
      OutputModel &_model;
      io::Logger &_logger;
      EXTRACTOR &_extractor;
      BasicSenteceExtractor<EXTRACTOR, 1> _sentence_extractor1;
      BasicSenteceExtractor<EXTRACTOR, 2> _sentence_extractor2;
      CRFsuiteTrainerPhase3SentenceExtractor<EXTRACTOR> _sentence_extractor3;
      ExtractionPhaseRunner<BasicSenteceExtractor<EXTRACTOR, 1>> _phase1_runner;
      ExtractionPhaseRunner<BasicSenteceExtractor<EXTRACTOR, 2>> _phase2_runner;
      ExtractionPhaseRunner<CRFsuiteTrainerPhase3SentenceExtractor<EXTRACTOR>> _phase3_runner;
      std::string _model_filename_suffix;
      third_party::crfsuite::crfsuite_data_t _data;
      third_party::crfsuite::crfsuite_trainer_t *_trainer;

      void _crfsuite_log(io::LogLevel level, const char *msg);

    public:
      CRFsuiteTrainer(EXTRACTOR &extractor, OutputModel &model, const CRFsuiteTrainerParams &params);
      ~CRFsuiteTrainer(void);

      void dump_crfsuite_data(io::OutputStream &out) const;

      std::string get_param(const std::string &key) const;
      void set_param(const std::string &key, const std::string &val);

      void set_model_filename_suffix(const std::string &suffix) { _model_filename_suffix = suffix; }

      template <typename IT, typename TRANSFORM>
      void extract(const IT docs_begin, const IT docs_end, const TRANSFORM &transformer=TRANSFORM());

      void train(void);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(CRFsuiteTrainer);
    };


    // ========================================================================
    // CRFsuiteLoadedModel
    // ========================================================================
    class CRFsuiteLoadedModel {
    private:
      third_party::crfsuite::crfsuite_model_t *_model;
      third_party::crfsuite::crfsuite_tagger_t *_tagger;
      third_party::crfsuite::crfsuite_dictionary_t *_attrs;
      third_party::crfsuite::crfsuite_dictionary_t *_labels;
      std::vector<const char *> _label_strings;

    public:
      explicit CRFsuiteLoadedModel(const std::string &model_path);
      ~CRFsuiteLoadedModel(void);

      inline int
      get_attr_id(const std::string &attr_str) {
        return _attrs->to_id(_attrs, attr_str.c_str());
      }

      inline const char *
      get_label_string(const int label_id) const {
        return _label_strings[label_id];
      }

      inline void
      set_instance(third_party::crfsuite::crfsuite_instance_t &instance) {
        const int ret = _tagger->set(_tagger, &instance);
        if (SCHWA_UNLIKELY(ret != 0))
          throw_crfsuite_error("tagger->set", ret);
      }

      inline void
      viterbi(int *const label_ids, third_party::crfsuite::floatval_t *const score) {
        const int ret = _tagger->viterbi(_tagger, label_ids, score);
        if (SCHWA_UNLIKELY(ret != 0))
          throw_crfsuite_error("tagger->viterbi", ret);
      }
    };


    // ========================================================================
    // CRFsuiteTaggerPhase3SentenceExtractor
    // ========================================================================
    template <typename EXTRACTOR>
    class CRFsuiteTaggerPhase3SentenceExtractor {
    public:
      static constexpr const unsigned int PHASE = 3;
      using extractor_type = EXTRACTOR;

    private:
      extractor_type &_extractor;
      CRFsuiteLoadedModel _cmodel;
      third_party::crfsuite::crfsuite_instance_t _instance;
      third_party::crfsuite::crfsuite_item_t *_item;
      std::vector<int> _label_ids;
      unsigned int _ntokens_correct;
      unsigned int _ntokens_total;
      unsigned int _nsentences_correct;
      unsigned int _nsentences_total;

      void _begin_item_sequence(size_t nitems);
      void _end_item_sequence(void);

      template <typename TO_STRING, typename FEATURES>
      void _add_item(TO_STRING &to_string_helper, const FEATURES &features);

    public:
      CRFsuiteTaggerPhase3SentenceExtractor(extractor_type &extractor, const std::string &model_path);
      ~CRFsuiteTaggerPhase3SentenceExtractor(void);

      void dump_accuracy(void) const;
      inline extractor_type &extractor(void) { return _extractor; }

      template <typename TO_STRING, typename FEATURES>
      void extract_sentence(canonical_schema::Sentence &sentence, TO_STRING &to_string_helper, FEATURES &features);
    };


    // ========================================================================
    // CRFsuiteTagger
    // ========================================================================
    template <typename EXTRACTOR>
    class CRFsuiteTagger {
    private:
      BasicSenteceExtractor<EXTRACTOR, 2> _sentence_extractor2;
      CRFsuiteTaggerPhase3SentenceExtractor<EXTRACTOR> _sentence_extractor3;
      ExtractionPhaseRunner<BasicSenteceExtractor<EXTRACTOR, 2>> _phase2_runner;
      ExtractionPhaseRunner<CRFsuiteTaggerPhase3SentenceExtractor<EXTRACTOR>> _phase3_runner;

    public:
      CRFsuiteTagger(EXTRACTOR &extractor, const std::string &model_path);
      ~CRFsuiteTagger(void);

      template <typename IT, typename TRANSFORM>
      void tag(const IT docs_begin, const IT docs_end, const TRANSFORM &transformer=TRANSFORM());
      template <typename TRANSFORM>
      void tag(canonical_schema::Doc &doc, const TRANSFORM &transformer=TRANSFORM());

      void dump_accuracy(void) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(CRFsuiteTagger);
    };


  }
}

#include <schwa/learn/crfsuite_impl.h>

#endif  // SCHWA_LEARN_CRFSUITE_H_
