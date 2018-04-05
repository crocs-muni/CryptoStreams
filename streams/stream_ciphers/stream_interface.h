#pragma once

#include "estream/ecrypt-portable.h"


struct stream_interface {
    stream_interface()
        : stream_interface(-1) {}

    stream_interface(int rounds)
        : _rounds(rounds) {}

    virtual ~stream_interface() = default;

    virtual void init() = 0;

    virtual void keysetup(const u8* key, u32 keysize, u32 ivsize) = 0;
    virtual void ivsetup(const u8* iv) = 0;

    virtual void encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) = 0;
    virtual void decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) = 0;

protected:
    const int _rounds;
};


struct estream_interface : stream_interface {
    estream_interface()
        : stream_interface() {}

    estream_interface(int rounds)
        : stream_interface(rounds) {}

    void init() override {
        ECRYPT_init();
    }

    void keysetup(const u8* key, u32 keysize, u32 ivsize) override {
        ECRYPT_keysetup(key, keysize, ivsize);
    }
    void ivsetup(const u8* iv) override {
        ECRYPT_ivsetup(iv);
    }

    void encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) override {
        ECRYPT_encrypt_bytes(plaintext, ciphertext, msglen);
    }
    void decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) override {
        ECRYPT_decrypt_bytes(ciphertext, plaintext, msglen);
    }

    virtual void ECRYPT_init() = 0;

    virtual void ECRYPT_keysetup(const u8* key, u32 keysize, u32 ivsize) = 0;
    virtual void ECRYPT_ivsetup(const u8* iv) = 0;

    virtual void ECRYPT_encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) = 0;
    virtual void ECRYPT_decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) = 0;
};
