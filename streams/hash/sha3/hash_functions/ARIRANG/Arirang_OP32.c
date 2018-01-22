///////////////////////////////////////////////////////////////////////////////////////////////////
//
// PROJECT : ARIRANG family(ARIRANG-224,ARIRANG-256,ARIRANG-384,ARIRANG-512)
//
// DATE    : 2008.10.23
//
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FILE  : ARIRANG_OP32.c
//
// NOTES : Optimized code of ARIRANG family
// 
//         Based on 32-bit platform (with four 32-bit S-box and sixteen 32-bit S-box version)
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <memory.h>
#include "Arirang_OP32.h"


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Macro

#undef SS
#undef SS1
#undef SS2

#define SS(x,n)				ARIRANG_ROTL_DWORD(x,n)
#define SS1(x1,x2,n)		ARIRANG_ROTL_DWORD1(x1,x2,n)
#define SS2(x1,x2,n)		ARIRANG_ROTL_DWORD2(x1,x2,n)


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Constant

static const unsigned int arirang_K256[16] = {0x517cc1b7, 0x76517cc1, 0xbd76517c, 0x2dbd7651,
							   0x272dbd76, 0xcb272dbd, 0x90cb272d, 0x0a90cb27,
							   0xec0a90cb, 0x5bec0a90, 0x9a5bec0a, 0xe69a5bec,
							   0xb7e69a5b, 0xc1b7e69a, 0x7cc1b7e6, 0x517cc1b7};

static const unsigned int arirang_K512[16][2] = {0x517cc1b7, 0x27220a94, 0x2db6517c, 0xc1b72722, 0xe6952db6, 0x517cc1b7, 0x90cbe695, 0x2db6517c, 
								  0x7cca90cb, 0xe6952db6, 0xcb237cca, 0x90cbe695, 0x765ecb23, 0x7cca90cb, 0xec01765e, 0xcb237cca, 
								  0xb7e9ec01, 0x765ecb23, 0xbd7db7e9, 0xec01765e, 0x9a5fbd7d, 0xb7e9ec01, 0x5be89a5f, 0xbd7db7e9, 
								  0x0a945be8, 0x9a5fbd7d, 0x27220a94, 0x5be89a5f, 0xc1b72722, 0x0a945be8, 0x517cc1b7, 0x27220a94};


unsigned int	arirang_MDS4[4][256];
unsigned int	arirang_MDS8[8][256][2];
unsigned char	arirang_sbx[256];

#define ff_mult(a, b)	(a && b ? pow_tab[(log_tab[a] + log_tab[b]) % 255] : 0)
#define byte(x, n)		((unsigned char)((x) >> (8 * n)))

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME  : arirang_gen_tabs
//
// DESCRIPTION    : Generate the s-box table and field multiplication table
//
// PARAMETERS     : void
//                  
//
// RETURNS        : void
//
///////////////////////////////////////////////////////////////////////////////////////////////////
void arirang_gen_tabs(void)
{   
	#ifndef __ARIRANG_GEN_TABS__
	#define __ARIRANG_GEN_TABS__ 100
	unsigned int i, t[2];
    unsigned char p, q;
	unsigned char	pow_tab[256];
	unsigned char	log_tab[256];
	unsigned int temp2, temp4, temp8;
 
	/* log and power tables for GF(2^8) finite field with  */
    /* 0x011b as modular polynomial - the simplest prmitive */
    /* root is 0x03, used here to generate the tables       */

    for (i = 0, p = 1; i < 256; ++i)
    {
        pow_tab[i] = (unsigned char)p;
		log_tab[p] = (unsigned char)i;

        p = p ^ (p << 1) ^ (p & 0x80 ? 0x01b : 0);
    }

    /* note that the affine byte transformation matrix in   */
    /* ARIRANG specification is in big endian format with  */
    /* bit 0 as the most significant bit. In the remainder  */
    /* of the specification the bits are numbered from the  */
    /* least significant end of a byte.                     */

    for (i = 0; i < 256; ++i)
    {   
        p = (i ? pow_tab[255 - log_tab[i]] : 0); q = p; 
        q = (q >> 7) | (q << 1); p ^= q; 
        q = (q >> 7) | (q << 1); p ^= q; 
        q = (q >> 7) | (q << 1); p ^= q; 
        q = (q >> 7) | (q << 1); p ^= q ^ 0x63; 
        arirang_sbx[i] = p;
    }

    for (i = 0; i < 256; ++i)
    {
        p = arirang_sbx[i];

        t[0] = ((unsigned int)ff_mult(2, p)) | ((unsigned int)p <<  8) | ((unsigned int)p << 16) | ((unsigned int)ff_mult(3, p) << 24);
        
        arirang_MDS4[0][i] = t[0];			arirang_MDS4[1][i] = SS(t[0],  8);	arirang_MDS4[2][i] = SS(t[0], 16);	arirang_MDS4[3][i] = SS(t[0], 24);
       		
		temp2=ff_mult(2,p);
		temp4=ff_mult(2,temp2);
		temp8=ff_mult(2,temp4);

		t[0] = ((unsigned int)temp8) | ((unsigned int)(temp8^p) <<  8) | ((unsigned int)(temp8^temp2) << 16) | ((unsigned int)(temp2) << 24);
        t[1] = ((unsigned int)p) | ((unsigned int)p <<  8) | ((unsigned int)temp4 << 16) | ((unsigned int)p << 24);
		        
        arirang_MDS8[0][i][1] = t[0];				arirang_MDS8[0][i][0] = t[1];
		arirang_MDS8[1][i][1] = SS1(t[0],t[1],  8);	arirang_MDS8[1][i][0] = SS2(t[0],t[1],  8);
		arirang_MDS8[2][i][1] = SS1(t[0],t[1], 16);	arirang_MDS8[2][i][0] = SS2(t[0],t[1], 16);	
		arirang_MDS8[3][i][1] = SS1(t[0],t[1], 24);	arirang_MDS8[3][i][0] = SS2(t[0],t[1], 24);
		arirang_MDS8[4][i][1] = t[1];				arirang_MDS8[4][i][0] = t[0];
		arirang_MDS8[5][i][1] = SS2(t[0],t[1],  8);	arirang_MDS8[5][i][0] = SS1(t[0],t[1],  8);
		arirang_MDS8[6][i][1] = SS2(t[0],t[1], 16);	arirang_MDS8[6][i][0] = SS1(t[0],t[1], 16);
		arirang_MDS8[7][i][1] = SS2(t[0],t[1], 24);	arirang_MDS8[7][i][0] = SS1(t[0],t[1], 24);
	}
	#endif
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME  : step256
//
// DESCRIPTION    : Step function of ARIRANG-224 and ARIRANG-256
//
// PARAMETERS     : A,B,C,D,E,F,G,H  - working variables
//                  M1,M2 - message block
//
// RETURNS        : void
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define Step256(A, B, C, D, E, F, G, H, M1, M2)					\
	A ^= M1;													\
	E ^= M2;													\
	temp1 =  arirang_MDS4[0][byte(A,0)] ^ arirang_MDS4[1][byte(A,1)] ^			\
			 arirang_MDS4[2][byte(A,2)] ^ arirang_MDS4[3][byte(A,3)] ;			\
	temp2 =  arirang_MDS4[0][byte(E,0)] ^ arirang_MDS4[1][byte(E,1)] ^			\
			 arirang_MDS4[2][byte(E,2)] ^ arirang_MDS4[3][byte(E,3)] ;			\
	B ^= temp1;													\
	C ^= SS(temp1, 13);											\
	D ^= SS(temp1, 23);											\
	F ^= temp2;													\
	G ^= SS(temp2, 29);											\
	H ^= SS(temp2, 7);											\

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME  : step512
//
// DESCRIPTION    : Step function of ARIRANG-384 and Arirang-512
//
// PARAMETERS     : A,B,C,D,E,F,G,H  - working variables
//                  M1,M2 - message block
//
// RETURNS        : void
//
///////////////////////////////////////////////////////////////////////////////////////////////////
#define Step512(A1, A2, B1, B2, C1, C2, D1, D2, E1, E2, F1, F2, G1, G2, H1, H2, M11, M12, M21, M22){	\
	A1 ^= M11;		A2 ^= M12;																			\
	E1 ^= M21;		E2 ^= M22;																			\
	temp[0] =  arirang_MDS8[0][byte(A2,0)][1] ^ arirang_MDS8[1][byte(A2,1)][1] ^										\
			   arirang_MDS8[2][byte(A2,2)][1] ^ arirang_MDS8[3][byte(A2,3)][1] ^										\
			   arirang_MDS8[4][byte(A1,0)][1] ^ arirang_MDS8[5][byte(A1,1)][1] ^										\
			   arirang_MDS8[6][byte(A1,2)][1] ^ arirang_MDS8[7][byte(A1,3)][1] ;										\
	temp[1] =  arirang_MDS8[0][byte(A2,0)][0] ^ arirang_MDS8[1][byte(A2,1)][0] ^										\
			   arirang_MDS8[2][byte(A2,2)][0] ^ arirang_MDS8[3][byte(A2,3)][0] ^										\
			   arirang_MDS8[4][byte(A1,0)][0] ^ arirang_MDS8[5][byte(A1,1)][0] ^										\
			   arirang_MDS8[6][byte(A1,2)][0] ^ arirang_MDS8[7][byte(A1,3)][0] ;										\
	temp[2] =  arirang_MDS8[0][byte(E2,0)][1] ^ arirang_MDS8[1][byte(E2,1)][1] ^										\
			   arirang_MDS8[2][byte(E2,2)][1] ^ arirang_MDS8[3][byte(E2,3)][1] ^										\
			   arirang_MDS8[4][byte(E1,0)][1] ^ arirang_MDS8[5][byte(E1,1)][1] ^										\
			   arirang_MDS8[6][byte(E1,2)][1] ^ arirang_MDS8[7][byte(E1,3)][1] ;										\
	temp[3] =  arirang_MDS8[0][byte(E2,0)][0] ^ arirang_MDS8[1][byte(E2,1)][0] ^										\
			   arirang_MDS8[2][byte(E2,2)][0] ^ arirang_MDS8[3][byte(E2,3)][0] ^										\
			   arirang_MDS8[4][byte(E1,0)][0] ^ arirang_MDS8[5][byte(E1,1)][0] ^										\
			   arirang_MDS8[6][byte(E1,2)][0] ^ arirang_MDS8[7][byte(E1,3)][0] ;										\
	B1 ^= temp[0];					B2 ^= temp[1];														\
	C1 ^= SS1(temp[0],temp[1], 29);	C2 ^= SS2(temp[0],temp[1], 29);										\
	D1 ^= SS2(temp[0],temp[1],  9);	D2 ^= SS1(temp[0],temp[1],  9);										\
	F1 ^= temp[2];					F2 ^= temp[3];														\
	G1 ^= SS2(temp[2],temp[3], 21);	G2 ^= SS1(temp[2],temp[3], 21);										\
	H1 ^= SS1(temp[2],temp[3], 13);	H2 ^= SS2(temp[2],temp[3], 13);										\
}



#define COUNT256(){												\
	      state->counter[0]++; if(state->counter[0]!=0x00);		\
	else {state->counter[1]++;}}								\

#define COUNT512(){												\
	      state->counter[0]++; if(state->counter[0]!=0x00);		\
	else {state->counter[1]++; if(state->counter[1]!=0x00);		\
	else {state->counter[2]++; if(state->counter[2]!=0x00);		\
	else {state->counter[3]++; }}}}								\


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME  : Arirang_Compression256
//
// DESCRIPTION    : Counter addition and compression function of Arirang-224 and ARIRANG-256
//
// PARAMETERS     : state - a structure that holds the hashState information
//
// RETURNS        : void
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void Arirang_Compression256(arirangHashState *state, const int rounds)
{
	unsigned int	R[8], W[32], temp1, temp2;
	int i;

	#if defined(ARIRANG_BIG_ENDIAN)
		#define GetData(x) x
	#else
		#define GetData(x) ARIRANG_ENDIAN_REVERSE_DWORD(x)
	#endif

	// Message Scheduling
	for (i = 0; i < 16; i++)
		W[i] = GetData(((unsigned int*)state->block)[i]);

	W[16] = SS((W[ 9] ^ W[11] ^ W[13] ^ W[15] ^ arirang_K256[ 0]),  5);
	W[17] = SS((W[ 8] ^ W[10] ^ W[12] ^ W[14] ^ arirang_K256[ 1]), 11);
	W[18] = SS((W[ 1] ^ W[ 3] ^ W[ 5] ^ W[ 7] ^ arirang_K256[ 2]), 19);
	W[19] = SS((W[ 0] ^ W[ 2] ^ W[ 4] ^ W[ 6] ^ arirang_K256[ 3]), 31);

	W[20] = SS((W[14] ^ W[ 4] ^ W[10] ^ W[ 0] ^ arirang_K256[ 4]),  5);
	W[21] = SS((W[11] ^ W[ 1] ^ W[ 7] ^ W[13] ^ arirang_K256[ 5]), 11);
	W[22] = SS((W[ 6] ^ W[12] ^ W[ 2] ^ W[ 8] ^ arirang_K256[ 6]), 19);
	W[23] = SS((W[ 3] ^ W[ 9] ^ W[15] ^ W[ 5] ^ arirang_K256[ 7]), 31);

	W[24] = SS((W[13] ^ W[15] ^ W[ 1] ^ W[ 3] ^ arirang_K256[ 8]),  5);
	W[25] = SS((W[ 4] ^ W[ 6] ^ W[ 8] ^ W[10] ^ arirang_K256[ 9]), 11);
	W[26] = SS((W[ 5] ^ W[ 7] ^ W[ 9] ^ W[11] ^ arirang_K256[10]), 19);
	W[27] = SS((W[12] ^ W[14] ^ W[ 0] ^ W[ 2] ^ arirang_K256[11]), 31);

	W[28] = SS((W[10] ^ W[ 0] ^ W[ 6] ^ W[12] ^ arirang_K256[12]),  5);
	W[29] = SS((W[15] ^ W[ 5] ^ W[11] ^ W[ 1] ^ arirang_K256[13]), 11);
	W[30] = SS((W[ 2] ^ W[ 8] ^ W[14] ^ W[ 4] ^ arirang_K256[14]), 19);
	W[31] = SS((W[ 7] ^ W[13] ^ W[ 3] ^ W[ 9] ^ arirang_K256[15]), 31);

	// Counter Addition
	state->workingvar[0] ^= state->counter[1];
	state->workingvar[4] ^= state->counter[0];

	// Initialize Register
	R[0] = state->workingvar[0];
	R[1] = state->workingvar[1];
	R[2] = state->workingvar[2];
	R[3] = state->workingvar[3];
	R[4] = state->workingvar[4];
	R[5] = state->workingvar[5];
	R[6] = state->workingvar[6];
	R[7] = state->workingvar[7];
	
	//NASTAVENIE RUND + DALSIE DOLE:
	if (rounds >= 1) {
	// 1 Round
	Step256(R[0], R[1], R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7], W[16], W[17]);
	Step256(R[7], R[0], R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6], W[ 0], W[ 1]);
	Step256(R[6], R[7], R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5], W[ 2], W[ 3]);
	Step256(R[5], R[6], R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4], W[ 4], W[ 5]);
	Step256(R[4], R[5], R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3], W[ 6], W[ 7]);

	Step256(R[3], R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2], W[18], W[19]);
	Step256(R[2], R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1], W[ 8], W[ 9]);
	Step256(R[1], R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0], W[10], W[11]);
	Step256(R[0], R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7], W[12], W[13]);
	Step256(R[7], R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6], W[14], W[15]);
	
	}

	if (rounds >= 2) {
	// 2 Round
	Step256(R[6], R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5], W[20], W[21]);
	Step256(R[5], R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4], W[ 3], W[ 6]);
	Step256(R[4], R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3], W[ 9], W[12]);
	Step256(R[3], R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2], W[15], W[ 2]);
	Step256(R[2], R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1], W[ 5], W[ 8]);

	Step256(R[1], R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0], W[22], W[23]);
	Step256(R[0], R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7], W[11], W[14]);
	Step256(R[7], R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6], W[ 1], W[ 4]);
	Step256(R[6], R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5], W[ 7], W[10]);
	Step256(R[5], R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4], W[13], W[ 0]);

	// Feedforward_1
	R[0] ^= state->workingvar[4];
	R[1] ^= state->workingvar[5];
	R[2] ^= state->workingvar[6];
	R[3] ^= state->workingvar[7];
	R[4] ^= state->workingvar[0];
	R[5] ^= state->workingvar[1];
	R[6] ^= state->workingvar[2];
	R[7] ^= state->workingvar[3];

	}

	if (rounds >= 3) {
	// 3 Round
	Step256(R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3], W[24], W[25]);
	Step256(R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2], W[12], W[ 5]);
	Step256(R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1], W[14], W[ 7]);
	Step256(R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0], W[ 0], W[ 9]);
	Step256(R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7], W[ 2], W[11]);

	Step256(R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6], W[26], W[27]);
	Step256(R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5], W[ 4], W[13]);
	Step256(R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4], W[ 6], W[15]);
	Step256(R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3], W[ 8], W[ 1]);
	Step256(R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2], W[10], W[ 3]);

	}

	if (rounds >= 4) {
	// 4 Round
	Step256(R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1], W[28], W[29]);
	Step256(R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0], W[ 7], W[ 2]);
	Step256(R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7], W[13], W[ 8]);
	Step256(R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6], W[ 3], W[14]);
	Step256(R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4] ,R[5], W[ 9], W[ 4]);

	Step256(R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3] ,R[4], W[30], W[31]);
	Step256(R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2] ,R[3], W[15], W[10]);
	Step256(R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1] ,R[2], W[ 5], W[ 0]);
	Step256(R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0] ,R[1], W[11], W[ 6]);
	Step256(R[1] ,R[2] ,R[3] ,R[4] ,R[5] ,R[6] ,R[7] ,R[0], W[ 1], W[12]);

	// Feedforward_2
	state->workingvar[0] ^= R[0];
	state->workingvar[1] ^= R[1];
	state->workingvar[2] ^= R[2];
	state->workingvar[3] ^= R[3];
	state->workingvar[4] ^= R[4];
	state->workingvar[5] ^= R[5];
	state->workingvar[6] ^= R[6];
	state->workingvar[7] ^= R[7];

	}

	COUNT256();
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION NAME  : Arirang_Compression512
//
// DESCRIPTION    : Counter addition and compression function of Arirang-384 and ARIRANG-512
//
// PARAMETERS     : state - a structure that holds the hashState information
//
// RETURNS        : void
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void Arirang_Compression512(arirangHashState *state, const int rounds)
{
	unsigned int	R[16], W[64], temp[4];
	int i;

	#if defined(ARIRANG_BIG_ENDIAN)
		#define GetData(x) x
	#else
		#define GetData(x) ARIRANG_ENDIAN_REVERSE_DWORD(x)
	#endif

	// Message Scheduling
	for (i = 0; i < 32; i++)
		W[i] = GetData(((unsigned int*)state->block)[i]);
									
	temp[0]=(W[18]^W[22]^W[26]^W[30]^arirang_K512[ 0][0]);	temp[1]=(W[19]^W[23]^W[27]^W[31]^arirang_K512[ 0][1]);
	W[32]=SS1(temp[0], temp[1], 11);				W[33]=SS2(temp[0], temp[1], 11);							
	temp[0]=(W[16]^W[20]^W[24]^W[28]^arirang_K512[ 1][0]);  temp[1]=(W[17]^W[21]^W[25]^W[29]^arirang_K512[ 1][1]);	
	W[34]=SS1(temp[0], temp[1], 23);				W[35]=SS2(temp[0], temp[1], 23);							
	temp[0]=(W[ 2]^W[ 6]^W[10]^W[14]^arirang_K512[ 2][0]);	temp[1]=(W[ 3]^W[ 7]^W[11]^W[15]^arirang_K512[ 2][1]);
	W[36]=SS2(temp[0], temp[1], 5);					W[37]=SS1(temp[0], temp[1], 5);								
	temp[0]=(W[ 0]^W[ 4]^W[ 8]^W[12]^arirang_K512[ 3][0]);	temp[1]=(W[ 1]^W[ 5]^W[ 9]^W[13]^arirang_K512[ 3][1]);
	W[38]=SS2(temp[0], temp[1], 27);				W[39]=SS1(temp[0], temp[1], 27);							
	temp[0]=(W[28]^W[ 8]^W[20]^W[ 0]^arirang_K512[ 4][0]);	temp[1]=(W[29]^W[ 9]^W[21]^W[ 1]^arirang_K512[ 4][1]);
	W[40]=SS1(temp[0], temp[1], 11);				W[41]=SS2(temp[0], temp[1], 11);							
	temp[0]=(W[22]^W[ 2]^W[14]^W[26]^arirang_K512[ 5][0]);	temp[1]=(W[23]^W[ 3]^W[15]^W[27]^arirang_K512[ 5][1]);
	W[42]=SS1(temp[0], temp[1], 23);				W[43]=SS2(temp[0], temp[1], 23);							
	temp[0]=(W[12]^W[24]^W[ 4]^W[16]^arirang_K512[ 6][0]);	temp[1]=(W[13]^W[25]^W[ 5]^W[17]^arirang_K512[ 6][1]);
	W[44]=SS2(temp[0], temp[1], 5);					W[45]=SS1(temp[0], temp[1], 5);								
	temp[0]=(W[ 6]^W[18]^W[30]^W[10]^arirang_K512[ 7][0]);	temp[1]=(W[ 7]^W[19]^W[31]^W[11]^arirang_K512[ 7][1]);
	W[46]=SS2(temp[0], temp[1], 27);				W[47]=SS1(temp[0], temp[1], 27);							
	temp[0]=(W[26]^W[30]^W[ 2]^W[ 6]^arirang_K512[ 8][0]);	temp[1]=(W[27]^W[31]^W[ 3]^W[ 7]^arirang_K512[ 8][1]);
	W[48]=SS1(temp[0], temp[1], 11);				W[49]=SS2(temp[0], temp[1], 11);							
	temp[0]=(W[ 8]^W[12]^W[16]^W[20]^arirang_K512[ 9][0]);	temp[1]=(W[ 9]^W[13]^W[17]^W[21]^arirang_K512[ 9][1]);
	W[50]=SS1(temp[0], temp[1], 23);				W[51]=SS2(temp[0], temp[1], 23);							
	temp[0]=(W[10]^W[14]^W[18]^W[22]^arirang_K512[10][0]);	temp[1]=(W[11]^W[15]^W[19]^W[23]^arirang_K512[10][1]);
	W[52]=SS2(temp[0], temp[1], 5);					W[53]=SS1(temp[0], temp[1], 5);								
	temp[0]=(W[24]^W[28]^W[ 0]^W[ 4]^arirang_K512[11][0]);	temp[1]=(W[25]^W[29]^W[ 1]^W[ 5]^arirang_K512[11][1]);
	W[54]=SS2(temp[0], temp[1], 27);				W[55]=SS1(temp[0], temp[1], 27);							
	temp[0]=(W[20]^W[ 0]^W[12]^W[24]^arirang_K512[12][0]);	temp[1]=(W[21]^W[ 1]^W[13]^W[25]^arirang_K512[12][1]);
	W[56]=SS1(temp[0], temp[1], 11);				W[57]=SS2(temp[0], temp[1], 11);							
	temp[0]=(W[30]^W[10]^W[22]^W[ 2]^arirang_K512[13][0]);	temp[1]=(W[31]^W[11]^W[23]^W[ 3]^arirang_K512[13][1]);
	W[58]=SS1(temp[0], temp[1], 23);				W[59]=SS2(temp[0], temp[1], 23);							
	temp[0]=(W[ 4]^W[16]^W[28]^W[ 8]^arirang_K512[14][0]);	temp[1]=(W[ 5]^W[17]^W[29]^W[ 9]^arirang_K512[14][1]);
	W[60]=SS2(temp[0], temp[1], 5);					W[61]=SS1(temp[0], temp[1], 5);								
	temp[0]=(W[14]^W[26]^W[ 6]^W[18]^arirang_K512[15][0]);	temp[1]=(W[15]^W[27]^W[ 7]^W[19]^arirang_K512[15][1]);
	W[62]=SS2(temp[0], temp[1], 27);				W[63]=SS1(temp[0], temp[1], 27);																														

	// Counter Addition
	state->workingvar[0] ^= state->counter[3];
	state->workingvar[1] ^= state->counter[2];
	state->workingvar[8] ^= state->counter[1];
	state->workingvar[9] ^= state->counter[0];

	// Initialize Register
	R[ 0] = state->workingvar[ 0];	R[ 1] = state->workingvar[ 1];
	R[ 2] = state->workingvar[ 2];	R[ 3] = state->workingvar[ 3];
	R[ 4] = state->workingvar[ 4];	R[ 5] = state->workingvar[ 5];
	R[ 6] = state->workingvar[ 6];	R[ 7] = state->workingvar[ 7];
	R[ 8] = state->workingvar[ 8];	R[ 9] = state->workingvar[ 9];
	R[10] = state->workingvar[10];	R[11] = state->workingvar[11];
	R[12] = state->workingvar[12];	R[13] = state->workingvar[13];
	R[14] = state->workingvar[14];	R[15] = state->workingvar[15];
	
	//NASTAVENIE RUND:
	if (rounds >= 1) {
	// 1 Round
	Step512(R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], W[32], W[33], W[34], W[35]);         
	Step512(R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], W[ 0], W[ 1], W[ 2], W[ 3]);
	Step512(R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], W[ 4], W[ 5], W[ 6], W[ 7]);
	Step512(R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], W[ 8], W[ 9], W[10], W[11]);
	Step512(R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], W[12], W[13], W[14], W[15]);
	Step512(R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], W[36], W[37], W[38], W[39]);
	Step512(R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], W[16], W[17], W[18], W[19]);
	Step512(R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], W[20], W[21], W[22], W[23]);
	Step512(R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], W[24], W[25], W[26], W[27]);
	Step512(R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], W[28], W[29], W[30], W[31]);
    
	}

	if (rounds >= 2) {
	// 2 round
	Step512(R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], W[40], W[41], W[42], W[43]);
	Step512(R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], W[ 6], W[ 7], W[12], W[13]);
	Step512(R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], W[18], W[19], W[24], W[25]);
	Step512(R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], W[30], W[31], W[ 4], W[ 5]);
	Step512(R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], W[10], W[11], W[16], W[17]);
	Step512(R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], W[44], W[45], W[46], W[47]);
	Step512(R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], W[22], W[23], W[28], W[29]);
	Step512(R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], W[ 2], W[ 3], W[ 8], W[ 9]);
	Step512(R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], W[14], W[15], W[20], W[21]);
	Step512(R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], W[26], W[27], W[ 0], W[ 1]);
	


	// Feedforward_1
	R[ 0] ^= state->workingvar[ 8];	R[ 1] ^= state->workingvar[ 9];
	R[ 2] ^= state->workingvar[10];	R[ 3] ^= state->workingvar[11];
	R[ 4] ^= state->workingvar[12];	R[ 5] ^= state->workingvar[13];
	R[ 6] ^= state->workingvar[14];	R[ 7] ^= state->workingvar[15];
	R[ 8] ^= state->workingvar[ 0];	R[ 9] ^= state->workingvar[ 1];
	R[10] ^= state->workingvar[ 2];	R[11] ^= state->workingvar[ 3];
	R[12] ^= state->workingvar[ 4];	R[13] ^= state->workingvar[ 5];
	R[14] ^= state->workingvar[ 6];	R[15] ^= state->workingvar[ 7];

	}

	if (rounds >= 3) {
	// 3 Round
	Step512(R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], W[48], W[49], W[50], W[51]);
	Step512(R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], W[24], W[25], W[10], W[11]);
	Step512(R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], W[28], W[29], W[14], W[15]);
	Step512(R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], W[ 0], W[ 1], W[18], W[19]);
	Step512(R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], W[ 4], W[ 5], W[22], W[23]);
	Step512(R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], W[52], W[53], W[54], W[55]);
	Step512(R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], W[ 8], W[ 9], W[26], W[27]);
	Step512(R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], W[12], W[13], W[30], W[31]);
	Step512(R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], W[16], W[17], W[ 2], W[ 3]);
	Step512(R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], W[20], W[21], W[ 6], W[ 7]);
    
	}

	if (rounds >= 4) {
	// 4 round
	Step512(R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], W[56], W[57], W[58], W[59]);
	Step512(R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], W[14], W[15], W[ 4], W[ 5]);
	Step512(R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], W[26], W[27], W[16], W[17]);
	Step512(R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], W[ 6], W[ 7], W[28], W[29]);
	Step512(R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], W[18], W[19], W[ 8], W[ 9]);
	Step512(R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], W[60], W[61], W[62], W[63]);
	Step512(R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], W[30], W[31], W[20], W[21]);
	Step512(R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], R[ 4], R[ 5], W[10], W[11], W[ 0], W[ 1]);
	Step512(R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], R[ 2], R[ 3], W[22], W[23], W[12], W[13]);
	Step512(R[ 2], R[ 3], R[ 4], R[ 5], R[ 6], R[ 7], R[ 8], R[ 9], R[10], R[11], R[12], R[13], R[14], R[15], R[ 0], R[ 1], W[ 2], W[ 3], W[24], W[25]);
	

	// Feedforward_2
	state->workingvar[ 0] ^= R[ 0];	state->workingvar[ 1] ^= R[ 1];
	state->workingvar[ 2] ^= R[ 2];	state->workingvar[ 3] ^= R[ 3];
	state->workingvar[ 4] ^= R[ 4];	state->workingvar[ 5] ^= R[ 5];
	state->workingvar[ 6] ^= R[ 6];	state->workingvar[ 7] ^= R[ 7];
	state->workingvar[ 8] ^= R[ 8];	state->workingvar[ 9] ^= R[ 9];
	state->workingvar[10] ^= R[10];	state->workingvar[11] ^= R[11];
	state->workingvar[12] ^= R[12];	state->workingvar[13] ^= R[13];
	state->workingvar[14] ^= R[14];	state->workingvar[15] ^= R[15];

	}
	
	COUNT512();														
}
