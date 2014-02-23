/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/config.h>
#include <schwa/io/logging.h>

#include <schwa/dr-dist/worker_main.cc>

namespace cf = schwa::config;
namespace io = schwa::io;


namespace {

class EgDoc : public dr::Doc {
public:
  class Schema;
};


class EgDoc::Schema : public dr::Doc::Schema<EgDoc> {
public:
  Schema(void) : dr::Doc::Schema<EgDoc>("EgDoc", "Example Doc class") { }
  virtual ~Schema(void) { }
};


void
process_doc(EgDoc &doc) {
  LOG(INFO) << "processing EgDoc instance " << &doc << std::endl;
}

}  // namespace


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg("drworker", "An example docrep parallelisation worker");

  return schwa::drdist::worker_main<EgDoc, io::PrettyLogger>(argc, argv, cfg, &process_doc);
}
