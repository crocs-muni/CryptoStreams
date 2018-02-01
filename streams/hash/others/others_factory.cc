#include "../hash_factory.h"
#include "../hash_interface.h"
#include <memory>
#include "hash_functions/hash_functions.h"

namespace others {

void _check_rounds(const std::string& algorithm, const unsigned rounds) {
    if (rounds > 0)
        throw std::runtime_error{"requested hash algorithm named \"" + algorithm +
                                 "\" cannot be limited in rounds"};
}

std::unique_ptr<hash::hash_interface> hash::hash_factory::create(const std::string& name, const unsigned rounds) {
    // clang-format off
    // clang-format on

    if (name == "SHA") return std::make_unique<sha1_factory>(rounds);

    throw std::runtime_error("requested hash algorithm named \"" + name +
                             "\" is either broken or does not exists");
}

} // namespace others
