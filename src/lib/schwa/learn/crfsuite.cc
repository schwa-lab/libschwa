#include <schwa/learn/crfsuite.h>

#include <iostream>


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
    max_line_search_iterations(*this, "max-line-search-iterations", "The maximum number of line search iterations to perform during LBFGS", 100)
  { }

CRFSuiteTrainerParams::~CRFSuiteTrainerParams(void) { }



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
        ret = data.attrs->to_string(data.attrs, item.contents[nattr].aid, &str);
        assert(ret == 0);
        if (ret == 0) {
          out << '\t';
          dump_crfsuite_value(out, str);
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
