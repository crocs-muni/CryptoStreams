#pragma once

/**
 * Sources:
 * https://github.com/openssl/openssl/blob/master/crypto/cast/
 * https://en.wikipedia.org/wiki/CAST-128
 * https://tools.ietf.org/html/rfc2144
 */

#include <stdexcept>
#include <cstddef>
#include <cstdint>

#include "../../block_cipher.h"

namespace block {
namespace cast {

# ifdef  __cplusplus
extern "C" {
# endif

# define CAST_ENCRYPT    1
# define CAST_DECRYPT    0

# define CAST_LONG unsigned int

# define CAST_BLOCK      8
# define CAST_KEY_LENGTH 16

typedef struct cast_key_st {
    CAST_LONG data[32];
    int short_key;              /* Use reduced rounds for short key */
} CAST_KEY;

void CAST_set_key(CAST_KEY *key, int len, const unsigned char *data);
void CAST_ecb_encrypt(const unsigned char *in, unsigned char *out,
                      const CAST_KEY *key, int enc, int nr);
void CAST_encrypt(CAST_LONG *data, const CAST_KEY *key, int nr);
void CAST_decrypt(CAST_LONG *data, const CAST_KEY *key, int nr);

# ifdef  __cplusplus
}
# endif

}}

