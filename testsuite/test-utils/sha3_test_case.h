#ifndef EACIRC_STREAMS_SHA3_TEST_CASE_H
#define EACIRC_STREAMS_SHA3_TEST_CASE_H

#include <streams/sha3/sha3_interface.h>
#include <string>
#include <algorithm>
#include <stream.h>
#include "common_functions.h"

namespace testsuite {

    class sha3_test_case {

    private:
        std::string _plaintext;
        std::string _hash;

        int _length;

    public:

        void operator()(std::unique_ptr<sha3_interface> &hasher);

        void operator()(std::unique_ptr<stream> &stream);

        friend std::istream &operator>>(std::istream &input, sha3_test_case &test_case) {
            input >> test_case._length;
            input >> test_case._plaintext;
            input >> test_case._hash;

            // Hexadecimal to lower case, in order to be compatible with std::hex
            std::transform(test_case._hash.begin(), test_case._hash.end(), test_case._hash.begin(), ::tolower);

            return input;
        }

        size_t length();
        std::string input();

    };
}


#endif //EACIRC_STREAMS_SHA3_TEST_CASE_H
