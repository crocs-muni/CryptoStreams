/* $Id$ */
/* chi-fast64.c: CHI hash 64 bit optimized implementation. */

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
#include <string.h>
#include <stdio.h>

#include "chi.h"

#if defined(_WIN32) || defined(_WIN64)
#include <stdlib.h>
#   define bswap_32 _byteswap_ulong
#elif defined(__APPLE__)            // EACIRC: manual edit: apple compatibility
#   include <libkern/OSByteOrder.h>
#   define bswap_16 OSSwapInt16
#   define bswap_32 OSSwapInt32
#   define bswap_64 OSSwapInt64
#else
#   include <byteswap.h>
#endif

#define U(X) X##U
#define L(X) X##L
// EACIRC: manual edit
// static const unsigned int* K32 = (const void*)Chi_K; -> static const unsigned int* K32 = (const unsigned int*)Chi_K;
static const unsigned int* K32 = (const unsigned int*)Chi_K;

/*
 * Regardless of endian-ness converts a byte to / from a big endian array of
 * bytes.
 */
#ifdef _BIG_ENDIAN
#    define BYTE2WORD32(b) (*(unsigned int *)(b))
#    define UPPER 0
#    define LOWER 1
#else
#    define BYTE2WORD32(b) bswap_32(*(unsigned int *)(b))
#    define UPPER 1
#    define LOWER 0
#endif

#define LOAD_STATE_FINAL(state, W, i)                                          \
    do {                                                                       \
        U(W) = ROTR32_U(state->hs_State.large32[(i) + UPPER],                  \
                state->hs_State.large32[(i)], 1);                              \
        L(W) = ROTR32_L(state->hs_State.large32[(i) + UPPER],                  \
                state->hs_State.large32[(i)], 1);                              \
        state->hs_State.large32[(i) + UPPER] = U(W);                           \
        state->hs_State.large32[(i) + LOWER] = L(W);                           \
    } while (0)

#define LOAD_STATE(state, W, i)                                                \
    do {                                                                       \
        U(W) = state->hs_State.large32[(i) + UPPER];                           \
        L(W) = state->hs_State.large32[(i) + LOWER];                           \
        state->hs_State.large32[(i) + UPPER] = ROTR32_U(U(W), L(W), 1);        \
        state->hs_State.large32[(i) + LOWER] = ROTR32_L(U(W), L(W), 1);        \
    } while (0)

#define SAVE_STATE(state, W, i)                                                \
    do {                                                                       \
        state->hs_State.large32[(i) + UPPER] ^= U(W);                          \
        state->hs_State.large32[(i) + LOWER] ^= L(W);                          \
    } while (0)

/*
 * Rotate unsigned int to the right by r
 */
#define ROTR32(X,r)     (((X) >> (r)) | ((X) << (32 - (r))))
#define ROTR32_U(U,L,r) (((U) >> (r)) | ((L) << (32 - (r))))
#define ROTR32_L(U,L,r) ROTR32_U(L,U,r)

/*
 * Byte SWAP
 */
#define SWAP8(X) bswap_32(X)

/*
 * Non-linear MAP function.
 */
static INLINE void
map
(
    unsigned int  R,
    unsigned int  S,
    unsigned int  T,
    unsigned int  U,
    unsigned int  *X
)
{
    unsigned int  i;

    i    = (T | U) ^ R;
    X[0] = (i &  S) | ((~T ^ (U & R)) & ~S);
    X[1] = (i & ~S) | (( U ^ (R & T)) &  S);
    X[2] = ( U & (S ^ (T & ~R)))
         | (~U & (R ^ (T &  S)));
}

/*
 * CHI-256
 */
#define _256_MAP(R, S, T, U, M)                                                \
    do {                                                                       \
        map(U(R), U(S), U(T), U(U), U(M));                                     \
        map(L(R), L(S), L(T), L(U), L(M));                                     \
    } while (0)

#define _256_THETA0_U(U) (ROTR32(U, 21) ^ ROTR32(U, 26) ^ ROTR32(U, 30))
#define _256_THETA0_L(L) _256_THETA0_U(L)

#define _256_THETA1_U(U) (ROTR32(U, 14) ^ ROTR32(U, 24) ^ ROTR32(U, 31))
#define _256_THETA1_L(L) _256_THETA1_U(L)

#define _256_MU0_U(U, L) ROTR32_L(U,L,4) ^ ROTR32_U(U,L,18) ^ ((U) >> 1)
#define _256_MU0_L(U, L) ROTR32_U(U,L,4)^ROTR32_L(U,L,18)^(((L)>>1)|((U)<<31))

#define _256_MU1_U(U, L) ROTR32_L(U,L,27) ^ ROTR32_L(U,L,5) ^ ((U) >> 10)
#define _256_MU1_L(U, L) ROTR32_U(U,L,27)^ROTR32_U(U,L,5)^(((L)>>10)|((U)<<22))

static INLINE void
_256_premix
(
    unsigned int  AU,
    unsigned int  AL,
    unsigned int  BU,
    unsigned int  BL,
    unsigned int  DU,
    unsigned int  DL,
    unsigned int  EU,
    unsigned int  EL,
    unsigned int  *RU,
    unsigned int  *RL,
    unsigned int  *SU,
    unsigned int  *SL,
    unsigned int  *TU,
    unsigned int  *TL,
    unsigned int  *UU,
    unsigned int  *UL
)
{
    *SU = AU;
    *TL = ROTR32(AU, 26);
    *UU = ROTR32(AU, 17);

    *UU ^= ROTR32(EL,  2);

    *SL = AL;
    *TU = ROTR32(AL,  7);
    *UL = ROTR32(AL, 12);

    *UL ^= ROTR32(EU, 23);

    *RL = ROTR32(BL,  8);

    *RL ^= ROTR32(DU, 1);
    *TU ^= ROTR32(DU, 14);
    *SL ^= ROTR32(DU, 17);

    *RU = ROTR32(BU,  8);

    *RU ^= ROTR32(DL, 5);
    *SU ^= ROTR32(DL, 18);
    *TL ^= ROTR32(DL, 22);
}

static INLINE void
_256_datainput
(
    unsigned int  V0U,
    unsigned int  V0L,
    unsigned int  V1U,
    unsigned int  V1L,
    unsigned int  *RU,
    unsigned int  *RL,
    unsigned int  *SU,
    unsigned int  *SL,
    unsigned int  *TU,
    unsigned int  *TL,
    unsigned int  *UU,
    unsigned int  *UL
)
{
    *RU ^= V0U;
    *RL ^= V0L;
    *SU ^= V1U;
    *SL ^= V1L;
    *TU ^= _256_THETA0_U(V0U);
    *TL ^= _256_THETA0_L(V0L);
    *UU ^= _256_THETA1_U(V1U);
    *UL ^= _256_THETA1_L(V1L);
}

static INLINE void
_256_postmix
(
    unsigned int  XU,
    unsigned int  XL,
    unsigned int  YU,
    unsigned int  YL,
    unsigned int  ZU,
    unsigned int  ZL,
    unsigned int  *FU,
    unsigned int  *FL,
    unsigned int  *CU,
    unsigned int  *CL
)
{
    unsigned long long  temp;
    unsigned int  U, L, YYU, YYL;

    YYU = SWAP8(ROTR32(YL, 5));
    YYL = SWAP8(ROTR32(YU, 5));
    temp = (((unsigned long long)XU << 32) | XL) + (((unsigned long long)YYU << 32) | YYL);
    U = (unsigned int)(temp >> 32);
    L = (unsigned int)temp;
    *FU ^= ROTR32_U(U, L, 16);
    *FL ^= ROTR32_L(U, L, 16);
    temp = (((unsigned long long)ZL << 32) | ZU) + (((unsigned long long)YYU << 32) | YYL);
    U = (unsigned int)(temp >> 32);
    L = (unsigned int)temp;
    *CU ^= ROTR32_L(U, L, 16);
    *CL ^= ROTR32_U(U, L, 16);
}

#define _256_PREMIX(A, B, D, E, R, S, T, U)                                    \
    _256_premix(U(A), L(A), U(B), L(B), U(D), L(D), U(E), L(E),                \
                  &U(R), &L(R), &U(S), &L(S), &U(T), &L(T), &U(U), &L(U))

#define _256_DATAINPUT(V0, V1, R, S, T, U)                                     \
    _256_datainput(U(V0),L(V0),U(V1),L(V1),                                    \
                  &U(R),&L(R),&U(S),&L(S),&U(T),&L(T),&U(U),&L(U))

#define _256_POSTMIX(M, F, C, i)                                               \
    _256_postmix(U(M)[(i+0)%3],L(M)[(i+0)%3],                                  \
                 U(M)[(i+1)%3],L(M)[(i+1)%3],                                  \
                 U(M)[(i+2)%3],L(M)[(i+2)%3],                                  \
                 &U(F), &L(F), &U(C), &L(C))

#define _256_PREMIX_AND_DATAINPUT(A, B, D, E, R, S, T, U, i)                   \
    do {                                                                       \
        _256_PREMIX(A, B, D, E, R, S, T, U);                                   \
        U(V0) = W[4*(i) + 0] ^ K32[4*(i) + 0 + UPPER];                         \
        L(V0) = W[4*(i) + 1] ^ K32[4*(i) + 0 + LOWER];                         \
        U(V1) = W[4*(i) + 2] ^ K32[4*(i) + 2 + UPPER];                         \
        L(V1) = W[4*(i) + 3] ^ K32[4*(i) + 2 + LOWER];                         \
        _256_DATAINPUT(V0, V1, R, S, T, U);                                    \
    } while(0)

#define _256_STEP(A, B, C, D, E, F, i)                                         \
    do {                                                                       \
        _256_PREMIX_AND_DATAINPUT(A, B, D, E, R, S, T, U, i);                  \
        _256_MAP(R, S, T, U, M);                                               \
        _256_POSTMIX(M, F, C, i);                                              \
    } while(0)

#define _256_STEP_2(A, B, C, D, E, F, i)                                       \
    do {                                                                       \
        _256_STEP(A, B, C, D, E, F, (i) + 0);                                  \
        _256_STEP(F, A, B, C, D, E, (i) + 1);                                  \
    } while(0)

#define _256_STEP_6(A, B, C, D, E, F, i)                                       \
    do {                                                                       \
        _256_STEP(A, B, C, D, E, F, (i) + 0);                                  \
        _256_STEP(F, A, B, C, D, E, (i) + 1);                                  \
        _256_STEP(E, F, A, B, C, D, (i) + 2);                                  \
        _256_STEP(D, E, F, A, B, C, (i) + 3);                                  \
        _256_STEP(C, D, E, F, A, B, (i) + 4);                                  \
        _256_STEP(B, C, D, E, F, A, (i) + 5);                                  \
    } while(0)

#define _256_MSG(W, i)                                                         \
    do {                                                                       \
        W[(i) + 0] = W[(i) - 2*8]                                              \
            ^ _256_MU0_U(W[(i) - 2*7],W[(i) - 2*7 + 1])                        \
            ^ _256_MU1_U(W[(i) - 2*2],W[(i) - 2*2 + 1]);                       \
        W[(i) + 1] = W[(i) - 2*8 + 1]                                          \
            ^ _256_MU0_L(W[(i) - 2*7],W[(i) - 2*7 + 1])                        \
            ^ _256_MU1_L(W[(i) - 2*2],W[(i) - 2*2 + 1]);                       \
    } while(0)

#define _256_MSG_EXP(W, rounds256)                                                        \
    do {                                                                       \
        W[ 0] = BYTE2WORD32(state->hs_DataBuffer +  0*4);                      \
        W[ 1] = BYTE2WORD32(state->hs_DataBuffer +  1*4);                      \
        W[ 2] = BYTE2WORD32(state->hs_DataBuffer +  2*4);                      \
        W[ 3] = BYTE2WORD32(state->hs_DataBuffer +  3*4);                      \
        W[ 4] = BYTE2WORD32(state->hs_DataBuffer +  4*4);                      \
        W[ 5] = BYTE2WORD32(state->hs_DataBuffer +  5*4);                      \
        W[ 6] = BYTE2WORD32(state->hs_DataBuffer +  6*4);                      \
        W[ 7] = BYTE2WORD32(state->hs_DataBuffer +  7*4);                      \
        W[ 8] = BYTE2WORD32(state->hs_DataBuffer +  8*4);                      \
        W[ 9] = BYTE2WORD32(state->hs_DataBuffer +  9*4);                      \
        W[10] = BYTE2WORD32(state->hs_DataBuffer + 10*4);                      \
        W[11] = BYTE2WORD32(state->hs_DataBuffer + 11*4);                      \
        W[12] = BYTE2WORD32(state->hs_DataBuffer + 12*4);                      \
        W[13] = BYTE2WORD32(state->hs_DataBuffer + 13*4);                      \
        W[14] = BYTE2WORD32(state->hs_DataBuffer + 14*4);                      \
        W[15] = BYTE2WORD32(state->hs_DataBuffer + 15*4);                      \
        for (i = 2*_CHI_256_MSG_N; i < 4*rounds256; i+=2)                         \
            _256_MSG(W, i);                                                    \
    } while(0)

/*
 * Parameters:
 *      state   The hash state.
 *      final   1 for the final block in the hash, 0 for intermediate blocks.
 */
void
_Chi_256_update
(
    chiHashState   *state,
    int         final,
	const int rounds256
)
{
    int         i;
    unsigned int      W[4*_CHI_256_STEPS];
    unsigned int      AU, AL, BU, BL, CU, CL, DU, DL, EU, EL, FU, FL;
    unsigned int      RU, RL, SU, SL, TU, TL, UU, UL;
    unsigned int      V0U, V0L, V1U, V1L;
    unsigned int      MU[3], ML[3];

    if (final)
    {
        LOAD_STATE_FINAL(state, A, 0);
        LOAD_STATE_FINAL(state, B, 2);
        LOAD_STATE_FINAL(state, C, 4);
        LOAD_STATE_FINAL(state, D, 6);
        LOAD_STATE_FINAL(state, E, 8);
        LOAD_STATE_FINAL(state, F,10);
    }
    else
    {
        LOAD_STATE(state, A, 0);
        LOAD_STATE(state, B, 2);
        LOAD_STATE(state, C, 4);
        LOAD_STATE(state, D, 6);
        LOAD_STATE(state, E, 8);
        LOAD_STATE(state, F,10);
    }

	_256_MSG_EXP(W, rounds256);

    _256_STEP_6(A, B, C, D, E, F,  0);
    _256_STEP_6(A, B, C, D, E, F,  6);
    _256_STEP_6(A, B, C, D, E, F, 12);
    _256_STEP_2(A, B, C, D, E, F, 18);

    SAVE_STATE(state, E, 0);
    SAVE_STATE(state, F, 2);
    SAVE_STATE(state, A, 4);
    SAVE_STATE(state, B, 6);
    SAVE_STATE(state, C, 8);
    SAVE_STATE(state, D,10);
}


/*
 * CHI-512
 */
#define _512_MAP(R, S, T, U, M)                                                \
    do {                                                                       \
        map(U(R), U(S), U(T), U(U), U(M));                                     \
        map(L(R), L(S), L(T), L(U), L(M));                                     \
    } while(0)

#define _512_THETA0_U(U,L) (ROTR32_U(U,L,5)^ROTR32_U(U,L,6)^ROTR32_L(U,L,11))
#define _512_THETA0_L(U,L) (ROTR32_L(U,L,5)^ROTR32_L(U,L,6)^ROTR32_U(U,L,11))

#define _512_THETA1_U(U,L) (ROTR32_U(U,L,20)^ROTR32_U(U,L,30)^ROTR32_L(U,L,17))
#define _512_THETA1_L(U,L) (ROTR32_L(U,L,20)^ROTR32_L(U,L,30)^ROTR32_U(U,L,17))

#define _512_MU0_U(U,L)    (ROTR32_L(U,L,4)^ROTR32_U(U,L,18)^(U>>1))
#define _512_MU0_L(U,L)    (ROTR32_U(U,L,4)^ROTR32_L(U,L,18)^((L>>1)|(U<<31)))

#define _512_MU1_U(U,L)    (ROTR32_L(U,L,28)^ROTR32_U(U,L,30)^(U>>3))
#define _512_MU1_L(U,L)    (ROTR32_U(U,L,28)^ROTR32_L(U,L,30)^((L>>3)|(U<<29)))

static INLINE void
_512_premix
(
    unsigned int AU,
    unsigned int AL,
    unsigned int BU,
    unsigned int BL,
    unsigned int DU,
    unsigned int DL,
    unsigned int EU,
    unsigned int EL,
    unsigned int GU,
    unsigned int GL,
    unsigned int PU,
    unsigned int PL,
    unsigned int *RU,
    unsigned int *RL,
    unsigned int *SU,
    unsigned int *SL,
    unsigned int *TU,
    unsigned int *TL,
    unsigned int *UU,
    unsigned int *UL
)
{
    *RU = ROTR32_U(BU, BL, 11) ^ ROTR32_U(DU, DL,  8) ^ ROTR32_U(GU, GL, 13);
    *RL = ROTR32_L(BU, BL, 11) ^ ROTR32_L(DU, DL,  8) ^ ROTR32_L(GU, GL, 13);
    *SU = AU                   ^ ROTR32_U(DU, DL, 21) ^ ROTR32_U(GU, GL, 29);
    *SL = AL                   ^ ROTR32_L(DU, DL, 21) ^ ROTR32_L(GU, GL, 29);
    *TU = ROTR32_U(AU, AL, 11) ^ ROTR32_L(DU, DL,  6) ^ PU;
    *TL = ROTR32_L(AU, AL, 11) ^ ROTR32_U(DU, DL,  6) ^ PL;
    *UU = ROTR32_U(AU, AL, 26) ^ ROTR32_L(EU, EL,  8) ^ ROTR32_L(GU, GL, 18);
    *UL = ROTR32_L(AU, AL, 26) ^ ROTR32_U(EU, EL,  8) ^ ROTR32_U(GU, GL, 18);
}

static INLINE void
_512_datainput
(
    unsigned int V0U,
    unsigned int V0L,
    unsigned int V1U,
    unsigned int V1L,
    unsigned int *RU,
    unsigned int *RL,
    unsigned int *SU,
    unsigned int *SL,
    unsigned int *TU,
    unsigned int *TL,
    unsigned int *UU,
    unsigned int *UL
)
{
    *RU ^= V0U;
    *RL ^= V0L;
    *SU ^= V1U;
    *SL ^= V1L;
    *TU ^= _512_THETA0_U(V0U, V0L);
    *TL ^= _512_THETA0_L(V0U, V0L);
    *UU ^= _512_THETA1_U(V1U, V1L);
    *UL ^= _512_THETA1_L(V1U, V1L);
}

static INLINE void
_512_postmix
(
    unsigned int XU,
    unsigned int XL,
    unsigned int YU,
    unsigned int YL,
    unsigned int ZU,
    unsigned int ZL,
    unsigned int *QU,
    unsigned int *QL,
    unsigned int *FU,
    unsigned int *FL,
    unsigned int *CU,
    unsigned int *CL
)
{
    unsigned long long  temp;
    unsigned int  U, L, YYU, YYL;

    YYU = SWAP8(ROTR32_L(YU, YL, 31));
    YYL = SWAP8(ROTR32_U(YU, YL, 31));

    temp = (((unsigned long long)XU << 32) | XL) + (((unsigned long long)YYU << 32) | YYL);
    U = (unsigned int)(temp >> 32);
    L = (unsigned int)temp;
    *QU ^= ROTR32_U(U,L,16);
    *QL ^= ROTR32_L(U,L,16);

    temp = (((unsigned long long)ZL << 32) | ZU) + (((unsigned long long)YYU << 32) | YYL);
    U = (unsigned int)(temp >> 32);
    L = (unsigned int)temp;
    *CU ^= ROTR32_L(U,L,16);
    *CL ^= ROTR32_U(U,L,16);

    U = (XU << 1) | (XL >> 31);
    L = XL <<  1;
    temp = (((unsigned long long)ZL << 32) | ZU) +  (((unsigned long long)U << 32) | L);
    U = (unsigned int)(temp >> 32);
    L = (unsigned int)temp;
    *FU ^= U;
    *FL ^= L;
}

#define _512_PREMIX(A, B, D, E, G, P, R, S, T, U)                              \
    _512_premix(U(A),L(A),U(B),L(B),U(D),L(D),U(E),L(E),U(G),L(G),U(P),L(P),   \
                  &U(R),&L(R),&U(S),&L(S),&U(T),&L(T),&U(U),&L(U))

#define _512_DATAINPUT(V0, V1, R, S, T, U)                                     \
    _512_datainput(U(V0),L(V0),U(V1),L(V1),                                    \
                  &U(R),&L(R),&U(S),&L(S),&U(T),&L(T),&U(U),&L(U))

#define _512_POSTMIX(M, Q, F, C, i)                                            \
    _512_postmix(U(M)[(i + 0) % 3],L(M)[(i + 0) % 3],                          \
                 U(M)[(i + 1) % 3],L(M)[(i + 1) % 3],                          \
                 U(M)[(i + 2) % 3],L(M)[(i + 2) % 3],                          \
                 &U(Q), &L(Q), &U(F), &L(F), &U(C), &L(C))

#define _512_PREMIX_AND_DATAINPUT(A, B, D, E, G, P, R, S, T, U, i)             \
    do {                                                                       \
        _512_PREMIX(A, B, D, E, G, P, R, S, T, U);                             \
        U(V0) = W[4*(i) + 0] ^ K32[4*(i) + 0 + UPPER];                         \
        L(V0) = W[4*(i) + 1] ^ K32[4*(i) + 0 + LOWER];                         \
        U(V1) = W[4*(i) + 2] ^ K32[4*(i) + 2 + UPPER];                         \
        L(V1) = W[4*(i) + 3] ^ K32[4*(i) + 2 + LOWER];                         \
        _512_DATAINPUT(V0, V1, R, S, T, U);                                    \
    } while (0)

#define _512_STEP(A, B, C, D, E, F, G, P, Q, i)                                \
    do {                                                                       \
        _512_PREMIX_AND_DATAINPUT(A, B, D, E, G, P, R, S, T, U, i);            \
        _512_MAP(R, S, T, U, M);                                               \
        _512_POSTMIX(M, Q, F, C, i);                                           \
    } while (0)

#define _512_STEP_4(A, B, C, D, E, F, G, P, Q, i)                              \
    do {                                                                       \
        _512_STEP(A, B, C, D, E, F, G, P, Q, (i) + 0);                         \
        _512_STEP(Q, A, B, C, D, E, F, G, P, (i) + 1);                         \
        _512_STEP(P, Q, A, B, C, D, E, F, G, (i) + 2);                         \
        _512_STEP(G, P, Q, A, B, C, D, E, F, (i) + 3);                         \
    } while (0)

#define _512_STEP_9(A, B, C, D, E, F, G, P, Q, i)                              \
    do {                                                                       \
        _512_STEP(A, B, C, D, E, F, G, P, Q, (i) + 0);                         \
        _512_STEP(Q, A, B, C, D, E, F, G, P, (i) + 1);                         \
        _512_STEP(P, Q, A, B, C, D, E, F, G, (i) + 2);                         \
        _512_STEP(G, P, Q, A, B, C, D, E, F, (i) + 3);                         \
        _512_STEP(F, G, P, Q, A, B, C, D, E, (i) + 4);                         \
        _512_STEP(E, F, G, P, Q, A, B, C, D, (i) + 5);                         \
        _512_STEP(D, E, F, G, P, Q, A, B, C, (i) + 6);                         \
        _512_STEP(C, D, E, F, G, P, Q, A, B, (i) + 7);                         \
        _512_STEP(B, C, D, E, F, G, P, Q, A, (i) + 8);                         \
    } while (0)

#define _512_MSG(W, i)                                                         \
    do {                                                                       \
        W[(i) + 0] = W[(i) - 2*16]                                             \
            ^ W[(i) -  2*7]                                                    \
            ^ _512_MU0_U(W[(i) - 2*15],W[(i) - 2*15 + 1])                      \
            ^ _512_MU1_U(W[(i) -  2*2],W[(i) - 2* 2 + 1]);                     \
        W[(i) + 1] = W[(i) - 2*16 + 1]                                         \
            ^ W[(i) -  2*7 + 1]                                                \
            ^ _512_MU0_L(W[(i) - 2*15],W[(i) - 2*15 + 1])                      \
            ^ _512_MU1_L(W[(i) -  2*2],W[(i) - 2* 2 + 1]);                     \
    } while (0)

#define _512_MSG_EXP(W, rounds512)                                                        \
    do {                                                                       \
        for (i =  0; i < 2*_CHI_512_MSG_N; i++)                                    \
            W[i] = BYTE2WORD32(state->hs_DataBuffer + 4*i);                    \
        for (i = 2*_CHI_512_MSG_N; i < 4*rounds512; i+=2)                         \
            _512_MSG(W, i);                                                    \
    } while (0)

/*
 * Parameters:
 *      state   The hash state.
 *      final   1 for the final block in the hash, 0 for intermediate blocks.
 */
void
_Chi_512_update
(
    chiHashState   *state,
    int         final,
	const int rounds512
)
{
    int         i;

    unsigned int      W[4*_CHI_512_STEPS];
    unsigned int      AU, AL, BU, BL, CU, CL, DU, DL;
    unsigned int      EU, EL, FU, FL, GU, GL, PU, PL, QU, QL;
    unsigned int      RU, RL, SU, SL, TU, TL, UU, UL;
    unsigned int      V0U, V0L, V1U, V1L;
    unsigned int      MU[3], ML[3];

    if (final)
    {
        LOAD_STATE_FINAL(state, A, 0);
        LOAD_STATE_FINAL(state, B, 2);
        LOAD_STATE_FINAL(state, C, 4);
        LOAD_STATE_FINAL(state, D, 6);
        LOAD_STATE_FINAL(state, E, 8);
        LOAD_STATE_FINAL(state, F,10);
        LOAD_STATE_FINAL(state, G,12);
        LOAD_STATE_FINAL(state, P,14);
        LOAD_STATE_FINAL(state, Q,16);
    }
    else
    {
        LOAD_STATE(state, A, 0);
        LOAD_STATE(state, B, 2);
        LOAD_STATE(state, C, 4);
        LOAD_STATE(state, D, 6);
        LOAD_STATE(state, E, 8);
        LOAD_STATE(state, F,10);
        LOAD_STATE(state, G,12);
        LOAD_STATE(state, P,14);
        LOAD_STATE(state, Q,16);
    }

    _512_MSG_EXP(W, rounds512);

    _512_STEP_9(A, B, C, D, E, F, G, P, Q, 0);
    _512_STEP_9(A, B, C, D, E, F, G, P, Q, 9);
    _512_STEP_9(A, B, C, D, E, F, G, P, Q,18);
    _512_STEP_9(A, B, C, D, E, F, G, P, Q,27);
    _512_STEP_4(A, B, C, D, E, F, G, P, Q,36);

    SAVE_STATE(state, F, 0);
    SAVE_STATE(state, G, 2);
    SAVE_STATE(state, P, 4);
    SAVE_STATE(state, Q, 6);
    SAVE_STATE(state, A, 8);
    SAVE_STATE(state, B,10);
    SAVE_STATE(state, C,12);
    SAVE_STATE(state, D,14);
    SAVE_STATE(state, E,16);
}
