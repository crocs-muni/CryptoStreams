#pragma once

/**
 * Sources:
 * https://github.com/openssl/openssl/tree/master/crypto
 * https://github.com/openssl/openssl/blob/master/crypto/include/internal/aria.h
 * https://en.wikipedia.org/wiki/ARIA_(cipher)
 * http://210.104.33.10/ARIA/index-e.html
 * http://210.104.33.10/ARIA/doc/ARIA-testvector-e.pdf
 */

#include <stdexcept>
#include <cstddef>
#include <cstdint>

#include "../../block_cipher.h"

# define ARIA_ENCRYPT     1
# define ARIA_DECRYPT     0

# define ARIA_BLOCK_SIZE    16  /* Size of each encryption/decryption block */
# define ARIA_MAX_KEYS      17  /* Number of keys needed in the worst case  */


namespace block {
namespace aria {

# ifdef  __cplusplus
extern "C" {
# endif

        typedef union {
            unsigned char c[ARIA_BLOCK_SIZE];
            unsigned int u[ARIA_BLOCK_SIZE / sizeof(unsigned int)];
        } ARIA_u128;

        typedef unsigned char ARIA_c128[ARIA_BLOCK_SIZE];

        struct aria_key_st {
            ARIA_u128 rd_key[ARIA_MAX_KEYS];
            unsigned int rounds;
        };
        typedef struct aria_key_st ARIA_KEY;


        int aria_set_encrypt_key(const unsigned char *userKey, int bits,
                                 ARIA_KEY *key);
        int aria_set_decrypt_key(const unsigned char *userKey, int bits,
                                 ARIA_KEY *key);

        void aria_encrypt(const unsigned char *in, unsigned char *out,
                          const ARIA_KEY *key,
                          unsigned nr);

# ifdef  __cplusplus
}
# endif

}
}