#ifndef CBA3_H
#define CBA3_H

#include "../../../CaesarInterface.h"

class Cba3 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Cba3(int numRounds);
    ~Cba3();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // CBA3_H
