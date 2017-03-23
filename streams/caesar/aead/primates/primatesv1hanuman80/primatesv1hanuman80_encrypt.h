#ifndef PRIMATESV1HANUMAN80_ENCRYPT_H
#define PRIMATESV1HANUMAN80_ENCRYPT_H

#include "primatesv1hanuman80_api.h"

namespace Primatesv1hanuman80_raw {
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
} // namespace Primatesv1hanuman80_raw

#endif // PRIMATESV1HANUMAN80_ENCRYPT_H
