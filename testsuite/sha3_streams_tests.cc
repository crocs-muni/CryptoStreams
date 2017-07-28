#include <gtest/gtest.h>
#include <eacirc-core/json.h>
#include <streams/sha3/sha3_factory.h>
#include <streams/sha3/sha3_interface.h>
#include <fstream>
#include "stream.h"
#include "streams.h"

#include "testsuite/test-utils/sha3_test_case.h"

// Source of test vectors https://ehash.iaik.tugraz.at/uploads/b/be/Abacus.pdf
TEST(abacus, test_vectors) {
    testsuite::sha3_test_case("Abacus", 135)();
}

TEST(arirang, test_vectors) {
    testsuite::sha3_test_case("ARIRANG", 4)();
}

TEST(tangle, test_vectors) {
    testsuite::sha3_test_case("Tangle", 72)();
    testsuite::sha3_test_case("Tangle", 80)();
    testsuite::sha3_test_case("Tangle", 96)();
    testsuite::sha3_test_case("Tangle", 112)();
    testsuite::sha3_test_case("Tangle", 128)();
    testsuite::sha3_test_case("Tangle", 144)();
}

TEST(dynamic_sha, test_vectors) {
    testsuite::sha3_test_case("DynamicSHA", 16)();
}

TEST(dynamic_sha2, test_vectors) {
    testsuite::sha3_test_case("DynamicSHA2", 17)();
}
