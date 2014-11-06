/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#include <schwa/formats/exception.h>


namespace schwa {
namespace formats {

ParseError::ParseError(const std::string &msg) : Exception(msg) { }

ParseError::ParseError(const ParseError &other) : Exception(other) { }

ParseError::~ParseError(void) throw() { }

}  // namespace formats
}  // namespace schwa
