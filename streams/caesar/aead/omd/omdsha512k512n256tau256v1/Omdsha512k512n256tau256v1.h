#ifndef OMDSHA512K512N256TAU256V1_H
#define OMDSHA512K512N256TAU256V1_H

#include "../../../CaesarInterface.h"

class Omdsha512k512n256tau256v1 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Omdsha512k512n256tau256v1(int numRounds);
    ~Omdsha512k512n256tau256v1();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // OMDSHA512K512N256TAU256V1_H
