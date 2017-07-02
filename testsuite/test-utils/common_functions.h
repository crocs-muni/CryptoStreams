#ifndef EACIRC_STREAMS_COMMON_FUNCTIONS_H
#define EACIRC_STREAMS_COMMON_FUNCTIONS_H

#include <stdexcept>

namespace testsuite {

    static unsigned char hex2char(char input) {
        if (input >= '0' && input <= '9')
            return input - '0';
        if (input >= 'A' && input <= 'F')
            return input - 'A' + 10;
        if (input >= 'a' && input <= 'f')
            return input - 'a' + 10;
        throw std::invalid_argument("Invalid input string");

    }

    static void hex_string2string(std::string& str) {
        assert(str.length()  % 2 == 0);

        for (int i = 0; i < str.size() / 2; i++) {
            str[i] = (hex2char(str.at(2 * i)) << 4) + hex2char(str.at(2 * i + 1));
        }

        str.resize(str.length()/2);
    }
}

#endif