#include "common_functions.h"

namespace testsuite {

value_type hex_to_bin(const char input) {
    if (input >= '0' && input <= '9')
        return input - '0';
    if (input >= 'A' && input <= 'F')
        return input - 'A' + 10;
    if (input >= 'a' && input <= 'f')
        return input - 'a' + 10;
    throw std::invalid_argument("Invalid input string");
}

std::vector<value_type> hex_string_to_binary(const std::string &str) {
    assert(str.length() % 2 == 0);
    std::vector<value_type> output(str.length() / 2);

    for (uint32_t i = 0; i < str.size() / 2; i++) {
        output[i] = (hex_to_bin(str.at(2 * i)) << 4) + hex_to_bin(str.at(2 * i + 1));
    }

    return output;
}

} // namespace testsuite
