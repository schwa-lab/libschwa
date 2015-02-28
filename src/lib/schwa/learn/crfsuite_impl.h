/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_CRFSUITE_IMPL_H_
#define SCHWA_LEARN_CRFSUITE_IMPL_H_

#include <cstring>
#include <sstream>

#include <schwa/io/logging.h>
#include <schwa/learn/features.h>


namespace schwa {
namespace learn {

// ========================================================================
// Helper to convert feature names to strings (e.g. feature hashed features)
// ========================================================================
template <typename T>
class FeatureToStringHelper {
private:
  std::ostringstream _ss;

public:
  inline std::string
  operator ()(const T &obj) {
    _ss.str("");
    _ss.clear();
    _ss << obj;
    return _ss.str();
  }
};

template <>
class FeatureToStringHelper<std::string> {
public:
  inline std::string
  operator ()(const std::string &obj) {
    return obj;
  }
};


// ========================================================================
// CRFsuiteTrainerPhase3SentenceExtractor
// ========================================================================
template <typename EXTRACTOR>
CRFsuiteTrainerPhase3SentenceExtractor<EXTRACTOR>::CRFsuiteTrainerPhase3SentenceExtractor(extractor_type &extractor, third_party::crfsuite::crfsuite_data_t &data) :
    _extractor(extractor),
    _data(data),
    _item(nullptr)
  { }

template <typename EXTRACTOR>
CRFsuiteTrainerPhase3SentenceExtractor<EXTRACTOR>::~CRFsuiteTrainerPhase3SentenceExtractor(void) { }


template <typename EXTRACTOR>
inline void
CRFsuiteTrainerPhase3SentenceExtractor<EXTRACTOR>::_begin_item_sequence(const size_t nitems) {
  using namespace third_party::crfsuite;
  // Initialise the item sequence with a known number of items.
  crfsuite_instance_init_n(&_instance, nitems);
  _item = _instance.items;
}


template <typename EXTRACTOR>
inline void
CRFsuiteTrainerPhase3SentenceExtractor<EXTRACTOR>::_end_item_sequence(void) {
  using namespace third_party::crfsuite;
  int ret;

  // Add the item sequence to the training data.
  ret = crfsuite_data_append(&_data, &_instance);
  if (SCHWA_UNLIKELY(ret != 0))
    throw_crfsuite_error("crfsuite_data_append", ret);

  // Deinitialise the item sequence.
  crfsuite_instance_finish(&_instance);
}


template <typename EXTRACTOR> template <typename TO_STRING, typename FEATURES>
inline void
CRFsuiteTrainerPhase3SentenceExtractor<EXTRACTOR>::_add_item(TO_STRING &to_string_helper, const FEATURES &features, const std::string &label) {
  using namespace third_party::crfsuite;

  // Initialise the item with a known number of attributes.
  crfsuite_item_init_n(_item, features.size());

  // Convert each of the provided features into attribute IDs and add them to the item.
  std::string attr_str;
  size_t a = 0;
  for (const auto &pair : features) {
    // Convert the feature value into a string and obtain its crfsuite attribute ID.
    attr_str.assign(to_string_helper(pair.first));
    const int attr_id = _data.attrs->get(_data.attrs, attr_str.c_str());

    // Set the attribute data and add the attribute to the item.
    crfsuite_attribute_set(&_item->contents[a++], attr_id, pair.second);
  }

  // Convert the label into its crfsuite label ID and add it to the item sequence.
  const int label_id = _data.labels->get(_data.labels, label.c_str());
  _instance.labels[_item - _instance.items] = label_id;

  // Increment the item pointer to the next pre-allocated item.
  ++_item;
}


template <typename EXTRACTOR> template <typename TO_STRING, typename FEATURES>
inline void
CRFsuiteTrainerPhase3SentenceExtractor<EXTRACTOR>::extract_sentence(canonical_schema::Sentence &sentence, TO_STRING &to_string_helper, FEATURES &features) {
  _extractor.phase3_bos(sentence);
  _begin_item_sequence(sentence.span.stop - sentence.span.start);

  for (canonical_schema::Token &token : sentence.span) {
    // Extract the features for the current token.
    _extractor.phase3_extract(sentence, token, features);

    // Add the features as an item in the current item sequence.
    _add_item(to_string_helper, features, _extractor.get_label(token));
    features.clear();
  }

  _end_item_sequence();
  _extractor.phase3_eos(sentence);

  // Update the history.
  for (canonical_schema::Token &token : sentence.span)
    _extractor.phase3_update_history(sentence, token, _extractor.get_label(token));
}


// ========================================================================
// CRFsuiteTrainer
// ========================================================================
template <typename EXTRACTOR>
CRFsuiteTrainer<EXTRACTOR>::CRFsuiteTrainer(EXTRACTOR &extractor, OutputModel &model, const CRFsuiteTrainerParams &params) :
    _model(model),
    _logger(model.logger()),
    _extractor(extractor),
    _sentence_extractor1(extractor),
    _sentence_extractor2(extractor),
    _sentence_extractor3(extractor, _data),
    _phase1_runner(_sentence_extractor1),
    _phase2_runner(_sentence_extractor2),
    _phase3_runner(_sentence_extractor3),
    _trainer(nullptr)
  {
  using namespace third_party::crfsuite;
  int ret;

  // Initialise crfsuite data.
  crfsuite_data_init(&_data);

  // Initialise the attribute and label dictionaries.
  ret = crfsuite_create_instance("dictionary", reinterpret_cast<void **>(&_data.attrs));
  if (SCHWA_UNLIKELY(ret != 0))
    throw_crfsuite_error("crfsuite_create_instance(\"dictionary\")", ret);
  ret = crfsuite_create_instance("dictionary", reinterpret_cast<void **>(&_data.labels));
  if (SCHWA_UNLIKELY(ret != 0))
    throw_crfsuite_error("crfsuite_create_instance(\"dictionary\")", ret);

  // Initialise the trainer.
  std::string tid = "train/crf1d/" + params.algorithm();
  ret = crfsuite_create_instance(tid.c_str(), reinterpret_cast<void **>(&_trainer));
  if (SCHWA_UNLIKELY(ret != 0)) {
    std::ostringstream ss;
    ss << "crfsuite_create_instance(\"" << tid << "\")";
    throw_crfsuite_error(ss.str(), ret);
  }
  _trainer->set_message_callback(_trainer, this, &_crfsuite_logging_callback);

  // Set the feature-related parameters.
  std::ostringstream ss;
  ss << params.feature_min_freq(); set_param("feature.minfreq", ss.str()); ss.str("");
  ss << params.feature_possible_states(); set_param("feature.possible_states", ss.str()); ss.str("");
  ss << params.feature_possible_transitions(); set_param("feature.possible_transitions", ss.str()); ss.str("");

  // Set the LBFGS-related parameters if we're using LBFGS.
  if (params.algorithm() == "lbfgs") {
    ss << params.c2(); set_param("c2", ss.str()); ss.str("");
    ss << params.max_iterations(); set_param("max_iterations", ss.str()); ss.str("");
    ss << params.num_memories(); set_param("num_memories", ss.str()); ss.str("");
    ss << params.epsilon(); set_param("epsilon", ss.str()); ss.str("");
    set_param("linesearch", params.line_search());
    ss << params.max_line_search_iterations(); set_param("max_linesearch", ss.str()); ss.str("");
  }
}

template <typename EXTRACTOR>
CRFsuiteTrainer<EXTRACTOR>::~CRFsuiteTrainer(void) {
  using namespace third_party::crfsuite;

  // Deinitialise the trainer.
  if (_trainer != nullptr) {
    _trainer->release(_trainer);
    _trainer = nullptr;
  }

  // Deinitialise the attribute and label dictionaries.
  if (_data.labels != nullptr) {
    _data.labels->release(_data.labels);
    _data.labels = nullptr;
  }
  if (_data.attrs != nullptr) {
    _data.attrs->release(_data.attrs);
    _data.attrs = nullptr;
  }

  // Deinitialise crfsuite data model objects.
  crfsuite_data_finish(&_data);
}


template <typename EXTRACTOR>
inline int
CRFsuiteTrainer<EXTRACTOR>::_crfsuite_logging_callback(void *const self, const char *const format, va_list args) {
  // Write the log string to the bufffer.
  char buffer[2048];
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
  const int nchars = std::vsnprintf(buffer, sizeof(buffer), format, args);
#pragma GCC diagnostic pop

  // Was the printf call was successful and did not overflow the buffer?
  if (nchars < 0) {
    std::ostringstream ss;
    ss << "Call to `vsnprintf` failed in crfsuite logging callback (ret=" << nchars << ")";
    static_cast<CRFsuiteTrainer<EXTRACTOR> *>(self)->_crfsuite_log(io::LogLevel::WARNING, ss.str().c_str());
  }
  else if (static_cast<unsigned int>(nchars) >= sizeof(buffer)) {
    std::ostringstream ss;
    ss << "Log message too large for buffer in crfsuite logging callback (nchars=" << nchars << ")";
    static_cast<CRFsuiteTrainer<EXTRACTOR> *>(self)->_crfsuite_log(io::LogLevel::WARNING, ss.str().c_str());
  }
  else {
    // If the last character written was a newline, log the line. Ignore otherwise.
    if (buffer[nchars - 1] == '\n') {
      buffer[nchars - 1] = '\0';
      static_cast<CRFsuiteTrainer<EXTRACTOR> *>(self)->_crfsuite_log(io::LogLevel::INFO, buffer);
    }
  }

  // Tell crfsuite to continue processing.
  return 0;
}


template <typename EXTRACTOR>
inline void
CRFsuiteTrainer<EXTRACTOR>::_crfsuite_log(const io::LogLevel level, const char *const msg) {
  LOGD2(level, _logger) << "CRFsuiteTrainer::_crfsuite_logging_callback " << msg << std::endl;
}


template <typename EXTRACTOR>
inline void
CRFsuiteTrainer<EXTRACTOR>::set_param(const std::string &key, const std::string &val) {
  using namespace third_party::crfsuite;
  int ret;

  crfsuite_params_t *const params = _trainer->params(_trainer);
  ret = params->set(params, key.c_str(), val.c_str());
  params->release(params);
  if (SCHWA_UNLIKELY(ret != 0)) {
    std::ostringstream ss;
    ss << "crfsuite_params_t::set(\"" << key << "\", \"" << val << "\")";
    throw_crfsuite_error(ss.str(), ret);
  }
}


template <typename EXTRACTOR>
inline void
CRFsuiteTrainer<EXTRACTOR>::train(void) {
  using namespace third_party::crfsuite;
  int ret;

  // Construct the path to the model file.
  const std::string path = _model.model_path() + _model_filename_suffix;

  // Train the model.
  LOG2(INFO, _logger) << "CRFsuiteTrainer::train begin (path=" << path << ")" << std::endl;
  ret = _trainer->train(_trainer, &_data, path.c_str(), -1);
  if (SCHWA_UNLIKELY(ret != 0))
    throw_crfsuite_error("crfsuite_trainer_t::train", ret);
  LOG2(INFO, _logger) << "CRFsuiteTrainer::train end" << std::endl;
}


template <typename EXTRACTOR> template <typename IT, typename TRANSFORM>
inline void
CRFsuiteTrainer<EXTRACTOR>::extract(const IT docs_begin, const IT docs_end, const TRANSFORM &transformer) {
  LOG2(INFO, _logger) << "CRFsuiteTrainer::extract begin" << std::endl;

  // Run phase 1.
  _extractor.phase1_begin();
  for (IT it = docs_begin; it != docs_end; ++it) {
    canonical_schema::Doc &doc = **it;
    _phase1_runner.extract(doc);
  }
  _extractor.phase1_end();

  // Create a Features instance.
  FeatureToStringHelper<typename TRANSFORM::value_type> to_string_helper;
  Features<TRANSFORM, third_party::crfsuite::floatval_t> features(transformer);

  // Run phase 2 and 3.
  for (IT it = docs_begin; it != docs_end; ++it) {
    canonical_schema::Doc &doc = **it;
    _phase2_runner.extract(doc);
    _phase3_runner.extract(doc, to_string_helper, features);
  }
  LOG2(INFO, _logger) << "CRFsuiteTrainer::extract end" << std::endl;
}


template <typename EXTRACTOR>
inline void
CRFsuiteTrainer<EXTRACTOR>::dump_crfsuite_data(io::OutputStream &out) const {
  LOG2(INFO, _logger) << "CRFsuiteTrainer::dump_crfsuite_data begin" << std::endl;
  learn::dump_crfsuite_data(out, _data);
  LOG2(INFO, _logger) << "CRFsuiteTrainer::dump_crfsuite_data end" << std::endl;
}



// ========================================================================
// CRFsuiteTaggerPhase3SentenceExtractor
// ========================================================================
template <typename EXTRACTOR>
CRFsuiteTaggerPhase3SentenceExtractor<EXTRACTOR>::CRFsuiteTaggerPhase3SentenceExtractor(extractor_type &extractor, const std::string &model_path) :
    _extractor(extractor),
    _cmodel(model_path),
    _item(nullptr)
  { }

template <typename EXTRACTOR>
CRFsuiteTaggerPhase3SentenceExtractor<EXTRACTOR>::~CRFsuiteTaggerPhase3SentenceExtractor(void) { }


template <typename EXTRACTOR>
inline void
CRFsuiteTaggerPhase3SentenceExtractor<EXTRACTOR>::_begin_item_sequence(const size_t nitems) {
  using namespace third_party::crfsuite;
  // Initialise the item sequence with a known number of items.
  crfsuite_instance_init_n(&_instance, nitems);
  _item = _instance.items;
}


template <typename EXTRACTOR>
inline void
CRFsuiteTaggerPhase3SentenceExtractor<EXTRACTOR>::_end_item_sequence(void) {
  // Set the taggers current instance to be this instance.
  _cmodel.set_instance(_instance);

  // Deinitialise the item sequence.
  crfsuite_instance_finish(&_instance);
}


template <typename EXTRACTOR> template <typename TO_STRING, typename FEATURES>
inline void
CRFsuiteTaggerPhase3SentenceExtractor<EXTRACTOR>::_add_item(TO_STRING &to_string_helper, const FEATURES &features) {
  using namespace third_party::crfsuite;
  int ret;

  // Initialise the item with an unknown number of attributes.
  crfsuite_item_init(_item);

  // Convert each of the provided features into attribute IDs and add them to the item.
  std::string attr_str;
  for (const auto &pair : features) {
    // Convert the feature value into a string and obtain its crfsuite attribute ID.
    attr_str.assign(to_string_helper(pair.first));
    const int attr_id = _cmodel.get_attr_id(attr_str);

    // Set the attribute data and add the attribute to the item, if the attribute was found.
    if (attr_id >= 0) {
      crfsuite_attribute_t attr;
      crfsuite_attribute_set(&attr, attr_id, pair.second);
      ret = crfsuite_item_append_attribute(_item, &attr);
      if (SCHWA_UNLIKELY(ret != 0))
        throw_crfsuite_error("crfsuite_item_append_attribute", ret);
    }
  }

  // Increment the item pointer to the next pre-allocated item.
  ++_item;
}


template <typename EXTRACTOR> template <typename TO_STRING, typename FEATURES>
inline void
CRFsuiteTaggerPhase3SentenceExtractor<EXTRACTOR>::extract_sentence(canonical_schema::Sentence &sentence, TO_STRING &to_string_helper, FEATURES &features) {
  // Start the item sequence.
  const size_t sentence_length = sentence.span.stop - sentence.span.start;
  _extractor.phase3_bos(sentence);
  _begin_item_sequence(sentence_length);

  for (canonical_schema::Token &token : sentence.span) {
    // Extract the features for the current token.
    _extractor.phase3_extract(sentence, token, features);

    // Add the features as an item in the current item sequence.
    _add_item(to_string_helper, features);
    features.clear();
  }

  // End the item sequence.
  _end_item_sequence();
  _extractor.phase3_eos(sentence);

  // Ensure our sequence label array is large enough to house the sentence.
  if (sentence_length > _label_ids.size())
    _label_ids.resize(sentence_length);

  // Run viterbi over the current item sequence.
  third_party::crfsuite::floatval_t score;
  _cmodel.viterbi(&_label_ids[0], &score);

  for (canonical_schema::Token &token : sentence.span) {
    // Set the string representation of the assigned label on the token.
    const int label_id = _label_ids[&token - sentence.span.start];
    const std::string label_string = _cmodel.get_label_string(label_id);
    _extractor.set_label(token, label_string);

    // Update history.
    _extractor.phase3_update_history(sentence, token, label_string);
  }
}


// ========================================================================
// CRFsuiteTagger
// ========================================================================
template <typename EXTRACTOR>
CRFsuiteTagger<EXTRACTOR>::CRFsuiteTagger(EXTRACTOR &extractor, const std::string &model_path) :
    _sentence_extractor2(extractor),
    _sentence_extractor3(extractor, model_path),
    _phase2_runner(_sentence_extractor2),
    _phase3_runner(_sentence_extractor3)
  { }

template <typename EXTRACTOR>
CRFsuiteTagger<EXTRACTOR>::~CRFsuiteTagger(void) { }


template <typename EXTRACTOR> template <typename IT, typename TRANSFORM>
inline void
CRFsuiteTagger<EXTRACTOR>::tag(const IT docs_begin, const IT docs_end, const TRANSFORM &transformer) {
  FeatureToStringHelper<typename TRANSFORM::value_type> to_string_helper;
  Features<TRANSFORM, third_party::crfsuite::floatval_t> features(transformer);

  for (IT it = docs_begin; it != docs_end; ++it) {
    canonical_schema::Doc &doc = **it;
    _phase2_runner.extract(doc);
    _phase3_runner.extract(doc, to_string_helper, features);
  }
}


template <typename EXTRACTOR> template <typename TRANSFORM>
inline void
CRFsuiteTagger<EXTRACTOR>::tag(canonical_schema::Doc &doc, const TRANSFORM &transformer) {
  FeatureToStringHelper<typename TRANSFORM::value_type> to_string_helper;
  Features<TRANSFORM, third_party::crfsuite::floatval_t> features(transformer);

  _phase2_runner.extract(doc);
  _phase3_runner.extract(doc, to_string_helper, features);
}

}  // namespace learn
}  // namespace schwa

#endif  // SCHWA_LEARN_CRFSUITE_IMPL_H_
