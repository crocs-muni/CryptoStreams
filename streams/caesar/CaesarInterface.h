#ifndef CAESARINTERFACE_H
#define CAESARINTERFACE_H

#include <string>
#include "CaesarConstants.h"

class CaesarInterface {
protected:
    //! CAESAR algorithm used
    int m_algorithm;
    //! number of rounds
    int m_numRounds;

public:
    CaesarInterface(int a, int nr, int kl, int smnl, int pmnl, int co);
    virtual ~CaesarInterface();

    /** Encryption utility
     * It must be possible to recover the plaintext and the secret message number from
     * the ciphertext, associated data, public message number, and key.
     * @param c     ciphertext (out)
     * @param clen  ciphertextlength (out) (at most mlen + CRYPTO_ABYTES)
     * @param m     plaintext (in)
     * @param mlen  plaintextlength (in)
     * @param ad    associated data (in)
     * @param adlen associated data length (in)
     * @param nsec  secret message number (in, length specified by cipher)
     * @param npub  public message number (in, length specified by cipher)
     * @param k     key (in, length specified by cipher)
     * @return status
     */
    virtual int encrypt(bits_t *c, length_t *clen, const bits_t *m, length_t mlen,
                        const bits_t *ad, length_t adlen, const bits_t *nsec, const bits_t *npub,
                        const bits_t *k) = 0;

    /** Decryption utility
     * It must be possible to recover the plaintext and the secret message number from
     * the ciphertext, associated data, public message number, and key.
     * @param m             decrypted plaintext (out)
     * @param outputmlen    length of the decrypted plaintext (out)
     * @param nsec          secret message output (in, length specified by cipher)
     * @param c             ciphertext (in)
     * @param clen          ciphertext length (in)
     * @param ad            associated data (in)
     * @param adlen         associated data length (in)
     * @param npub          public message number (in, length specified by cipher)
     * @param k             key (in, length specified by cipher)
     * @return
     */
    virtual int decrypt(bits_t *m, length_t *outputmlen, bits_t *nsec,
                        const bits_t *c, length_t clen, const bits_t *ad, length_t adlen,
                        const bits_t *npub, const bits_t *k) = 0;

    /** human readable ag=lgorithm description */
    virtual std::string shortDescription() const = 0;

    /** allocate new caesar algorithm according to parameters
      * @param algorithm        CAESAR algorithm constant
      * @param numRounds        number of rounds used
      * @param mode             cipher mode (if needed)
      * @return allocated CAESAR algorithm obejct
      */
    static CaesarInterface* getCaesarFunction(int algorithm, int numRounds);
};

#endif // CAESARINTERFACE_H
