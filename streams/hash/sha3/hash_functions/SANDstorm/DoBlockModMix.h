/*
 *	Name:  DoBlockModMix.h
 *
 *  Date:  10/30/08
 *
 *  Version:
 *
 *  Abstract:
 *		This file includes optimized macros and definitions to perform the SANDstorm Hash Algorithm
 *
 *  History:
 *
 *  Authors: Sandia National Laboratories
 *		Rich Schroeppel
 *		Tim Draelos
 *		Sean Malone
 *		Andrea Walker
 *		Nathan Dauhtenhan
 *		Sammy Smith
 *		Mark Torgerson
 */

/* High-level hashing macros */
#define SAND_ROT_LEFT64(x,n)		(((x)<<(n)) | ((x)>>(64-(n))))
#define SAND_ROT_LEFT128(x,n)	(((x)<<(n)) | ((x)>>(128-(n))))
#define SAND_BIT_MIX()	dtmp = (SAND_ws[0] ^ SAND_ws[2]) & SAND_J6; SAND_ws[0] ^= dtmp; SAND_ws[2] ^= dtmp; \
					dtmp = (SAND_ws[1] ^ SAND_ws[3]) & SAND_J3; SAND_ws[1] ^= dtmp; SAND_ws[3] ^= dtmp; \
					dtmp = (SAND_ws[0] ^ SAND_ws[1]) & SAND_J5; SAND_ws[0] ^= dtmp; SAND_ws[1] ^= dtmp; \
					dtmp = (SAND_ws[2] ^ SAND_ws[3]) & SAND_J5; SAND_ws[2] ^= dtmp; SAND_ws[3] ^= dtmp

#define SAND_RND1_IN(prev_block)		SAND_ws [0] ^= prev_block[0]; \
								SAND_ws [1] ^= prev_block[1]; \
								SAND_ws [2] ^= prev_block[2]; \
								SAND_ws [3] ^= prev_block[3]
#define SAND_RND_IN(prev_block, msi)	SAND_ws [0] = prev_block[0] ^ SAND_MSd [msi]; \
								SAND_ws [1] = prev_block[1] ^ SAND_MSd [msi + 1]; \
								SAND_ws [2] = prev_block[2] ^ SAND_MSd [msi + 2]; \
								SAND_ws [3] = prev_block[3] ^ SAND_MSd [msi + 3]
#define SAND_RND_OUT(prev_block, current_block)	current_block[0] = prev_block[0] ^ SAND_ws[0]; \
											current_block[1] = prev_block[1] ^ SAND_ws[1]; \
											current_block[2] = prev_block[2] ^ SAND_ws[2]; \
											current_block[3] = prev_block[3] ^ SAND_ws[3]
#define SAND_RND4_OUT(current_block)	current_block[0] = SAND_ws[0]; \
								current_block[1] = SAND_ws[1]; \
								current_block[2] = SAND_ws[2]; \
								current_block[3] = SAND_ws[3]

/* Low-level hashing macros */
#define SAND_S_CH(mi)		(SAND_MSd [mi-3] ^ (SAND_MSd [mi-1] & (SAND_MSd [mi-2] ^ SAND_MSd [mi-3])))
#define SAND_R_CH(wi)		(SAND_ws [(wi+1)&3] ^ (SAND_ws [(wi+3)&3] & (SAND_ws [(wi+2)&3] ^ SAND_ws [(wi+1)&3])))

/* Temporary variable used in low-level macros */
unsigned long long dtmp;

#if SAND_ASM_CODE == 0

#define SAND_F64(ww)		(((unsigned long long)((unsigned int) ww) * (unsigned int) ww) + ((unsigned long long)((unsigned int)(ww >> 32)) * ((unsigned int)(ww >> 32))))
#define SAND_G64(mm)		(SAND_F64(mm) + SAND_ROT_LEFT64((unsigned long long)((unsigned int)((mm >> 32) + SAND_ACONST_256)) * ((unsigned int)(mm + SAND_BCONST_256)), 32))
#define SAND_S_T(mi)		(SAND_MSd [mi-8] + SAND_B [mi-8] + SAND_G64(SAND_MSd [mi-1]) + SAND_S_CH(mi) + SAND_MSd [mi-4])
#define SAND_R_T(r,wi)	(SAND_ws [wi] + SAND_F64(SAND_ws [(wi+3)&3]) + SAND_R_CH(wi) + SAND_B [32-(4*r+wi)-8])

// Message Schedule macro
#define SAND_MSG_SCH(mi)	dtmp = SAND_S_T(mi); \
					SAND_MSd [mi] = SAND_ROT_LEFT64(dtmp ^ sand_fsbox [(unsigned char)dtmp], SAND_MS_ROT_BITS)

// Round Function macro
#define SAND_MOD_MIX_RND(r,wi)	dtmp = SAND_R_T(r, wi); \
							SAND_ws [wi] = SAND_ROT_LEFT64(dtmp ^ sand_fsbox [(unsigned char)dtmp], SAND_R_ROT_BITS)
#else
// Assembly language implementation of SAND_F64 macro
#define Sand_Fw64(fwi)	__asm	{								\
						__asm push	eax						\
						__asm push	edx						\
						__asm mov	eax, DWORD PTR fwi		\
						__asm mul	eax						\
						__asm mov	DWORD PTR dtmp, eax		\
						__asm mov	DWORD PTR dtmp+4, edx	\
						__asm mov	eax, DWORD PTR fwi+4	\
						__asm mul	eax						\
						__asm add	DWORD PTR dtmp, eax		\
						__asm adc	DWORD PTR dtmp+4, edx	\
						__asm pop	edx						\
						__asm pop	eax						\
}
#define Sand_Fm64(fmi)	__asm	{								\
						__asm push	eax						\
						__asm push	edx						\
						__asm mov	eax, DWORD PTR fmi		\
						__asm mul	eax						\
						__asm mov	DWORD PTR dtmp, eax		\
						__asm mov	DWORD PTR dtmp+4, edx	\
						__asm mov	eax, DWORD PTR fmi+4	\
						__asm mul	eax						\
						__asm add	DWORD PTR dtmp, eax		\
						__asm adc	DWORD PTR dtmp+4, edx	\
						__asm pop	edx						\
						__asm pop	eax						\
}
#define SAND_G64(mm)		(dtmp + SAND_ROT_LEFT64((unsigned long long)((unsigned int)((mm >> 32) + SAND_ACONST_256)) * ((unsigned int)(mm + SAND_BCONST_256)), 32))
#define SAND_S_T(mi)		(SAND_MSd [mi-8] + SAND_B [mi-8] + SAND_G64(SAND_MSd [mi-1]) + SAND_S_CH(mi) + SAND_MSd [mi-4])
#define SAND_R_T(r,wi)	(SAND_ws [wi] + SAND_R_CH(wi) + SAND_B [32-(4*r+wi)-8])

// Message Schedule macro
#define SAND_MSG_SCH(mi)	Sand_Fm64(SAND_MSd[(mi-1)*8]); \
					dtmp = SAND_S_T(mi); \
					SAND_MSd [mi] = SAND_ROT_LEFT64(dtmp ^ sand_fsbox [(unsigned char)dtmp], SAND_MS_ROT_BITS)
// Round Function macro
#define SAND_MOD_MIX_RND(r,wi)	Sand_Fw64(SAND_ws [((wi+3)&3)*8]); \
							dtmp += SAND_R_T(r, wi); \
							SAND_ws [wi] = SAND_ROT_LEFT64(dtmp ^ sand_fsbox [(unsigned char)dtmp], SAND_R_ROT_BITS)
#endif
