#ifndef JULIUSV1DRAFT_H
#define JULIUSV1DRAFT_H

#include "../../../CaesarInterface.h"

class Juliusv1draft : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Juliusv1draft(int numRounds);
    ~Juliusv1draft();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // JULIUSV1DRAFT_H
