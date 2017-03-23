/* ecrypt-sync.h */
/* ecrypt-sync.h   v1.0   April 2005
 * EDON80 C api code
 * intervention made by:  Danilo Gligoroski
 *
 * This code is placed as a reference code for EDON80
 * call for Stream Ciphers.
 */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef EDON80_SYNC
#define EDON80_SYNC

#include "../../estream_interface.h"
#include "../ecrypt-portable.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.
 */
#define EDON80_NAME "Edon80" /* [edit] */
#define EDON80_PROFILE "___H3"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; EDON80_KEYSIZE(i) <= EDON80_MAXKEYSIZE; ++i)
 *   {
 *     keysize = EDON80_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define EDON80_MAXKEYSIZE 80           /* [edit] */
#define EDON80_KEYSIZE(i) (80 + (i)*8) /* [edit] */
                                       /*
                                          The design of Edon80 in principle is not restricted by the key size.
                                          However, for EDON80 call for Stream Ciphers - PROFILE 2, we restrict
                                          Edon80 key size only on requested size of 80 bits.
                                       */

#define EDON80_MAXIVSIZE 64           /* [edit] */
#define EDON80_IVSIZE(i) (64 + (i)*8) /* [edit] */
                                      /*
                                         We repeat the same comment from above for IVSIZE. For EDON80 call for
                                         Stream Ciphers, we restrict Edon80 key size only on requested size of 64 bits.
                                      */

/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * EDON80_ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

typedef struct {
    /* Edon80 is a Stream Cipher based on Quasigropup String Transformations.  */
    /* For the definition of Edon80 we need quasigroups of order 4     */
    u8 Q[EDON80_MAXKEYSIZE][4][4];
    /* Counter is internal variable that has values in the range 0 to 3 */
    u8 Counter;
    /* The working size of the key (in pairs of bits). */
    u32 keysize;
    /* The values of the Initial Vector are kept in this array. */
    u8 key[EDON80_MAXKEYSIZE / 2];
    /* The working size of the Initial Vector (in pairs of bits). */
    u32 ivsize;
    /* The values of the Initial Vector are kept in this array. */
    u8 iv[EDON80_MAXKEYSIZE / 2];
    /* The actual number of internal states. */
    u32 NumberOfInternalStates;
    /* All internal states are kept in this array. */
    u8 InternalState[EDON80_MAXKEYSIZE];
    /*
     * [edit]
     *
     * Put here all state variable needed during the encryption process.
     */
} EDON80_ctx;

/* ------------------------------------------------------------------------- */
class ECRYPT_Edon80 : public estream_interface {
    EDON80_ctx _ctx;

public:
    /* Mandatory functions */

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
 * reset the EDON80_GENERATES_KEYSTREAM flag.
 */

#define EDON80_GENERATES_KEYSTREAM
#ifdef EDON80_GENERATES_KEYSTREAM

    void EDON80_keystream_bytes(EDON80_ctx* ctx,
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
 * undef the EDON80_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define EDON80_USES_DEFAULT_ALL_IN_ONE /* [edit] */

    void EDON80_encrypt_packet(
            EDON80_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void EDON80_decrypt_packet(
            EDON80_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * EDON80_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define EDON80_BLOCKLENGTH 4 /* [edit] */

#define EDON80_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef EDON80_USES_DEFAULT_BLOCK_MACROS

#define EDON80_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                  \
    ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*EDON80_BLOCKLENGTH)

#define EDON80_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                  \
    ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*EDON80_BLOCKLENGTH)

#ifdef EDON80_GENERATES_KEYSTREAM

#define EDON80_keystream_blocks(ctx, keystream, blocks)                                            \
    EDON80_keystream_bytes(ctx, keystream, (blocks)*EDON80_BLOCKLENGTH)

#endif

#else

    void EDON80_encrypt_blocks(EDON80_ctx* ctx,
                               const u8* plaintext,
                               u8* ciphertext,
                               u32 blocks); /* Message length in blocks. */

    void EDON80_decrypt_blocks(EDON80_ctx* ctx,
                               const u8* ciphertext,
                               u8* plaintext,
                               u32 blocks); /* Message length in blocks. */

#ifdef EDON80_GENERATES_KEYSTREAM

    void EDON80_keystream_blocks(EDON80_ctx* ctx,
                                 const u8* keystream,
                                 u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};

/*
 * If your cipher can be implemented in different ways, you can use
 * the EDON80_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DEDON80_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (EDON80_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same EDON80_BLOCKLENGTH, etc.).
 */
#define EDON80_MAXVARIANT 1 /* [edit] */

#ifndef EDON80_VARIANT
#define EDON80_VARIANT 1
#endif

#if (EDON80_VARIANT > EDON80_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
