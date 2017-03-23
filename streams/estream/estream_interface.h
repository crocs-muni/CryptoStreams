#pragma once

#include "ciphers/ecrypt-portable.h"

struct estream_interface {
    estream_interface()
        : estream_interface(-1) {}

    estream_interface(int rounds)
        : _rounds(rounds) {}

    virtual ~estream_interface() = default;

    virtual void ECRYPT_init() = 0;

    virtual void ECRYPT_keysetup(const u8* key, u32 keysize, u32 ivsize) = 0;
    virtual void ECRYPT_ivsetup(const u8* iv) = 0;

    virtual void ECRYPT_encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) = 0;
    virtual void ECRYPT_decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) = 0;

protected:
    const int _rounds;
};
