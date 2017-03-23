/* ecrypt-sync.h */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef MIR1_SYNC
#define MIR1_SYNC

#include "../../estream_interface.h"
#include "../ecrypt-portable.h"

#ifndef _Mir1_DATASTRUCTURES_AND_TYPES_DEFINED
#define _Mir1_DATASTRUCTURES_AND_TYPES_DEFINED
/* ------------------------------------------------------------------------- */
/* Data Types used in coding */
/* ------------------------------------------------------------------------- */
typedef u64 ull;
typedef u32 ul;
typedef u8 uc;

#if defined(ECRYPT_LITTLE_ENDIAN)
typedef union {
    struct _b {
        uc b0, b1, b2, b3, b4, b5, b6, b7;
    } b; /* 8 -bit access */
    struct _w {
        ul w0, w1;
    } w;   /* 32-bit access */
    ull v; /* 64-bit access */
} Int;
#elif defined(ECRYPT_BIG_ENDIAN)
typedef union {
    struct _b {
        uc b7, b6, b5, b4, b3, b2, b1, b0;
    } b; /* 8 -bit access */
    struct _w {
        ul w1, w0;
    } w;   /* 32-bit access */
    ull v; /* 64-bit access */
} Int;
#else
#error Could not detect endianness
#endif

/* ------------------------------------------------------------------------- */
/* Data structures */
/* ------------------------------------------------------------------------- */
typedef struct {
    Int a, b, x0, x1, x2, x3; /* Internal State */
    uc S[256];                /* secret S-box   */

    u8 key[16];

} MIR1_ctx;

#endif

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.
 */
#define MIR1_NAME "Mir-1" /* [edit] */
#define MIR1_PROFILE "_____"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; MIR1_KEYSIZE(i) <= MIR1_MAXKEYSIZE; ++i)
 *   {
 *     keysize = MIR1_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define MIR1_MAXKEYSIZE 128            /* [edit] */
#define MIR1_KEYSIZE(i) (128 + (i)*32) /* [edit] */

#define MIR1_MAXIVSIZE 64            /* [edit] */
#define MIR1_IVSIZE(i) (64 + (i)*64) /* [edit] */

/* ------------------------------------------------------------------------- */
class ECRYPT_Mir : public estream_interface {
    MIR1_ctx _ctx;

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
 * reset the MIR1_GENERATES_KEYSTREAM flag.
 */

#define MIR1_GENERATES_KEYSTREAM
#ifdef MIR1_GENERATES_KEYSTREAM

    void MIR1_keystream_bytes(MIR1_ctx* ctx,
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
 * undef the MIR1_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define MIR1_USES_DEFAULT_ALL_IN_ONE /* [edit] */

    void MIR1_encrypt_packet(
            MIR1_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void MIR1_decrypt_packet(
            MIR1_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * MIR1_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define MIR1_BLOCKLENGTH 8 /* [edit] */

#define MIR1_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef MIR1_USES_DEFAULT_BLOCK_MACROS

#define MIR1_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                    \
    ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*MIR1_BLOCKLENGTH)

#define MIR1_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                    \
    ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*MIR1_BLOCKLENGTH)

#ifdef MIR1_GENERATES_KEYSTREAM

#define MIR1_keystream_blocks(ctx, keystream, blocks)                                              \
    MIR1_keystream_bytes(ctx, keystream, (blocks)*MIR1_BLOCKLENGTH)

#endif

#else

    void MIR1_encrypt_blocks(MIR1_ctx* ctx,
                             const u8* plaintext,
                             u8* ciphertext,
                             u32 blocks); /* Message length in blocks. */

    void MIR1_decrypt_blocks(MIR1_ctx* ctx,
                             const u8* ciphertext,
                             u8* plaintext,
                             u32 blocks); /* Message length in blocks. */

#ifdef MIR1_GENERATES_KEYSTREAM

    void MIR1_keystream_blocks(MIR1_ctx* ctx,
                               const u8* keystream,
                               u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};

/*
 * If your cipher can be implemented in different ways, you can use
 * the MIR1_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DMIR1_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (MIR1_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same MIR1_BLOCKLENGTH, etc.).
 */
#define MIR1_MAXVARIANT 1 /* [edit] */

#ifndef MIR1_VARIANT
#define MIR1_VARIANT 1
#endif

#if (MIR1_VARIANT > MIR1_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
