/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWATEST_TEST_UTILS_H_
#define SCHWATEST_TEST_UTILS_H_

#include <string>

#include <schwa/_base.h>

#include <UnitTest++.h>

namespace schwatest {

void
compare_bytes(const uint8_t *actual, const size_t actual_size, const uint8_t *expected, const size_t size, UnitTest::TestResults &results, const UnitTest::TestDetails &details);

void
compare_bytes(const uint8_t *expected, const size_t expected_size, const std::string &actual, UnitTest::TestResults &results, const UnitTest::TestDetails &details);

void
compare_bytes(const std::string &expected, const std::string &actual, UnitTest::TestResults &results, const UnitTest::TestDetails &details);

}

#define CHECK_COMPARE_BYTES2(expected, actual) \
  do \
  { \
    try { \
      schwatest::compare_bytes(expected, actual, *UnitTest::CurrentTest::Results(), UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
    } \
    catch (...) { \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
          "Unhandled exception in CHECK_COMPARE_BYTES2(" #expected ", " #actual ")"); \
    } \
  } while (0)

#define CHECK_COMPARE_BYTES3(expected, expected_size, actual) \
  do \
  { \
    try { \
      schwatest::compare_bytes(expected, expected_size, actual, *UnitTest::CurrentTest::Results(), UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
    } \
    catch (...) { \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
          "Unhandled exception in CHECK_COMPARE_BYTES3(" #expected ", " #expected_size ", " #actual ")"); \
    } \
  } while (0)

#define CHECK_COMPARE_BYTES4(expected, expected_size, actual, actual_size) \
  do \
  { \
    try { \
      schwatest::compare_bytes(expected, expected_size, actual, actual_size, *UnitTest::CurrentTest::Results(), UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
    } \
    catch (...) { \
      UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
          "Unhandled exception in CHECK_COMPARE_BYTES4(" #expected ", " #expected_size ", " #actual ", " #actual_size ")"); \
    } \
  } while (0)

#endif  // SCHWATEST_TEST_UTILS_H_
