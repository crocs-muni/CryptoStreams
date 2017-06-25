#include <ios>
#include <sstream>
#include <memory>
#include <iostream>
#include <iomanip>
#include <gtest/gtest.h>
#include "sha3_test_case.h"


void sha3_test_case::operator()(std::unique_ptr<sha3_interface>& hasher) {
    unsigned char hash[_output.size()/2];
    unsigned char input[_input.size()/2];

    assert(_input.size() % 2 == 0);

    for (int i = 0; i < _input.size()/2; i++) {
        input[i] = (hex2char(_input.at(2*i)) << 4) + hex2char(_input.at(2*i + 1));
    }

    hasher->Hash(_output.size()*4, input, _length, hash);

    std::stringstream ss;

    for (auto byte : hash) {
        ss << std::setfill('0') << std::setw(2) << std::hex << (int) byte;
    }

    ASSERT_EQ(ss.str(), _output);
}