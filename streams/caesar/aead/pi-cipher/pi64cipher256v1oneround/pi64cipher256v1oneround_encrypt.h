#ifndef PI64CIPHER256V1ONEROUND_ENCRYPT_H
#define PI64CIPHER256V1ONEROUND_ENCRYPT_H

#include "pi64cipher256v1oneround_api.h"

namespace Pi64cipher256v1oneround_raw {
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
} // namespace Pi64cipher256v1oneround_raw

#endif // PI64CIPHER256V1ONEROUND_ENCRYPT_H
