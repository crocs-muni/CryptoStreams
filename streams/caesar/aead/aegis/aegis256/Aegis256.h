#ifndef AEGIS256_H
#define AEGIS256_H

#include "../../../CaesarInterface.h"

class Aegis256 : public CaesarInterface {
    static const int maxNumRounds = -1;
public:
    Aegis256(int numRounds);
    ~Aegis256();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // AEGIS256_H
