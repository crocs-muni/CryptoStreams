#pragma once

#include <memory>

struct sha3_interface;

struct sha3_factory {
    static std::unique_ptr<sha3_interface> create(const std::string& algorithm, unsigned rounds);
};
