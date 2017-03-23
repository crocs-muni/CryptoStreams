/* ecrypt-sync.h */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef HERMES_SYNC
#define HERMES_SYNC

#include "../../estream_interface.h"
#include "../ecrypt-portable.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.   UKA 20060507 ******************
 */
#define HERMES_NAME "Hermes8F-128" /* [edit] */
#define HERMES_PROFILE "_____"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; HERMES_KEYSIZE(i) <= HERMES_MAXKEYSIZE; ++i)
 *   {
 *     keysize = HERMES_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define HERMES_MAXKEYSIZE 128           /* [edit] */
#define HERMES_KEYSIZE(i) (128 + (i)*8) /* [edit] */

#define HERMES_MAXIVSIZE 136          /* [edit]   37*8 -> 17*8 */
#define HERMES_IVSIZE(i) (64 + (i)*8) /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * HERMES_ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

typedef struct {
    u8 key[16]; /* key          */

    u8 k[16];             /* key space    */
    u8 x[17];             /* cipher state */
    u8 ni, accu, p2, src; /* parameters   */
    u32 counter;          /* round counter */

} HERMES_ctx;

/* ------------------------------------------------------------------------- */
class ECRYPT_Hermes : public estream_interface {
    HERMES_ctx _ctx;

public:
    ECRYPT_Hermes(int rounds)
        : estream_interface(rounds) {}
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

/*
 * By default ECRYPT_encrypt_bytes() and ECRYPT_decrypt_bytes() are
 * defined as macros which redirect the call to a single function
 * ECRYPT_process_bytes(). If you want to provide separate encryption
 * and decryption functions, please undef
 * HERMES_HAS_SINGLE_BYTE_FUNCTION.
 */
#define HERMES_HAS_SINGLE_BYTE_FUNCTION /* [edit] */
#ifdef HERMES_HAS_SINGLE_BYTE_FUNCTION

    void ECRYPT_encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) override;

    void ECRYPT_decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) override;

    void HERMES_process_bytes(int action, /* 0 = encrypt; 1 = decrypt; */
                              void* ctx,
                              const u8* input,
                              u8* output,
                              u32 msglen); /* Message length in bytes. */

#else

    void ECRYPT_encrypt_bytes(void* ctx,
                              const u8* plaintext,
                              u8* ciphertext,
                              u32 msglen); /* Message length in bytes. */

    void ECRYPT_decrypt_bytes(void* ctx,
                              const u8* ciphertext,
                              u8* plaintext,
                              u32 msglen); /* Message length in bytes. */

#endif

/* ------------------------------------------------------------------------- */

/* Optional features */

/*
 * For testing purposes it can sometimes be useful to have a function
 * which immediately generates keystream without having to provide it
 * with a zero plaintext. If your cipher cannot provide this function
 * (e.g., because it is not strictly a synchronous cipher), please
 * reset the HERMES_GENERATES_KEYSTREAM flag.
 */

#undef HERMES_GENERATES_KEYSTREAM
#ifdef HERMES_GENERATES_KEYSTREAM

    void HERMES_keystream_bytes(HERMES_ctx* ctx,
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
 * undef the HERMES_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define HERMES_USES_DEFAULT_ALL_IN_ONE /* [edit] */

/*
 * Undef HERMES_HAS_SINGLE_PACKET_FUNCTION if you want to provide
 * separate packet encryption and decryption functions.
 */
#define HERMES_HAS_SINGLE_PACKET_FUNCTION /* [edit] */
#ifdef HERMES_HAS_SINGLE_PACKET_FUNCTION

#define HERMES_encrypt_packet(ctx, iv, plaintext, ciphertext, mglen)                               \
    HERMES_process_packet(0, ctx, iv, plaintext, ciphertext, mglen)

#define HERMES_decrypt_packet(ctx, iv, ciphertext, plaintext, mglen)                               \
    HERMES_process_packet(1, ctx, iv, ciphertext, plaintext, mglen)

    void HERMES_process_packet(int action, /* 0 = encrypt; 1 = decrypt; */
                               HERMES_ctx* ctx,
                               const u8* iv,
                               const u8* input,
                               u8* output,
                               u32 msglen);

#else

    void HERMES_encrypt_packet(
            HERMES_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void HERMES_decrypt_packet(
            HERMES_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

#endif

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * HERMES_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define HERMES_BLOCKLENGTH 8 /* [edit] */

#define HERMES_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef HERMES_USES_DEFAULT_BLOCK_MACROS

#define HERMES_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                  \
    ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*HERMES_BLOCKLENGTH)

#define HERMES_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                  \
    ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*HERMES_BLOCKLENGTH)

#ifdef HERMES_GENERATES_KEYSTREAM

#define HERMES_keystream_blocks(ctx, keystream, blocks)                                            \
    HERMES_keystream_bytes(ctx, keystream, (blocks)*HERMES_BLOCKLENGTH)

#endif

#else

/*
 * Undef HERMES_HAS_SINGLE_BLOCK_FUNCTION if you want to provide
 * separate block encryption and decryption functions.
 */
#define HERMES_HAS_SINGLE_BLOCK_FUNCTION /* [edit] */
#ifdef HERMES_HAS_SINGLE_BLOCK_FUNCTION

#define HERMES_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                  \
    HERMES_process_blocks(0, ctx, plaintext, ciphertext, blocks)

#define HERMES_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                  \
    HERMES_process_blocks(1, ctx, ciphertext, plaintext, blocks)

    void HERMES_process_blocks(int action, /* 0 = encrypt; 1 = decrypt; */
                               HERMES_ctx* ctx,
                               const u8* input,
                               u8* output,
                               u32 blocks); /* Message length in blocks. */

#else

    void HERMES_encrypt_blocks(HERMES_ctx* ctx,
                               const u8* plaintext,
                               u8* ciphertext,
                               u32 blocks); /* Message length in blocks. */

    void HERMES_decrypt_blocks(HERMES_ctx* ctx,
                               const u8* ciphertext,
                               u8* plaintext,
                               u32 blocks); /* Message length in blocks. */

#endif

#ifdef HERMES_GENERATES_KEYSTREAM

    void HERMES_keystream_blocks(HERMES_ctx* ctx,
                                 u8* keystream,
                                 u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};
/*
 * If your cipher can be implemented in different ways, you can use
 * the HERMES_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DHERMES_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (HERMES_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same HERMES_BLOCKLENGTH, etc.).
 */
#define HERMES_MAXVARIANT 1 /* [edit] */

#ifndef HERMES_VARIANT
#define HERMES_VARIANT 1
#endif

#if (HERMES_VARIANT > HERMES_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
