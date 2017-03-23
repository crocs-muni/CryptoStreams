#include "Wheeshtv1mr3fr1t128.h"
#include "wheeshtv1mr3fr1t128_encrypt.h"
#include "../../common/api.h"
#include "EACglobals.h"

Wheeshtv1mr3fr1t128::Wheeshtv1mr3fr1t128(int numRounds)
    : CaesarInterface(CAESAR_WHEESHTV1MR3FR1T128, numRounds, CRYPTO_KEYBYTES, CRYPTO_NSECBYTES, CRYPTO_NPUBBYTES, CRYPTO_ABYTES) {
    if (numRounds < -1 || numRounds > maxNumRounds) {
        mainLogger.out(LOGGER_WARNING) << "Weird number of rouds (" << numRounds << ") for " << shortDescription() << endl;
    }
    if (numRounds == -1) {
        Wheeshtv1mr3fr1t128_raw::numRounds = maxNumRounds;
        CaesarCommon::numRounds = maxNumRounds;
    } else {
        Wheeshtv1mr3fr1t128_raw::numRounds = m_numRounds;
        CaesarCommon::numRounds = m_numRounds;
    }
}

Wheeshtv1mr3fr1t128::~Wheeshtv1mr3fr1t128() { }

int Wheeshtv1mr3fr1t128::encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                       const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                       const bits_t *k) {
    return Wheeshtv1mr3fr1t128_raw::crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
}

int Wheeshtv1mr3fr1t128::decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                       const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                       const bits_t *npub, const bits_t *k) {
    return Wheeshtv1mr3fr1t128_raw::crypto_aead_decrypt(m, outputmlen, nsec, c, clen, ad, adlen, npub, k);
}

std::string Wheeshtv1mr3fr1t128::shortDescription() const {
    return "Wheeshtv1mr3fr1t128";
}
