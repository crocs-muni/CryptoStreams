#include <eacirc-core/json.h>
#include <eacirc-core/optional.h>
#include <eacirc-core/seed.h>
#include <fstream>
#include <gtest/gtest.h>
#include <streams/stream_ciphers/stream_cipher.h>
#include <streams/stream_ciphers/stream_interface.h>
#include <testsuite/test_utils/common_functions.h>
#include <testsuite/test_utils/stream_ciphers_test_case.h>

TEST(chacha, test_vectors) {
    testsuite::stream_cipher_test_case("Chacha", 20)();
    testsuite::stream_cipher_test_case("Chacha", 12)();
}

TEST(rc4, test_vectors) {
    testsuite::stream_cipher_test_case("RC4", 1)();
}

TEST(decim, test_vectors) {
    testsuite::stream_cipher_test_case("DECIM", 8)();
}

TEST(fubuki, test_vectors) {
    testsuite::stream_cipher_test_case("Fubuki", 4)();
}

TEST(ffcsr, test_vectors) {
    testsuite::stream_cipher_test_case("F-FCSR", 5)();
}

TEST(grain, test_vectors) {
    testsuite::stream_cipher_test_case("Grain", 13)();
}

TEST(hc128, test_vectors) {
    testsuite::stream_cipher_test_case("HC-128", 1)();
}

TEST(hermes, test_vectors) {
    testsuite::stream_cipher_test_case("Hermes", 2)();
}

TEST(lex, test_vectors) {
    testsuite::stream_cipher_test_case("LEX", 10)();
}

TEST(mickey, test_vectors) {
    testsuite::stream_cipher_test_case("MICKEY", 1)();
}

TEST(rabbit, test_vectors) {
    testsuite::stream_cipher_test_case("Rabbit", 4)();
}

TEST(salsa20, test_vectors) {
    testsuite::stream_cipher_test_case("Salsa20", 20)();
}

TEST(sosemanuk, test_vectors) {
    testsuite::stream_cipher_test_case("SOSEMANUK", 25)();
}

TEST(tsc4, test_vectors) {
    testsuite::stream_cipher_test_case("TSC-4", 32)();
}

TEST(trivium, test_vectors) {
    testsuite::stream_cipher_test_case("Trivium", 9)();
}
