/* ecrypt-sync.h */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef POMARANCH_SYNC
#define POMARANCH_SYNC

#include "../../estream_interface.h"
#include "../ecrypt-portable.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.
 */
#define POMARANCH_NAME "POMARANCH-v1" /* [edit] */
#define POMARANCH_PROFILE "_____"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; POMARANCH_KEYSIZE(i) <= POMARANCH_MAXKEYSIZE; ++i)
 *   {
 *     keysize = POMARANCH_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define POMARANCH_MAXKEYSIZE 128       /* [edit] */
#define POMARANCH_KEYSIZE(i) (128 + i) /* [edit] */

#define POMARANCH_MAXIVSIZE 112         /* [edit] */
#define POMARANCH_IVSIZE(i) (8 + 8 * i) /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * POMARANCH_ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

typedef struct {
    u16 state[9]; /* states of the registers */
    u16 Key[8];   /* 128-bit key in 2-byte words */
    u8 IV_size;   /* size of the IV in bits */
} POMARANCH_ctx;

/* ------------------------------------------------------------------------- */
class ECRYPT_Pomaranch : public estream_interface {
    POMARANCH_ctx _ctx;

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

/*
 * By default ECRYPT_encrypt_bytes() and ECRYPT_decrypt_bytes() are
 * defined as macros which redirect the call to a single function
 * ECRYPT_process_bytes(). If you want to provide separate encryption
 * and decryption functions, please undef
 * ECRYPT_HAS_SINGLE_BYTE_FUNCTION.
 */
#define POMARANCH_HAS_SINGLE_BYTE_FUNCTION /* [edit] */
#ifdef POMARANCH_HAS_SINGLE_BYTE_FUNCTION

    void ECRYPT_encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) override;

    void ECRYPT_decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) override;

    void POMARANCH_process_bytes(int action, /* 0 = encrypt; 1 = decrypt; */
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
 * reset the POMARANCH_GENERATES_KEYSTREAM flag.
 */

#define POMARANCH_GENERATES_KEYSTREAM
#ifdef POMARANCH_GENERATES_KEYSTREAM

    void POMARANCH_keystream_bytes(POMARANCH_ctx* ctx,
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
 * undef the POMARANCH_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define POMARANCH_USES_DEFAULT_ALL_IN_ONE /* [edit] */

/*
 * Undef POMARANCH_HAS_SINGLE_PACKET_FUNCTION if you want to provide
 * separate packet encryption and decryption functions.
 */
#define POMARANCH_HAS_SINGLE_PACKET_FUNCTION /* [edit] */
#ifdef POMARANCH_HAS_SINGLE_PACKET_FUNCTION

#define POMARANCH_encrypt_packet(ctx, iv, plaintext, ciphertext, mglen)                            \
    POMARANCH_process_packet(0, ctx, iv, plaintext, ciphertext, mglen)

#define POMARANCH_decrypt_packet(ctx, iv, ciphertext, plaintext, mglen)                            \
    POMARANCH_process_packet(1, ctx, iv, ciphertext, plaintext, mglen)

    void POMARANCH_process_packet(int action, /* 0 = encrypt; 1 = decrypt; */
                                  POMARANCH_ctx* ctx,
                                  const u8* iv,
                                  const u8* input,
                                  u8* output,
                                  u32 msglen);

#else

    void POMARANCH_encrypt_packet(
            POMARANCH_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void POMARANCH_decrypt_packet(
            POMARANCH_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

#endif

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * POMARANCH_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define POMARANCH_BLOCKLENGTH 4 /* [edit] */

#define POMARANCH_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef POMARANCH_USES_DEFAULT_BLOCK_MACROS

#define POMARANCH_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                               \
    ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*POMARANCH_BLOCKLENGTH)

#define POMARANCH_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                               \
    ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*POMARANCH_BLOCKLENGTH)

#ifdef POMARANCH_GENERATES_KEYSTREAM

#define POMARANCH_keystream_blocks(ctx, keystream, blocks)                                         \
    POMARANCH_keystream_bytes(ctx, keystream, (blocks)*POMARANCH_BLOCKLENGTH)

#endif

#else

/*
 * Undef POMARANCH_HAS_SINGLE_BLOCK_FUNCTION if you want to provide
 * separate block encryption and decryption functions.
 */
#define POMARANCH_HAS_SINGLE_BLOCK_FUNCTION /* [edit] */
#ifdef POMARANCH_HAS_SINGLE_BLOCK_FUNCTION

#define POMARANCH_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                               \
    POMARANCH_process_blocks(0, ctx, plaintext, ciphertext, blocks)

#define POMARANCH_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                               \
    POMARANCH_process_blocks(1, ctx, ciphertext, plaintext, blocks)

    void POMARANCH_process_blocks(int action, /* 0 = encrypt; 1 = decrypt; */
                                  POMARANCH_ctx* ctx,
                                  const u8* input,
                                  u8* output,
                                  u32 blocks); /* Message length in blocks. */

#else

    void POMARANCH_encrypt_blocks(POMARANCH_ctx* ctx,
                                  const u8* plaintext,
                                  u8* ciphertext,
                                  u32 blocks); /* Message length in blocks. */

    void POMARANCH_decrypt_blocks(POMARANCH_ctx* ctx,
                                  const u8* ciphertext,
                                  u8* plaintext,
                                  u32 blocks); /* Message length in blocks. */

#endif

#ifdef POMARANCH_GENERATES_KEYSTREAM

    void POMARANCH_keystream_blocks(POMARANCH_ctx* ctx,
                                    u8* keystream,
                                    u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};
/*
 * If your cipher can be implemented in different ways, you can use
 * the POMARANCH_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DECRYPT_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (POMARANCH_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same POMARANCH_BLOCKLENGTH, etc.).
 */
#define POMARANCH_MAXVARIANT 1 /* [edit] */

#ifndef POMARANCH_VARIANT
#define POMARANCH_VARIANT 1
#endif

#if (POMARANCH_VARIANT > POMARANCH_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
