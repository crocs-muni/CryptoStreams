#ifndef AES256AVALANCHEV1_ENCRYPT_H
#define AES256AVALANCHEV1_ENCRYPT_H

#include "aes256avalanchev1_api.h"

namespace Aes256avalanchev1_raw {
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
} // namespace Aes256avalanchev1_raw

#endif // AES256AVALANCHEV1_ENCRYPT_H
