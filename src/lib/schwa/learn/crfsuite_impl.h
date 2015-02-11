/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_CRFSUITE_IMPL_H_
#define SCHWA_LEARN_CRFSUITE_IMPL_H_

#include <sstream>

#include <schwa/exception.h>
#include <schwa/io/logging.h>
#include <schwa/learn/features.h>


namespace schwa {
  namespace learn {

    // ========================================================================
    // Helper to convert feature names to strings (e.g. feature hashed features)
    // ========================================================================
    template <typename T>
    class _FeatureToStringHelper {
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
    class _FeatureToStringHelper<std::string> {
    public:
      inline std::string
      operator ()(const std::string &obj) {
        return obj;
      }
    };


    // ========================================================================
    // CRFSuiteTrainer
    // ========================================================================
    template <typename EXTRACTOR>
    CRFSuiteTrainer<EXTRACTOR>::CRFSuiteTrainer(EXTRACTOR &extractor, OutputModel &model, const CRFSuiteTrainerParams &params) :
        _model(model),
        _logger(model.logger()),
        _extractor(extractor),
        _trainer(nullptr)
      {
      using namespace ::schwa::third_party::crfsuite;
      int ret;

      // Initialise crfsuite data.
      crfsuite_data_init(&_data);

      // Initialise the attribute and label dictionaries.
      ret = crfsuite_create_instance("dictionary", reinterpret_cast<void **>(&_data.attrs));
      if (ret != 0)
        _crfsuite_error("crfsuite_create_instance(\"dictionary\")", ret);
      ret = crfsuite_create_instance("dictionary", reinterpret_cast<void **>(&_data.labels));
      if (ret != 0)
        _crfsuite_error("crfsuite_create_instance(\"dictionary\")", ret);

      // Initialise the trainer.
      std::string tid = "train/crf1d/" + params.algorithm();
      ret = crfsuite_create_instance(tid.c_str(), reinterpret_cast<void **>(&_trainer));
      if (ret != 0) {
        std::ostringstream ss;
        ss << "crfsuite_create_instance(\"" << tid << "\")";
        _crfsuite_error(ss.str(), ret);
      }
      _trainer->set_message_callback(_trainer, this, &_crfsuite_logging_callback);

      // Set the LBFGS-related parameters if we're using LBFGS.
      if (params.algorithm() == "lbfgs") {
        std::ostringstream ss;

        ss << params.c2(); set_param("c2", ss.str()); ss.str("");
        ss << params.max_iterations(); set_param("max_iterations", ss.str()); ss.str("");
        ss << params.num_memories(); set_param("num_memories", ss.str()); ss.str("");
        ss << params.epsilon(); set_param("epsilon", ss.str()); ss.str("");
        set_param("linesearch", params.line_search());
        ss << params.max_line_search_iterations(); set_param("max_linesearch", ss.str()); ss.str("");
      }
    }

    template <typename EXTRACTOR>
    CRFSuiteTrainer<EXTRACTOR>::~CRFSuiteTrainer(void) {
      using namespace ::schwa::third_party::crfsuite;

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
    CRFSuiteTrainer<EXTRACTOR>::_crfsuite_logging_callback(void *const self, const char *const format, va_list args) {
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
        static_cast<CRFSuiteTrainer<EXTRACTOR> *>(self)->_crfsuite_log(io::LogLevel::WARNING, ss.str().c_str());
      }
      else if (static_cast<unsigned int>(nchars) >= sizeof(buffer)) {
        std::ostringstream ss;
        ss << "Log message too large for buffer in crfsuite logging callback (nchars=" << nchars << ")";
        static_cast<CRFSuiteTrainer<EXTRACTOR> *>(self)->_crfsuite_log(io::LogLevel::WARNING, ss.str().c_str());
      }
      else {
        // If the last character written was a newline, log the line. Ignore otherwise.
        if (buffer[nchars - 1] == '\n') {
          buffer[nchars - 1] = '\0';
          static_cast<CRFSuiteTrainer<EXTRACTOR> *>(self)->_crfsuite_log(io::LogLevel::INFO, buffer);
        }
      }

      // Tell crfsuite to continue processing.
      return 0;
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::_crfsuite_error(const std::string &api_call, const int ret) {
      std::ostringstream ss;
      ss << "crfsuite API call `" << api_call << "` failed (ret=" << ret << ")";
      throw Exception(ss.str());
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::_crfsuite_log(const io::LogLevel level, const char *const msg) {
      LOGD2(level, _logger) << "CRFSuiteTrainer::_crfsuite_logging_callback " << msg << std::endl;
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::_begin_item_sequence(const size_t nitems) {
      using namespace ::schwa::third_party::crfsuite;
      // Initialise the item sequence with a known number of items.
      crfsuite_instance_init_n(&_instance, nitems);
      _item = _instance.items;
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::_end_item_sequence(void) {
      using namespace ::schwa::third_party::crfsuite;
      int ret;

      // Add the item sequence to the training data.
      ret = crfsuite_data_append(&_data, &_instance);
      if (ret != 0)
        _crfsuite_error("crfsuite_data_append", ret);

      // Deinitialise the item sequence.
      crfsuite_instance_finish(&_instance);
    }


    template <typename EXTRACTOR> template <typename TO_STRING, typename FEATURES>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::_add_item(TO_STRING &to_string_helper, const FEATURES &features, const std::string &label) {
      using namespace ::schwa::third_party::crfsuite;

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


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::set_param(const std::string &key, const std::string &val) {
      using namespace ::schwa::third_party::crfsuite;
      int ret;

      crfsuite_params_t *const params = _trainer->params(_trainer);
      ret = params->set(params, key.c_str(), val.c_str());
      params->release(params);
      if (ret != 0) {
        std::ostringstream ss;
        ss << "crfsuite_params_t::set(\"" << key << "\", \"" << val << "\")";
        _crfsuite_error(ss.str(), ret);
      }
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::train(void) {
      using namespace ::schwa::third_party::crfsuite;
      int ret;

      LOG2(INFO, _logger) << "CRFSuiteTrainer::train begin" << std::endl;
      ret = _trainer->train(_trainer, &_data, _model.model_path().c_str(), -1);
      if (ret != 0)
        _crfsuite_error("crfsuite_trainer_t::train", ret);
      LOG2(INFO, _logger) << "CRFSuiteTrainer::train end" << std::endl;
    }


    template <typename EXTRACTOR> template <typename TRANSFORM>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::extract(ResettableDocrepReader<canonical_schema::Doc> &doc_reader, const TRANSFORM &transformer) {
      LOG2(INFO, _logger) << "CRFSuiteTrainer::exact begin" << std::endl;
      _FeatureToStringHelper<typename TRANSFORM::value_type> to_string_helper;

      canonical_schema::Doc *doc;
      size_t ndocs_read;

      // Run phase 1.
      _extractor.phase1_begin();
      for (ndocs_read = 0; (doc = doc_reader.next()) != nullptr; ++ndocs_read) {
        _extractor.phase1_bod(*doc);
        for (canonical_schema::Sentence &sentence : doc->sentences) {
          _extractor.phase1_bos(sentence);
          size_t i = 0;
          for (canonical_schema::Token &token : sentence.span) {
            _extractor.phase1_extract(token, i++);
          }
          _extractor.phase1_eos(sentence);
        }
        _extractor.phase1_eod(*doc);
      }
      _extractor.phase1_end();
      LOG2(INFO, _logger) << "CRFSuiteTrainer::extract ndocs_read=" << ndocs_read << std::endl;

      // Reset the document reader.
      doc_reader.reset();

      // Create a Features instance.
      Features<TRANSFORM, ::schwa::third_party::crfsuite::floatval_t> features(transformer);

      // Run phase 2.
      _extractor.phase2_begin();
      for (ndocs_read = 0; (doc = doc_reader.next()) != nullptr; ++ndocs_read) {
        _extractor.phase2_bod(*doc);
        for (canonical_schema::Sentence &sentence : doc->sentences) {
          _extractor.phase2_bos(sentence);
          _begin_item_sequence(sentence.span.stop - sentence.span.start);

          size_t i = 0;
          for (canonical_schema::Token &token : sentence.span) {
            // Extract the features for the current token.
            _extractor.phase2_extract(token, i++, features);

            // Add the features as an item in the current item sequence.
            _add_item(to_string_helper, features, _extractor.get_label(token));
            features.clear();
          }

          _end_item_sequence();
          _extractor.phase2_eos(sentence);
        }
        _extractor.phase2_eod(*doc);
      }
      _extractor.phase2_end();
      LOG2(INFO, _logger) << "CRFSuiteTrainer::extract ndocs_read=" << ndocs_read << std::endl;
      LOG2(INFO, _logger) << "CRFSuiteTrainer::exact end" << std::endl;
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::dump_crfsuite_data(io::OutputStream &out) const {
      LOG2(INFO, _logger) << "CRFSuiteTrainer::dump_crfsuite_data begin" << std::endl;
      learn::dump_crfsuite_data(out, _data);
      LOG2(INFO, _logger) << "CRFSuiteTrainer::dump_crfsuite_data end" << std::endl;
    }


    // ========================================================================
    // CRFSuiteTagger
    // ========================================================================
    template <typename EXTRACTOR>
    CRFSuiteTagger<EXTRACTOR>::CRFSuiteTagger(EXTRACTOR &extractor, InputModel &model) :
        _model(model),
        _extractor(extractor),
        _cmodel(nullptr),
        _tagger(nullptr),
        _attrs(nullptr),
        _labels(nullptr),
        _label_sequence(nullptr),
        _label_sequence_length(0)
      {
      using namespace ::schwa::third_party::crfsuite;
      int ret;

      // Open the crfsuite model.
      ret = crfsuite_create_instance_from_file(_model.model_path().c_str(), reinterpret_cast<void **>(&_cmodel));
      if (SCHWA_UNLIKELY(ret != 0)) {
        std::ostringstream ss;
        ss << "crfsuite_create_instance_from_file(\"" << _model.model_path() << "\")";
        _crfsuite_error(ss.str(), ret);
      }

      // Open a reference to the tagger instance for the loaded model.
      ret = _cmodel->get_tagger(_cmodel, &_tagger);
      if (SCHWA_UNLIKELY(ret != 0))
        _crfsuite_error("Failed to obtain the tagger from the loaded model", ret);

      // Open a reference to the attributes dictionary for the loaded model.
      ret = _cmodel->get_attrs(_cmodel, &_attrs);
      if (SCHWA_UNLIKELY(ret != 0))
        _crfsuite_error("Failed to obtain the attributes dictionary from the loaded model", ret);

      // Open a reference to the labels dictionary for the loaded model.
      ret = _cmodel->get_labels(_cmodel, &_labels);
      if (SCHWA_UNLIKELY(ret != 0))
        _crfsuite_error("Failed to obtain the labels dictionary from the loaded model", ret);
    }

    template <typename EXTRACTOR>
    CRFSuiteTagger<EXTRACTOR>::~CRFSuiteTagger(void) {
      using namespace ::schwa::third_party::crfsuite;

      delete [] _label_sequence;

      // Deinitialise the model and its related data.
      if (_labels != nullptr) {
        _labels->release(_labels);
        _labels = nullptr;
      }
      if (_attrs != nullptr) {
        _attrs->release(_attrs);
        _attrs = nullptr;
      }
      if (_tagger != nullptr) {
        _tagger->release(_tagger);
        _tagger = nullptr;
      }
      if (_cmodel != nullptr) {
        _cmodel->release(_cmodel);
        _cmodel = nullptr;
      }
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTagger<EXTRACTOR>::_crfsuite_error(const std::string &api_call, const int ret) {
      std::ostringstream ss;
      ss << "crfsuite API call `" << api_call << "` failed (ret=" << ret << ")";
      throw Exception(ss.str());
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTagger<EXTRACTOR>::_begin_item_sequence(const size_t nitems) {
      using namespace ::schwa::third_party::crfsuite;
      // Initialise the item sequence with a known number of items.
      crfsuite_instance_init_n(&_instance, nitems);
      _item = _instance.items;
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTagger<EXTRACTOR>::_end_item_sequence(void) {
      using namespace ::schwa::third_party::crfsuite;
      int ret;

      // Set the taggers current instance to be this instance.
      ret = _tagger->set(_tagger, &_instance);
      if (ret != 0)
        _crfsuite_error("tagger->set", ret);

      // Deinitialise the item sequence.
      crfsuite_instance_finish(&_instance);
    }

    template <typename EXTRACTOR> template <typename TO_STRING, typename FEATURES>
    inline void
    CRFSuiteTagger<EXTRACTOR>::_add_item(TO_STRING &to_string_helper, const FEATURES &features) {
      using namespace ::schwa::third_party::crfsuite;

      // Initialise the item with a known number of attributes.
      crfsuite_item_init_n(_item, features.size());

      // Convert each of the provided features into attribute IDs and add them to the item.
      std::string attr_str;
      size_t a = 0;
      for (const auto &pair : features) {
        // Convert the feature value into a string and obtain its crfsuite attribute ID.
        attr_str.assign(to_string_helper(pair.first));
        const int attr_id = _attrs->get(_attrs, attr_str.c_str());

        // Set the attribute data and add the attribute to the item.
        crfsuite_attribute_set(&_item->contents[a++], attr_id, pair.second);
      }

      // Increment the item pointer to the next pre-allocated item.
      ++_item;
    }


    template <typename EXTRACTOR>
    inline ::schwa::third_party::crfsuite::floatval_t
    CRFSuiteTagger<EXTRACTOR>::_viterbi(void) {
      using namespace ::schwa::third_party::crfsuite;
      int ret;

      // Set the taggers current instance to be this instance.
      floatval_t score;
      ret = _tagger->viterbi(_tagger, _label_sequence, &score);
      if (ret != 0)
        _crfsuite_error("tagger->viterbi", ret);
      return score;
    }


    template <typename EXTRACTOR> template <typename TRANSFORM>
    inline void
    CRFSuiteTagger<EXTRACTOR>::tag(ResettableDocrepReader<canonical_schema::Doc> &doc_reader, const TRANSFORM &transformer) {
      LOG(INFO) << "CRFSuiteTagger::tag begin" << std::endl;
      _FeatureToStringHelper<typename TRANSFORM::value_type> to_string_helper;

      canonical_schema::Doc *doc;
      size_t ndocs_read;
      size_t max_sentence_length = 0, sentence_length;

      // Run phase 1.
      _extractor.phase1_begin();
      for (ndocs_read = 0; (doc = doc_reader.next()) != nullptr; ++ndocs_read) {
        _extractor.phase1_bod(*doc);
        for (canonical_schema::Sentence &sentence : doc->sentences) {
          sentence_length = sentence.span.stop - sentence.span.start;
          if (sentence_length > max_sentence_length)
            max_sentence_length = sentence_length;

          _extractor.phase1_bos(sentence);
          size_t i = 0;
          for (canonical_schema::Token &token : sentence.span) {
            _extractor.phase1_extract(token, i++);
          }
          _extractor.phase1_eos(sentence);
        }
        _extractor.phase1_eod(*doc);
      }
      _extractor.phase1_end();
      LOG(INFO) << "CRFSuiteTagger::tag ndocs_read=" << ndocs_read << std::endl;

      // Ensure our sequence label array is large enough to house the longest sentence.
      if (max_sentence_length > _label_sequence_length) {
        delete [] _label_sequence;
        _label_sequence = new int[max_sentence_length];
        _label_sequence_length = max_sentence_length;
      }

      // Reset the document reader.
      doc_reader.reset();

      // Create a Features instance.
      Features<TRANSFORM, ::schwa::third_party::crfsuite::floatval_t> features(transformer);

      // Run phase 2.
      _extractor.phase2_begin();
      for (ndocs_read = 0; (doc = doc_reader.next()) != nullptr; ++ndocs_read) {
        _extractor.phase2_bod(*doc);
        for (canonical_schema::Sentence &sentence : doc->sentences) {
          sentence_length = sentence.span.stop - sentence.span.start;

          _extractor.phase2_bos(sentence);
          _begin_item_sequence(sentence_length);

          size_t i = 0;
          for (canonical_schema::Token &token : sentence.span) {
            // Extract the features for the current token.
            _extractor.phase2_extract(token, i++, features);

            // Add the features as an item in the current item sequence.
            _add_item(to_string_helper, features);
            features.clear();
          }

          _end_item_sequence();
          _extractor.phase2_eos(sentence);

          // Run viterbi over the current item sequence.
          const third_party::crfsuite::floatval_t score = _viterbi();

          std::cout << "score=" << score;
          i = 0;
          for (canonical_schema::Token &token : sentence.span) {
            std::cout << " " << _label_sequence[i++] << "|" << token.raw;
          }
          std::cout << std::endl;
        }
        _extractor.phase2_eod(*doc);
      }
      _extractor.phase2_end();
      LOG(INFO) << "CRFSuiteTagger::tag ndocs_read=" << ndocs_read << std::endl;
      LOG(INFO) << "CRFSuiteTagger::tag end" << std::endl;
    }

  }
}

#endif  // SCHWA_LEARN_CRFSUITE_IMPL_H_
