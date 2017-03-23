#ifndef PI64CIPHER256V1TWOROUNDS_H
#define PI64CIPHER256V1TWOROUNDS_H

#include "../../../CaesarInterface.h"

class Pi64cipher256v1tworounds : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Pi64cipher256v1tworounds(int numRounds);
    ~Pi64cipher256v1tworounds();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // PI64CIPHER256V1TWOROUNDS_H
