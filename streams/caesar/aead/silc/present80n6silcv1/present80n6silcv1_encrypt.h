#ifndef PRESENT80N6SILCV1_ENCRYPT_H
#define PRESENT80N6SILCV1_ENCRYPT_H

#include "present80n6silcv1_api.h"

namespace Present80n6silcv1_raw {
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
} // namespace Present80n6silcv1_raw

#endif // PRESENT80N6SILCV1_ENCRYPT_H
