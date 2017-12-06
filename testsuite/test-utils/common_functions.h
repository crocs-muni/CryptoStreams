#ifndef EACIRC_STREAMS_COMMON_FUNCTIONS_H
#define EACIRC_STREAMS_COMMON_FUNCTIONS_H

#include <stdexcept>
#include <stream.h>

namespace testsuite {

    /**
     * Converts hexadecimal string representation of char to byte representation
     * @param input String to convert (characters 0[48]-9[57] and A[65] - F[70] and a[97] - f[102])
     * @return one byte (0-15)
     */
    static value_type hex_to_bin(const char input) {
        if (input >= '0' && input <= '9')
            return input - '0';
        if (input >= 'A' && input <= 'F')
            return input - 'A' + 10;
        if (input >= 'a' && input <= 'f')
            return input - 'a' + 10;
        throw std::invalid_argument("Invalid input string");

    }

    /**
     * For each character calls hex_to_bin function
     * It treats two adjacent characters as one byte
     * @param str String to convert
     * @attention Function works only for strings with even length
     */
    static std::vector<value_type> hex_string_to_binary(const std::string& str) {
        assert(str.length() % 2 == 0);
        std::vector<value_type> output(str.length() / 2);

        for (uint32_t i = 0; i < str.size() / 2; i++) {
            output[i] = (hex_to_bin(str.at(2 * i)) << 4) + hex_to_bin(str.at(2 * i + 1));
        }

        return output;
    }
}

#endif
