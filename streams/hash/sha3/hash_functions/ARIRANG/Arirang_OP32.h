///////////////////////////////////////////////////////////////////////////////////////////////////
//
// PROJECT : Arirang family
//
// DATE    : 2008.10.23
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE  : Arirang_OP32.h
//
// NOTES : Optimized code of Arirang family
// 
//         Based on 32-bit platform (with four 32-bit S-box and sixteen 32-bit S-box version)

///////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef _ARIRANG_OP32_H__
#define _ARIRANG_OP32_H__ 100
#include <stdlib.h>
//#include "SHA3api_OP32.h"

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Definition
#define ARIRANG224_BLOCK_LEN	64
#define ARIRANG224_DIGEST_LEN	28

#define ARIRANG256_BLOCK_LEN	64
#define ARIRANG256_DIGEST_LEN	32

#define ARIRANG384_BLOCK_LEN	128
#define ARIRANG384_DIGEST_LEN	48

#define ARIRANG512_BLOCK_LEN	128
#define ARIRANG512_DIGEST_LEN	64

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Structure
typedef struct {

	// Hash length
	int hashbitlen;
	
	// Counter
	unsigned int counter[4];

	// Count
	unsigned int count[4];

	// Message block
	unsigned char block[ARIRANG512_BLOCK_LEN];

	// Working variables
	unsigned int workingvar[16];

	// hash block length
	unsigned int blocklen;

	// bit_length % 7
	unsigned int remainderbit;


} arirangHashState;



///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Macro

// Define the Endianness
#undef ARIRANG_BIG_ENDIAN
#undef ARIRANG_LITTLE_ENDIAN

#if defined(USER_BIG_ENDIAN)
	#define ARIRANG_BIG_ENDIAN
#elif defined(USER_LITTLE_ENDIAN)
	#define ARIRANG_LITTLE_ENDIAN
#else
    #if 0
		#define ARIRANG_BIG_ENDIAN
	#elif defined(_MSC_VER)
		#define ARIRANG_LITTLE_ENDIAN
    #elif defined(__linux__)             // EACIRC: manual edit: these two lines added
        #define ARIRANG_LITTLE_ENDIAN    // edit
    #elif defined(__WIN32__)             // EACIRC: manual edit: these two lines added
        #define ARIRANG_LITTLE_ENDIAN    // edit
	#elif defined(__APPLE__)             // EACIRC: manual edit: these two lines added
        #define ARIRANG_LITTLE_ENDIAN    // edit
	#else
		#error
	#endif
#endif

// Left and rigth rotation
#if defined(_MSC_VER)
	#define ARIRANG_ROTL_DWORD(x, n) _lrotl((x), (n))
	#define ARIRANG_ROTR_DWORD(x, n) _lrotr((x), (n))
#else
	#define ARIRANG_ROTL_DWORD(x, n) ( (unsigned int)((x) << (n)) | (unsigned int)((x) >> (32-(n))) )
	#define ARIRANG_ROTR_DWORD(x, n) ( (unsigned int)((x) >> (n)) | (unsigned int)((x) << (32-(n))) )
#endif

#define ARIRANG_ROTL_DWORD1(x1, x2, n) ( (unsigned int)((x1) << (n)) | (unsigned int)((x2) >> (32-(n))) )
#define ARIRANG_ROTL_DWORD2(x1, x2, n) ( (unsigned int)((x2) << (n)) | (unsigned int)((x1) >> (32-(n))) )

#define ARIRANG_ROTR_DWORD1(x1, x2, n) ( (unsigned int)((x1) >> (n)) | (unsigned int)((x2) << (32-(n))) )
#define ARIRANG_ROTR_DWORD2(x1, x2, n) ( (unsigned int)((x2) >> (n)) | (unsigned int)((x1) << (32-(n))) );

// Reverse the byte order of DWORD and WORD.
#define ARIRANG_ENDIAN_REVERSE_DWORD(dwS)	( (ARIRANG_ROTL_DWORD((dwS),  8) & 0x00ff00ff) | (ARIRANG_ROTL_DWORD((dwS), 24) & 0xff00ff00) )

// Move DWORD type to BYTE type and BYTE type to DWORD type
#if defined(ARIRANG_BIG_ENDIAN)
	#define ARIRANG_BIG_B2D(B, D)		D = *(unsigned int *)(B)
	#define ARIRANG_BIG_D2B(D, B)		*(unsigned int *)(B) = (unsigned int)(D)
	#define ARIRANG_LITTLE_B2D(B, D)	D = ARIRANG_ENDIAN_REVERSE_DWORD(*(unsigned int *)(B))
	#define ARIRANG_LITTLE_D2B(D, B)	*(unsigned int *)(B) = ARIRANG_ENDIAN_REVERSE_DWORD(D)
#elif defined(ARIRANG_LITTLE_ENDIAN)
	#define ARIRANG_BIG_B2D(B, D)		D = ARIRANG_ENDIAN_REVERSE_DWORD(*(unsigned int *)(B))
	#define ARIRANG_BIG_D2B(D, B)		*(unsigned int *)(B) = ARIRANG_ENDIAN_REVERSE_DWORD(D)
	#define ARIRANG_LITTLE_B2D(B, D)	D = *(unsigned int *)(B)
	#define ARIRANG_LITTLE_D2B(D, B)	*(unsigned int *)(B) = (unsigned int)(D)
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Function

void arirang_gen_tabs(void);
void Arirang_Compression256(arirangHashState *state, const int numberOfRounds);
void Arirang_Compression512(arirangHashState *state, const int numberOfRounds);


#endif
