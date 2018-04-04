#include <gtest/gtest.h>
#include <streams/estream/estream_interface.h>
#include <eacirc-core/json.h>
#include <eacirc-core/optional.h>
#include <testsuite/test-utils/common_functions.h>
#include <streams/estream/estream_cipher.h>
#include <fstream>
#include <testsuite/test-utils/estream_test_case.h>
#include <eacirc-core/seed.h>

TEST(chacha, test_vectors) {
    testsuite::estream_test_case("Chacha", 20)();
    testsuite::estream_test_case("Chacha", 12)();
}

TEST(decim, test_vectors) {
    testsuite::estream_test_case("DECIM", 8)();
}

TEST(fubuki, test_vectors) {
    testsuite::estream_test_case("Fubuki", 4)();
}

TEST(grain, test_vectors) {
    testsuite::estream_test_case("Grain", 13)();
}

TEST(hermes, test_vectors) {
    testsuite::estream_test_case("Hermes", 2)();
}

TEST(lex, test_vectors) {
    testsuite::estream_test_case("LEX", 10)();
}

TEST(mickey, test_vectors) {
    testsuite::estream_test_case("MICKEY", 8)();
}

TEST(salsa20, test_vectors) {
    testsuite::estream_test_case("Salsa20", 20)();
}

TEST(tsc4, test_vectors) {
    testsuite::estream_test_case("TSC-4", 32)();
}

TEST(trivium, test_vectors) {
    testsuite::estream_test_case("Trivium", 9)();
}
