#ifndef ENCRYPTOR_H
#define ENCRYPTOR_H

#include "CaesarConstants.h"
#include "CaesarInterface.h"
#include "XMLProcessor.h"

class Encryptor {
private:
    //! cipher to use
    CaesarInterface* m_cipher;
    //! key buffer
    bits_t* m_key;
    //! associated data
    bits_t* m_ad;
    //! secret message number
    bits_t* m_smn;
    //! public message number
    bits_t* m_pmn;
    //! buffer for plaintext
    bits_t* m_plaintext;
    //! buffer for decrypted message for verification
    bits_t* m_decryptedPlaintext;
    //! buffer for decrypted secret message number
    bits_t* m_decryptedSmn;
    //! decrypted plaintext message length
    length_t m_decryptedPlaintextLength;
    //! setup already performed?
    bool m_setup;

    /** initialize given array
     * @param data          array to fill (already allocated)
     * @param dataLength    length of data
     * @param dataType      constant describing the desired data type
     * @return              status
     */
    static int initArray(bits_t* data, length_t dataLength, int dataType);

    /** increase counter in multibyte array
     * @param data          counter array
     * @param dataLength    data length
     */
    static void increaseArray(bits_t* data, length_t dataLength);

public:
    /** constructor
     * - allocate cipher
     * - allocate buffers for key, secret message number
     *   public message number, plaintext, ciphertext
     */
    Encryptor();

    /** destructor, free buffers */
    ~Encryptor();

    /** setup internal buffers (plaintext, key, ad, smn, pmn)
     * @return status
     */
    int setup();

    /** update internal buffers (plaintext, key, ad, smn, pmn)
     * @return
     */
    int update();

    /** encrypt set buffers and verify ciphertext
     * @param c         ciphertext
     * @param clen      length of the ciphertext
     * @return          verification status
     */
    int encrypt(bits_t *c, length_t *clen);

    /**
     * @return shortDescription of the cipher
     */
    std::string shortDescription();

    /** save state of encryptor
    * - current cipher keys and counter states
    * @param pRoot    allocated XML element 'encryptor' to be filled with state
    * @return status
    */
    int saveState(TiXmlNode* pRoot) const;

    /** load state of encryptor
    * - current cipher keys and counter states
    * @param pRoot    XML element 'encryptor'
    * @return status
    */
    int loadState(TiXmlNode* pRoot);
};

#endif // ENCRYPTOR_H
