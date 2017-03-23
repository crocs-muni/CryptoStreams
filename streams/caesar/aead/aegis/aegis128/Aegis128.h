#ifndef AEGIS128_H
#define AEGIS128_H

#include "../../../CaesarInterface.h"

class Aegis128 : public CaesarInterface {
    static const int maxNumRounds = -1;
public:
    Aegis128(int numRounds);
    ~Aegis128();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // AEGIS128_H
