/* chi.h: CHI hash */

/*
THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE AND AGAINST
INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#ifndef _CHI_H
#define _CHI_H


/*
 * Startard sizes
 */
/*#if defined(_WIN32) || defined(_WIN64)
    typedef unsigned __int32 uint32_t;
    typedef unsigned __int64 uint64_t;
#else
#   include <stdint.h>
#endif

typedef uint32_t WORD32;
typedef uint64_t WORD64;*/

/*
 * Function inlining
 */
#if !defined(USE_INLINE) && !defined(DONT_USE_INLINE)
#   define USE_INLINE
#endif

#ifdef USE_INLINE
#   if defined(_WIN32) || defined(_WIN64)
#       define INLINE __inline
#   else
#       define INLINE inline
#   endif
#else
#   define INLINE
#endif

/*
 * Fundamental algorithm constants.
 */
#define _CHI_256_STEPS      20      /* Steps in 256 bit hash.                     */
#define _CHI_512_STEPS      40      /* Steps in 512 bit hash.                     */
#define _CHI_256_STATE_N    6       /* The number of words in the internal state
                                 * for the 256 bit hash.
                                 */
#define _CHI_512_STATE_N    9       /* The number of words in the internal state
                                 * for the 512 bit hash.
                                 */
#define _CHI_256_MSG_N      (_CHI_256_MSG_BLK_LEN / 64)
                                /* The length in 64 bit ints of the message
                                 * block in bits for the 256 bit hash.
                                 */
#define _CHI_512_MSG_N      (_CHI_512_MSG_BLK_LEN / 64)
                                /* The length in 64 bit ints of the message
                                 * block in bits for the 256 bit hash.
                                 */

/*
 * Valid message block lengths.
 * */
typedef enum {
    _CHI_256_MSG_BLK_LEN = 512,
    _CHI_512_MSG_BLK_LEN = 1024
} ChiMsgBlkLen;
/*
 * _CHI_256_MSG_BLK_LEN     The length in bits of the message block for the 256
 *                      bit hash.
 * _CHI_512_MSG_BLK_LEN     The length in bits of the message block for the 512
 *                      bit hash.
 */

/*
 * Valid hash bit lengths.
 */
typedef enum {
    _CHI_224_HASH_BIT_LEN = 224,
    _CHI_256_HASH_BIT_LEN = 256,
    _CHI_384_HASH_BIT_LEN = 384,
    _CHI_512_HASH_BIT_LEN = 512
} ChiHashBitLen;

/*
 * Standard CHI_MIN macro
 */
#define CHI_MIN(x, y)  ((x) < (y) ? (x) : (y))

/*
 * Step constants
 */
const unsigned long long
Chi_K[] = {
    0x9B05688C2B3E6C1FULL, 0xDBD99E6FF3C90BDCULL,
    0x4DBC64712A5BB168ULL, 0x767E27C3CF76C8E7ULL,
    0x21EE9AC5EF4C823AULL, 0xB36CCFC1204A9BD8ULL,
    0x754C8A7FB36BD941ULL, 0xCF20868F04A825E9ULL,
    0xABB379E0A8838BB0ULL, 0x12D8B70A5959E391ULL,
    0xB59168FA9F69E181ULL, 0x15C7D4918739F18FULL,
    0x4B547673EA8D68E0ULL, 0x3CED7326FCD0EF81ULL,
    0x09F1D77309998460ULL, 0x1AF3937415E91F32ULL,
    0x1F83D9ABFB41BD6BULL, 0x23C4654C2A217583ULL,
    0x2842012131573F2AULL, 0xA59916ACA3991FCAULL,
    0xEC1B7C06FD19D256ULL, 0x1EC785BCDC8BAF26ULL,
    0x69CA4E0FF2E6BDD8ULL, 0xCA2575EE6C950D0EULL,
    0x5BCF66D2FB3D99F6ULL, 0x9D6D08C7BBCA18F3ULL,
    0xEEF64039F2175E83ULL, 0x00ED5AEBAA2AB6E0ULL,
    0x5040712FC29AD308ULL, 0x6DAFE433438D2C43ULL,
    0xBD7FAA3F06C71F15ULL, 0x03B5AA8CE9B6A4DDULL,
    0x5BE0CD19137E2179ULL, 0x867F5E3B72221265ULL,
    0x43B6CBE0D67F4A20ULL, 0xDB99D767CB0E4933ULL,
    0xDC450DBC469248BDULL, 0xFE1E5E4876100D6FULL,
    0xB799D29EA1733137ULL, 0x16EA7ABCF92053C4ULL,
    0xBE3ECE968DBA92ACULL, 0x18538F84D82C318BULL,
    0x38D79F4E9C8A18C0ULL, 0xA8BBC28F1271F1F7ULL,
    0x2796E71067D2C8CCULL, 0xDE1BF2334EDB3FF6ULL,
    0xB094D782A857F9C1ULL, 0x639B484B0C1DAED1ULL,
    0xCBBB9D5DC1059ED8ULL, 0xE7730EAFF25E24A3ULL,
    0xF367F2FC266A0373ULL, 0xFE7A4D34486D08AEULL,
    0xD41670A136851F32ULL, 0x663914B66B4B3C23ULL,
    0x1B9E3D7740A60887ULL, 0x63C11D86D446CB1CULL,
    0xD167D2469049D628ULL, 0xDDDBB606B9A49E38ULL,
    0x557F1C8BEE68A7F7ULL, 0xF99DC58B50F924BDULL,
    0x205ACC9F653512A5ULL, 0x67C66344E4BAB193ULL,
    0x18026E467960D0C8ULL, 0xA2F5D84DAECA8980ULL,
    0x629A292A367CD507ULL, 0x98E67012D90CBB6DULL,
    0xEED758D1D18C7E35ULL, 0x031C02E4437DC71EULL,
    0x79B63D6482198EB7ULL, 0x936A9D7E8C9E4B33ULL,
    0xB30CA682C3E6C65DULL, 0xCC442382BA4262FAULL,
    0x51179BA5A1D37FF6ULL, 0x7202BDE7A98EEA51ULL,
    0x2B9F65D1DF9C610FULL, 0xF56B742B0AF1CE83ULL,
    0xF9989D199B75848BULL, 0xD142F19D8B46D578ULL,
    0x7A7580514D75EA33ULL, 0xB74F9690808E704DULL
};

/*
 * Initial state values for different bit lengths
 */
const unsigned long long
Chi_224_init[] = {
    0xA54FF53A5F1D36F1ULL, 0xCEA7E61FC37A20D5ULL,
    0x4A77FE7B78415DFCULL, 0x8E34A6FE8E2DF92AULL,
    0x4E5B408C9C97D4D8ULL, 0x24A05EEE29922401ULL
};

const unsigned long long
Chi_256_init[] = {
    0x510E527FADE682D1ULL, 0xDE49E330E42B4CBBULL,
    0x29BA5A455316E0C6ULL, 0x5507CD18E9E51E69ULL,
    0x4F9B11C81009A030ULL, 0xE3D3775F155385C6ULL
};

const unsigned long long
Chi_384_init[] = {
    0xA54FF53A5F1D36F1ULL, 0xCEA7E61FC37A20D5ULL,
    0x4A77FE7B78415DFCULL, 0x8E34A6FE8E2DF92AULL,
    0x4E5B408C9C97D4D8ULL, 0x24A05EEE29922401ULL,
    0x5A8176CFFC7C2224ULL, 0xC3EDEBDA29BEC4C8ULL,
    0x8A074C0F4D999610ULL
};

const unsigned long long
Chi_512_init[] = {
    0x510E527FADE682D1ULL, 0xDE49E330E42B4CBBULL,
    0x29BA5A455316E0C6ULL, 0x5507CD18E9E51E69ULL,
    0x4F9B11C81009A030ULL, 0xE3D3775F155385C6ULL,
    0x489221632788FB30ULL, 0x41921DB8FEEB38C2ULL,
    0x9AF94A7C48BBD5B6ULL
};



/* NIST API */

/*
 * BitSequence type for Message Data
 */
typedef unsigned char BitSequence;

/*
 * DataLength type big, for big data lengths!.
 */
typedef unsigned long long DataLength;

/*
 * Error code for external Interface
 */
typedef enum {
    CHI_SUCCESS   = 0,
    CHI_FAIL      = 1,
    CHI_BAD_HASHLEN   = 2,
    CHI_BAD_STATE     = 3,
    CHI_BAD_DATA      = 4,
    CHI_TOO_MUCH_DATA = 5,
    CHI_BAD_HASHVALPTR
} ChiHashReturn;
/*
 * CHI_BAD_STATE            State either NULL or corrupted.
 * CHI_BAD_DATA             Either NULL passed for data or Update called with
 *                      a partial byte, then called again with more data.
 * CHI_TOO_MUCH_DATA        Very unlikely case that too much data was passed to
 *                      hash function.
 * CHI_BAD_HASHVALPTR       Pointer for returning hash value invalid.
 */

/*
 * Structure that stores the hash state.
 */
#undef small
typedef struct {
    union {
        unsigned long long small[_CHI_256_STATE_N];
        unsigned long long large[_CHI_512_STATE_N];
        unsigned int small32[2*_CHI_256_STATE_N];
        unsigned int large32[2*_CHI_512_STATE_N];
    } hs_State;                     /* Contents of internal state             */

    ChiHashBitLen  hs_HashBitLen;      /* Length of the Hash.  Passed into Init. */
    ChiMsgBlkLen   hs_MessageLen;      /* Length of message block in bits.  This
                                     * is set in Init depending on hashbitlen
                                     * to _256_MSG_LENGTH for 224,256 bit hash
                                     * and _512_MSG_LENGTH for 384,512 bit
                                     * hash.
                                     */
    unsigned long long    hs_TotalLenLow;     /* Total length of message in bits.  Does
                                     * not include length of padding.
                                     */
    unsigned long long    hs_TotalLenHigh;    /* For exceedingly long messages.
                                     */
    unsigned int    hs_DataLen;         /* Number of unprocess bits in
                                     * hs_DataBuffer.
                                     */
    BitSequence hs_DataBuffer[128]; /* Buffer for accumulating message.       */
} chiHashState;

/*
 * Local function declarations
 */
void _Chi_256_update(chiHashState *state, int final, const int rounds256);
void _Chi_512_update(chiHashState *state, int final, const int rounds512);

/*
 * External Interface
 */
/*ChiHashReturn Init(chiHashState*, int); 
ChiHashReturn Update(chiHashState*, const BitSequence*, DataLength); 
ChiHashReturn Final(chiHashState*, BitSequence*); 
ChiHashReturn Hash(int, const BitSequence*, DataLength, BitSequence*);*/

#endif /* _CHI_H */
