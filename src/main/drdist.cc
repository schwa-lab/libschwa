/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <cstring>
#include <condition_variable>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include <schwa/config.h>
#include <schwa/msgpack.h>

#include <zmq.hpp>

namespace cf = schwa::config;
namespace mp = schwa::msgpack;


static bool NOISY;
static std::mutex input_lock;
static std::mutex output_lock;


static bool
read_doc(std::istream &in, std::ostream &out) {
  char *buf = nullptr;
  size_t size = 0;

  mp::WireType type;
  if (in.peek() == EOF)
    return false;

  // <version> (omitted in version 1)
  if (!mp::read_lazy(in, out, type))
    return false;

  if (mp::is_int(type)) {
    // <klasses> header
    if (!mp::read_lazy(in, out, type))
      return false;
  }
  if (!mp::is_array(type))
    return false;

  // <stores> header
  if (!mp::is_array(mp::header_type(in.peek())))
    return false;
  int nstores = mp::read_array_size(in);
  mp::write_array_size(out, nstores);
  for (int i = 0; i < nstores; ++i)
    if (!mp::read_lazy(in, out, type))
      return false;

  // instances (nstores + 1 size-data pairs)
  for (; nstores >= 0; --nstores) {
    const uint64_t nbytes = mp::read_uint(in);
    mp::write_uint(out, nbytes);

    if (nbytes > size) {
      delete [] buf;
      buf = new char[nbytes];
      size = nbytes;
    }
    in.read(buf, nbytes);
    out.write(buf, nbytes);
  }

  if (buf != nullptr)
    delete [] buf;

  return true;
}


static void
run_thread(zmq::context_t &context, const std::string &bind, std::istream &in, std::ostream &out) {
  // prepare the ØMQ source socket
  zmq::socket_t socket(context, ZMQ_REQ);
  if (NOISY) std::cerr << "[drdist] connecting to " << bind << std::endl;
  socket.connect(bind.c_str());

  zmq::message_t msg;
  while (true) {
    // lazily read the next document off the input stream
    std::stringstream ss;
    input_lock.lock();
    const bool doc_found = read_doc(in, ss);
    input_lock.unlock();
    if (NOISY) std::cerr << "[drdist] doc_found=" << doc_found << std::endl;
    if (!doc_found)
      break;

    // if we have a document to send, send it through
    const size_t size = ss.tellp();
    ss.seekg(0);
    msg.rebuild(size);
    std::memcpy(msg.data(), ss.str().c_str(), size);
    socket.send(msg);

    // wait for the reply back
    socket.recv(&msg);
    if (NOISY) std::cerr << "[drdist] recv size=" << msg.size() << std::endl;

    // write the contents out to the ostream
    output_lock.lock();
    out.write(static_cast<const char *>(msg.data()), msg.size());
    out.flush();
    output_lock.unlock();
  }

  if (NOISY) std::cerr << "[drdist] shutting down" << std::endl;
}


int
main(int argc, char **argv) {
  // construct an option parser
  cf::OpMain cfg("drdist", "A docrep parallelisation source and sink");
  cf::IStreamOp input(cfg, "input", "The input file");
  cf::OStreamOp output(cfg, "output", "The output file");
  cf::Op<std::string> server(cfg, "server", "The network address of the server", "tcp://localhost:7300");
  cf::Op<unsigned long> nthreads(cfg, "nthreads", "The number of threads to fire up", 8);
  cf::Op<bool> quiet(cfg, "quiet", "Quiet mode", false);

  // parse argv
  cfg.main(argc, argv);

  // extract values out of config options
  std::istream &in = input.file();
  std::ostream &out = output.file();
  NOISY = !quiet();

  // prepare the ØMQ context and sockets
  zmq::context_t context(1);

  // run the threads
  std::vector<std::thread> threads;
  for (unsigned long i = 0; i != nthreads(); ++i)
    threads.push_back(std::thread(&run_thread, std::ref(context), server(), std::ref(in), std::ref(out)));
  for (auto &t : threads)
    t.join();

  return 0;
}
