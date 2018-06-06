#pragma once

/**
 * Source: https://github.com/B-Con/crypto-algorithms
 */

#include "../../stream_interface.h"
#include <memory>

namespace stream_ciphers {
namespace others {

class rc4 : public stream_interface {

    /* Data structures */

    struct rc4_ctx {

        rc4_ctx()
            : state{0} {}

        std::uint8_t state[256];
        std::unique_ptr<std::uint8_t[]> key;
        std::uint32_t key_size;
        int i, j;
    } _ctx;

public:
    rc4(int rounds)
        : stream_interface(rounds) {}

    void init() override {}

    void keysetup(const u8* key, const u32 key_bitsize, const u32 iv_bitsize) override;

    void ivsetup(const u8* iv) override;

    void encrypt_bytes(const u8* plaintext, u8* ciphertext, const u32 ptx_size) override;

    void decrypt_bytes(const u8* ciphertext, u8* plaintext, const u32 ctx_size) override;
};

} // namespace others
} // namespace stream_ciphers
