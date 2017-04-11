/* ecrypt-sync.h */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef TEA_SYNC
#define TEA_SYNC

#include "../../estream_interface.h"
#include "../ecrypt-portable.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.
 */
#define TEA_NAME "TEA" /* [edit] */
#define TEA_PROFILE "_____"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; TEA_KEYSIZE(i) <= TEA_MAXKEYSIZE; ++i)
 *   {
 *     keysize = TEA_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define TEA_MAXKEYSIZE 128             /* [edit] */
#define TEA_KEYSIZE(i) (128 + (i)*128) /* [edit] */

#define TEA_MAXIVSIZE 0 /* [edit] */
#define TEA_IVSIZE(i) 0 /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * TEA_ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

typedef struct {
    u32 input[2]; // 2*32b inputs /* could be compressed */
    u32 key[4];
    /*
     * [edit]
     *
     * Put here all state variable needed during the encryption process.
     */
} TEA_ctx;

/* ------------------------------------------------------------------------- */
class ECRYPT_TEA : public estream_interface {
    TEA_ctx _ctx;

public:
    /* Mandatory functions */
    ECRYPT_TEA(int rounds)
        : estream_interface(rounds) {}
    /*
     * Key and message independent initialization. This function will be
     * called once when the program starts (e.g., to build expanded S-box
     * tables).
     */
    void ECRYPT_init() override;

    /*
     * Key setup. It is the user's responsibility to select the values of
     * keysize and ivsize from the set of supported values specified
     * above.
     */
    void ECRYPT_keysetup(const u8* key,
                         u32 keysize,          /* Key size in bits. */
                         u32 ivsize) override; /* IV size in bits. */

    /*
     * IV setup. After having called ECRYPT_keysetup(), the user is
     * allowed to call ECRYPT_ivsetup() different times in order to
     * encrypt/decrypt different messages with the same key but different
     * IV's.
     */
    void ECRYPT_ivsetup(const u8* iv) override;

    /*
     * Encryption/decryption of arbitrary length messages.
     *
     * For efficiency reasons, the API provides two types of
     * encrypt/decrypt functions. The ECRYPT_encrypt_bytes() function
     * (declared here) encrypts byte strings of arbitrary length, while
     * the ECRYPT_encrypt_blocks() function (defined later) only accepts iv
     * lengths which are multiples of ECRYPT_BLOCKLENGTH.
     *
     * The user is allowed to make multiple calls to
     * ECRYPT_encrypt_blocks() to incrementally encrypt a long message,
     * but he is NOT allowed to make additional encryption calls once he
     * has called ECRYPT_encrypt_bytes() (unless he starts a new message
     * of course). For example, this sequence of calls is acceptable:
     *
     * ECRYPT_keysetup();
     *
     * ECRYPT_ivsetup();
     * ECRYPT_encrypt_blocks();
     * ECRYPT_encrypt_blocks();
     * ECRYPT_encrypt_bytes();
     *
     * ECRYPT_ivsetup();
     * ECRYPT_encrypt_blocks();
     * ECRYPT_encrypt_blocks();
     *
     * ECRYPT_ivsetup();
     * ECRYPT_encrypt_bytes();
     *
     * The following sequence is not:
     *
     * ECRYPT_keysetup();
     * ECRYPT_ivsetup();
     * ECRYPT_encrypt_blocks();
     * ECRYPT_encrypt_bytes();
     * ECRYPT_encrypt_blocks();
     */

    void ECRYPT_encrypt_bytes(const u8* plaintext,
                              u8* ciphertext,
                              u32 msglen) override; /* Message length in bytes. */

    void ECRYPT_decrypt_bytes(const u8* ciphertext,
                              u8* plaintext,
                              u32 msglen) override; /* Message length in bytes. */

/* ------------------------------------------------------------------------- */

/* Optional features */

/*
 * For testing purposes it can sometimes be useful to have a function
 * which immediately generates keystream without having to provide it
 * with a zero plaintext. If your cipher cannot provide this function
 * (e.g., because it is not strictly a synchronous cipher), please
 * reset the TEA_GENERATES_KEYSTREAM flag.
 */

#define TEA_GENERATES_KEYSTREAM
#ifdef TEA_GENERATES_KEYSTREAM

    void
    TEA_keystream_bytes(void* ctx, u8* keystream, u32 length); /* Length of keystream in bytes. */

#endif

/* ------------------------------------------------------------------------- */

/* Optional optimizations */

/*
 * By default, the functions in this section are implemented using
 * calls to functions declared above. However, you might want to
 * implement them differently for performance reasons.
 */

/*
 * All-in-one encryption/decryption of (short) packets.
 *
 * The default definitions of these functions can be found in
 * "ecrypt-sync.c". If you want to implement them differently, please
 * undef the TEA_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define TEA_USES_DEFAULT_ALL_IN_ONE /* [edit] */

    void
    TEA_encrypt_packet(TEA_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void
    TEA_decrypt_packet(TEA_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * TEA_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define TEA_BLOCKLENGTH 64 /* [edit] */

#define TEA_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef TEA_USES_DEFAULT_BLOCK_MACROS

#define TEA_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                     \
    ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*TEA_BLOCKLENGTH)

#define TEA_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                     \
    ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*TEA_BLOCKLENGTH)

#ifdef TEA_GENERATES_KEYSTREAM

#define TEA_keystream_blocks(ctx, keystream, blocks)                                               \
    TEA_keystream_bytes(ctx, keystream, (blocks)*TEA_BLOCKLENGTH)

#endif

#else

    void TEA_encrypt_blocks(TEA_ctx* ctx,
                            const u8* plaintext,
                            u8* ciphertext,
                            u32 blocks); /* Message length in blocks. */

    void TEA_decrypt_blocks(TEA_ctx* ctx,
                            const u8* ciphertext,
                            u8* plaintext,
                            u32 blocks); /* Message length in blocks. */

#ifdef TEA_GENERATES_KEYSTREAM

    void TEA_keystream_blocks(TEA_ctx* ctx,
                              const u8* keystream,
                              u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};
/*
 * If your cipher can be implemented in different ways, you can use
 * the TEA_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DTEA_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (TEA_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same TEA_BLOCKLENGTH, etc.).
 */
#define TEA_MAXVARIANT 1 /* [edit] */

#ifndef TEA_VARIANT
#define TEA_VARIANT 1
#endif

#if (TEA_VARIANT > TEA_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
