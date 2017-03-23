#ifndef WHEESHTV1MR3FR1T256_ENCRYPT_H
#define WHEESHTV1MR3FR1T256_ENCRYPT_H

#include "wheeshtv1mr3fr1t256_api.h"

namespace Wheeshtv1mr3fr1t256_raw {
extern int numRounds;

int crypto_aead_encrypt(unsigned char *c, unsigned long long *clen,
                        const unsigned char *m, unsigned long long mlen,
                        const unsigned char *ad, unsigned long long adlen,
                        const unsigned char *nsec, const unsigned char *npub,
                        const unsigned char *k);

int crypto_aead_decrypt(unsigned char *m, unsigned long long *outputmlen,
                        unsigned char *nsec,
                        const unsigned char *c, unsigned long long clen,
                        const unsigned char *ad, unsigned long long adlen,
                        const unsigned char *npub, const unsigned char *k);
} // namespace Wheeshtv1mr3fr1t256_raw

#endif // WHEESHTV1MR3FR1T256_ENCRYPT_H
