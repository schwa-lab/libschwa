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
#include <schwa/third-party/crfsuite/crfsuite.h>


namespace schwa {
  namespace learn {

    // ========================================================================
    // CRFsuite API helpers.
    // ========================================================================
    void throw_crfsuite_error(const std::string &api_call, int ret);


    // ========================================================================
    // crfsuite flat file format helper functions
    // ========================================================================
    void dump_crfsuite_data(std::ostream &out, const third_party::crfsuite::crfsuite_data_t &data);
    void dump_crfsuite_value(std::ostream &out, const char *value);


    // ========================================================================
    // CRFSuiteTrainerParams
    // ========================================================================
    class CRFSuiteTrainerParams : public config::Group {
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

      CRFSuiteTrainerParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags=config::Flags::NONE);
      virtual ~CRFSuiteTrainerParams(void);
    };


    // ========================================================================
    // CRFSuiteTrainer
    // ========================================================================
    template <typename EXTRACTOR>
    class CRFSuiteTrainer {
    private:
      static int _crfsuite_logging_callback(void *self, const char *format, va_list args);

    private:
      OutputModel &_model;
      io::Logger &_logger;
      EXTRACTOR &_extractor;
      std::string _model_filename_suffix;
      third_party::crfsuite::crfsuite_data_t _data;
      third_party::crfsuite::crfsuite_instance_t _instance;
      third_party::crfsuite::crfsuite_item_t *_item;
      third_party::crfsuite::crfsuite_trainer_t *_trainer;

      void _crfsuite_log(io::LogLevel level, const char *msg);

      void _begin_item_sequence(size_t nitems);
      void _end_item_sequence(void);

      template <typename TO_STRING, typename FEATURES>
      void _add_item(TO_STRING &to_string_helper, const FEATURES &features, const std::string &label);

    public:
      CRFSuiteTrainer(EXTRACTOR &extractor, OutputModel &model, const CRFSuiteTrainerParams &params);
      ~CRFSuiteTrainer(void);

      void set_model_filename_suffix(const std::string &suffix) { _model_filename_suffix = suffix; }
      std::string get_param(const std::string &key) const;
      void set_param(const std::string &key, const std::string &val);

      template <typename IT, typename TRANSFORM>
      void extract(const IT docs_begin, const IT docs_end, const TRANSFORM &transformer=TRANSFORM());

      void dump_crfsuite_data(io::OutputStream &out) const;
      void train(void);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(CRFSuiteTrainer);
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
    // CRFSuiteTagger
    // ========================================================================
    template <typename EXTRACTOR>
    class CRFSuiteTagger {
    private:
      EXTRACTOR &_extractor;
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

      template <typename TO_STRING, typename FEATURES>
      void _tag(canonical_schema::Doc &doc, TO_STRING &to_string_helper, FEATURES &features);

    public:
      CRFSuiteTagger(EXTRACTOR &extractor, const std::string &model_path);
      ~CRFSuiteTagger(void);

      template <typename IT, typename TRANSFORM>
      void tag(const IT docs_begin, const IT docs_end, const TRANSFORM &transformer=TRANSFORM());
      template <typename TRANSFORM>
      void tag(canonical_schema::Doc &doc, const TRANSFORM &transformer=TRANSFORM());

      void dump_accuracy(void) const;

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(CRFSuiteTagger);
    };


  }
}

#include <schwa/learn/crfsuite_impl.h>

#endif  // SCHWA_LEARN_CRFSUITE_H_
