#ifndef PRIMATESV1GIBBON120_H
#define PRIMATESV1GIBBON120_H

#include "../../../CaesarInterface.h"

class Primatesv1gibbon120 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Primatesv1gibbon120(int numRounds);
    ~Primatesv1gibbon120();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // PRIMATESV1GIBBON120_H
