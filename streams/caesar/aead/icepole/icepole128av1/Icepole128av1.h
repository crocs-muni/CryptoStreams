#ifndef ICEPOLE128AV1_H
#define ICEPOLE128AV1_H

#include "../../../CaesarInterface.h"

class Icepole128av1 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Icepole128av1(int numRounds);
    ~Icepole128av1();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // ICEPOLE128AV1_H
