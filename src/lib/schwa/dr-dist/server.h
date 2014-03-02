/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_DRDIST_SERVER_H_
#define SCHWA_DRDIST_SERVER_H_

#include <schwa/_base.h>

#include <iosfwd>
#include <string>


namespace schwa {
  namespace dr_dist {

    bool sink(const std::string &sink_addr, const std::string &control_addr, bool preserve_order, bool kill_clients, std::ostream &output);
    bool source(const std::string &source_addr, const std::string &direct_sink_addr, std::istream &input);

  }
}

#endif  // SCHWA_DRDIST_SERVER_H_
