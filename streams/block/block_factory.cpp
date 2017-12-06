#include "block_factory.h"

namespace block {

struct block_cipher;

std::unique_ptr<block_cipher> make_block_cipher(const std::string& name, unsigned round, unsigned block_size, bool encrypt) {
    // clang-format off
    if (name == "TEA")  return std::make_unique<tea>(round);
    if (name == "AES")  return std::make_unique<aes>(round);
    if (name == "RC4")  return std::make_unique<rc4>(round, block_size);
    if (name == "SINGLE-DES")  return std::make_unique<single_des>(round, encrypt);
    if (name == "TRIPLE-DES")  return std::make_unique<triple_des>(round, encrypt);
    if (name == "BLOWFISH") return std::make_unique<blowfish_factory>(round);
    // clang-format on

    throw std::runtime_error("requested block cipher named \"" + name +
                             "\" is either broken or does not exists");
}

} //namespace block
