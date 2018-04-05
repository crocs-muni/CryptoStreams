//
// Created by Dusan Klinec on 23/03/2018.
// https://tools.ietf.org/html/rfc7539
// https://github.com/wg/c20p1305/blob/master/ecrypt-sync.h
// https://github.com/sysrepo/openssh-sysrepo/blob/master/chacha.c
// https://botan.randombit.net/doxygen/chacha_8cpp_source.html
// https://tools.ietf.org/html/draft-strombergson-chacha-test-vectors-01


#pragma once

#include "../../stream_interface.h"


namespace stream_ciphers {
namespace others {

#define CHACHA_FULL_ROUNDS 20

class Chacha : public stream_interface {

    /* Data structures */
    struct CHACHA_ctx {
        u32 input[16]; /* could be compressed */
    } _ctx;

public:
    Chacha(int rounds=CHACHA_FULL_ROUNDS)
        : stream_interface(rounds) {}

    void init() override {}

    void keysetup(const u8* key, const u32 key_bitsize, const u32 iv_bitsize) override;

    void ivsetup(const u8* iv) override;

    void encrypt_bytes(const u8* plaintext, u8* ciphertext, const u32 ptx_size) override;

    void decrypt_bytes(const u8* ciphertext, u8* plaintext, const u32 ctx_size) override;
};

} // namespace others
} // namespace stream_ciphers
