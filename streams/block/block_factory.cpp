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
    if (name == "ARIA")  return std::make_unique<aria::aria>(round, encrypt);
    if (name == "CAMELLIA")  return std::make_unique<camellia::camellia>(round, encrypt);
    if (name == "CAST")  return std::make_unique<cast::cast>(round);
    if (name == "IDEA")  return std::make_unique<idea::idea>(round, encrypt);
    if (name == "RC4")  return std::make_unique<rc4>(round, block_size);
    if (name == "GOST")  return std::make_unique<gost>(round);
    if (name == "SIMON")  return std::make_unique<simon>(round, block_size, key_size);
    if (name == "SPECK")  return std::make_unique<speck>(round, block_size, key_size);
    if (name == "SINGLE-DES")  return std::make_unique<single_des>(round, encrypt);
    if (name == "TRIPLE-DES")  return std::make_unique<triple_des>(round, encrypt);
    if (name == "BLOWFISH") return std::make_unique<blowfish_factory>(round);
    if (name == "MARS") return std::make_unique<mars::mars>(round, encrypt);
    if (name == "RC6") return std::make_unique<rc6::rc6>(round, encrypt);
    if (name == "SERPENT") return std::make_unique<serpent::serpent>(round, encrypt);
    if (name == "TWOFISH") return std::make_unique<twofish::twofish>(round);
    if (name == "SEED") return std::make_unique<seed::seed>(round);
    // clang-format on

    throw std::runtime_error("requested block cipher named \"" + name +
                             "\" is either broken or does not exists");
}

} //namespace block
