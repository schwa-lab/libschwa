/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <iostream>
#include <string>
#include <thread>

#include <schwa/config.h>
#include <schwa/dr/reader.h>
#include <schwa/dr-dist/helpers.h>
#include <schwa/dr-dist/server.h>
#include <schwa/io/logging.h>

#include <dr-dist/main.h>

namespace cf = schwa::config;
namespace io = schwa::io;


int
main(int argc, char **argv) {
  // Construct an option parser.
  cf::Main cfg(schwa::dr_dist::PROGRAM_NAME, schwa::dr_dist::PROGRAM_DESC);
  cf::OpIStream input(cfg, "input", 'i', "The input file");
  cf::OpOStream output(cfg, "output", 'o', "The output file");
  cf::Op<std::string> bind_host(cfg, "bind-host", "The network hostname to bind to", "*");
  cf::Op<uint32_t> source_port(cfg, "source-port", "The network port to bind to on which to push docrep documents", 7301);
  cf::Op<uint32_t> sink_port(cfg, "sink-port", "The network port to bind to on which to pull docrep documents", 7302);
  cf::Op<uint32_t> control_port(cfg, "control-port", "The network port to bind to on which to publish control messages", 7303);
  cf::Op<bool> preserve_order(cfg, "preserve-order", "Whether or not the order of documents written out should be preserved", true);
  cf::Op<bool> kill_clients(cfg, "kill-clients", "Whether or not to instruct the clients to terminate once all of the documents have been processed", true);

  // Parse argv.
  cfg.main<io::ThreadsafePrettyLogger>(argc, argv);

  // Construct the network address strings.
  const std::string source_addr = schwa::dr_dist::build_socket_addr(bind_host(), source_port());
  const std::string sink_addr = schwa::dr_dist::build_socket_addr(bind_host(), sink_port());
  const std::string control_addr = schwa::dr_dist::build_socket_addr(bind_host(), control_port());
  const std::string direct_sink_addr = schwa::dr_dist::build_socket_addr(bind_host() == "*" ? "127.0.0.1" : bind_host(), sink_port());

  // Run the source and sink threads.
  bool success_source, success_sink;
  auto wrap_source = [&](std::istream &input) {
    success_source = schwa::dr_dist::source(source_addr, direct_sink_addr, input);
  };
  auto wrap_sink = [&](std::ostream &output) {
    success_sink = schwa::dr_dist::sink(sink_addr, control_addr, preserve_order(), kill_clients(), output);
  };
  std::thread source_thread(wrap_source, std::ref(input.file()));
  std::thread sink_thread(wrap_sink, std::ref(output.file()));
  source_thread.join();
  sink_thread.join();

  return success_source && success_sink ? 0 : 1;
}
