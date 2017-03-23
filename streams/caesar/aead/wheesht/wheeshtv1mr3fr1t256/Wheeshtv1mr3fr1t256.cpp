#include "Wheeshtv1mr3fr1t256.h"
#include "wheeshtv1mr3fr1t256_encrypt.h"
#include "../../common/api.h"
#include "EACglobals.h"

Wheeshtv1mr3fr1t256::Wheeshtv1mr3fr1t256(int numRounds)
    : CaesarInterface(CAESAR_WHEESHTV1MR3FR1T256, numRounds, CRYPTO_KEYBYTES, CRYPTO_NSECBYTES, CRYPTO_NPUBBYTES, CRYPTO_ABYTES) {
    if (numRounds < -1 || numRounds > maxNumRounds) {
        mainLogger.out(LOGGER_WARNING) << "Weird number of rouds (" << numRounds << ") for " << shortDescription() << endl;
    }
    if (numRounds == -1) {
        Wheeshtv1mr3fr1t256_raw::numRounds = maxNumRounds;
        CaesarCommon::numRounds = maxNumRounds;
    } else {
        Wheeshtv1mr3fr1t256_raw::numRounds = m_numRounds;
        CaesarCommon::numRounds = m_numRounds;
    }
}

Wheeshtv1mr3fr1t256::~Wheeshtv1mr3fr1t256() { }

int Wheeshtv1mr3fr1t256::encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                       const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                       const bits_t *k) {
    return Wheeshtv1mr3fr1t256_raw::crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
}

int Wheeshtv1mr3fr1t256::decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                       const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                       const bits_t *npub, const bits_t *k) {
    return Wheeshtv1mr3fr1t256_raw::crypto_aead_decrypt(m, outputmlen, nsec, c, clen, ad, adlen, npub, k);
}

std::string Wheeshtv1mr3fr1t256::shortDescription() const {
    return "Wheeshtv1mr3fr1t256";
}
