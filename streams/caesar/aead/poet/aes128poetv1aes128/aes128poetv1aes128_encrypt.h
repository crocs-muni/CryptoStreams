#ifndef AES128POETV1AES128_ENCRYPT_H
#define AES128POETV1AES128_ENCRYPT_H

#include "aes128poetv1aes128_api.h"

namespace Aes128poetv1aes128_raw {
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
} // namespace Aes128poetv1aes128_raw

#endif // AES128POETV1AES128_ENCRYPT_H
