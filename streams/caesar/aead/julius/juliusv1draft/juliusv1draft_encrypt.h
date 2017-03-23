#ifndef JULIUSV1DRAFT_ENCRYPT_H
#define JULIUSV1DRAFT_ENCRYPT_H

#include "juliusv1draft_api.h"

namespace Juliusv1draft_raw {
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
} // namespace Juliusv1draft_raw

#endif // JULIUSV1DRAFT_ENCRYPT_H
