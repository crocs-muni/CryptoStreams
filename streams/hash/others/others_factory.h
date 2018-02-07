#include "../hash_factory.h"

namespace others {

class others_factory : hash::hash_factory {

    std::unique_ptr<hash::hash_interface> create(const std::string& algorithm, const unsigned rounds);

};

} // namespace others
