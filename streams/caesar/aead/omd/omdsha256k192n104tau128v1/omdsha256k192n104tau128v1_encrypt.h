#ifndef OMDSHA256K192N104TAU128V1_ENCRYPT_H
#define OMDSHA256K192N104TAU128V1_ENCRYPT_H

#include "omdsha256k192n104tau128v1_api.h"

namespace Omdsha256k192n104tau128v1_raw {
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
} // namespace Omdsha256k192n104tau128v1_raw

#endif // OMDSHA256K192N104TAU128V1_ENCRYPT_H
