#include <schwa/learn/crfsuite.h>

#include <iostream>
#include <sstream>

#include <schwa/exception.h>


namespace schwa {
namespace learn {

// ========================================================================
// CRFSuiteTrainerParams
// ========================================================================
CRFSuiteTrainerParams::CRFSuiteTrainerParams(config::Group &group, const std::string &name, const std::string &desc, config::Flags flags) :
    config::Group(group, name, desc, flags),
    algorithm(*this, "algorithm", "Which training algorithm to use", {"lbfgs", "l2sgd", "ap", "pa", "arow"}, "lbfgs"),
    c2(*this, "c2", "Coefficient for L2 regularisation during LBFGS", 0.707),
    max_iterations(*this, "max-iterations", "Maximum number of iterations to run LBFGS", 500),
    num_memories(*this, "num-memories", "The number of inverse Hessian matrices to store during LBFGS", 10),
    epsilon(*this, "epsilon", "The epsilon parameter for convergence testing during LBFGS", 1e-6),
    line_search(*this, "line-search", "Which line search algorithm to use during LBFGS", {"MoreTheunte", "Backtracking", "StrongBacktracking"}, "StrongBacktracking"),
    max_line_search_iterations(*this, "max-line-search-iterations", "The maximum number of line search iterations to perform during LBFGS", 100),
    feature_min_freq(*this, "feature-min-freq", "Cut-off threshold for occurrence frequency of a feature", 0),
    feature_possible_states(*this, "feature-possible-states", "Whether crfsuite generates state features that do not occur in the training data (i.e., negative state features)", false),
    feature_possible_transitions(*this, "feature-possible-transitions", "Whether crfsuite generates transition features that do not occur in the training data (i.e., negative transition features)", false)
  { }

CRFSuiteTrainerParams::~CRFSuiteTrainerParams(void) { }


// ========================================================================
// CRFsuiteLoadedModel
// ========================================================================
CRFsuiteLoadedModel::CRFsuiteLoadedModel(const std::string &model_path) :
    _model(nullptr),
    _tagger(nullptr),
    _attrs(nullptr),
    _labels(nullptr)
  {
  using namespace ::schwa::third_party::crfsuite;
  int ret;

  // Open the crfsuite model.
  ret = crfsuite_create_instance_from_file(model_path.c_str(), reinterpret_cast<void **>(&_model));
  if (SCHWA_UNLIKELY(ret != 0)) {
    std::ostringstream ss;
    ss << "crfsuite_create_instance_from_file(\"" << model_path << "\")";
    throw_crfsuite_error(ss.str(), ret);
  }

  // Open a reference to the tagger instance for the loaded model.
  ret = _model->get_tagger(_model, &_tagger);
  if (SCHWA_UNLIKELY(ret != 0))
    throw_crfsuite_error("Failed to obtain the tagger from the loaded model", ret);

  // Open a reference to the attributes dictionary for the loaded model.
  ret = _model->get_attrs(_model, &_attrs);
  if (SCHWA_UNLIKELY(ret != 0))
    throw_crfsuite_error("Failed to obtain the attributes dictionary from the loaded model", ret);

  // Open a reference to the labels dictionary for the loaded model.
  ret = _model->get_labels(_model, &_labels);
  if (SCHWA_UNLIKELY(ret != 0))
    throw_crfsuite_error("Failed to obtain the labels dictionary from the loaded model", ret);

  // Obtain a copy of the string representation of all of the labels.
  const int nlabels = _labels->num(_labels);
  if (nlabels > 0) {
    _label_strings.resize(nlabels);
    for (int i = 0; i != nlabels; ++i) {
      ret = _labels->to_string(_labels, i, &_label_strings[i]);
      if (SCHWA_UNLIKELY(ret != 0))
        throw_crfsuite_error("Failed to obtain the label string from the loaded model", ret);
    }
  }
}

CRFsuiteLoadedModel::~CRFsuiteLoadedModel(void) {
  using namespace ::schwa::third_party::crfsuite;

  // Free the label strings obtained from crfsuite.
  for (const char *str : _label_strings)
    _labels->free(_labels, str);

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
  if (_model != nullptr) {
    _model->release(_model);
    _model = nullptr;
  }
}


// ========================================================================
// CRFsuite API helpers.
// ========================================================================
void
throw_crfsuite_error(const std::string &api_call, const int ret) {
  std::ostringstream ss;
  ss << "CRFsuite API call `" << api_call << "` failed (ret=" << ret << ")";
  throw Exception(ss.str());
}



// ========================================================================
// dump crfsuite data
// ========================================================================
void
dump_crfsuite_value(std::ostream &out, const char *const value) {
  for (const char *c = value; *c != '\0'; ++c) {
    if (*c == ':')
      out << "\\:";
    else if (*c == '\\')
      out << "\\\\";
    else
      out << *c;
  }
}


void
dump_crfsuite_data(std::ostream &out, const third_party::crfsuite::crfsuite_data_t &data) {
  using namespace ::schwa::third_party::crfsuite;
  int ret;
  const char *str;

  // Iterate through each instance.
  for (int ninstance = 0; ninstance != data.num_instances; ++ninstance) {
    // Output the blank line between instances.
    if (ninstance != 0)
      out << std::endl;

    // Iterate through each item in the current instance.
    crfsuite_instance_t &instance = data.instances[ninstance];
    for (int nitem = 0; nitem != instance.num_items; ++nitem) {
      // Output the label.
      ret = data.labels->to_string(data.labels, instance.labels[nitem], &str);
      assert(ret == 0);
      if (ret == 0)
        dump_crfsuite_value(out, str);
      data.labels->free(data.labels, str);

      // Iterate through each attribute for the current item.
      crfsuite_item_t &item = instance.items[nitem];
      for (int nattr = 0; nattr != item.num_contents; ++nattr) {
        // Output the attribute value.
        const crfsuite_attribute_t &attr = item.contents[nattr];
        ret = data.attrs->to_string(data.attrs, attr.aid, &str);
        assert(ret == 0);
        if (ret == 0) {
          out << '\t';
          dump_crfsuite_value(out, str);
          out << ':' << attr.value;
        }
        data.attrs->free(data.attrs, str);
      }

      // Newline for the current item.
      out << std::endl;
    }
  }
}

}  // namespace learn
}  // namespace schwa
