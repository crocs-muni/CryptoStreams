#pragma once

#include <stdexcept>
#include <stream.h>

namespace testsuite {

/**
 * Converts hexadecimal string representation of char to byte representation
 * @param input String to convert (characters 0[48]-9[57] and A[65] - F[70] and a[97] - f[102])
 * @return one byte (0-15)
 */
value_type hex_to_bin(const char input);

/**
 * For each character calls hex_to_bin function
 * It treats two adjacent characters as one byte
 * @param str String to convert
 * @attention Function works only for strings with even length
 */
std::vector<value_type> hex_string_to_binary(const std::string &str);

} // namespace testsuite
