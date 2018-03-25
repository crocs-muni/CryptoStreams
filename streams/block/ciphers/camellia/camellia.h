#pragma once

/**
 * Sources:
 *
 * https://info.isl.ntt.co.jp/crypt/eng/camellia/source.html
 * https://tls.mbed.org/camellia-source-code
 * https://en.wikipedia.org/wiki/Camellia_(cipher)
 */

#include <stdexcept>
#include <cstddef>
#include <cstdint>

#include "../../block_cipher.h"

namespace block {
namespace camellia {

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \brief          CAMELLIA context structure
 */
typedef struct {
    int nr;                     /*!<  number of rounds  */
    uint32_t rk[68];            /*!<  CAMELLIA round keys    */
}
mbedtls_camellia_context;

/**
 * \brief          Initialize CAMELLIA context
 *
 * \param ctx      CAMELLIA context to be initialized
 */
void mbedtls_camellia_init(mbedtls_camellia_context *ctx);

/**
* \brief          Clear CAMELLIA context
*
* \param ctx      CAMELLIA context to be cleared
*/
void mbedtls_camellia_free(mbedtls_camellia_context *ctx);

/**
* \brief          CAMELLIA key schedule (encryption)
*
* \param ctx      CAMELLIA context to be initialized
* \param key      encryption key
* \param keybits  must be 128, 192 or 256
*
* \return         0 if successful, or MBEDTLS_ERR_CAMELLIA_INVALID_KEY_LENGTH
*/
int mbedtls_camellia_setkey_enc(mbedtls_camellia_context *ctx, const unsigned char *key,
                                unsigned int keybits);

/**
* \brief          CAMELLIA key schedule (decryption)
*
* \param ctx      CAMELLIA context to be initialized
* \param key      decryption key
* \param keybits  must be 128, 192 or 256
*
* \return         0 if successful, or MBEDTLS_ERR_CAMELLIA_INVALID_KEY_LENGTH
*/
int mbedtls_camellia_setkey_dec(mbedtls_camellia_context *ctx, const unsigned char *key,
                                unsigned int keybits);

/**
* \brief          CAMELLIA-ECB block encryption/decryption
*
* \param ctx      CAMELLIA context
* \param mode     MBEDTLS_CAMELLIA_ENCRYPT or MBEDTLS_CAMELLIA_DECRYPT
* \param input    16-byte input block
* \param output   16-byte output block
*
* \return         0 if successful
*/
int mbedtls_camellia_crypt_ecb(mbedtls_camellia_context *ctx,
                               int mode,
                               const unsigned char input[16],
                               unsigned char output[16],
                               unsigned rounds);

#ifdef __cplusplus
}
#endif

}
}
