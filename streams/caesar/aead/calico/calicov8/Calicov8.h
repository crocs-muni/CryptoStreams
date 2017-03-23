#ifndef CALICOV8_H
#define CALICOV8_H

#include "../../../CaesarInterface.h"

class Calicov8 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Calicov8(int numRounds);
    ~Calicov8();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // CALICOV8_H
