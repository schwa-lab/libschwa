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
    const std::string CRFSuiteTrainer<EXTRACTOR>::DEFAULT_ALGORITHM = "lbfgs";

    template <typename EXTRACTOR>
    CRFSuiteTrainer<EXTRACTOR>::CRFSuiteTrainer(EXTRACTOR &extractor, const std::string &algorithm) : _extractor(extractor), _trainer(nullptr) {
      using namespace ::schwa::third_party::crfsuite;
      int ret;

      // Initialise crfsuite data model objects.
      crfsuite_data_init(&_data);
      crfsuite_attribute_init(&_attribute);

      // Initialise the attribute and label dictionaries.
      ret = crfsuite_create_instance("dictionary", reinterpret_cast<void **>(&_data.attrs));
      if (ret != 0)
        _crfsuite_error("crfsuite_create_instance(\"dictionary\")", ret);
      ret = crfsuite_create_instance("dictionary", reinterpret_cast<void **>(&_data.labels));
      if (ret != 0)
        _crfsuite_error("crfsuite_create_instance(\"dictionary\")", ret);

      // Initialise the trainer.
      std::string tid = "train/crf1d/" + algorithm;
      ret = crfsuite_create_instance(tid.c_str(), reinterpret_cast<void **>(&_trainer));
      if (ret != 0) {
        std::ostringstream ss;
        ss << "crfsuite_create_instance(\"" << tid << "\")";
        _crfsuite_error(ss.str(), ret);
      }
      _trainer->set_message_callback(_trainer, this, &_crfsuite_logging_callback);
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
    CRFSuiteTrainer<EXTRACTOR>::_crfsuite_logging_callback(void *, const char *const format, va_list args) {
      // Write the log string to the bufffer.
      char buffer[65536];
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wformat-nonliteral"
      const int nchars = std::vsnprintf(buffer, sizeof(buffer) - 1, format, args);
#pragma GCC diagnostic pop

      // Was the printf call was successful and did not overflow the buffer?
      if (nchars >= 0 && static_cast<unsigned int>(nchars) <= sizeof(buffer) - 1) {
        // If the last character written was a newline, log the line. Ignore otherwise.
        if (buffer[nchars - 1] == '\n') {
          buffer[nchars - 1] = '\0';
          LOG(INFO) << "CRFSuiteTrainer::_crfsuite_logging_callback '" << buffer << "'" << std::endl;
        }
      }

      return 0;
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::_crfsuite_error(const std::string &api_call, const int ret) {
      std::ostringstream ss;
      ss << "CRFSuite API call `" << api_call << "` failed (ret=" << ret << ")";
      throw Exception(ss.str());
    }


    template <typename EXTRACTOR>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::_begin_item_sequence(const size_t nitems) {
      using namespace ::schwa::third_party::crfsuite;
      // Initialise the item sequence with a known number of items.
      crfsuite_instance_init_n(&_instance, nitems);
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
      int ret;

      // Initialise the item with a known number of attributes.
      crfsuite_item_init_n(&_item, features.size());

      // Convert each of the provided features into attribute IDs and add them to the item.
      std::string attr_str;
      for (const auto &pair : features) {
        // Convert the feature value into a string and obtain its crfsuite attribute ID.
        attr_str.assign(to_string_helper(pair.first));
        const int attr_id = _data.attrs->get(_data.attrs, attr_str.c_str());

        // Set the attribute data and add the attribute to the item.
        crfsuite_attribute_set(&_attribute, attr_id, pair.second);
        ret = crfsuite_item_append_attribute(&_item, &_attribute);
        if (ret != 0)
          _crfsuite_error("crfsuite_item_append_attribute", ret);
      }

      // Convert the label into its crfsuite label ID.
      const int label_id = _data.labels->get(_data.labels, label.c_str());

      // Add the item to the item sequence.
      ret = crfsuite_instance_append(&_instance, &_item, label_id);
      if (ret != 0)
        _crfsuite_error("crfsuite_instance_append", ret);

      // Deinitialise the item.
      crfsuite_item_finish(&_item);
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
    CRFSuiteTrainer<EXTRACTOR>::train(const std::string &model_path) {
      using namespace ::schwa::third_party::crfsuite;
      int ret;

      LOG(INFO) << "CRFSuiteTrainer::train begin" << std::endl;
      ret = _trainer->train(_trainer, &_data, model_path.c_str(), -1);
      if (ret != 0)
        _crfsuite_error("crfsuite_trainer_t::train", ret);
      LOG(INFO) << "CRFSuiteTrainer::train end" << std::endl;
    }


    template <typename EXTRACTOR> template <typename TRANSFORM>
    inline void
    CRFSuiteTrainer<EXTRACTOR>::extract(const std::vector<canonical_schema::Doc *> &docs, const TRANSFORM &transformer) {
      LOG(INFO) << "CRFSuiteTrainer::exact begin" << std::endl;
      _FeatureToStringHelper<typename TRANSFORM::value_type> to_string_helper;

      // Run phase 1.
      _extractor.phase1_begin();
      for (canonical_schema::Doc *doc : docs) {
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

      // Create a Features instance.
      Features<TRANSFORM, ::schwa::third_party::crfsuite::floatval_t> features(transformer);

      // Run phase 2.
      _extractor.phase2_begin();
      for (canonical_schema::Doc *doc : docs) {
        _extractor.phase2_bod(*doc);
        for (canonical_schema::Sentence &sentence : doc->sentences) {
          _extractor.phase2_bos(sentence);
          _begin_item_sequence(sentence.span.stop - sentence.span.start);

          size_t i = 0;
          for (canonical_schema::Token &token : sentence.span) {
            // Extract the features for the current token.
            features.clear();
            _extractor.phase2_extract(token, i++, features);

            // Add the features as an item in the current item sequence.
            _add_item(to_string_helper, features, token.pos);
          }

          _end_item_sequence();
          _extractor.phase2_eos(sentence);
        }
        _extractor.phase2_eod(*doc);
      }
      _extractor.phase2_end();
      LOG(INFO) << "CRFSuiteTrainer::exact end" << std::endl;
    }

  }
}

#endif  // SCHWA_LEARN_CRFSUITE_IMPL_H_
