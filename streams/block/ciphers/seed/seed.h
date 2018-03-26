#pragma once

/**
 * Sources:
 *
 * https://github.com/openssl/openssl/blob/master/crypto/seed/seed.c
 * https://en.wikipedia.org/wiki/SEED
 * http://seed.kisa.or.kr/html/egovframework/iwt/ds/ko/ref/%5B2%5D_SEED+128_Specification_english_M.pdf
 * http://seed.kisa.or.kr/iwt/ko/sup/EgovSeedInfo.do
 * https://tools.ietf.org/html/rfc4269
 */

#include <stdexcept>
#include <cstddef>
#include <cstdint>

#include "../../block_cipher.h"

namespace block {
namespace seed {


#ifdef  __cplusplus
extern "C" {
#endif

# ifdef AES_LONG
#  ifndef SEED_LONG
#   define SEED_LONG 1
#  endif
# endif

//#include <sys/types.h>

# define SEED_BLOCK_SIZE 16
# define SEED_KEY_LENGTH 16

typedef struct seed_key_st {
# ifdef SEED_LONG
    unsigned long data[32];
# else
    unsigned int data[32];
# endif
} SEED_KEY_SCHEDULE;

void SEED_set_key(const unsigned char rawkey[SEED_KEY_LENGTH],
                  SEED_KEY_SCHEDULE *ks);

void SEED_encrypt(const unsigned char s[SEED_BLOCK_SIZE],
                  unsigned char d[SEED_BLOCK_SIZE],
                  const SEED_KEY_SCHEDULE *ks,
                  int nr);
void SEED_decrypt(const unsigned char s[SEED_BLOCK_SIZE],
                  unsigned char d[SEED_BLOCK_SIZE],
                  const SEED_KEY_SCHEDULE *ks,
                  int nr);

# ifdef  __cplusplus
}
# endif

}
}