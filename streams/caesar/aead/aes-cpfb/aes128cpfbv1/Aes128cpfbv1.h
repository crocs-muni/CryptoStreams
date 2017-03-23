#ifndef AES128CPFBV1_H
#define AES128CPFBV1_H

#include "../../../CaesarInterface.h"

class Aes128cpfbv1 : public CaesarInterface {
    const int maxNumRounds = -1;
public:
    Aes128cpfbv1(int numRounds);
    ~Aes128cpfbv1();
    int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k);
    int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k);
    std::string shortDescription() const;
};

#endif // AES128CPFBV1_H
