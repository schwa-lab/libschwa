/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWATEST_TEST_UTILS_H_
#define SCHWATEST_TEST_UTILS_H_

#include <string>

#include <schwa/_base.h>

#include <boost/test/unit_test.hpp>

namespace schwatest {

boost::test_tools::predicate_result
compare_bytes(const std::string &str, const uint8_t *expected, const size_t size);

boost::test_tools::predicate_result
compare_bytes(const std::string &str, const std::string &expected);

}

#endif  // SCHWATEST_TEST_UTILS_H_
