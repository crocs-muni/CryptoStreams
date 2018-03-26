#pragma once

/**
 * Sources:
 *
 * https://github.com/openssl/openssl/tree/master/crypto/idea
 * https://en.wikipedia.org/wiki/International_Data_Encryption_Algorithm
 */

#include <stdexcept>
#include <cstddef>
#include <cstdint>

#include "../../block_cipher.h"

namespace block {
namespace idea {

# ifdef  __cplusplus
extern "C" {
# endif

typedef unsigned int IDEA_INT;

# define IDEA_ENCRYPT    1
# define IDEA_DECRYPT    0

# define IDEA_BLOCK      8
# define IDEA_KEY_LENGTH 16

typedef struct idea_key_st {
    IDEA_INT data[9][6];
} IDEA_KEY_SCHEDULE;

const char *IDEA_options(void);
void IDEA_ecb_encrypt(const unsigned char *in, unsigned char *out,
                      IDEA_KEY_SCHEDULE *ks);
void IDEA_set_encrypt_key(const unsigned char *key, IDEA_KEY_SCHEDULE *ks);
void IDEA_set_decrypt_key(IDEA_KEY_SCHEDULE *ek, IDEA_KEY_SCHEDULE *dk);
void IDEA_encrypt(unsigned long *in, IDEA_KEY_SCHEDULE *ks);

# ifdef  __cplusplus
}
# endif

}
}

