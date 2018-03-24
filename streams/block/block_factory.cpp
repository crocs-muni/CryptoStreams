#include "block_factory.h"

namespace block {

struct block_cipher;

std::unique_ptr<block_cipher> make_block_cipher(const std::string& name,
                                                const std::size_t round,
                                                const std::size_t block_size,
                                                const std::size_t key_size,
                                                const bool encrypt) {
    // clang-format off
    if (name == "TEA")  return std::make_unique<tea>(round);
    if (name == "AES")  return std::make_unique<aes>(round);
    if (name == "RC4")  return std::make_unique<rc4>(round, block_size);
    if (name == "SIMON")  return std::make_unique<simon>(round, block_size, key_size);
    if (name == "SPECK")  return std::make_unique<speck>(round, block_size, key_size);
    if (name == "SINGLE-DES")  return std::make_unique<single_des>(round, encrypt);
    if (name == "TRIPLE-DES")  return std::make_unique<triple_des>(round, encrypt);
    if (name == "BLOWFISH") return std::make_unique<blowfish_factory>(round);
    if (name == "TWOFISH") return std::make_unique<twofish_factory>(round);
    if (name == "RC6") return std::make_unique<rc6_factory>(round);
    // clang-format on

    throw std::runtime_error("requested block cipher named \"" + name +
                             "\" is either broken or does not exists");
}

} //namespace block
