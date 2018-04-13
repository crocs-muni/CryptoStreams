#include "stream_cipher.h"
#include "stream_interface.h"
#include <algorithm>
#include <streams.h>

#include "estream/abc/ecrypt-sync.h"
#include "estream/achterbahn/ecrypt-sync.h"
#include "estream/cryptmt/ecrypt-sync.h"
#include "estream/decim/ecrypt-sync.h"
#include "estream/dicing/ecrypt-sync.h"
#include "estream/dragon/ecrypt-sync.h"
#include "estream/edon80/ecrypt-sync.h"
#include "estream/ffcsr/ecrypt-sync.h"
#include "estream/fubuki/ecrypt-sync.h"
#include "estream/grain/ecrypt-sync.h"
#include "estream/hc-128/ecrypt-sync.h"
#include "estream/hermes/ecrypt-sync.h"
#include "estream/lex/ecrypt-sync.h"
#include "estream/mag/ecrypt-sync.h"
#include "estream/mickey/ecrypt-sync.h"
#include "estream/mir-1/ecrypt-sync.h"
#include "estream/pomaranch/ecrypt-sync.h"
#include "estream/py/ecrypt-sync.h"
#include "estream/rabbit/ecrypt-sync.h"
#include "estream/salsa20/ecrypt-sync.h"
#include "estream/sfinks/ecrypt-sync.h"
#include "estream/sosemanuk/ecrypt-sync.h"
#include "estream/trivium/ecrypt-sync.h"
#include "estream/tsc-4/ecrypt-sync.h"
#include "estream/wg/ecrypt-sync.h"
// #include "estream/yamb/ecrypt-sync.h"        // stopped working after IDE update
#include "estream/zk-crypt/ecrypt-sync.h"

#include "other/chacha/chacha.h"
#include "other/rc4/rc4.h"

namespace stream_ciphers {

std::unique_ptr<stream_interface> create_stream_cipher(const std::string &name,
                                                       const unsigned round) {
    // clang-format off
    // eSTREAM
    if (name == "ABC")              return std::make_unique<estream::ECRYPT_ABC>();
    if (name == "Achterbahn")       return std::make_unique<estream::ECRYPT_Achterbahn>();
    if (name == "CryptMT")          return std::make_unique<estream::ECRYPT_Cryptmt>();
    if (name == "DECIM")            return std::make_unique<estream::ECRYPT_Decim>(round);
    if (name == "DICING")           return std::make_unique<estream::ECRYPT_Dicing>();
    if (name == "Dragon")           return std::make_unique<estream::ECRYPT_Dragon>(round);
    if (name == "Edon80")           return std::make_unique<estream::ECRYPT_Edon80>();
    if (name == "F-FCSR")           return std::make_unique<estream::ECRYPT_FFCSR>(round);
    if (name == "Fubuki")           return std::make_unique<estream::ECRYPT_Fubuki>(round);
    if (name == "Grain")            return std::make_unique<estream::ECRYPT_Grain>(round);
    if (name == "HC-128")           return std::make_unique<estream::ECRYPT_HC128>();
    if (name == "Hermes")           return std::make_unique<estream::ECRYPT_Hermes>(round);
    if (name == "LEX")              return std::make_unique<estream::ECRYPT_Lex>(round);
    if (name == "MAG")              return std::make_unique<estream::ECRYPT_Mag>();
    if (name == "MICKEY")           return std::make_unique<estream::ECRYPT_Mickey>(round);
    if (name == "Mir-1")            return std::make_unique<estream::ECRYPT_Mir>();
    if (name == "Pomaranch")        return std::make_unique<estream::ECRYPT_Pomaranch>();
    if (name == "Py")               return std::make_unique<estream::ECRYPT_Py>();
    if (name == "Rabbit")           return std::make_unique<estream::ECRYPT_Rabbit>(round);
    if (name == "Salsa20")          return std::make_unique<estream::ECRYPT_Salsa>(round);
    if (name == "SFINKS")           return std::make_unique<estream::ECRYPT_Sfinks>();
    if (name == "SOSEMANUK")        return std::make_unique<estream::ECRYPT_Sosemanuk>(round);
    if (name == "Trivium")          return std::make_unique<estream::ECRYPT_Trivium>(round);
    if (name == "TSC-4")            return std::make_unique<estream::ECRYPT_Tsc4>(round);
    if (name == "WG")               return std::make_unique<estream::ECRYPT_Wg>();
    // if (name == "Yamb")             return std::make_unique<estream::ECRYPT_Yamb>();
    if (name == "Zk-Crypt")         return std::make_unique<estream::ECRYPT_Zkcrypt>();

    // other
    if (name == "Chacha")           return std::make_unique<others::Chacha>(round);
    if (name == "RC4")              return std::make_unique<others::rc4>(round);
    // clang-format on

    throw std::runtime_error("requested eSTREAM cipher named \"" + name +
                             "\" is either broken or does not exists");
}

stream_cipher::stream_cipher(const std::string &name,
                             const unsigned round,
                             const std::size_t iv_size,
                             const std::size_t key_size)
    : _iv(iv_size)
    , _key(key_size)
    , _encryptor(create_stream_cipher(name, round))
    , _decryptor(create_stream_cipher(name, round)) {
    _encryptor->init();
    _decryptor->init();
}

stream_cipher::stream_cipher(stream_cipher &&) = default;
stream_cipher::~stream_cipher() = default;

void stream_cipher::setup_key_iv(std::unique_ptr<stream> &key, std::unique_ptr<stream> &iv) {
    vec_cview key_data = key->next();
    _key.assign(key_data.begin(), key_data.end());

    _encryptor->keysetup(_key.data(), u32(8 * key->osize()), u32(8 * iv->osize()));
    _decryptor->keysetup(_key.data(), u32(8 * key->osize()), u32(8 * iv->osize()));

    vec_cview iv_data = iv->next();
    _iv.assign(iv_data.begin(), iv_data.end());

    _encryptor->ivsetup(_iv.data());
    _decryptor->ivsetup(_iv.data());
}

void stream_cipher::encrypt(const u8 *plaintext, u8 *ciphertext, std::size_t size) {
    // BEWARE: only able to proccess max 2GB of plaintext
    _encryptor->encrypt_bytes(plaintext, ciphertext, u32(size));
}

void stream_cipher::decrypt(const u8 *ciphertext, u8 *plaintext, std::size_t size) {
    // BEWARE: only able to proccess max 2GB of plaintext
    _decryptor->decrypt_bytes(ciphertext, plaintext, u32(size));
}

} // namespace stream_ciphers
