#include "estream_cipher.h"
#include "estream_interface.h"
#include <algorithm>
#include <core/memory.h>

#include "ciphers/abc/ecrypt-sync.h"
#include "ciphers/achterbahn/ecrypt-sync.h"
#include "ciphers/cryptmt/ecrypt-sync.h"
#include "ciphers/decim/ecrypt-sync.h"
#include "ciphers/dicing/ecrypt-sync.h"
#include "ciphers/dragon/ecrypt-sync.h"
#include "ciphers/edon80/ecrypt-sync.h"
#include "ciphers/ffcsr/ecrypt-sync.h"
#include "ciphers/fubuki/ecrypt-sync.h"
#include "ciphers/grain/ecrypt-sync.h"
#include "ciphers/hc-128/ecrypt-sync.h"
#include "ciphers/hermes/ecrypt-sync.h"
#include "ciphers/lex/ecrypt-sync.h"
#include "ciphers/mag/ecrypt-sync.h"
#include "ciphers/mickey/ecrypt-sync.h"
#include "ciphers/mir-1/ecrypt-sync.h"
#include "ciphers/pomaranch/ecrypt-sync.h"
#include "ciphers/py/ecrypt-sync.h"
#include "ciphers/rabbit/ecrypt-sync.h"
#include "ciphers/salsa20/ecrypt-sync.h"
#include "ciphers/sfinks/ecrypt-sync.h"
#include "ciphers/sosemanuk/ecrypt-sync.h"
#include "ciphers/tea/ecrypt-sync.h"
// #include "ciphers/trivium/ecrypt-sync.h"     // stopped working after IDE update
#include "ciphers/tsc-4/ecrypt-sync.h"
#include "ciphers/wg/ecrypt-sync.h"
// #include "ciphers/yamb/ecrypt-sync.h"        // stopped working after IDE update
#include "ciphers/zk-crypt/ecrypt-sync.h"

static estream_ivtype create_ivtype(const std::string& ivtype) {
  // clang-format off
    if (ivtype == "zeros")      return estream_ivtype::ZEROS;
    if (ivtype == "ones")       return estream_ivtype::ONES;
    if (ivtype == "random")     return estream_ivtype::RANDOM;
    if (ivtype == "biasrandom") return estream_ivtype::BIASRANDOM;
  // clang-format on

  throw std::runtime_error("requested eSTREAM IV type named \"" + ivtype + "\" does not exist");
}

static estream_keytype create_keytype(const std::string& ivtype) {
  // clang-format off
    if (ivtype == "zeros")      return estream_keytype::ZEROS;
    if (ivtype == "ones")       return estream_keytype::ONES;
    if (ivtype == "random")     return estream_keytype::RANDOM;
    if (ivtype == "biasrandom") return estream_keytype::BIASRANDOM;
  // clang-format on

  throw std::runtime_error("requested eSTREAM key type named \"" + ivtype + "\" does not exist");
}

static std::unique_ptr<estream_interface> create_cipher(const std::string& name,
                                                        core::optional<unsigned> round) {
  // clang-format off
    if (name == "ABC")              return std::make_unique<ECRYPT_ABC>();
    if (name == "Achterbahn")       return std::make_unique<ECRYPT_Achterbahn>();
    if (name == "CryptMT")          return std::make_unique<ECRYPT_Cryptmt>();
    if (name == "DECIM")            return std::make_unique<ECRYPT_Decim>(!round ? 8 : *round);
    if (name == "DICING")           return std::make_unique<ECRYPT_Dicing>();
    if (name == "Dragon")           return std::make_unique<ECRYPT_Dragon>();
    if (name == "Edon80")           return std::make_unique<ECRYPT_Edon80>();
    if (name == "F-FCSR")           return std::make_unique<ECRYPT_FFCSR>();
    if (name == "Fubuki")           return std::make_unique<ECRYPT_Fubuki>(!round ? 4 : *round);
    if (name == "Grain")            return std::make_unique<ECRYPT_Grain>(!round ? 13 : *round);
    if (name == "HC-128")           return std::make_unique<ECRYPT_HC128>();
    if (name == "Hermes")           return std::make_unique<ECRYPT_Hermes>(!round ? 10 : *round);
    if (name == "LEX")              return std::make_unique<ECRYPT_Lex>(!round ? 10 : *round);
    if (name == "MAG")              return std::make_unique<ECRYPT_Mag>();
    if (name == "MICKEY")           return std::make_unique<ECRYPT_Mickey>();
    if (name == "Mir-1")            return std::make_unique<ECRYPT_Mir>();
    if (name == "Pomaranch")        return std::make_unique<ECRYPT_Pomaranch>();
    if (name == "Py")               return std::make_unique<ECRYPT_Py>();
    if (name == "Rabbit")           return std::make_unique<ECRYPT_Rabbit>();
    if (name == "Salsa20")          return std::make_unique<ECRYPT_Salsa>(!round ? 12 : *round);
    if (name == "SFINKS")           return std::make_unique<ECRYPT_Sfinks>();
    if (name == "SOSEMANUK")        return std::make_unique<ECRYPT_Sosemanuk>();
    if (name == "TEA")              return std::make_unique<ECRYPT_TEA>();
    // if (name == "Trivium")          return std::make_unique<ECRYPT_Trivium>();
    if (name == "TSC-4")            return std::make_unique<ECRYPT_Tsc4>(!round ? 32 : *round);
    if (name == "WG")               return std::make_unique<ECRYPT_Wg>();
    // if (name == "Yamb")             return std::make_unique<ECRYPT_Yamb>();
    if (name == "Zk-Crypt")         return std::make_unique<ECRYPT_Zkcrypt>();
  // clang-format on

  throw std::runtime_error("requested eSTREAM cipher named \"" + name +
                           "\" is either broken or does not exists");
}

estream_cipher::estream_cipher(const std::string& name, core::optional<unsigned> round,
                               const std::string& ivtype, const std::string& keytype)
    : _ivtype(create_ivtype(ivtype))
    , _keytype(create_keytype(keytype))
    , _encryptor(create_cipher(name, round))
    , _decryptor(create_cipher(name, round)) {}

estream_cipher::estream_cipher(estream_cipher&&) = default;
estream_cipher::~estream_cipher() = default;

void estream_cipher::setup_iv(polymorphic_generator& rng) {
  switch (_ivtype) {
  case estream_ivtype::ZEROS:
    std::fill(_iv.begin(), _iv.end(), 0x00u);
    break;
  case estream_ivtype::ONES:
    std::fill(_iv.begin(), _iv.end(), 0x01u);
    break;
  case estream_ivtype::RANDOM:
    std::generate(_iv.begin(), _iv.end(), rng);
    break;
  case estream_ivtype::BIASRANDOM:
    throw std::logic_error("feature not yet implemented");
  }

  _encryptor->ECRYPT_ivsetup(_iv.data());
  _decryptor->ECRYPT_ivsetup(_iv.data());
}

void estream_cipher::setup_key(polymorphic_generator& rng) {
  switch (_keytype) {
  case estream_keytype::ZEROS:
    std::fill(_key.begin(), _key.end(), 0x00u);
    break;
  case estream_keytype::ONES:
    std::fill(_key.begin(), _key.end(), 0x01u);
    break;
  case estream_keytype::RANDOM:
    std::generate(_key.begin(), _key.end(), rng);
    break;
  case estream_keytype::BIASRANDOM:
    throw std::logic_error("feature not yet implemented");
  }

  _encryptor->ECRYPT_keysetup(_key.data(), 8 * block_size, 8 * block_size);
  _decryptor->ECRYPT_keysetup(_key.data(), 8 * block_size, 8 * block_size);
}

void estream_cipher::encrypt(const u8* plaintext, u8* ciphertext, std::size_t size) {
  // BEWARE: only able to proccess max 2GB of plaintext
  _encryptor->ECRYPT_encrypt_bytes(plaintext, ciphertext, u32(size));
}

void estream_cipher::decrypt(const u8* ciphertext, u8* plaintext, std::size_t size) {
  // BEWARE: only able to proccess max 2GB of plaintext
  _decryptor->ECRYPT_decrypt_bytes(ciphertext, plaintext, u32(size));
}
