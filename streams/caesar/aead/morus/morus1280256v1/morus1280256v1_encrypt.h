#ifndef MORUS1280256V1_ENCRYPT_H
#define MORUS1280256V1_ENCRYPT_H

#include "morus1280256v1_api.h"

namespace Morus1280256v1_raw {
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
} // namespace Morus1280256v1_raw

#endif // MORUS1280256V1_ENCRYPT_H
