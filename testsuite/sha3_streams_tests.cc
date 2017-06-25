#include <gtest/gtest.h>
#include <eacirc-core/json.h>
#include <streams/sha3/sha3_factory.h>
#include <streams/sha3/sha3_interface.h>
#include <fstream>

#include "test-utils/sha3_test_case.h"

TEST(abacus_stream, with_test_vectors) {
    std::unique_ptr<sha3_interface> hasher = sha3_factory::create("Abacus", 135);
    std::ifstream test_vectors("resources/test_vectors/Abacus/test_vectors.txt", std::ifstream::in);

    sha3_test_case test_case;
    while (test_vectors >> test_case) {
        test_case(hasher);
    }
}