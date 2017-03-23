#ifndef OCEANKEYAKV1_H
#define OCEANKEYAKV1_H

#include "../../../CaesarInterface.h"

class Oceankeyakv1 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Oceankeyakv1(int numRounds);
    ~Oceankeyakv1();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // OCEANKEYAKV1_H
