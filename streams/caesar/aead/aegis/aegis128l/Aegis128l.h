#ifndef AEGIS128L_H
#define AEGIS128L_H

#include "../../../CaesarInterface.h"

class Aegis128l : public CaesarInterface {
	static const int maxNumRounds = -1;
public:
    Aegis128l(int numRounds);
    ~Aegis128l();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // AEGIS128L_H
