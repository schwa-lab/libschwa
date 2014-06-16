/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cctype>
#include <cstring>
#include <exception>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/io/logging.h>
#include <schwa/pool.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace schwa {
namespace ccg_pprint {

class Node {
private:
  char *const _data;
  std::vector<Node *, PoolAllocator<Node *>> _children;

  void
  _pprint_hierarchy(std::ostream &out, const unsigned int depth, std::vector<unsigned int> &nchildren_left) const {
    if (depth != 0) {
      for (unsigned int i = 0; i != depth - 1; ++i) {
        if (nchildren_left[i] == 0)
          out << "  ";
        else
          out << "| ";
      }
      out << "+-";
      --nchildren_left[depth - 1];
    }
    out << _data << '\n';
    if (!_children.empty()) {
      nchildren_left.push_back(_children.size());
      for (auto &child : _children)
        child->_pprint_hierarchy(out, depth + 1, nchildren_left);
      nchildren_left.pop_back();
    }
  }

public:
  Node(Pool &pool, char *data) : _data(data), _children(PoolAllocator<char>(pool)) { }

  inline void add_child(Node *child) { _children.push_back(child); }

  std::ostream &
  pprint_hierarchy(std::ostream &out) const {
    std::vector<unsigned int> nchildren_left;
    _pprint_hierarchy(out, 0, nchildren_left);
    return out;
  }

  std::ostream &
  pprint_ptb(std::ostream &out, unsigned int indent=0) const {
    out << '(' << _data;
    indent += 1 + std::strlen(_data);
    if (!_children.empty()) {
      out << ' ';
      ++indent;
      for (decltype(_children)::size_type i = 0; i != _children.size(); ++i) {
        if (i != 0) {
          out << '\n';
          for (unsigned int j = 0; j != indent; ++j)
            out << ' ';
        }
        _children[i]->pprint_ptb(out, indent);
      }
    }
    return out << ')';
  }
};


struct ParseException : public std::exception {
  ParseException(void) { }
};


Node *
parse(const char **upto, Pool &pool, Node *const parent) {
  if (**upto == '\0')
    return nullptr;
  else if (**upto != '(')
    throw ParseException();
  ++*upto;

  const char *const start = *upto;
  if (**upto != '<')
    throw ParseException();
  for (; **upto != '>'; ++*upto) { }
  ++*upto;
  const char *const end = *upto;

  char *data = pool.alloc<char *>((end - start) + 1);
  std::memcpy(data, start, end - start);
  data[end - start] = '\0';
  Node *const node = new (pool) Node(pool, data);

  if (**upto != ')') {
    while (true) {
      ++*upto;
      if (**upto == '(')
        parse(upto, pool, node);
      else if (**upto == ')')
        break;
    }
  }
  ++*upto;
  if (parent)
    parent->add_child(node);

  return node;
}


static void
main(std::istream &in, std::ostream &out, const bool ptb) {
  char buf[4096];
  char *end;

  // Read in lines from the input.
  while (in.getline(buf, sizeof(buf))) {
    // Strip spaces from the end of the line.
    for (end = buf; *end != '\0'; ++end) { }
    --end;
    while (std::isspace(*end)) {
      *end = '\0';
      --end;
    }

    // Attempt to parse the line.
    Pool pool(4 * 1024);
    Node *tree = nullptr;
    const char *start = buf;
    try {
      tree = parse(&start, pool, nullptr);
    }
    catch (ParseException) { }

    if (tree == nullptr)
      out << buf;
    else {
      if (ptb)
        tree->pprint_ptb(out);
      else
        tree->pprint_hierarchy(out);
    }
    out << std::endl;
  }
}


static void
main(const std::vector<std::string> &input_paths, std::ostream &out, const bool ptb) {
  for (const auto &input_path : input_paths) {
    io::InputStream in(input_path);
    main(in, out, ptb);
  }
}

}  // namespace ccg_pprint
}  // namespace schwa




int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("ccg-pprint", "Pretty-print CCG derivations");
  cf::Op<std::string> input_path(cfg, "input", 'i', "The input path", io::STDIN_STRING);
  cf::Op<std::string> output_path(cfg, "output", 'o', "The output path", io::STDOUT_STRING);
  cf::Op<bool> ptb(cfg, "ptb", 'p', "Pretty-print the trees in PTB format", false);

  cfg.allow_unclaimed_args("[input-path...]");

  SCHWA_MAIN(cfg, [&] {
    // Parse argv.
    cfg.main<io::PrettyLogger>(argc, argv);

    // Work out which input paths to read from.
    std::vector<std::string> input_paths;
    if (input_path.was_mentioned() || cfg.unclaimed_args().empty())
      input_paths.push_back(input_path());
    else
      input_paths = cfg.unclaimed_args();

    // Open the ouptut stream.
    io::OutputStream out(output_path());

    // Dispatch main.
    schwa::ccg_pprint::main(input_paths, out, ptb());
  })
  return 0;
}
