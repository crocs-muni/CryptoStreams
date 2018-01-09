#include <gtest/gtest.h>
#include <testsuite/test-utils/block_test_case.h>

TEST(aes, test_vectors) {
    testsuite::block_test_case("AES", 10)();
}

TEST(blowfish, test_vectors) {
    testsuite::block_test_case("BLOWFISH", 16)();
}

TEST(single_des, test_vectors) {
    testsuite::block_test_case("SINGLE-DES", 16)();
}

TEST(triple_des, test_vectors) {
    testsuite::block_test_case("TRIPLE-DES", 16)();
}

TEST(rc4, test_vectors) {
    testsuite::block_test_case("RC4", 1)();
}

TEST(tea, test_vectors) {
    testsuite::block_test_case("TEA", 32)();
}
