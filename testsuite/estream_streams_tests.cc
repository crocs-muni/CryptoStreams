#include <gtest/gtest.h>
#include <streams/estream/estream_interface.h>
#include <eacirc-core/optional.h>
#include <testsuite/test-utils/common_functions.h>
#include <streams/estream/estream_cipher.cc>
#include <fstream>
#include <testsuite/test-utils/estream_test_case.h>

//static std::unique_ptr<estream_interface> create_cipher(const std::string& name, core::optional<unsigned> round);


TEST(decim_stream, function_with_test_vectors) {
    std::unique_ptr<estream_interface> decim = create_cipher("DECIM", 8);
    std::ifstream test_vectors("resources/test-resources/estream/decim-v2/test-vectors.txt", std::ifstream::in);

    decim->ECRYPT_init();

    testsuite::estream_test_case test_case;

    while(test_vectors >> test_case) {
        test_case(decim);
    }
}