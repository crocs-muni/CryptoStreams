#pragma once

#include <memory>
#include <string>

namespace sha3 {

struct sha3_interface;

void _check_rounds(const std::string& algorithm, const unsigned rounds);

struct sha3_factory {
    static std::unique_ptr<sha3_interface> create(const std::string& algorithm, const unsigned rounds);
};

} // namespace sha3
