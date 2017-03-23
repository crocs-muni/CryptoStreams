#ifndef AEADAES128OCBTAGLEN64V1_H
#define AEADAES128OCBTAGLEN64V1_H

#include "../../../CaesarInterface.h"

class Aeadaes128ocbtaglen64v1 : public CaesarInterface {
    static const int maxNumRounds = -1;
public:
    Aeadaes128ocbtaglen64v1(int numRounds);
    ~Aeadaes128ocbtaglen64v1();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // AEADAES128OCBTAGLEN64V1_H
