#include <stddef.h>
#include <string.h>
#include "Shabal_sha3.h"

/*
 * Part of this code was automatically generated (the part between
 * the "BEGIN" and "END" markers). See optgen.c for details.
 */

/*
 * If the "p" and "e" parameters are modified for research purpose,
 * the the automatic code generation program must be run again.
 */
#ifndef SHABAL_PARAM_P
#define SHABAL_PARAM_P   3
#endif
#ifndef SHABAL_PARAM_E
#define SHABAL_PARAM_E   3
#endif

#if SHABAL_BLOCK_SIZE != 16
#error Non-standard block size; this code is not valid
#endif
#define shabal_sM    SHABAL_BLOCK_SIZE
#define shabal_u32   shabal_word32

/*
 * We perform explicit truncations to 32 bits. This is necessary on some
 * quite exotic platforms, where the "shabal_u32" type turns out to have more
 * than 32 bits. On the more common platforms where "shabal_u32" has exactly
 * 32 bits, then the compiler is smart enough to notice that SHABAL_T32() has
 * no effect.
 */
#define SHABAL_C32(x)   ((shabal_u32)x ## UL)
#define SHABAL_T32(x)   ((x) & SHABAL_C32(0xFFFFFFFF))

#define SHABAL_O1   13
#define SHABAL_O2    9
#define SHABAL_O3    6

/*
 * C compilers following the current C standard (ISO 9899:1999), and
 * older GNU C compiler versions, understand the "inline" keyword.
 * Old Microsoft Visual C compilers know it under the "__inline"
 * keyword. For other compilers, we just rely on the compiler to
 * guess which calls should be inlined.
 */
#if (defined __STDC__ && __STDC_VERSION__ >= 199901L) || defined __GNUC__
#define INLINE inline
#elif defined _MSC_VER
#define INLINE __inline
#else
#define INLINE
#endif

/*
 * Decode a 32-bit word with little-endian convention, assuming that
 * the input data is correctly aligned for a direct 32-bit access
 * (alignment is the reason why we defined the "buffer[]" array to be the
 * first field in the "hashState" structure).
 *
 * On x86 processors (either in 32-bit or 64-bit mode), we know that
 * the local convention is little-endian, hence we ca perform a
 * direct access. On SPARC v9 processors, convention is big-endian but
 * there are special opcodes for little-endian access, which we use
 * here (only if using GNU C, because we use inline assembly with a
 * syntax specific to GNU C). On all other platforms, the generic code
 * works. This could be enhanced with specific code for other platforms
 * and compilers. Note that message decoding does not contribute much
 * to the overall computation speed.
 */
static INLINE shabal_u32 shabal_dec32le_aligned(const void *src)
{
#if defined __i386__ || defined __x86_64
	return *(const shabal_u32 *)src;
#elif defined __GNUC__ && defined __sparcv9 && !defined SHABAL_NO_ASM
	shabal_u32 tmp;

	__asm__ __volatile__ (
		"lda [%1]0x88,%0" : "=r" (tmp) : "r" (src));
	return tmp;
#else
	return (shabal_u32)(((const unsigned char *)src)[0])
		| ((shabal_u32)(((const unsigned char *)src)[1]) << 8)
		| ((shabal_u32)(((const unsigned char *)src)[2]) << 16)
		| ((shabal_u32)(((const unsigned char *)src)[3]) << 24);
#endif
}

/*
 * Encode a 32-bit word into bytes, using little-endian convention.
 * This works regardless of the data alignment. We do not optimize
 * this function since it is used only at the end of the hash
 * computation, to encode the output.
 */
static INLINE void
shabal_enc32le(void *dst, shabal_u32 val)
{
	((unsigned char *)dst)[0] = val;
	((unsigned char *)dst)[1] = val >> 8;
	((unsigned char *)dst)[2] = val >> 16;
	((unsigned char *)dst)[3] = val >> 24;
}

/*
 * We copy the state into local variables, so that the compiler knows
 * that it can optimize them at will.
 */

/* BEGIN -- automatically generated code. */

#if SHABAL_PARAM_P != 3 || SHABAL_PARAM_R != 12
#error Parameters changed; code below is not valid
#endif

#define SHABAL_DECL_STATE   \
	shabal_u32 A00, A01, A02, A03, A04, A05, A06, A07, \
	    A08, A09, A0A, A0B; \
	shabal_u32 B0, B1, B2, B3, B4, B5, B6, B7, \
	    B8, B9, BA, BB, BC, BD, BE, BF; \
	shabal_u32 C0, C1, C2, C3, C4, C5, C6, C7, \
	    C8, C9, CA, CB, CC, CD, CE, CF; \
	shabal_u32 M0, M1, M2, M3, M4, M5, M6, M7, \
	    M8, M9, MA, MB, MC, MD, ME, MF; \
	shabal_u32 Wlow, Whigh;

#define SHABAL_READ_STATE(state)   do { \
		A00 = (state)->A[0]; \
		A01 = (state)->A[1]; \
		A02 = (state)->A[2]; \
		A03 = (state)->A[3]; \
		A04 = (state)->A[4]; \
		A05 = (state)->A[5]; \
		A06 = (state)->A[6]; \
		A07 = (state)->A[7]; \
		A08 = (state)->A[8]; \
		A09 = (state)->A[9]; \
		A0A = (state)->A[10]; \
		A0B = (state)->A[11]; \
		B0 = (state)->B[0]; \
		B1 = (state)->B[1]; \
		B2 = (state)->B[2]; \
		B3 = (state)->B[3]; \
		B4 = (state)->B[4]; \
		B5 = (state)->B[5]; \
		B6 = (state)->B[6]; \
		B7 = (state)->B[7]; \
		B8 = (state)->B[8]; \
		B9 = (state)->B[9]; \
		BA = (state)->B[10]; \
		BB = (state)->B[11]; \
		BC = (state)->B[12]; \
		BD = (state)->B[13]; \
		BE = (state)->B[14]; \
		BF = (state)->B[15]; \
		C0 = (state)->C[0]; \
		C1 = (state)->C[1]; \
		C2 = (state)->C[2]; \
		C3 = (state)->C[3]; \
		C4 = (state)->C[4]; \
		C5 = (state)->C[5]; \
		C6 = (state)->C[6]; \
		C7 = (state)->C[7]; \
		C8 = (state)->C[8]; \
		C9 = (state)->C[9]; \
		CA = (state)->C[10]; \
		CB = (state)->C[11]; \
		CC = (state)->C[12]; \
		CD = (state)->C[13]; \
		CE = (state)->C[14]; \
		CF = (state)->C[15]; \
		Wlow = (state)->Wlow; \
		Whigh = (state)->Whigh; \
	} while (0)

#define SHABAL_WRITE_STATE(state)   do { \
		(state)->A[0] = A00; \
		(state)->A[1] = A01; \
		(state)->A[2] = A02; \
		(state)->A[3] = A03; \
		(state)->A[4] = A04; \
		(state)->A[5] = A05; \
		(state)->A[6] = A06; \
		(state)->A[7] = A07; \
		(state)->A[8] = A08; \
		(state)->A[9] = A09; \
		(state)->A[10] = A0A; \
		(state)->A[11] = A0B; \
		(state)->B[0] = B0; \
		(state)->B[1] = B1; \
		(state)->B[2] = B2; \
		(state)->B[3] = B3; \
		(state)->B[4] = B4; \
		(state)->B[5] = B5; \
		(state)->B[6] = B6; \
		(state)->B[7] = B7; \
		(state)->B[8] = B8; \
		(state)->B[9] = B9; \
		(state)->B[10] = BA; \
		(state)->B[11] = BB; \
		(state)->B[12] = BC; \
		(state)->B[13] = BD; \
		(state)->B[14] = BE; \
		(state)->B[15] = BF; \
		(state)->C[0] = C0; \
		(state)->C[1] = C1; \
		(state)->C[2] = C2; \
		(state)->C[3] = C3; \
		(state)->C[4] = C4; \
		(state)->C[5] = C5; \
		(state)->C[6] = C6; \
		(state)->C[7] = C7; \
		(state)->C[8] = C8; \
		(state)->C[9] = C9; \
		(state)->C[10] = CA; \
		(state)->C[11] = CB; \
		(state)->C[12] = CC; \
		(state)->C[13] = CD; \
		(state)->C[14] = CE; \
		(state)->C[15] = CF; \
		(state)->Wlow = Wlow; \
		(state)->Whigh = Whigh; \
	} while (0)

#define SHABAL_DECODE_BLOCK   do { \
		M0 = shabal_dec32le_aligned(buffer + 0); \
		M1 = shabal_dec32le_aligned(buffer + 4); \
		M2 = shabal_dec32le_aligned(buffer + 8); \
		M3 = shabal_dec32le_aligned(buffer + 12); \
		M4 = shabal_dec32le_aligned(buffer + 16); \
		M5 = shabal_dec32le_aligned(buffer + 20); \
		M6 = shabal_dec32le_aligned(buffer + 24); \
		M7 = shabal_dec32le_aligned(buffer + 28); \
		M8 = shabal_dec32le_aligned(buffer + 32); \
		M9 = shabal_dec32le_aligned(buffer + 36); \
		MA = shabal_dec32le_aligned(buffer + 40); \
		MB = shabal_dec32le_aligned(buffer + 44); \
		MC = shabal_dec32le_aligned(buffer + 48); \
		MD = shabal_dec32le_aligned(buffer + 52); \
		ME = shabal_dec32le_aligned(buffer + 56); \
		MF = shabal_dec32le_aligned(buffer + 60); \
	} while (0)

#define SHABAL_INPUT_BLOCK_ADD   do { \
		B0 = SHABAL_T32(B0 + M0); \
		B1 = SHABAL_T32(B1 + M1); \
		B2 = SHABAL_T32(B2 + M2); \
		B3 = SHABAL_T32(B3 + M3); \
		B4 = SHABAL_T32(B4 + M4); \
		B5 = SHABAL_T32(B5 + M5); \
		B6 = SHABAL_T32(B6 + M6); \
		B7 = SHABAL_T32(B7 + M7); \
		B8 = SHABAL_T32(B8 + M8); \
		B9 = SHABAL_T32(B9 + M9); \
		BA = SHABAL_T32(BA + MA); \
		BB = SHABAL_T32(BB + MB); \
		BC = SHABAL_T32(BC + MC); \
		BD = SHABAL_T32(BD + MD); \
		BE = SHABAL_T32(BE + ME); \
		BF = SHABAL_T32(BF + MF); \
	} while (0)

#define SHABAL_INPUT_BLOCK_SUB   do { \
		C0 = SHABAL_T32(C0 - M0); \
		C1 = SHABAL_T32(C1 - M1); \
		C2 = SHABAL_T32(C2 - M2); \
		C3 = SHABAL_T32(C3 - M3); \
		C4 = SHABAL_T32(C4 - M4); \
		C5 = SHABAL_T32(C5 - M5); \
		C6 = SHABAL_T32(C6 - M6); \
		C7 = SHABAL_T32(C7 - M7); \
		C8 = SHABAL_T32(C8 - M8); \
		C9 = SHABAL_T32(C9 - M9); \
		CA = SHABAL_T32(CA - MA); \
		CB = SHABAL_T32(CB - MB); \
		CC = SHABAL_T32(CC - MC); \
		CD = SHABAL_T32(CD - MD); \
		CE = SHABAL_T32(CE - ME); \
		CF = SHABAL_T32(CF - MF); \
	} while (0)

#define SHABAL_XOR_W   do { \
		A00 ^= Wlow; \
		A01 ^= Whigh; \
	} while (0)

#define SHABAL_SWAP(v1, v2)   do { \
		shabal_u32 tmp = (v1); \
		(v1) = (v2); \
		(v2) = tmp; \
	} while (0)

#define SHABAL_SWAP_BC   do { \
		SHABAL_SWAP(B0, C0); \
		SHABAL_SWAP(B1, C1); \
		SHABAL_SWAP(B2, C2); \
		SHABAL_SWAP(B3, C3); \
		SHABAL_SWAP(B4, C4); \
		SHABAL_SWAP(B5, C5); \
		SHABAL_SWAP(B6, C6); \
		SHABAL_SWAP(B7, C7); \
		SHABAL_SWAP(B8, C8); \
		SHABAL_SWAP(B9, C9); \
		SHABAL_SWAP(BA, CA); \
		SHABAL_SWAP(BB, CB); \
		SHABAL_SWAP(BC, CC); \
		SHABAL_SWAP(BD, CD); \
		SHABAL_SWAP(BE, CE); \
		SHABAL_SWAP(BF, CF); \
	} while (0)

#define SHABAL_PERM_ELT(xa0, xa1, xb0, xb1, xb2, xb3, xc, xm)   do { \
		xa0 = SHABAL_T32((xa0 \
			^ (((xa1 << 15) | (xa1 >> 17)) * 5U) \
			^ xc) * 3U) \
			^ xb1 ^ (xb2 & ~xb3) ^ xm; \
		xb0 = SHABAL_T32(~(((xb0 << 1) | (xb0 >> 31)) ^ xa0)); \
	} while (0)

#define SHABAL_PERM_STEP_0   do { \
		SHABAL_PERM_ELT(A00, A0B, B0, BD, B9, B6, C8, M0); \
		SHABAL_PERM_ELT(A01, A00, B1, BE, BA, B7, C7, M1); \
		SHABAL_PERM_ELT(A02, A01, B2, BF, BB, B8, C6, M2); \
		SHABAL_PERM_ELT(A03, A02, B3, B0, BC, B9, C5, M3); \
		SHABAL_PERM_ELT(A04, A03, B4, B1, BD, BA, C4, M4); \
		SHABAL_PERM_ELT(A05, A04, B5, B2, BE, BB, C3, M5); \
		SHABAL_PERM_ELT(A06, A05, B6, B3, BF, BC, C2, M6); \
		SHABAL_PERM_ELT(A07, A06, B7, B4, B0, BD, C1, M7); \
		SHABAL_PERM_ELT(A08, A07, B8, B5, B1, BE, C0, M8); \
		SHABAL_PERM_ELT(A09, A08, B9, B6, B2, BF, CF, M9); \
		SHABAL_PERM_ELT(A0A, A09, BA, B7, B3, B0, CE, MA); \
		SHABAL_PERM_ELT(A0B, A0A, BB, B8, B4, B1, CD, MB); \
		SHABAL_PERM_ELT(A00, A0B, BC, B9, B5, B2, CC, MC); \
		SHABAL_PERM_ELT(A01, A00, BD, BA, B6, B3, CB, MD); \
		SHABAL_PERM_ELT(A02, A01, BE, BB, B7, B4, CA, ME); \
		SHABAL_PERM_ELT(A03, A02, BF, BC, B8, B5, C9, MF); \
	} while (0)

#define SHABAL_PERM_STEP_1   do { \
		SHABAL_PERM_ELT(A04, A03, B0, BD, B9, B6, C8, M0); \
		SHABAL_PERM_ELT(A05, A04, B1, BE, BA, B7, C7, M1); \
		SHABAL_PERM_ELT(A06, A05, B2, BF, BB, B8, C6, M2); \
		SHABAL_PERM_ELT(A07, A06, B3, B0, BC, B9, C5, M3); \
		SHABAL_PERM_ELT(A08, A07, B4, B1, BD, BA, C4, M4); \
		SHABAL_PERM_ELT(A09, A08, B5, B2, BE, BB, C3, M5); \
		SHABAL_PERM_ELT(A0A, A09, B6, B3, BF, BC, C2, M6); \
		SHABAL_PERM_ELT(A0B, A0A, B7, B4, B0, BD, C1, M7); \
		SHABAL_PERM_ELT(A00, A0B, B8, B5, B1, BE, C0, M8); \
		SHABAL_PERM_ELT(A01, A00, B9, B6, B2, BF, CF, M9); \
		SHABAL_PERM_ELT(A02, A01, BA, B7, B3, B0, CE, MA); \
		SHABAL_PERM_ELT(A03, A02, BB, B8, B4, B1, CD, MB); \
		SHABAL_PERM_ELT(A04, A03, BC, B9, B5, B2, CC, MC); \
		SHABAL_PERM_ELT(A05, A04, BD, BA, B6, B3, CB, MD); \
		SHABAL_PERM_ELT(A06, A05, BE, BB, B7, B4, CA, ME); \
		SHABAL_PERM_ELT(A07, A06, BF, BC, B8, B5, C9, MF); \
	} while (0)

#define SHABAL_PERM_STEP_2   do { \
		SHABAL_PERM_ELT(A08, A07, B0, BD, B9, B6, C8, M0); \
		SHABAL_PERM_ELT(A09, A08, B1, BE, BA, B7, C7, M1); \
		SHABAL_PERM_ELT(A0A, A09, B2, BF, BB, B8, C6, M2); \
		SHABAL_PERM_ELT(A0B, A0A, B3, B0, BC, B9, C5, M3); \
		SHABAL_PERM_ELT(A00, A0B, B4, B1, BD, BA, C4, M4); \
		SHABAL_PERM_ELT(A01, A00, B5, B2, BE, BB, C3, M5); \
		SHABAL_PERM_ELT(A02, A01, B6, B3, BF, BC, C2, M6); \
		SHABAL_PERM_ELT(A03, A02, B7, B4, B0, BD, C1, M7); \
		SHABAL_PERM_ELT(A04, A03, B8, B5, B1, BE, C0, M8); \
		SHABAL_PERM_ELT(A05, A04, B9, B6, B2, BF, CF, M9); \
		SHABAL_PERM_ELT(A06, A05, BA, B7, B3, B0, CE, MA); \
		SHABAL_PERM_ELT(A07, A06, BB, B8, B4, B1, CD, MB); \
		SHABAL_PERM_ELT(A08, A07, BC, B9, B5, B2, CC, MC); \
		SHABAL_PERM_ELT(A09, A08, BD, BA, B6, B3, CB, MD); \
		SHABAL_PERM_ELT(A0A, A09, BE, BB, B7, B4, CA, ME); \
		SHABAL_PERM_ELT(A0B, A0A, BF, BC, B8, B5, C9, MF); \
	} while (0)

#define SHABAL_APPLY_P   do { \
		B0 = SHABAL_T32(B0 << 17) | (B0 >> 15); \
		B1 = SHABAL_T32(B1 << 17) | (B1 >> 15); \
		B2 = SHABAL_T32(B2 << 17) | (B2 >> 15); \
		B3 = SHABAL_T32(B3 << 17) | (B3 >> 15); \
		B4 = SHABAL_T32(B4 << 17) | (B4 >> 15); \
		B5 = SHABAL_T32(B5 << 17) | (B5 >> 15); \
		B6 = SHABAL_T32(B6 << 17) | (B6 >> 15); \
		B7 = SHABAL_T32(B7 << 17) | (B7 >> 15); \
		B8 = SHABAL_T32(B8 << 17) | (B8 >> 15); \
		B9 = SHABAL_T32(B9 << 17) | (B9 >> 15); \
		BA = SHABAL_T32(BA << 17) | (BA >> 15); \
		BB = SHABAL_T32(BB << 17) | (BB >> 15); \
		BC = SHABAL_T32(BC << 17) | (BC >> 15); \
		BD = SHABAL_T32(BD << 17) | (BD >> 15); \
		BE = SHABAL_T32(BE << 17) | (BE >> 15); \
		BF = SHABAL_T32(BF << 17) | (BF >> 15); \
		SHABAL_PERM_STEP_0; \
		SHABAL_PERM_STEP_1; \
		SHABAL_PERM_STEP_2; \
		A0B = SHABAL_T32(A0B + C6); \
		A0A = SHABAL_T32(A0A + C5); \
		A09 = SHABAL_T32(A09 + C4); \
		A08 = SHABAL_T32(A08 + C3); \
		A07 = SHABAL_T32(A07 + C2); \
		A06 = SHABAL_T32(A06 + C1); \
		A05 = SHABAL_T32(A05 + C0); \
		A04 = SHABAL_T32(A04 + CF); \
		A03 = SHABAL_T32(A03 + CE); \
		A02 = SHABAL_T32(A02 + CD); \
		A01 = SHABAL_T32(A01 + CC); \
		A00 = SHABAL_T32(A00 + CB); \
		A0B = SHABAL_T32(A0B + CA); \
		A0A = SHABAL_T32(A0A + C9); \
		A09 = SHABAL_T32(A09 + C8); \
		A08 = SHABAL_T32(A08 + C7); \
		A07 = SHABAL_T32(A07 + C6); \
		A06 = SHABAL_T32(A06 + C5); \
		A05 = SHABAL_T32(A05 + C4); \
		A04 = SHABAL_T32(A04 + C3); \
		A03 = SHABAL_T32(A03 + C2); \
		A02 = SHABAL_T32(A02 + C1); \
		A01 = SHABAL_T32(A01 + C0); \
		A00 = SHABAL_T32(A00 + CF); \
		A0B = SHABAL_T32(A0B + CE); \
		A0A = SHABAL_T32(A0A + CD); \
		A09 = SHABAL_T32(A09 + CC); \
		A08 = SHABAL_T32(A08 + CB); \
		A07 = SHABAL_T32(A07 + CA); \
		A06 = SHABAL_T32(A06 + C9); \
		A05 = SHABAL_T32(A05 + C8); \
		A04 = SHABAL_T32(A04 + C7); \
		A03 = SHABAL_T32(A03 + C6); \
		A02 = SHABAL_T32(A02 + C5); \
		A01 = SHABAL_T32(A01 + C4); \
		A00 = SHABAL_T32(A00 + C3); \
	} while (0)

#define SHABAL_INCR_W   do { \
		if ((Wlow = SHABAL_T32(Wlow + 1)) == 0) \
			Whigh = SHABAL_T32(Whigh + 1); \
	} while (0)

static const shabal_u32 SHABAL_A_init_192[] = {
	SHABAL_C32(0xFD749ED4), SHABAL_C32(0xB798E530), SHABAL_C32(0x33904B6F), SHABAL_C32(0x46BDA85E),
	SHABAL_C32(0x076934B4), SHABAL_C32(0x454B4058), SHABAL_C32(0x77F74527), SHABAL_C32(0xFB4CF465),
	SHABAL_C32(0x62931DA9), SHABAL_C32(0xE778C8DB), SHABAL_C32(0x22B3998E), SHABAL_C32(0xAC15CFB9)
};

static const shabal_u32 SHABAL_B_init_192[] = {
	SHABAL_C32(0x58BCBAC4), SHABAL_C32(0xEC47A08E), SHABAL_C32(0xAEE933B2), SHABAL_C32(0xDFCBC824),
	SHABAL_C32(0xA7944804), SHABAL_C32(0xBF65BDB0), SHABAL_C32(0x5A9D4502), SHABAL_C32(0x59979AF7),
	SHABAL_C32(0xC5CEA54E), SHABAL_C32(0x4B6B8150), SHABAL_C32(0x16E71909), SHABAL_C32(0x7D632319),
	SHABAL_C32(0x930573A0), SHABAL_C32(0xF34C63D1), SHABAL_C32(0xCAF914B4), SHABAL_C32(0xFDD6612C)
};

static const shabal_u32 SHABAL_C_init_192[] = {
	SHABAL_C32(0x61550878), SHABAL_C32(0x89EF2B75), SHABAL_C32(0xA1660C46), SHABAL_C32(0x7EF3855B),
	SHABAL_C32(0x7297B58C), SHABAL_C32(0x1BC67793), SHABAL_C32(0x7FB1C723), SHABAL_C32(0xB66FC640),
	SHABAL_C32(0x1A48B71C), SHABAL_C32(0xF0976D17), SHABAL_C32(0x088CE80A), SHABAL_C32(0xA454EDF3),
	SHABAL_C32(0x1C096BF4), SHABAL_C32(0xAC76224B), SHABAL_C32(0x5215781C), SHABAL_C32(0xCD5D2669)
};

static const shabal_u32 SHABAL_A_init_224[] = {
	SHABAL_C32(0xA5201467), SHABAL_C32(0xA9B8D94A), SHABAL_C32(0xD4CED997), SHABAL_C32(0x68379D7B),
	SHABAL_C32(0xA7FC73BA), SHABAL_C32(0xF1A2546B), SHABAL_C32(0x606782BF), SHABAL_C32(0xE0BCFD0F),
	SHABAL_C32(0x2F25374E), SHABAL_C32(0x069A149F), SHABAL_C32(0x5E2DFF25), SHABAL_C32(0xFAECF061)
};

static const shabal_u32 SHABAL_B_init_224[] = {
	SHABAL_C32(0xEC9905D8), SHABAL_C32(0xF21850CF), SHABAL_C32(0xC0A746C8), SHABAL_C32(0x21DAD498),
	SHABAL_C32(0x35156EEB), SHABAL_C32(0x088C97F2), SHABAL_C32(0x26303E40), SHABAL_C32(0x8A2D4FB5),
	SHABAL_C32(0xFEEE44B6), SHABAL_C32(0x8A1E9573), SHABAL_C32(0x7B81111A), SHABAL_C32(0xCBC139F0),
	SHABAL_C32(0xA3513861), SHABAL_C32(0x1D2C362E), SHABAL_C32(0x918C580E), SHABAL_C32(0xB58E1B9C)
};

static const shabal_u32 SHABAL_C_init_224[] = {
	SHABAL_C32(0xE4B573A1), SHABAL_C32(0x4C1A0880), SHABAL_C32(0x1E907C51), SHABAL_C32(0x04807EFD),
	SHABAL_C32(0x3AD8CDE5), SHABAL_C32(0x16B21302), SHABAL_C32(0x02512C53), SHABAL_C32(0x2204CB18),
	SHABAL_C32(0x99405F2D), SHABAL_C32(0xE5B648A1), SHABAL_C32(0x70AB1D43), SHABAL_C32(0xA10C25C2),
	SHABAL_C32(0x16F1AC05), SHABAL_C32(0x38BBEB56), SHABAL_C32(0x9B01DC60), SHABAL_C32(0xB1096D83)
};

static const shabal_u32 SHABAL_A_init_256[] = {
	SHABAL_C32(0x52F84552), SHABAL_C32(0xE54B7999), SHABAL_C32(0x2D8EE3EC), SHABAL_C32(0xB9645191),
	SHABAL_C32(0xE0078B86), SHABAL_C32(0xBB7C44C9), SHABAL_C32(0xD2B5C1CA), SHABAL_C32(0xB0D2EB8C),
	SHABAL_C32(0x14CE5A45), SHABAL_C32(0x22AF50DC), SHABAL_C32(0xEFFDBC6B), SHABAL_C32(0xEB21B74A)
};

static const shabal_u32 SHABAL_B_init_256[] = {
	SHABAL_C32(0xB555C6EE), SHABAL_C32(0x3E710596), SHABAL_C32(0xA72A652F), SHABAL_C32(0x9301515F),
	SHABAL_C32(0xDA28C1FA), SHABAL_C32(0x696FD868), SHABAL_C32(0x9CB6BF72), SHABAL_C32(0x0AFE4002),
	SHABAL_C32(0xA6E03615), SHABAL_C32(0x5138C1D4), SHABAL_C32(0xBE216306), SHABAL_C32(0xB38B8890),
	SHABAL_C32(0x3EA8B96B), SHABAL_C32(0x3299ACE4), SHABAL_C32(0x30924DD4), SHABAL_C32(0x55CB34A5)
};

static const shabal_u32 SHABAL_C_init_256[] = {
	SHABAL_C32(0xB405F031), SHABAL_C32(0xC4233EBA), SHABAL_C32(0xB3733979), SHABAL_C32(0xC0DD9D55),
	SHABAL_C32(0xC51C28AE), SHABAL_C32(0xA327B8E1), SHABAL_C32(0x56C56167), SHABAL_C32(0xED614433),
	SHABAL_C32(0x88B59D60), SHABAL_C32(0x60E2CEBA), SHABAL_C32(0x758B4B8B), SHABAL_C32(0x83E82A7F),
	SHABAL_C32(0xBC968828), SHABAL_C32(0xE6E00BF7), SHABAL_C32(0xBA839E55), SHABAL_C32(0x9B491C60)
};

static const shabal_u32 SHABAL_A_init_384[] = {
	SHABAL_C32(0xC8FCA331), SHABAL_C32(0xE55C504E), SHABAL_C32(0x003EBF26), SHABAL_C32(0xBB6B8D83),
	SHABAL_C32(0x7B0448C1), SHABAL_C32(0x41B82789), SHABAL_C32(0x0A7C9601), SHABAL_C32(0x8D659CFF),
	SHABAL_C32(0xB6E2673E), SHABAL_C32(0xCA54C77B), SHABAL_C32(0x1460FD7E), SHABAL_C32(0x3FCB8F2D)
};

static const shabal_u32 SHABAL_B_init_384[] = {
	SHABAL_C32(0x527291FC), SHABAL_C32(0x2A16455F), SHABAL_C32(0x78E627E5), SHABAL_C32(0x944F169F),
	SHABAL_C32(0x1CA6F016), SHABAL_C32(0xA854EA25), SHABAL_C32(0x8DB98ABE), SHABAL_C32(0xF2C62641),
	SHABAL_C32(0x30117DCB), SHABAL_C32(0xCF5C4309), SHABAL_C32(0x93711A25), SHABAL_C32(0xF9F671B8),
	SHABAL_C32(0xB01D2116), SHABAL_C32(0x333F4B89), SHABAL_C32(0xB285D165), SHABAL_C32(0x86829B36)
};

static const shabal_u32 SHABAL_C_init_384[] = {
	SHABAL_C32(0xF764B11A), SHABAL_C32(0x76172146), SHABAL_C32(0xCEF6934D), SHABAL_C32(0xC6D28399),
	SHABAL_C32(0xFE095F61), SHABAL_C32(0x5E6018B4), SHABAL_C32(0x5048ECF5), SHABAL_C32(0x51353261),
	SHABAL_C32(0x6E6E36DC), SHABAL_C32(0x63130DAD), SHABAL_C32(0xA9C69BD6), SHABAL_C32(0x1E90EA0C),
	SHABAL_C32(0x7C35073B), SHABAL_C32(0x28D95E6D), SHABAL_C32(0xAA340E0D), SHABAL_C32(0xCB3DEE70)
};

static const shabal_u32 SHABAL_A_init_512[] = {
	SHABAL_C32(0x20728DFD), SHABAL_C32(0x46C0BD53), SHABAL_C32(0xE782B699), SHABAL_C32(0x55304632),
	SHABAL_C32(0x71B4EF90), SHABAL_C32(0x0EA9E82C), SHABAL_C32(0xDBB930F1), SHABAL_C32(0xFAD06B8B),
	SHABAL_C32(0xBE0CAE40), SHABAL_C32(0x8BD14410), SHABAL_C32(0x76D2ADAC), SHABAL_C32(0x28ACAB7F)
};

static const shabal_u32 SHABAL_B_init_512[] = {
	SHABAL_C32(0xC1099CB7), SHABAL_C32(0x07B385F3), SHABAL_C32(0xE7442C26), SHABAL_C32(0xCC8AD640),
	SHABAL_C32(0xEB6F56C7), SHABAL_C32(0x1EA81AA9), SHABAL_C32(0x73B9D314), SHABAL_C32(0x1DE85D08),
	SHABAL_C32(0x48910A5A), SHABAL_C32(0x893B22DB), SHABAL_C32(0xC5A0DF44), SHABAL_C32(0xBBC4324E),
	SHABAL_C32(0x72D2F240), SHABAL_C32(0x75941D99), SHABAL_C32(0x6D8BDE82), SHABAL_C32(0xA1A7502B)
};

static const shabal_u32 SHABAL_C_init_512[] = {
	SHABAL_C32(0xD9BF68D1), SHABAL_C32(0x58BAD750), SHABAL_C32(0x56028CB2), SHABAL_C32(0x8134F359),
	SHABAL_C32(0xB5D469D8), SHABAL_C32(0x941A8CC2), SHABAL_C32(0x418B2A6E), SHABAL_C32(0x04052780),
	SHABAL_C32(0x7F07D787), SHABAL_C32(0x5194358F), SHABAL_C32(0x3C60D665), SHABAL_C32(0xBE97D79A),
	SHABAL_C32(0x950C3434), SHABAL_C32(0xAED9A06D), SHABAL_C32(0x2537DC8D), SHABAL_C32(0x7CDB5969)
};

/* END -- automatically generated code. */

/* see shabal_opt.h */
int
Shabal::Init(int hashbitlen)
{
	/*
	 * We have precomputed initial states for all the supported
	 * output bit lengths.
	 */
	const shabal_u32 *A_init, *B_init, *C_init;

	switch (hashbitlen) {
	case 192:
		A_init = SHABAL_A_init_192;
		B_init = SHABAL_B_init_192;
		C_init = SHABAL_C_init_192;
		break;
	case 224:
		A_init = SHABAL_A_init_224;
		B_init = SHABAL_B_init_224;
		C_init = SHABAL_C_init_224;
		break;
	case 256:
		A_init = SHABAL_A_init_256;
		B_init = SHABAL_B_init_256;
		C_init = SHABAL_C_init_256;
		break;
	case 384:
		A_init = SHABAL_A_init_384;
		B_init = SHABAL_B_init_384;
		C_init = SHABAL_C_init_384;
		break;
	case 512:
		A_init = SHABAL_A_init_512;
		B_init = SHABAL_B_init_512;
		C_init = SHABAL_C_init_512;
		break;
	default:
		return BAD_HASHBITLEN;
	}
	shabalState.hashbitlen = hashbitlen;
	memcpy(shabalState.A, A_init, sizeof shabalState.A);
	memcpy(shabalState.B, B_init, sizeof shabalState.B);
	memcpy(shabalState.C, C_init, sizeof shabalState.C);
	shabalState.Wlow = 1;
	shabalState.Whigh = 0;
	shabalState.buffer_ptr = 0;
	shabalState.last_byte_significant_bits = 0;
	return SUCCESS;
}

/* see shabal_opt.h */
int
Shabal::Update(const BitSequence *data, DataLength databitlen)
{
	/*
	 * This function is quite similar to the Update() function in
	 * the reference code, except that we use our unrolling macros
	 * instead of function calls.
	 */
	unsigned char *buffer;
	size_t len, ptr;
	SHABAL_DECL_STATE

	shabalState.last_byte_significant_bits = (unsigned)databitlen & 0x07;
	len = (size_t)(databitlen >> 3);
	buffer = shabalState.buffer;
	ptr = shabalState.buffer_ptr;

	/*
	 * We do not want to copy the state to local variables if the
	 * amount of data is less than what is needed to complete the
	 * current block. Note that it is anyway suboptimal to call
	 * this method many times for small chunks of data.
	 */
	if (len < (sizeof shabalState.buffer) - ptr) {
		memcpy(buffer + ptr, data, len);
		ptr += len;
		if (shabalState.last_byte_significant_bits != 0)
			buffer[ptr] = data[ptr];
		shabalState.buffer_ptr = ptr;
		return SUCCESS;
	}

	SHABAL_READ_STATE(&shabalState);
	while (len > 0) {
		size_t clen;

		clen = (sizeof shabalState.buffer) - ptr;
		if (clen > len)
			clen = len;
		memcpy(buffer + ptr, data, clen);
		ptr += clen;
		data += clen;
		len -= clen;
		if (ptr == sizeof shabalState.buffer) {
			SHABAL_DECODE_BLOCK;
			SHABAL_INPUT_BLOCK_ADD;
			SHABAL_XOR_W;
			SHABAL_APPLY_P;
			SHABAL_INPUT_BLOCK_SUB;
			SHABAL_SWAP_BC;
			SHABAL_INCR_W;
			ptr = 0;
		}
	}
	SHABAL_WRITE_STATE(&shabalState);
	if (shabalState.last_byte_significant_bits != 0)
		buffer[ptr] = *data;
	shabalState.buffer_ptr = ptr;
	return SUCCESS;
}

/* see shabal_opt.h */
int
Shabal::Final(BitSequence *hashval)
{
	unsigned char *buffer;
	size_t ptr;
	unsigned lbsb;
	int i;
	unsigned char tmp_out[64];
	size_t out_len;
	SHABAL_DECL_STATE

	buffer = shabalState.buffer;
	ptr = shabalState.buffer_ptr;
	lbsb = shabalState.last_byte_significant_bits;
	buffer[ptr] = (buffer[ptr] & ~(0x7F >> lbsb)) | (0x80U >> lbsb);
	memset(buffer + ptr + 1, 0, (sizeof shabalState.buffer) - (ptr + 1));
	SHABAL_READ_STATE(&shabalState);
	SHABAL_DECODE_BLOCK;
	SHABAL_INPUT_BLOCK_ADD;
	SHABAL_XOR_W;
	SHABAL_APPLY_P;
	for (i = 0; i < SHABAL_PARAM_E; i ++) {
		SHABAL_SWAP_BC;
		SHABAL_XOR_W;
		SHABAL_APPLY_P;
	}

	/*
	 * We just use our local variables; no need to go through
	 * the state structure. In order to share some code, we
	 * emit the relevant words into a temporary buffer, which
	 * we finally copy into the destination array.
	 */
	switch (shabalState.hashbitlen >> 5) {
	case 16:
		shabal_enc32le(tmp_out +  0, B0);
		shabal_enc32le(tmp_out +  4, B1);
		shabal_enc32le(tmp_out +  8, B2);
		shabal_enc32le(tmp_out + 12, B3);
		/* fall through */
	case 12:
		shabal_enc32le(tmp_out + 16, B4);
		shabal_enc32le(tmp_out + 20, B5);
		shabal_enc32le(tmp_out + 24, B6);
		shabal_enc32le(tmp_out + 28, B7);
		/* fall through */
	case 8:
		shabal_enc32le(tmp_out + 32, B8);
		/* fall through */
	case 7:
		shabal_enc32le(tmp_out + 36, B9);
		/* fall through */
	case 6:
		shabal_enc32le(tmp_out + 40, BA);
		shabal_enc32le(tmp_out + 44, BB);
		shabal_enc32le(tmp_out + 48, BC);
		shabal_enc32le(tmp_out + 52, BD);
		shabal_enc32le(tmp_out + 56, BE);
		shabal_enc32le(tmp_out + 60, BF);
		break;
	default:
		/*
		 * Not a supported output length: state was altered or
		 * incorrectly initialized.
		 */
		return FAIL;
	}
	out_len = shabalState.hashbitlen >> 3;
	memcpy(hashval, tmp_out + (sizeof tmp_out) - out_len, out_len);
	return SUCCESS;
}

/* see shabal_opt.h */
int
Shabal::Hash(int hashbitlen, const BitSequence *data,
	DataLength databitlen, BitSequence *hashval)
{
	//hashState st;
	int r;

	r = Shabal::Init(hashbitlen);
	if (r != SUCCESS)
		return r;
	r = Shabal::Update(data, databitlen);
	if (r != SUCCESS)
		return r;
	return Shabal::Final(hashval);
}