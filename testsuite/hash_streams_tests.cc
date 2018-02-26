#include <gtest/gtest.h>
#include <eacirc-core/json.h>
#include <streams/hash/hash_factory.h>
#include <streams/hash/sha3/sha3_interface.h>
#include <fstream>
#include "stream.h"
#include "streams.h"

#include "testsuite/test-utils/hash_test_case.h"

/** Source of test vectors http://csrc.nist.gov/groups/ST/hash/sha-3/index.html */


TEST(abacus, test_vectors) {
    testsuite::sha3_test_case("Abacus", 135)();
}

TEST(arirang, test_vectors) {
    testsuite::sha3_test_case("ARIRANG", 4)();
}

TEST(aurora, test_vectors) {
    testsuite::sha3_test_case("AURORA", 17)();
}

TEST(blake, test_vectors) {
    testsuite::sha3_test_case("BLAKE", 10)();
    testsuite::sha3_test_case("BLAKE", 14)();
}

TEST(cheetah, test_vectors) {
    testsuite::sha3_test_case("Cheetah", 16)();
}

TEST(cube_hash, test_vectors) {
    testsuite::sha3_test_case("CubeHash", 8)();
}

TEST(dch, test_vectors) {
    testsuite::sha3_test_case("DCH", 4)();
}

TEST(dynamic_sha, test_vectors) {
    testsuite::sha3_test_case("DynamicSHA", 16)();
}

TEST(dynamic_sha2, test_vectors) {
    testsuite::sha3_test_case("DynamicSHA2", 17)();
}

TEST(echo, test_vectors) {
    testsuite::sha3_test_case("ECHO", 8)();
    testsuite::sha3_test_case("ECHO", 10)();
}

TEST(grostl, test_vectors) {
    testsuite::sha3_test_case("Grostl", 10)();
    testsuite::sha3_test_case("Grostl", 14)();
}

TEST(hamsi, test_vectors) {
    testsuite::sha3_test_case("Hamsi", 3)();
    testsuite::sha3_test_case("Hamsi", 6)();
}

TEST(jh, test_vectors) {
    testsuite::sha3_test_case("JH", 42)();
}

TEST(lesamnta, test_vectors) {
    testsuite::sha3_test_case("Lesamnta", 32)();
}

TEST(luffa, test_vectors) {
    testsuite::sha3_test_case("Luffa", 8)();
}

TEST(md6, test_vectors) {
    testsuite::sha3_test_case("MD6", 96)();
    testsuite::sha3_test_case("MD6", 104)();
    testsuite::sha3_test_case("MD6", 105)();
}

TEST(simd, test_vectors) {
    testsuite::sha3_test_case("SIMD", 4)();
}

TEST(tangle, test_vectors) {
    testsuite::sha3_test_case("Tangle", 72)();
    testsuite::sha3_test_case("Tangle", 80)();
    testsuite::sha3_test_case("Tangle", 96)();
    testsuite::sha3_test_case("Tangle", 112)();
    testsuite::sha3_test_case("Tangle", 128)();
    testsuite::sha3_test_case("Tangle", 144)();
}

TEST(twister, test_vectors) {
    testsuite::sha3_test_case("Twister", 9)();
    testsuite::sha3_test_case("Twister", 10)();
}
