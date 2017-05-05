/* ecrypt-sync.h */

/*
 * Header file for synchronous stream ciphers without authentication
 * mechanism.
 *
 * *** Please only edit parts marked with "[edit]". ***
 */

#ifndef DECIM_SYNC
#define DECIM_SYNC

#include "../../estream_interface.h"
#include "../ecrypt-portable.h"
#include <cstdint>

/* ------------------------------------------------------------------------- */

/* Cipher parameters */

/*
 * The name of your cipher.
 */
#define DECIM_NAME "DECIM-v2"
#define DECIM_PROFILE "___H3"

/*
 * Specify which key and IV sizes are supported by your cipher. A user
 * should be able to enumerate the supported sizes by running the
 * following code:
 *
 * for (i = 0; DECIM_KEYSIZE(i) <= DECIM_MAXKEYSIZE; ++i)
 *   {
 *     keysize = DECIM_KEYSIZE(i);
 *
 *     ...
 *   }
 *
 * All sizes are in bits.
 */

#define DECIM_MAXKEYSIZE                                                                           \
    128 /* DECIM is a hardware candidate                                                           \
         *  so the key has only 80 bits */
#define DECIM_KEYSIZE(i) (128 + (i))

#define DECIM_MAXIVSIZE 128         /* DECIM is a hardware candidate */
#define DECIM_IVSIZE(i) (128 + (i)) /* iv has 32 or 64 bits */

/* ------------------------------------------------------------------------- */

/* Data structures */

/*
 * DECIM_ctx is the structure containing the representation of the
 * internal state of your cipher.
 */

typedef struct {
    /*
   * the key
   */
    u8 key[16];

    /*
     * the LFSR related
     * lfsr_state :  state of length 288 bytes
     * lfsr_state[288]: next_bit to enter the state
     * */
    u8 lfsr_state[289];

    /*
     * ABSG related
     * bit_searched     : the bit ABSG is currently looking for
     * searching        : are we searching the bit_searched bit?
     *                    (1=we are, 0=we aren't)
     * immediate_finding: did we instantaneously find the bit_searched bit?
     *                    (1=we did, 0=we don't)
     */
    u8 searching;
    u8 bit_searched;
    u8 immediate_finding;

    /*
     * Filtering related
     * */
    u8 out;
    u8 bool_out;

    /*
     * Buffer related
     * bits_in_byte : number of bits already packed in the byte
     * stream_byte  : the byte where bits are packed
     * buffer       : the array where bits are stored, waiting for being handled
     * buffer_end   : marker to the end of buffer
     * */
    u8 bits_in_byte;
    u8 stream_byte;
    u8 buffer[64];
    u8 buffer_end;

    /* the size of the iv*/
    u8 iv_size;
} DECIM_ctx;


/* ------------------------------------------------------------------------- */
class ECRYPT_Decim : public estream_interface {
    DECIM_ctx _ctx;

private:
    std::uint64_t _LFSR01;
    std::uint64_t _LFSR02;
    std::uint64_t _LFSR03;
    std::uint64_t _LFSR04;
    std::uint64_t _clock;
    std::uint64_t _ABSG01;
    std::uint64_t _ABSG02;
    std::uint64_t _ABSG03;
    std::uint64_t _ABSG04;

public:
    ECRYPT_Decim(int rounds, std::uint64_t heatmap)
        : estream_interface(rounds)
        , _LFSR01(heatmap & 0x0001)
        , _LFSR02(heatmap & 0x0002)
        , _LFSR03(heatmap & 0x0004)
        , _LFSR04(heatmap & 0x0008)
        , _clock(heatmap & 0x0010)
        , _ABSG01(heatmap & 0x0020)
        , _ABSG02(heatmap & 0x0040)
        , _ABSG03(heatmap & 0x0080)
        , _ABSG04(heatmap & 0x0100) {}


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
#define DECIM_HAS_SINGLE_BYTE_FUNCTION /* [edit] */
#ifdef DECIM_HAS_SINGLE_BYTE_FUNCTION

    void ECRYPT_encrypt_bytes(const u8* plaintext, u8* ciphertext, u32 msglen) override;

    void ECRYPT_decrypt_bytes(const u8* ciphertext, u8* plaintext, u32 msglen) override;

    void DECIM_process_bytes(int action, /* 0 = encrypt; 1 = decrypt; */
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
 * reset the DECIM_GENERATES_KEYSTREAM flag.
 */

#define DECIM_GENERATES_KEYSTREAM
#ifdef DECIM_GENERATES_KEYSTREAM

    void DECIM_keystream_bytes(DECIM_ctx* ctx,
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
 * undef the DECIM_USES_DEFAULT_ALL_IN_ONE flag.
 */
#define DECIM_USES_DEFAULT_ALL_IN_ONE /* [edit] */

/*
 * Undef DECIM_HAS_SINGLE_PACKET_FUNCTION if you want to provide
 * separate packet encryption and decryption functions.
 */
#define DECIM_HAS_SINGLE_PACKET_FUNCTION /* [edit] */
#ifdef DECIM_HAS_SINGLE_PACKET_FUNCTION

#define DECIM_encrypt_packet(ctx, iv, plaintext, ciphertext, mglen)                                \
    DECIM_process_packet(0, ctx, iv, plaintext, ciphertext, mglen)

#define DECIM_decrypt_packet(ctx, iv, ciphertext, plaintext, mglen)                                \
    DECIM_process_packet(1, ctx, iv, ciphertext, plaintext, mglen)

    void DECIM_process_packet(int action, /* 0 = encrypt; 1 = decrypt; */
                              DECIM_ctx* ctx,
                              const u8* iv,
                              const u8* input,
                              u8* output,
                              u32 msglen);

#else

    void DECIM_encrypt_packet(
            DECIM_ctx* ctx, const u8* iv, const u8* plaintext, u8* ciphertext, u32 msglen);

    void DECIM_decrypt_packet(
            DECIM_ctx* ctx, const u8* iv, const u8* ciphertext, u8* plaintext, u32 msglen);

#endif

/*
 * Encryption/decryption of blocks.
 *
 * By default, these functions are defined as macros. If you want to
 * provide a different implementation, please undef the
 * DECIM_USES_DEFAULT_BLOCK_MACROS flag and implement the functions
 * declared below.
 */

#define DECIM_BLOCKLENGTH 4 /* [edit] */

#define DECIM_USES_DEFAULT_BLOCK_MACROS /* [edit] */
#ifdef DECIM_USES_DEFAULT_BLOCK_MACROS

#define DECIM_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                   \
    ECRYPT_encrypt_bytes(ctx, plaintext, ciphertext, (blocks)*DECIM_BLOCKLENGTH)

#define DECIM_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                   \
    ECRYPT_decrypt_bytes(ctx, ciphertext, plaintext, (blocks)*DECIM_BLOCKLENGTH)

#ifdef DECIM_GENERATES_KEYSTREAM

#define DECIM_keystream_blocks(ctx, keystream, blocks)                                             \
    DECIM_keystream_bytes(ctx, keystream, (blocks)*DECIM_BLOCKLENGTH)

#endif

#else

/*
 * Undef DECIM_HAS_SINGLE_BLOCK_FUNCTION if you want to provide
 * separate block encryption and decryption functions.
 */
#define DECIM_HAS_SINGLE_BLOCK_FUNCTION /* [edit] */
#ifdef DECIM_HAS_SINGLE_BLOCK_FUNCTION

#define DECIM_encrypt_blocks(ctx, plaintext, ciphertext, blocks)                                   \
    DECIM_process_blocks(0, ctx, plaintext, ciphertext, blocks)

#define DECIM_decrypt_blocks(ctx, ciphertext, plaintext, blocks)                                   \
    DECIM_process_blocks(1, ctx, ciphertext, plaintext, blocks)

    void DECIM_process_blocks(int action, /* 0 = encrypt; 1 = decrypt; */
                              DECIM_ctx* ctx,
                              const u8* input,
                              u8* output,
                              u32 blocks); /* Message length in blocks. */

#else

    void DECIM_encrypt_blocks(DECIM_ctx* ctx,
                              const u8* plaintext,
                              u8* ciphertext,
                              u32 blocks); /* Message length in blocks. */

    void DECIM_decrypt_blocks(DECIM_ctx* ctx,
                              const u8* ciphertext,
                              u8* plaintext,
                              u32 blocks); /* Message length in blocks. */

#endif

#ifdef DECIM_GENERATES_KEYSTREAM

    void DECIM_keystream_blocks(DECIM_ctx* ctx,
                                u8* keystream,
                                u32 blocks); /* Keystream length in blocks. */

#endif

#endif
};
/*
 * If your cipher can be implemented in different ways, you can use
 * the DECIM_VARIANT parameter to allow the user to choose between
 * them at compile time (e.g., gcc -DECRYPT_VARIANT=3 ...). Please
 * only use this possibility if you really think it could make a
 * significant difference and keep the number of variants
 * (DECIM_MAXVARIANT) as small as possible (definitely not more than
 * 10). Note also that all variants should have exactly the same
 * external interface (i.e., the same DECIM_BLOCKLENGTH, etc.).
 */
#define DECIM_MAXVARIANT 1 /* [edit] */

#ifndef DECIM_VARIANT
#define DECIM_VARIANT 1
#endif

#if (DECIM_VARIANT > DECIM_MAXVARIANT)
#error this variant does not exist
#endif

/* ------------------------------------------------------------------------- */

#endif
