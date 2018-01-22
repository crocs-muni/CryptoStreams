#pragma once

#include <memory>

namespace others {

struct others_interface;

void _check_rounds(const std::string& algorithm, const unsigned rounds);

struct others_factory {
    static std::unique_ptr<others_interface> create(const std::string& algorithm, const unsigned rounds);
};

} // namespace others
