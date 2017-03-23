#ifndef TWINE80N6CLOCV1_H
#define TWINE80N6CLOCV1_H

#include "../../../CaesarInterface.h"

class Twine80n6clocv1 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Twine80n6clocv1(int numRounds);
    ~Twine80n6clocv1();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // TWINE80N6CLOCV1_H
