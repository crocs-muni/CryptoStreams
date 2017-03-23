#ifndef WHEESHTV1MR3FR3T256_H
#define WHEESHTV1MR3FR3T256_H

#include "../../../CaesarInterface.h"

class Wheeshtv1mr3fr3t256 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Wheeshtv1mr3fr3t256(int numRounds);
    ~Wheeshtv1mr3fr3t256();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // WHEESHTV1MR3FR3T256_H
