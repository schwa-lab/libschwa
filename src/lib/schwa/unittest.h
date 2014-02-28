/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_UNITTEST_H_
#define SCHWA_UNITTEST_H_

#include <string>

#include <schwa/_base.h>

#include <UnitTest++.h>

namespace schwa {
  namespace unittest {

    void
    compare_bytes(const uint8_t *actual, const size_t actual_size, const uint8_t *expected, const size_t size, UnitTest::TestResults &results, const UnitTest::TestDetails &details);

    void
    compare_bytes(const uint8_t *expected, const size_t expected_size, const std::string &actual, UnitTest::TestResults &results, const UnitTest::TestDetails &details);

    void
    compare_bytes(const std::string &expected, const std::string &actual, UnitTest::TestResults &results, const UnitTest::TestDetails &details);

    #define CHECK_COMPARE_BYTES2(expected, actual) \
      do \
      { \
        try { \
          schwa::unittest::compare_bytes(expected, actual, *UnitTest::CurrentTest::Results(), UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
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
          schwa::unittest::compare_bytes(expected, expected_size, actual, *UnitTest::CurrentTest::Results(), UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
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
          schwa::unittest::compare_bytes(expected, expected_size, actual, actual_size, *UnitTest::CurrentTest::Results(), UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__)); \
        } \
        catch (...) { \
          UnitTest::CurrentTest::Results()->OnTestFailure(UnitTest::TestDetails(*UnitTest::CurrentTest::Details(), __LINE__), \
              "Unhandled exception in CHECK_COMPARE_BYTES4(" #expected ", " #expected_size ", " #actual ", " #actual_size ")"); \
        } \
      } while (0)

  }
}
#endif  // SCHWA_UNITTEST_H_
