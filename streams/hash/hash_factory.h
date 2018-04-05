#pragma once

#include <memory>
#include <string>

namespace hash {

    struct hash_interface;

    void _check_rounds(const std::string& algorithm, const unsigned rounds);

    struct hash_factory {
        static std::unique_ptr<hash_interface> create(const std::string& algorithm, const unsigned rounds);
    };

} // namespace hash
