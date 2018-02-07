#include <streams/hash/others/hash_functions/sha1/sha1_factory.h>
#include <streams/hash/others/hash_functions/sha2/sha256_factory.h>
#include <streams/hash/others/hash_functions/md5/md5_factory.h>
#include "others_factory.h"

namespace others {

std::unique_ptr<hash::hash_interface> create(const std::string& algorithm, const unsigned rounds){
    // clang-format off
    if (algorithm == "SHA1")         return std::make_unique<others::sha1_factory>(rounds);
    if (algorithm == "SHA2")        return std::make_unique<others::sha256_factory>(rounds);
    if (algorithm == "MD5")         return std::make_unique<others::md5_factory>(rounds);
    // clang-format on
}

} // namespace others