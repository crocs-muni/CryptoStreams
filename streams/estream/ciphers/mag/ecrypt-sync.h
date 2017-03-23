/* ecrypt-sync.h */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef MAG_SYNC
#define MAG_SYNC

#include "../../estream_interface.h"
#include "../ecrypt-portable.h"

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.
 */
#define MAG_NAME "MAG-v3" /* [edit] */
#define MAG_PROFILE "_____"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; MAG_KEYSIZE(i) <= MAG_MAXKEYSIZE; ++i)
 *   {
 *     keysize = MAG_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define MAG_MAXKEYSIZE 256            /* [edit] */
#define MAG_KEYSIZE(i) (256 + (i)*64) /* [edit] */

#define MAG_MAXIVSIZE 64            /* [edit] */
#define MAG_IVSIZE(i) (64 + (i)*64) /* [edit] */

/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * MAG_ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

typedef struct {
    u64 key[4];
    u64 mask[4];
    u64 cell[128];
    u64 carry;
    u32 pos;
    u32 maskpos;

} MAG_ctx;

/* ------------------------------------------------------------------------- */
class ECRYPT_Mag : public estream_interface {
    MAG_ctx _ctx;

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
#define MAG_HAS_SINGLE_BYTE_FUNCTION 1 /* [edit] */
#ifdef MAG_HAS_SINGLE_BYTE_FUNCTION

    void ECRYPT_encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) override;

    void ECRYPT_decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) override;

    void MAG_process_bytes(int action, /* 0 = encrypt; 1 = decrypt; */
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
 * reset the MAG_GENERATES_KEYSTREAM flag.
 */

#define MAG_GENERATES_KEYSTREAM
#ifdef MAG_GENERATES_KEYSTREAM

    void MAG_keystream_bytes(MAG_ctx* ctx,
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
 * undef the MAG_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define MAG_USES_DEFAULT_ALL_IN_ONE /* [edit] */

/*
 * Undef MAG_HAS_SINGLE_PACKET_FUNCTION if you want to provide
 * separate packet encryption and decryption functions.
 */
#define MAG_HAS_SINGLE_PACKET_FUNCTION /* [edit] */
#ifdef MAG_HAS_SINGLE_PACKET_FUNCTION

#define MAG_encrypt_packet(ctx, iv, plaintext, ciphertext, mglen)                                  \
    MAG_process_packet(0, ctx, iv, plaintext, ciphertext, mglen)

#define MAG_decrypt_packet(ctx, iv, ciphertext, plaintext, mglen)                                  \
    MAG_process_packet(1, ctx, iv, ciphertext, plaintext, mglen)

    void MAG_process_packet(int action, /* 0 = encrypt; 1 = decrypt; */
                            MAG_ctx* ctx,
                            const u8* iv,
                            const u8* input,
                            u8* output,
                            u32 msglen);

#else

    void
    MAG_encrypt_packet(MAG_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void
    MAG_decrypt_packet(MAG_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

#endif

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * MAG_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define MAG_BLOCKLENGTH 1024 /* [edit] */

#define MAG_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef MAG_USES_DEFAULT_BLOCK_MACROS

#define MAG_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                     \
    ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*MAG_BLOCKLENGTH)

#define MAG_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                     \
    ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*MAG_BLOCKLENGTH)

#ifdef MAG_GENERATES_KEYSTREAM

#define MAG_keystream_blocks(ctx, keystream, blocks)                                               \
    MAG_keystream_bytes(ctx, keystream, (blocks)*MAG_BLOCKLENGTH)

#endif

#else

/*
 * Undef MAG_HAS_SINGLE_BLOCK_FUNCTION if you want to provide
 * separate block encryption and decryption functions.
 */
#define MAG_HAS_SINGLE_BLOCK_FUNCTION /* [edit] */
#ifdef MAG_HAS_SINGLE_BLOCK_FUNCTION

#define MAG_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                     \
    MAG_process_blocks(0, ctx, plaintext, ciphertext, blocks)

#define MAG_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                     \
    MAG_process_blocks(1, ctx, ciphertext, plaintext, blocks)

    void MAG_process_blocks(int action, /* 0 = encrypt; 1 = decrypt; */
                            MAG_ctx* ctx,
                            const u8* input,
                            u8* output,
                            u32 blocks); /* Message length in blocks. */

#else

    void MAG_encrypt_blocks(MAG_ctx* ctx,
                            const u8* plaintext,
                            u8* ciphertext,
                            u32 blocks); /* Message length in blocks. */

    void MAG_decrypt_blocks(MAG_ctx* ctx,
                            const u8* ciphertext,
                            u8* plaintext,
                            u32 blocks); /* Message length in blocks. */

#endif

#ifdef MAG_GENERATES_KEYSTREAM

    void
    MAG_keystream_blocks(MAG_ctx* ctx, u8* keystream, u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};

/*
 * If your cipher can be implemented in different ways, you can use
 * the MAG_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DMAG_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (MAG_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same MAG_BLOCKLENGTH, etc.).
 */
#define MAG_MAXVARIANT 1 /* [edit] */

#ifndef MAG_VARIANT
#define MAG_VARIANT 1
#endif

#if (MAG_VARIANT > MAG_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
