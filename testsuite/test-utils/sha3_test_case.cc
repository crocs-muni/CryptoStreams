#include <ios>
#include <sstream>
#include <memory>
#include <iostream>
#include <iomanip>
#include <gtest/gtest.h>
#include "sha3_test_case.h"

namespace testsuite {

    void sha3_test_case::operator()(std::unique_ptr<sha3_interface> &hasher) {
        unsigned char hash[_hash.size() / 2];

        hex_string2string(_plaintext); //

        hasher->Hash(_hash.size() * 4, (unsigned char*) _plaintext.data(), _length, hash);

        std::stringstream ss;

        for (auto byte : hash) {
            ss << std::setfill('0') << std::setw(2) << std::hex << (int) byte;
        }

        ASSERT_EQ(ss.str(), _hash);
    }

    size_t sha3_test_case::length() {
        return (size_t) _length;
    }

    void sha3_test_case::operator()(std::unique_ptr<stream> &s) {
        std::stringstream ss;

        for (auto byte : s->next()) {
            ss << std::setfill('0') << std::setw(2) << std::hex << (int) byte;
        }

        ASSERT_EQ(ss.str(), _hash);
    }

    std::string sha3_test_case::input() {
        return _plaintext;
    }

}