#include "estream_cipher.h"
#include "estream_interface.h"
#include <algorithm>
#include <streams.h>

#include "ciphers/abc/ecrypt-sync.h"
#include "ciphers/achterbahn/ecrypt-sync.h"
#include "ciphers/chacha/ecrypt-sync.h"
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
#include "ciphers/trivium/ecrypt-sync.h"
#include "ciphers/tsc-4/ecrypt-sync.h"
#include "ciphers/wg/ecrypt-sync.h"
// #include "ciphers/yamb/ecrypt-sync.h"        // stopped working after IDE update
#include "ciphers/zk-crypt/ecrypt-sync.h"

std::unique_ptr<estream_interface> create_estream_cipher(const std::string& name, const unsigned round) {
  // clang-format off
    if (name == "ABC")              return std::make_unique<ECRYPT_ABC>();
    if (name == "Achterbahn")       return std::make_unique<ECRYPT_Achterbahn>();
    if (name == "Chacha")           return std::make_unique<ECRYPT_Chacha>(round);
    if (name == "CryptMT")          return std::make_unique<ECRYPT_Cryptmt>();
    if (name == "DECIM")            return std::make_unique<ECRYPT_Decim>(round);
    if (name == "DICING")           return std::make_unique<ECRYPT_Dicing>();
    if (name == "Dragon")           return std::make_unique<ECRYPT_Dragon>(round);
    if (name == "Edon80")           return std::make_unique<ECRYPT_Edon80>();
    if (name == "F-FCSR")           return std::make_unique<ECRYPT_FFCSR>(round);
    if (name == "Fubuki")           return std::make_unique<ECRYPT_Fubuki>(round);
    if (name == "Grain")            return std::make_unique<ECRYPT_Grain>(round);
    if (name == "HC-128")           return std::make_unique<ECRYPT_HC128>();
    if (name == "Hermes")           return std::make_unique<ECRYPT_Hermes>(round);
    if (name == "LEX")              return std::make_unique<ECRYPT_Lex>(round);
    if (name == "MAG")              return std::make_unique<ECRYPT_Mag>();
    if (name == "MICKEY")           return std::make_unique<ECRYPT_Mickey>(round);
    if (name == "Mir-1")            return std::make_unique<ECRYPT_Mir>();
    if (name == "Pomaranch")        return std::make_unique<ECRYPT_Pomaranch>();
    if (name == "Py")               return std::make_unique<ECRYPT_Py>();
    if (name == "Rabbit")           return std::make_unique<ECRYPT_Rabbit>(round);
    if (name == "Salsa20")          return std::make_unique<ECRYPT_Salsa>(round);
    if (name == "SFINKS")           return std::make_unique<ECRYPT_Sfinks>();
    if (name == "SOSEMANUK")        return std::make_unique<ECRYPT_Sosemanuk>(round);
    if (name == "Trivium")          return std::make_unique<ECRYPT_Trivium>(round);
    if (name == "TSC-4")            return std::make_unique<ECRYPT_Tsc4>(round);
    if (name == "WG")               return std::make_unique<ECRYPT_Wg>();
    // if (name == "Yamb")             return std::make_unique<ECRYPT_Yamb>();
    if (name == "Zk-Crypt")         return std::make_unique<ECRYPT_Zkcrypt>();
  // clang-format on

  throw std::runtime_error("requested eSTREAM cipher named \"" + name +
                           "\" is either broken or does not exists");
}

estream_cipher::estream_cipher(const std::string& name, const unsigned round, std::size_t iv_size, std::size_t key_size)
    : _iv(iv_size)
    , _key(key_size)
    , _encryptor(create_estream_cipher(name, round))
    , _decryptor(create_estream_cipher(name, round)) {
    _encryptor->ECRYPT_init();
    _decryptor->ECRYPT_init();
}

estream_cipher::estream_cipher(estream_cipher&&) = default;
estream_cipher::~estream_cipher() = default;

void estream_cipher::setup_key_iv(std::unique_ptr<stream>& key, std::unique_ptr<stream>& iv) {
  vec_cview key_data = key->next();
  _key.assign(key_data.begin(), key_data.end());

  _encryptor->ECRYPT_keysetup(_key.data(), u32(8 * key->osize()), u32(8 * iv->osize()));
  _decryptor->ECRYPT_keysetup(_key.data(), u32(8 * key->osize()), u32(8 * iv->osize()));

  vec_cview  iv_data = iv->next();
  _iv.assign(iv_data.begin(), iv_data.end());

  _encryptor->ECRYPT_ivsetup(_iv.data());
  _decryptor->ECRYPT_ivsetup(_iv.data());
}

void estream_cipher::encrypt(const u8* plaintext, u8* ciphertext, std::size_t size) {
  // BEWARE: only able to proccess max 2GB of plaintext
  _encryptor->ECRYPT_encrypt_bytes(plaintext, ciphertext, u32(size));
}

void estream_cipher::decrypt(const u8* ciphertext, u8* plaintext, std::size_t size) {
  // BEWARE: only able to proccess max 2GB of plaintext
  _decryptor->ECRYPT_decrypt_bytes(ciphertext, plaintext, u32(size));
}
