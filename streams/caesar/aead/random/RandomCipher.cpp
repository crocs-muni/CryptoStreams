#include "EACglobals.h"
#include "generators/IRndGen.h"
#include "RandomCipher.h"

#define CRYPTO_ABYTES 16

RandomCipher::RandomCipher() : CaesarInterface(CAESAR_RANDOM, -1, 0, 0, 0, CRYPTO_ABYTES) { }

RandomCipher::~RandomCipher() { }

int RandomCipher::encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                       const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                       const bits_t *k) {
    // copy plaintext to ciphertext
    memcpy(c, m, mlen);
    // set ciphertext length as plaintext length + tag length
    *clen = mlen + CRYPTO_ABYTES;
    // get random data for tag
    for (int byte = 0; byte < CRYPTO_ABYTES; byte++) {
        rndGen->getRandomFromInterval(255, c + mlen + byte);
    }
    return STAT_OK;
}

int RandomCipher::decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                       const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                       const bits_t *npub, const bits_t *k) {
    // copy ciphertext to plaintext
    memcpy(m, c, clen - CRYPTO_ABYTES);
    // set messaage length as ciphertext length - tag length
    *outputmlen = clen - CRYPTO_ABYTES;
    return STAT_OK;
}

std::string RandomCipher::shortDescription() const {
    return "Random stream -- reference case";
}
