/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_LEARN_CRFSUITE_H_
#define SCHWA_LEARN_CRFSUITE_H_

#include <iosfwd>
#include <string>

#include <schwa/_base.h>
#include <schwa/canonical-schema.h>
#include <schwa/io/streams.h>
#include <schwa/learn/io.h>
#include <schwa/third-party/crfsuite/crfsuite.h>


namespace schwa {
  namespace learn {

    // ========================================================================
    // POSExtractor
    // ========================================================================
    template <typename EXTRACTOR>
    class CRFSuiteTrainer {
    public:
      static const std::string DEFAULT_ALGORITHM;

    private:
      static int _crfsuite_logging_callback(void *self, const char *format, va_list args);

    private:
      EXTRACTOR &_extractor;
      third_party::crfsuite::crfsuite_data_t _data;
      third_party::crfsuite::crfsuite_instance_t _instance;
      third_party::crfsuite::crfsuite_item_t _item;
      third_party::crfsuite::crfsuite_attribute_t _attribute;
      third_party::crfsuite::crfsuite_trainer_t *_trainer;

      void _crfsuite_error(const std::string &api_call, int ret);

      void _begin_item_sequence(size_t nitems);
      void _end_item_sequence(void);

      template <typename TO_STRING, typename FEATURES>
      void _add_item(TO_STRING &to_string_helper, const FEATURES &features, const std::string &label);

    public:
      explicit CRFSuiteTrainer(EXTRACTOR &extractor, const std::string &algorithm=DEFAULT_ALGORITHM);
      ~CRFSuiteTrainer(void);

      std::string get_param(const std::string &key) const;
      void set_param(const std::string &key, const std::string &val);

      template <typename TRANSFORM>
      void extract(ResettableDocrepReader<canonical_schema::Doc> &doc_reader, const TRANSFORM &transformer=TRANSFORM());

      void dump_crfsuite_data(io::OutputStream &out) const;
      void train(const std::string &model_path);

    private:
      SCHWA_DISALLOW_COPY_AND_ASSIGN(CRFSuiteTrainer);
    };


    void dump_crfsuite_data(std::ostream &out, const third_party::crfsuite::crfsuite_data_t &data);
    void dump_crfsuite_value(std::ostream &out, const char *value);


  }
}

#include <schwa/learn/crfsuite_impl.h>

#endif  // SCHWA_LEARN_CRFSUITE_H_
