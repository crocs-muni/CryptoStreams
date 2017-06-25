#ifndef EACIRC_STREAMS_SHA3_TEST_CASE_H
#define EACIRC_STREAMS_SHA3_TEST_CASE_H

#include <streams/sha3/sha3_interface.h>
#include <string>
#include <stdexcept>
#include <algorithm>

class sha3_test_case {

private:
    std::string _input;
    std::string _output;

    int _length;

public:

    void operator()(std::unique_ptr<sha3_interface>& hasher);

    friend std::istream& operator>>( std::istream& input, sha3_test_case& test_case) {
        input >> test_case._length;
        input >> test_case._input;
        input >> test_case._output;

        // Hexadecimal to lower case
        std::transform(test_case._output.begin(), test_case._output.end(), test_case._output.begin(), ::tolower);

        return input;
    }

private:
    unsigned char hex2char(char input) {
        if(input >= '0' && input <= '9')
            return input - '0';
        if(input >= 'A' && input <= 'F')
            return input - 'A' + 10;
        if(input >= 'a' && input <= 'f')
            return input - 'a' + 10;
        throw std::invalid_argument("Invalid input string");
    }
};


#endif //EACIRC_STREAMS_SHA3_TEST_CASE_H
