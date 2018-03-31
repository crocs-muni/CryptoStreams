#pragma once

/**
 * Source: https://github.com/inmcm/Simon_Speck_Ciphers/
 */

#include "../../block_cipher.h"
#include <memory>

namespace block {

// enum speck_mode_t { ECB, CTR, CBC, CFB, OFB };
// const uint8_t speck_block_sizes[] = {32, 48, 48, 64, 64, 96, 96, 128, 128, 128};
// const uint16_t speck_key_sizes[] = {64, 72, 96, 96, 128, 96, 144, 128, 192, 256};

#ifndef CIPHER_CONSTANTS
#define CIPHER_CONSTANTS
enum mode_t { ECB, CTR, CBC, CFB, OFB };
#endif

enum speck_cipher_config_t {
    Speck_64_32,
    Speck_72_48,
    Speck_96_48,
    Speck_96_64,
    Speck_128_64,
    Speck_96_96,
    Speck_144_96,
    Speck_128_128,
    Speck_192_128,
    Speck_256_128
} ;

typedef struct {
    enum speck_cipher_config_t cipher_cfg;
    void (*encryptPtr)(const uint8_t, const uint8_t *, const uint8_t *, uint8_t *);
    void (*decryptPtr)(const uint8_t, const uint8_t *, const uint8_t *, uint8_t *);
    uint16_t key_size;
    uint8_t block_size;
    uint8_t round_limit;
    uint8_t init_vector[16];
    uint8_t counter[16];
    uint8_t key_schedule[576];
    uint8_t alpha;
    uint8_t beta;
} Speck_Cipher;

typedef struct _bitword24_t{
  uint32_t data: 24;
} bitword24_t;

typedef struct _bytes3_t{
        uint8_t data[3];
} bytes3_t;

typedef struct _bitword48_t{
  uint64_t data: 48;
} bitword48_t;

typedef struct _bytes6_t{
        uint8_t data[6];
} bytes6_t;


uint8_t Speck_Init(Speck_Cipher *cipher_object, enum speck_cipher_config_t cipher_cfg, enum mode_t c_mode, const uint8_t *key, uint8_t *iv, uint8_t *counter);

uint8_t Speck_Encrypt(Speck_Cipher cipher_object, const uint8_t *plaintext, uint8_t *ciphertext);

uint8_t Speck_Decrypt(Speck_Cipher cipher_object, const uint8_t *ciphertext, uint8_t *plaintext);

void Speck_Encrypt_32(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Speck_Encrypt_48(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Speck_Encrypt_64(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Speck_Encrypt_96(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                      uint8_t *ciphertext);
void Speck_Encrypt_128(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *plaintext,
                       uint8_t *ciphertext);

void Speck_Decrypt_32(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Speck_Decrypt_48(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Speck_Decrypt_64(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Speck_Decrypt_96(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                      uint8_t *plaintext);
void Speck_Decrypt_128(const uint8_t round_limit, const uint8_t *key_schedule, const uint8_t *ciphertext,
                       uint8_t *plaintext);

class speck : public block_cipher {

    /* Data structures */

    struct speck_ctx {
        speck_ctx(std::size_t block_size, std::size_t key_size)
            : cipher_object(std::make_unique<Speck_Cipher>()) {
            std::size_t key_bit_size = key_size * 8;
            std::size_t block_bit_size = block_size * 8;
            if (key_bit_size == 64 && block_bit_size == 32)
                cipher_config = Speck_64_32;
            else if (key_bit_size == 72 && block_bit_size == 48)
                cipher_config = Speck_72_48;
            else if (key_bit_size == 96 && block_bit_size == 48)
                cipher_config = Speck_96_48;
            else if (key_bit_size == 96 && block_bit_size == 64)
                cipher_config = Speck_96_64;
            else if (key_bit_size == 128 && block_bit_size == 64)
                cipher_config = Speck_128_64;
            else if (key_bit_size == 96 && block_bit_size == 96)
                cipher_config = Speck_96_96;
            else if (key_bit_size == 144 && block_bit_size == 96)
                cipher_config = Speck_144_96;
            else if (key_bit_size == 128 && block_bit_size == 128)
                cipher_config = Speck_128_128;
            else if (key_bit_size == 192 && block_bit_size == 128)
                cipher_config = Speck_192_128;
            else if (key_bit_size == 256 && block_bit_size == 128)
                cipher_config = Speck_256_128;
            else throw std::runtime_error("Unsupported Speck block or key size.");
        }

         std::unique_ptr<Speck_Cipher> cipher_object;
         speck_cipher_config_t cipher_config;
    } _ctx;

public:
    speck(std::size_t rounds, std::size_t block_size, std::size_t key_size)
        : block_cipher(rounds)
        , _ctx(block_size, key_size) {}

    void keysetup(const std::uint8_t* key, const std::uint64_t keysize) override;

    void ivsetup(const std::uint8_t* iv, const std::uint64_t ivsize) override;

    void encrypt(const std::uint8_t* plaintext,
                 std::uint8_t* ciphertext) override;

    void decrypt(const std::uint8_t* ciphertext,
                 std::uint8_t* plaintext) override;

private:
    void endianity_flip(const std::uint8_t* source, std::uint8_t* destination, const size_t length);
};
}
