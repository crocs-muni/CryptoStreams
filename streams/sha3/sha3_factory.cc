#include "sha3_factory.h"
#include "hash_functions/hash_functions.h"
#include "sha3_interface.h"
#include <core/memory.h>

static void _check_rounds(const std::string& algorithm, unsigned rounds) {
    if (rounds > 0)
        throw std::runtime_error{"requested SHA-3 algorithm named \"" + algorithm +
                                 "\" cannot be limited in rounds"};
}

std::unique_ptr<sha3_interface> sha3_factory::create(const std::string& name, unsigned rounds) {
    // clang-format off
    if (name == "Abacus")         return std::make_unique<Abacus>(rounds);
    if (name == "ARIRANG")        return std::make_unique<Arirang>(rounds);
    if (name == "AURORA")         return std::make_unique<Aurora>(rounds);
    if (name == "BLAKE")          return std::make_unique<Blake>(rounds);
    if (name == "Blender")        return std::make_unique<Blender>(rounds);
    if (name == "BMW")            return std::make_unique<BMW>(rounds);
    if (name == "Boole")          return std::make_unique<Boole>(rounds);
    if (name == "Cheetah")        return std::make_unique<Cheetah>(rounds);
    if (name == "CHI")            return std::make_unique<Chi>(rounds);
    if (name == "CRUNCH")         return std::make_unique<Crunch>(rounds);
    if (name == "CubeHash")       return std::make_unique<Cubehash>(rounds);
    if (name == "DCH")            return std::make_unique<DCH>(rounds);
    if (name == "DynamicSHA")     return std::make_unique<DSHA>(rounds);
    if (name == "DynamicSHA2")    return std::make_unique<DSHA2>(rounds);
    if (name == "ECHO")           return std::make_unique<Echo>(rounds);
    // if (name == "ECOH")           return std::make_unique<Ecoh>(rounds);
    if (name == "EDON") {         _check_rounds(name, rounds);
                                  return std::make_unique<Edon>();
    }
    // if (name == "EnRUPT")         return std::make_unique<Enrupt>(rounds);
    if (name == "ESSENCE")        return std::make_unique<Essence>(rounds);
    if (name == "Fugue")          return std::make_unique<Fugue>(rounds);
    if (name == "Grostl")         return std::make_unique<Grostl>(rounds);
    if (name == "Hamsi")          return std::make_unique<Hamsi>(rounds);
    if (name == "JH")             return std::make_unique<JH>(rounds);
    if (name == "Keccak")         return std::make_unique<Keccak>(rounds);
    if (name == "Khichidi") {     _check_rounds(name, rounds);
                                  return std::make_unique<Khichidi>();
    }
    if (name == "LANE")           return std::make_unique<Lane>(rounds);
    if (name == "Lesamnta")       return std::make_unique<Lesamnta>(rounds);
    if (name == "Luffa")          return std::make_unique<Luffa>(rounds);
    // if (name == "LUX")            return std::make_unique<Lux>(rounds);
    if (name == "MCSSHA3") {      _check_rounds(name, rounds);
                                  return std::make_unique<Mscsha>();
    }
    if (name == "MD6")            return std::make_unique<MD6>(rounds);
    if (name == "MeshHash")       return std::make_unique<MeshHash>(rounds);
    if (name == "NaSHA") {        _check_rounds(name, rounds);
                                  return std::make_unique<Nasha>();
    }
    // if (name == "SANDstorm")      return std::make_unique<SandStorm>(rounds);
    if (name == "Sarmal")         return std::make_unique<Sarmal>(rounds);
    if (name == "Shabal") {       _check_rounds(name, rounds);
                                  return std::make_unique<Shabal>();
    }
    if (name == "SHAMATA") {      _check_rounds(name, rounds);
                                  return std::make_unique<Shamata>();
    }
    if (name == "SHAvite3")       return std::make_unique<SHAvite>(rounds);
    if (name == "SIMD")           return std::make_unique<Simd>(rounds);
    if (name == "Skein")          return std::make_unique<Skein>(rounds);
    if (name == "SpectralHash") { _check_rounds(name, rounds);
                                  return std::make_unique<SpectralHash>();
    }
    if (name == "StreamHash") {   _check_rounds(name, rounds);
                                  return std::make_unique<StreamHash>();
    }
    // if (name == "SWIFFTX")        return std::make_unique<Swifftx>(rounds);
    if (name == "Tangle")         return std::make_unique<Tangle>(rounds);
    // if (name == "TIB3")           return std::make_unique<Tib>(rounds);
    if (name == "Twister")        return std::make_unique<Twister>(rounds);
    //if (name == "Vortex")         return std::make_unique<Vortex>(rounds);
    if (name == "WaMM")           return std::make_unique<WaMM>(rounds);
    if (name == "Waterfall")      return std::make_unique<Waterfall>(rounds);
    if (name == "Tangle2")        return std::make_unique<Tangle2>(rounds);
    // clang-format on

    throw std::runtime_error("requested SHA-3 algorithm named \"" + name +
                             "\" is either broken or does not exists");
}
