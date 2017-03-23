#include "Primatesv1gibbon80.h"
#include "primatesv1gibbon80_encrypt.h"
#include "../../common/api.h"
#include "EACglobals.h"

Primatesv1gibbon80::Primatesv1gibbon80(int numRounds)
    : CaesarInterface(CAESAR_PRIMATESV1GIBBON80, numRounds, CRYPTO_KEYBYTES, CRYPTO_NSECBYTES, CRYPTO_NPUBBYTES, CRYPTO_ABYTES) {
    if (numRounds < -1 || numRounds > maxNumRounds) {
        mainLogger.out(LOGGER_WARNING) << "Weird number of rouds (" << numRounds << ") for " << shortDescription() << endl;
    }
    if (numRounds == -1) {
        Primatesv1gibbon80_raw::numRounds = maxNumRounds;
        CaesarCommon::numRounds = maxNumRounds;
    } else {
        Primatesv1gibbon80_raw::numRounds = m_numRounds;
        CaesarCommon::numRounds = m_numRounds;
    }
}

Primatesv1gibbon80::~Primatesv1gibbon80() { }

int Primatesv1gibbon80::encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                       const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                       const bits_t *k) {
    return Primatesv1gibbon80_raw::crypto_aead_encrypt(c, clen, m, mlen, ad, adlen, nsec, npub, k);
}

int Primatesv1gibbon80::decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                       const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                       const bits_t *npub, const bits_t *k) {
    return Primatesv1gibbon80_raw::crypto_aead_decrypt(m, outputmlen, nsec, c, clen, ad, adlen, npub, k);
}

std::string Primatesv1gibbon80::shortDescription() const {
    return "Primatesv1gibbon80";
}
