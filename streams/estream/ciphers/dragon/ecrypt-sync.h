/* ecrypt-sync.h */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef DRAGON_SYNC
#define DRAGON_SYNC

#include "../../estream_interface.h"
#include "../ecrypt-portable.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.
 */
#define DRAGON_NAME "Dragon" /* [edit] */
#define DRAGON_PROFILE "S3___"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; DRAGON_KEYSIZE(i) <= DRAGON_MAXKEYSIZE; ++i)
 *   {
 *     keysize = DRAGON_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define DRAGON_MAXKEYSIZE 256             /* [edit] */
#define DRAGON_KEYSIZE(i) (128 + (i)*128) /* [edit] */

#define DRAGON_MAXIVSIZE 256             /* [edit] */
#define DRAGON_IVSIZE(i) (128 + (i)*128) /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * DRAGON_ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

#define DRAGON_NLFSR_SIZE 32    /* size of NLFSR in 32-bit multiples */
#define DRAGON_KEYSTREAM_SIZE 2 /* size of output in 32-bit multiples */

#ifdef _DRAGON_OPT
#define DRAGON_BUFFER_SIZE 16 /* number of keystream blocks to buffer */
#else
#define DRAGON_BUFFER_SIZE 1
#endif

#define DRAGON_BUFFER_BYTES (DRAGON_BUFFER_SIZE * DRAGON_KEYSTREAM_SIZE * 4)

typedef struct {
    /* The NLFSR and counter comprise the state of Dragon */
    u32 nlfsr_word[DRAGON_NLFSR_SIZE];

#ifdef _DRAGON_OPT_
    u32 state_counter[2];
#else
    u64 state_counter;
#endif
    /* NLFSR shifting is modelled by the decrement of the nlfsr_offset
     * pointer, which indicates the 0th element of the NLFSR
     */
    u32 nlfsr_offset;

    /* Although key and IV injection are not seperated processes in Dragon
     * the DRAGON API requires that they are added to the state separately.
     * Thus, to maintain consistency, the state at the end of the key
     * injection needs to be recalled for IV injection.
     */
    u32 init_state[DRAGON_NLFSR_SIZE];
    u32 key_size;

    /* Dragon is a block-cipher but the DRAGON API mandates that partial
     * blocks must be able to be encrypted. Usually this will involve
     * caller-managed buffering, but the DRAGON API makes no provision
     * for this, so buffering unforutnately needs to be managed internally
     * to the primitive.
     */
    u8 keystream_buffer[DRAGON_BUFFER_BYTES];
    u32 buffer_index;
} DRAGON_ctx;

/* ------------------------------------------------------------------------- */

class ECRYPT_Dragon : public estream_interface {
    DRAGON_ctx _ctx;

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
     * IV setup. After having called DRAGON_keysetup(), the user is
     * allowed to call DRAGON_ivsetup() different times in order to
     * encrypt/decrypt different messages with the same key but different
     * IV's.
     */
    void ECRYPT_ivsetup(const u8* iv) override;

/*
 * Encryption/decryption of arbitrary length messages.
 *
 * For efficiency reasons, the API provides two types of
 * encrypt/decrypt functions. The DRAGON_encrypt_bytes() function
 * (declared here) encrypts byte strings of arbitrary length, while
 * the DRAGON_encrypt_blocks() function (defined later) only accepts
 * lengths which are multiples of DRAGON_BLOCKLENGTH.
 *
 * The user is allowed to make multiple calls to
 * DRAGON_encrypt_blocks() to incrementally encrypt a long message,
 * but he is NOT allowed to make additional encryption calls once he
 * has called DRAGON_encrypt_bytes() (unless he starts a new message
 * of course). For example, this sequence of calls is acceptable:
 *
 * DRAGON_keysetup();
 *
 * DRAGON_ivsetup();
 * DRAGON_encrypt_blocks();
 * DRAGON_encrypt_blocks();
 * DRAGON_encrypt_bytes();
 *
 * DRAGON_ivsetup();
 * DRAGON_encrypt_blocks();
 * DRAGON_encrypt_blocks();
 *
 * DRAGON_ivsetup();
 * DRAGON_encrypt_bytes();
 *
 * The following sequence is not:
 *
 * DRAGON_keysetup();
 * DRAGON_ivsetup();
 * DRAGON_encrypt_blocks();
 * DRAGON_encrypt_bytes();
 * DRAGON_encrypt_blocks();
 */

/*
 * By default DRAGON_encrypt_bytes() and DRAGON_decrypt_bytes() are
 * defined as macros which redirect the call to a single function
 * DRAGON_process_bytes(). If you want to provide separate encryption
 * and decryption functions, please undef
 * DRAGON_HAS_SINGLE_BYTE_FUNCTION.
 */
#define DRAGON_HAS_SINGLE_BYTE_FUNCTION /* [edit] */
#ifdef DRAGON_HAS_SINGLE_BYTE_FUNCTION

    void ECRYPT_encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) override;

    void ECRYPT_decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) override;

    void DRAGON_process_bytes(int action, /* 0 = encrypt; 1 = decrypt; */
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
 * reset the DRAGON_GENERATES_KEYSTREAM flag.
 */

#define DRAGON_GENERATES_KEYSTREAM
#ifdef DRAGON_GENERATES_KEYSTREAM

    void DRAGON_keystream_bytes(DRAGON_ctx* ctx,
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
 * undef the DRAGON_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define DRAGON_USES_DEFAULT_ALL_IN_ONE /* [edit] */

/*
 * Undef DRAGON_HAS_SINGLE_PACKET_FUNCTION if you want to provide
 * separate packet encryption and decryption functions.
 */
#define DRAGON_HAS_SINGLE_PACKET_FUNCTION /* [edit] */
#ifdef DRAGON_HAS_SINGLE_PACKET_FUNCTION

#define DRAGON_encrypt_packet(ctx, iv, plaintext, ciphertext, mglen)                               \
    DRAGON_process_packet(0, ctx, iv, plaintext, ciphertext, mglen)

#define DRAGON_decrypt_packet(ctx, iv, ciphertext, plaintext, mglen)                               \
    DRAGON_process_packet(1, ctx, iv, ciphertext, plaintext, mglen)

    void DRAGON_process_packet(int action, /* 0 = encrypt; 1 = decrypt; */
                               DRAGON_ctx* ctx,
                               const u8* iv,
                               const u8* input,
                               u8* output,
                               u32 msglen);

#else

    void DRAGON_encrypt_packet(
            DRAGON_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void DRAGON_decrypt_packet(
            DRAGON_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

#endif

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * DRAGON_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define DRAGON_BLOCKLENGTH 8 /* [edit] */

#undef DRAGON_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef DRAGON_USES_DEFAULT_BLOCK_MACROS

#define DRAGON_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                  \
    DRAGON_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*DRAGON_BLOCKLENGTH)

#define DRAGON_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                  \
    DRAGON_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*DRAGON_BLOCKLENGTH)

#ifdef DRAGON_GENERATES_KEYSTREAM

#define DRAGON_keystream_blocks(ctx, keystream, blocks)                                            \
    DRAGON_keystream_bytes(ctx, keystream, (blocks)*DRAGON_BLOCKLENGTH)

#endif

#else

/*
 * Undef DRAGON_HAS_SINGLE_BLOCK_FUNCTION if you want to provide
 * separate block encryption and decryption functions.
 */
#define DRAGON_HAS_SINGLE_BLOCK_FUNCTION /* [edit] */
#ifdef DRAGON_HAS_SINGLE_BLOCK_FUNCTION

#define DRAGON_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                  \
    DRAGON_process_blocks(0, ctx, plaintext, ciphertext, blocks)

#define DRAGON_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                  \
    DRAGON_process_blocks(1, ctx, ciphertext, plaintext, blocks)

    void DRAGON_process_blocks(int action, /* 0 = encrypt; 1 = decrypt; */
                               void* ctx,
                               const u8* input,
                               u8* output,
                               u32 blocks); /* Message length in blocks. */

#else

    void DRAGON_encrypt_blocks(DRAGON_ctx* ctx,
                               const u8* plaintext,
                               u8* ciphertext,
                               u32 blocks); /* Message length in blocks. */

    void DRAGON_decrypt_blocks(DRAGON_ctx* ctx,
                               const u8* ciphertext,
                               u8* plaintext,
                               u32 blocks); /* Message length in blocks. */

#endif

#ifdef DRAGON_GENERATES_KEYSTREAM

    void DRAGON_keystream_blocks(DRAGON_ctx* ctx,
                                 u8* keystream,
                                 u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};

/*
 * If your cipher can be implemented in different ways, you can use
 * the DRAGON_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DDRAGON_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (DRAGON_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same DRAGON_BLOCKLENGTH, etc.).
 */
#define DRAGON_MAXVARIANT 1 /* [edit] */

#ifndef DRAGON_VARIANT
#define DRAGON_VARIANT 1
#endif

#if (DRAGON_VARIANT > DRAGON_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
