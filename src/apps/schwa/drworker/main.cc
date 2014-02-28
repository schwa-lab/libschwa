/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>
#include <schwa/dr.h>
#include <schwa/io/logging.h>

#include <schwa/dr-dist/worker_main.h>

namespace cf = schwa::config;
namespace dr = schwa::dr;
namespace io = schwa::io;


namespace {

void
process_doc(dr::FauxDoc &doc) {
  LOG(INFO) << "processing Doc instance " << &doc << std::endl;
}

}


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("drworker", "An example docrep parallelisation worker");

  return schwa::dr_dist::worker_main<dr::FauxDoc, io::PrettyLogger>(argc, argv, cfg, &process_doc);
}
