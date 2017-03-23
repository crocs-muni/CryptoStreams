#ifndef ACORN128_H
#define ACORN128_H

#include "../../../CaesarInterface.h"

class Acorn128 : public CaesarInterface {
    static const int maxNumRounds = -1;
public:
    Acorn128(int numRounds);
    ~Acorn128();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // ACORN128_H
