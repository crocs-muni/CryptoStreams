/* ecrypt-sync.h */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef LEX_SYNC
#define LEX_SYNC

#include "../../estream_interface.h"
#include "../ecrypt-portable.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.
 */
#define LEX_NAME "LEX-v2" /* [edit] */
#define LEX_PROFILE "S3___"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; LEX_KEYSIZE(i) <= LEX_MAXKEYSIZE; ++i)
 *   {
 *     keysize = LEX_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define LEX_MAXKEYSIZE 128            /* [edit] */
#define LEX_KEYSIZE(i) (128 + (i)*64) /* [edit] */

#define LEX_MAXIVSIZE 128            /* [edit] */
#define LEX_IVSIZE(i) (128 + (i)*64) /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * LEX_ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

#define NUMBEROFROUNDS 10 /* Valid values are 10/12/14 for key sizes 128/192/256 */
#define NUMWORDS NUMBEROFROUNDS

typedef struct {
    u32 subkeys[4 * (NUMBEROFROUNDS + 1)]; /* Typically 10 round subkeys for 128-bit key Rijndael */
    u32 blockstate[4];                     /* Intermediate state of a block-cipher, 128-bit block */
    u32 ks[NUMWORDS];                      /* Keystream words collected from leaks */

} LEX_ctx;

/* ------------------------------------------------------------------------- */
class ECRYPT_Lex : public estream_interface {
    LEX_ctx _ctx;

public:
    ECRYPT_Lex(int rounds)
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
 * ECRYPT_HAS_SINGLE_BYTE_FUNCTION.
 */
#define LEX_HAS_SINGLE_BYTE_FUNCTION /* [edit] */
#ifdef LEX_HAS_SINGLE_BYTE_FUNCTION

    void ECRYPT_encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) override;

    void ECRYPT_decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) override;

    void LEX_process_bytes(int action, /* 0 = encrypt; 1 = decrypt; */
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
 * reset the LEX_GENERATES_KEYSTREAM flag.
 */

#define LEX_GENERATES_KEYSTREAM
#ifdef LEX_GENERATES_KEYSTREAM

    void LEX_keystream_bytes(LEX_ctx* ctx,
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
 * undef the LEX_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define LEX_USES_DEFAULT_ALL_IN_ONE /* [edit] */

/*
 * Undef LEX_HAS_SINGLE_PACKET_FUNCTION if you want to provide
 * separate packet encryption and decryption functions.
 */
#define LEX_HAS_SINGLE_PACKET_FUNCTION /* [edit] */
#ifdef LEX_HAS_SINGLE_PACKET_FUNCTION

#define LEX_encrypt_packet(ctx, iv, plaintext, ciphertext, mglen)                                  \
    LEX_process_packet(0, ctx, iv, plaintext, ciphertext, mglen)

#define LEX_decrypt_packet(ctx, iv, ciphertext, plaintext, mglen)                                  \
    LEX_process_packet(1, ctx, iv, ciphertext, plaintext, mglen)

    void LEX_process_packet(int action, /* 0 = encrypt; 1 = decrypt; */
                            LEX_ctx* ctx,
                            const u8* iv,
                            const u8* input,
                            u8* output,
                            u32 msglen);

#else

    void
    LEX_encrypt_packet(LEX_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void
    LEX_decrypt_packet(LEX_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

#endif

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * LEX_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define LEX_BLOCKLENGTH 40 /* [edit] */

#define LEX_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef LEX_USES_DEFAULT_BLOCK_MACROS

#define LEX_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                     \
    ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*LEX_BLOCKLENGTH)

#define LEX_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                     \
    ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*LEX_BLOCKLENGTH)

#ifdef LEX_GENERATES_KEYSTREAM

#define LEX_keystream_blocks(ctx, keystream, blocks)                                               \
    LEX_keystream_bytes(ctx, keystream, (blocks)*LEX_BLOCKLENGTH)

#endif

#else

/*
 * Undef LEX_HAS_SINGLE_BLOCK_FUNCTION if you want to provide
 * separate block encryption and decryption functions.
 */
#define LEX_HAS_SINGLE_BLOCK_FUNCTION /* [edit] */
#ifdef LEX_HAS_SINGLE_BLOCK_FUNCTION

#define LEX_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                     \
    LEX_process_blocks(0, ctx, plaintext, ciphertext, blocks)

#define LEX_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                     \
    LEX_process_blocks(1, ctx, ciphertext, plaintext, blocks)

    void LEX_process_blocks(int action, /* 0 = encrypt; 1 = decrypt; */
                            LEX_ctx* ctx,
                            const u8* input,
                            u8* output,
                            u32 blocks); /* Message length in blocks. */

#else

    void LEX_encrypt_blocks(LEX_ctx* ctx,
                            const u8* plaintext,
                            u8* ciphertext,
                            u32 blocks); /* Message length in blocks. */

    void LEX_decrypt_blocks(LEX_ctx* ctx,
                            const u8* ciphertext,
                            u8* plaintext,
                            u32 blocks); /* Message length in blocks. */

#endif

#ifdef LEX_GENERATES_KEYSTREAM

    void
    LEX_keystream_blocks(LEX_ctx* ctx, u8* keystream, u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};
/*
 * If your cipher can be implemented in different ways, you can use
 * the LEX_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DECRYPT_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (LEX_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same LEX_BLOCKLENGTH, etc.).
 */
#define LEX_MAXVARIANT 1 /* [edit] */

#ifndef LEX_VARIANT
#define LEX_VARIANT 1
#endif

#if (LEX_VARIANT > LEX_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
