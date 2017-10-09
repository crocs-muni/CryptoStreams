#ifndef EACIRC_STREAMS_COMMON_FUNCTIONS_H
#define EACIRC_STREAMS_COMMON_FUNCTIONS_H

#include <stdexcept>

namespace testsuite {

    /**
     * Converts hexadecimal string representation of char to byte representation
     * @param input String to convert (characters 0[48]-9[57] and A[65] - F[70] and a[97] - f[102])
     * @return one byte (0-15)
     */
    static unsigned char hex2char(char input) {
        if (input >= '0' && input <= '9')
            return input - '0';
        if (input >= 'A' && input <= 'F')
            return input - 'A' + 10;
        if (input >= 'a' && input <= 'f')
            return input - 'a' + 10;
        throw std::invalid_argument("Invalid input string");

    }

    /**
     * For each character calls hex2char function
     * It treats two adjacent characters as one byte
     * @param str String to convert
     * @attention Function works only for strings with even length
     */
    static void hex_string2string(std::string& str) {
        assert(str.length()  % 2 == 0);

        for (int i = 0; i < str.size() / 2; i++) {
            str[i] = (hex2char(str.at(2 * i)) << 4) + hex2char(str.at(2 * i + 1));
        }

        str.resize(str.length()/2);
    }
}

#endif