#include "hash_factory.h"
#include "others/hash_functions/hash_functions.h"
#include "sha3/hash_functions/hash_functions.h"
#include "hash_interface.h"

namespace hash {

void _check_rounds(const std::string& algorithm, const unsigned rounds) {
    if (rounds > 0)
        throw std::runtime_error{"requested hash algorithm named \"" + algorithm +
                                 "\" cannot be limited in rounds"};
}

std::unique_ptr<hash_interface> hash_factory::create(const std::string& name, const unsigned rounds) {
    // clang-format off
    if (name == "Abacus")         return std::make_unique<sha3::Abacus>(rounds);
    if (name == "ARIRANG")        return std::make_unique<sha3::Arirang>(rounds);
    if (name == "AURORA")         return std::make_unique<sha3::Aurora>(rounds);
    if (name == "BLAKE")          return std::make_unique<sha3::Blake>(rounds);
    if (name == "Blender")        return std::make_unique<sha3::Blender>(rounds);
    if (name == "BMW")            return std::make_unique<sha3::BMW>(rounds);
    if (name == "Boole")          return std::make_unique<sha3::Boole>(rounds);
    if (name == "Cheetah")        return std::make_unique<sha3::Cheetah>(rounds);
    if (name == "CHI")            return std::make_unique<sha3::Chi>(rounds);
    if (name == "CRUNCH")         return std::make_unique<sha3::Crunch>(rounds);
    if (name == "CubeHash")       return std::make_unique<sha3::Cubehash>(rounds);
    if (name == "DCH")            return std::make_unique<sha3::DCH>(rounds);
    if (name == "DynamicSHA")     return std::make_unique<sha3::DSHA>(rounds);
    if (name == "DynamicSHA2")    return std::make_unique<sha3::DSHA2>(rounds);
    if (name == "ECHO")           return std::make_unique<sha3::Echo>(rounds);
    // if (name == "ECOH")           return std::make_unique<Ecoh>(rounds);
    if (name == "EDON") {         _check_rounds(name, rounds);
                                  return std::make_unique<sha3::Edon>();
    }
    // if (name == "EnRUPT")         return std::make_unique<Enrupt>(rounds);
    if (name == "ESSENCE")        return std::make_unique<sha3::Essence>(rounds);
    if (name == "Fugue")          return std::make_unique<sha3::Fugue>(rounds);
    if (name == "Grostl")         return std::make_unique<sha3::Grostl>(rounds);
    if (name == "Hamsi")          return std::make_unique<sha3::Hamsi>(rounds);
    if (name == "JH")             return std::make_unique<sha3::JH>(rounds);
    if (name == "Keccak")         return std::make_unique<sha3::Keccak>(rounds);
    if (name == "Khichidi") {     _check_rounds(name, rounds);
                                  return std::make_unique<sha3::Khichidi>();
    }
    if (name == "LANE")           return std::make_unique<sha3::Lane>(rounds);
    if (name == "Lesamnta")       return std::make_unique<sha3::Lesamnta>(rounds);
    if (name == "Luffa")          return std::make_unique<sha3::Luffa>(rounds);
    // if (name == "LUX")            return std::make_unique<Lux>(rounds);
    if (name == "MCSSHA3") {      _check_rounds(name, rounds);
                                  return std::make_unique<sha3::Mscsha>();
    }
    if (name == "MD6")            return std::make_unique<sha3::MD6>(rounds);
    if (name == "MeshHash")       return std::make_unique<sha3::MeshHash>(rounds);
    if (name == "NaSHA") {        _check_rounds(name, rounds);
                                  return std::make_unique<sha3::Nasha>();
    }
    // if (name == "SANDstorm")      return std::make_unique<SandStorm>(rounds);
    if (name == "Sarmal")         return std::make_unique<sha3::Sarmal>(rounds);
    if (name == "Shabal") {       _check_rounds(name, rounds);
                                  return std::make_unique<sha3::Shabal>();
    }
    if (name == "SHAMATA") {      _check_rounds(name, rounds);
                                  return std::make_unique<sha3::Shamata>();
    }
    if (name == "SHAvite3")       return std::make_unique<sha3::SHAvite>(rounds);
    if (name == "SIMD")           return std::make_unique<sha3::Simd>(rounds);
    if (name == "Skein")          return std::make_unique<sha3::Skein>(rounds);
    if (name == "SpectralHash") { _check_rounds(name, rounds);
                                  return std::make_unique<sha3::SpectralHash>();
    }
    if (name == "StreamHash") {   _check_rounds(name, rounds);
                                  return std::make_unique<sha3::StreamHash>();
    }
    // if (name == "SWIFFTX")        return std::make_unique<Swifftx>(rounds);
    if (name == "Tangle")         return std::make_unique<sha3::Tangle>(rounds);
    // if (name == "TIB3")           return std::make_unique<Tib>(rounds);
    if (name == "Twister")        return std::make_unique<sha3::Twister>(rounds);
    //if (name == "Vortex")         return std::make_unique<Vortex>(rounds);
    if (name == "WaMM")           return std::make_unique<sha3::WaMM>(rounds);
    if (name == "Waterfall")      return std::make_unique<sha3::Waterfall>(rounds);
    if (name == "Tangle2")        return std::make_unique<sha3::Tangle2>(rounds);

    if (name == "SHA1")           return std::make_unique<others::sha1_factory>(rounds);
    if (name == "SHA2")           return std::make_unique<others::sha256_factory>(rounds);
    if (name == "MD5")            return std::make_unique<others::md5_factory>(rounds);
    if (name == "RIPEMD160")      return std::make_unique<others::Ripemd160>(rounds);
    if (name == "Tiger")          return std::make_unique<others::Tiger>(rounds);
    if (name == "Whirlpool")      return std::make_unique<others::Whirlpool>(rounds);
    // clang-format on

    throw std::runtime_error("requested hash algorithm named \"" + name +
                             "\" is either broken or does not exists");
}

} // namespace hash
