#ifndef ICEPOLE256AV1_H
#define ICEPOLE256AV1_H

#include "../../../CaesarInterface.h"

class Icepole256av1 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Icepole256av1(int numRounds);
    ~Icepole256av1();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // ICEPOLE256AV1_H
