#include <gtest/gtest.h>
#include <streams/estream/estream_interface.h>
#include <eacirc-core/json.h>
#include <eacirc-core/optional.h>
#include <testsuite/test-utils/common_functions.h>
#include <streams/estream/estream_cipher.cc>
#include <fstream>
#include <testsuite/test-utils/estream_test_case.h>
#include <eacirc-core/seed.h>

TEST(decim, test_vectors) {
    testsuite::estream_test_case("DECIM", 8)();
}