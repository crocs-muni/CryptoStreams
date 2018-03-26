#include <gtest/gtest.h>
#include <testsuite/test-utils/block_test_case.h>

TEST(aes, test_vectors) {
    testsuite::block_test_case("AES", 10)();
}

TEST(blowfish, test_vectors) {
    testsuite::block_test_case("BLOWFISH", 16)();
}

TEST(simon, test_vectors) {
    testsuite::block_test_case("SIMON", 32)();
    testsuite::block_test_case("SIMON", 36)();
    testsuite::block_test_case("SIMON", 42)();
    testsuite::block_test_case("SIMON", 44)();
    testsuite::block_test_case("SIMON", 52)();
    testsuite::block_test_case("SIMON", 54)();
    testsuite::block_test_case("SIMON", 68)();
    testsuite::block_test_case("SIMON", 69)();
    testsuite::block_test_case("SIMON", 72)();
}

TEST(speck, test_vectors) {
    testsuite::block_test_case("SPECK", 22)();
    testsuite::block_test_case("SPECK", 23)();
    testsuite::block_test_case("SPECK", 26)();
    testsuite::block_test_case("SPECK", 28)();
    testsuite::block_test_case("SPECK", 29)();
    testsuite::block_test_case("SPECK", 32)();
    testsuite::block_test_case("SPECK", 33)();
    testsuite::block_test_case("SPECK", 34)();
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

TEST(gost_block, test_vectors) {
    testsuite::block_test_case("GOST", 32)();
}
