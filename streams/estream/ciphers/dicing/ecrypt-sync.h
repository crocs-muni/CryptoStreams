/* ecrypt-sync.h */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef DICING_SYNC
#define DICING_SYNC

#include "../../estream_interface.h"
#include "../ecrypt-portable.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.
 */
#define DICING_NAME "DICING-v2" /* [edit] */
#define DICING_PROFILE "_____"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; DICING_KEYSIZE(i) <= DICING_MAXKEYSIZE; ++i)
 *   {
 *     keysize = DICING_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define DICING_MAXKEYSIZE 256             /* [edit] */
#define DICING_KEYSIZE(i) (128 + (i)*128) /* [edit] */

#define DICING_MAXIVSIZE 256             /* [edit] */
#define DICING_IVSIZE(i) (128 + (i)*128) /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * DICING_ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

typedef struct {
    /*
     * [edit]
     *
     * Put here all state variable needed during the encryption process.
     */

    u8 skey1[80];
    u8 skey2[80];
    u32 ckey1[4];
    u32 ckey2[4];
    u32 key[8];
    u8 mkey[16];

    u32 ch[4];
    u32 var1[4];
    u32 var2[4];
    u32 sbox0[256];
    u32 sbox1[256];
    u32 sbox2[256];
    u32 sbox3[256];

    u32 key_size;
    u32 iv_size;
    int cyl;

} DICING_ctx;

/* ------------------------------------------------------------------------- */
class ECRYPT_Dicing : public estream_interface {
    DICING_ctx _ctx;

public:
    /* Mandatory functions */

    /*
     * Key and message independent initialization. This function will be
     * called once when the program starts (e.g., to build expanded S-box
     * tables).
     */
    void ECRYPT_init(void) override;

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
     * the ECRYPT_encrypt_blocks() function (defined later) only accepts
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
 * reset the DICING_GENERATES_KEYSTREAM flag.
 */

#define DICING_GENERATES_KEYSTREAM
#ifdef DICING_GENERATES_KEYSTREAM

    void DICING_keystream_bytes(DICING_ctx* ctx,
                                u8* keystream,
                                u32 length); /* Length of keystream in bytes. */

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
 * undef the DICING_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define DICING_USES_DEFAULT_ALL_IN_ONE /* [edit] */

    void DICING_encrypt_packet(
            DICING_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void DICING_decrypt_packet(
            DICING_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * DICING_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define DICING_BLOCKLENGTH 16 /* [edit] */

#define DICING_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef DICING_USES_DEFAULT_BLOCK_MACROS

#define DICING_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                  \
    ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*DICING_BLOCKLENGTH)

#define DICING_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                  \
    ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*DICING_BLOCKLENGTH)

#ifdef DICING_GENERATES_KEYSTREAM

#define DICING_keystream_blocks(ctx, keystream, blocks)                                            \
    DICING_keystream_bytes(ctx, keystream, (blocks)*DICING_BLOCKLENGTH)

#endif

#else

    void DICING_encrypt_blocks(DICING_ctx* ctx,
                               const u8* plaintext,
                               u8* ciphertext,
                               u32 blocks); /* Message length in blocks. */

    void DICING_decrypt_blocks(DICING_ctx* ctx,
                               const u8* ciphertext,
                               u8* plaintext,
                               u32 blocks); /* Message length in blocks. */

#ifdef DICING_GENERATES_KEYSTREAM

    void DICING_keystream_blocks(DICING_ctx* ctx,
                                 u8* keystream,
                                 u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};
/*
 * If your cipher can be implemented in different ways, you can use
 * the DICING_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DDICING_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (DICING_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same DICING_BLOCKLENGTH, etc.).
 */
#define DICING_MAXVARIANT 1 /* [edit] */

#ifndef DICING_VARIANT
#define DICING_VARIANT 1
#endif

#if (DICING_VARIANT > DICING_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
