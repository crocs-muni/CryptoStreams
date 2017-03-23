#pragma once

#include <memory>
#include "block_cipher.h"

#include <algorithm>
#include <core/memory.h>

#include "ciphers/tea/tea.h"
#include "ciphers/aes/aes.h"
#include "ciphers/rc4/rc4.h"
#include "ciphers/des/single_des.h"
#include "ciphers/des/triple_des.h"


namespace block {
    struct block_cipher;

    std::unique_ptr<block_cipher> make_block_cipher(const std::string& name, unsigned round, unsigned block_size, bool encrypt) {
        // clang-format off
        if (name == "TEA")  return std::make_unique<tea>(round);
        if (name == "AES")  return std::make_unique<aes>(round);
        if (name == "RC4")  return std::make_unique<rc4>(round, block_size);
        if (name == "SINGLE-DES")  return std::make_unique<single_des>(round, encrypt);
        if (name == "TRIPLE-DES")  return std::make_unique<triple_des>(round, encrypt);
        // clang-format on

        throw std::runtime_error("requested block cipher named \"" + name +
                                 "\" is either broken or does not exists");
    }
};
