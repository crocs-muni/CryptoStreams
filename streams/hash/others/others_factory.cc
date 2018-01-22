#include "others_factory.h"
#include "others_interface.h"
#include <eacirc-core/memory.h>

namespace others {

void _check_rounds(const std::string& algorithm, const unsigned rounds) {
    if (rounds > 0)
        throw std::runtime_error{"requested hash algorithm named \"" + algorithm +
                                 "\" cannot be limited in rounds"};
}

std::unique_ptr<others_interface> others_factory::create(const std::string& name, const unsigned rounds) {
    // clang-format off
    // clang-format on

    throw std::runtime_error("requested hash algorithm named \"" + name +
                             "\" is either broken or does not exists");
}

} // namespace others
