#include <gtest/gtest.h>
#include <eacirc-core/json.h>
#include <streams/sha3/sha3_factory.h>
#include <streams/sha3/sha3_interface.h>
#include <fstream>
#include <eacirc-core/seed.h>
#include "stream.h"
#include "streams.h"

#include "testsuite/test-utils/sha3_test_case.h"

const static seed seed1 = seed::create("1fe40505e131963c");

// Source of test vectors https://ehash.iaik.tugraz.at/uploads/b/be/Abacus.pdf
TEST(abacus_stream, function_with_test_vectors) {
    std::unique_ptr<sha3_interface> hasher = sha3_factory::create("Abacus", 135);
    std::ifstream test_vectors("resources/test-resources/Abacus/test-vectors.txt", std::ifstream::in);

    testsuite::sha3_test_case test_case;
    while (test_vectors >> test_case) {
        test_case(hasher);
    }
}

TEST(abacus_stream, stream_with_test_vectors) {
    json config = {
            {"type", "sha3"},
            {"algorithm", "Abacus"},
            {"hash-bitsize", 256},
            {"round", 135},
            {"source", {
                       {"type", "test-stream"},
                       }
            }
    };

    seed_seq_from<pcg32> seeder(seed1);
    std::ifstream test_vectors("resources/test-resources/Abacus/test-vectors.txt", std::ifstream::in);

    testsuite::sha3_test_case test_case;

    while (test_vectors >> test_case) {
        if (test_case.length() != 0 && test_case.length() % 8 == 0) { // unfortunately our streams are built so that it fits only multiple of 8
            config["source"]["outputs"] = {test_case.input()};
            config["source"]["size"] = test_case.length() / 8;
            std::unique_ptr<stream> s = make_stream(config, seeder, 256 / 8);
            test_case(s);
        }
    }
}