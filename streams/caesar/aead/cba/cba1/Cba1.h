#ifndef CBA1_H
#define CBA1_H

#include "../../../CaesarInterface.h"

class Cba1 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Cba1(int numRounds);
    ~Cba1();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // CBA1_H
