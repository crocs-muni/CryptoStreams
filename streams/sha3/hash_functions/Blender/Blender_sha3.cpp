#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

#include "Blender_sha3.h"

//char *Author = "Colin Bradbury";
//char *Algorithm = "Blender";

	// hash engine codes
typedef enum { Vgen=1,			// general case - no optimised routine available
		V96, V112, V128, V144, V160,		// 16-bit words
		//V168, V216, V240,					// 24-bit words
		V192, V224, V256, V288, V320,		// 32-bit words
		//V280, V360, V400,					// 40-bit words
		//V336, V432, V480,					// 48-bit words
		//V392, V504, V560,					// 56-bit words
		V384, V448, V512, V576, V640		// 64-bit words
} BlenderVariant;

	// hash engine lookup table - index by [wordSizeBytes-2][wordCount-6]
const Uint8 blender_memberTable[7][5] = {
	{ V96, V112, V128, V144, V160 },		// 16-bit words
	{ Vgen, Vgen, Vgen, Vgen, Vgen },		// 24-bit words
	{ V192, V224, V256, V288, V320 },		// 32-bit words
	{ Vgen, Vgen, Vgen, Vgen, Vgen },		// 40-bit words
	{ Vgen, Vgen, Vgen, Vgen, Vgen },		// 48-bit words
	{ Vgen, Vgen, Vgen, Vgen, Vgen },		// 56-bit words
	{ V384, V448, V512, V576, V640 }		// 64-bit words
};

// constants used in padding the last byte of the message
const BitSequence blender_bigendbit[8] = {
	0x00, 0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02 };
const BitSequence blender_bigendfill[8] = {
	0x00, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01 };
const BitSequence blender_littleendbit[8] = {
	0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40 };
const BitSequence blender_littleendfill[8] = {
	0x00, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80 };

// initial values of state variables
const Uint64 blender_Hinit64 [] = {		// SHA-512 and SHA-384 H0 values
	0x6a09e667f3bcc908, 0xbb67ae8584caa73b, 0x3c6ef372fe94f82b,
	0xa54ff53a5f1d36f1, 0x510e527fade682d1, 0x9b05688c2b3e6c1f,
	0x1f83d9abfb41bd6b, 0x5be0cd19137e2179,
	0xcbbb9d5dc1059ed8, 0x629a292a367cd507, 0x9159015a3070dd17,
	0x152fecd8f70e5939, 0x67332667ffc00b31, 0x8eb44a8768581511 };

const Uint32 blender_Hinit32 [] = {		// SHA-256 and SHA-224 H0 values
	0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a, 0x510e527f,
	0x9b05688c, 0x1f83d9ab, 0x5be0cd19, 0xcbbb9d5d, 0x629a292a,
	0xc1059ed8, 0x367cd507, 0x3070dd17, 0xf70e5939,
	0xffc00b31, 0x68581511, 0x64f98fa7, 0xbefa4fa4 };

const Uint16 blender_Hinit16 [] = {		// SHA-1 H0 values expressed as shorts
	0x6745, 0x2301, 0xefcd, 0xab89, 0x98ba, 0xdcfe,
	0x1032, 0x5476, 0xc3d2, 0xe1f0 };


//========================================================
// Initialise the state structure appropriately for the given
// digest length - the settings may be overridden after this call
//========================================================
int Blender::Init (int hashbitlen) {
	register int b, f, w, i = 0;
	// first extract the word size and number of words
	if (((hashbitlen % 64) == 0) && (hashbitlen >= 384)) {
		b = f = 8;					// 8 bytes / word
		w = hashbitlen / 64;		// 6 to 10 words
	} else if (((hashbitlen % 32) == 0) && (hashbitlen > 160)) {
		b = f = 4;					// 4 bytes / word
		w = hashbitlen / 32;		// 6 to 10 words
	} else if ((hashbitlen % 16) == 0) {
		b = f = 2;					// 2 bytes / word
		w = hashbitlen / 16;		// 6 to 10 words
	}	// any others need to be set in the structure outside here
	else
		return BAD_HASHBITLEN;				// unknown digest length
	// validate the derived parameters
	if ((w < 6) || (w > BLENDER_MAXWORDS) || (b < 2) || (b > 8))
		return BAD_HASHBITLEN;				// invalid parameters
	if (f == b)								// not gate-reduced
		i = blender_memberTable[b-2][w-6];			// look up member id
	else
		i = Vgen;		// use generalised/universal  hash routine

	memset(&blenderState, 0x00, sizeof(blenderState));	// clear whole sructure
	//blenderState.hashBitLen = hashbitlen;		// not used
	blenderState.member = i;				// remember which variant is being used
	blenderState.wordCount = w;			// save number of words in digest
	blenderState.wordSizeBytes = b;		// save number of bytes per word
	blenderState.fragSizeBytes = f;		// save number of bytes per fragment
	blenderState.bitEndianity = 1;		// default to big-endian bit packing
									// change outside if little-endian
	blenderState.blockSizeFrags = 16;		// default block size is 16 fragments
	//NASTAVENIE RUND:
	blenderState.minimumRounds = blenderNumRounds;		// note these can be changed outside
	blenderState.roundsToGo = blenderNumRounds;			// first "block" is minimum rounds
	if (blenderState.wordSizeBytes > 4) {		// 64-bit variants
		if (hashbitlen == 384) {		// 384-bit digest - load SHA-384 Hinit
			for (i=0; i<5; ++i)
				blenderState.Word[i].ui64 = blender_Hinit64 [8+i];
		} else {					// 512 or 640 - load SHA-512 Hinit
			for (i=0; i<10; ++i)
				blenderState.Word[i].ui64 = blender_Hinit64 [i];
		}
	}
	else if (blenderState.wordSizeBytes > 2) {	// 32-bit variants
		if (hashbitlen == 224) {			// 224-bit digest - load SHA-224 Hinit
			for (i=0; i<8; ++i)
				blenderState.Word[i].ui32 = blender_Hinit32 [10+i];
		} else {					// 256 or 320 - load SHA-256 Hinit
			for (i=0; i<BLENDER_MAXWORDS; ++i)
				blenderState.Word[i].ui32 = blender_Hinit32 [i];
		}
	}
	else {							// 16-bit variants - load SHA-1 Hinit
			for (i=0; i<BLENDER_MAXWORDS; ++i)
				blenderState.Word[i].ui16 = blender_Hinit16 [i];
	}

	return SUCCESS;
}

//=============================================================================


//========================================================
// hash engine for 32-bit words, dual compression function
// note that the bytecount is guaranteed to be a multiple of 4
//========================================================
void Blender::doHash32x2 (const BitSequence *dta, Uint32 byteCount) {
	// declare register variables in order of frequency of use
	register Uint32 fragment;			// holds assembled input bytes
	register const BitSequence *dptr;	// fast pointer to data
	register Uint32 T1, T2, temp;		// work resisters
	register Uint32 bcount;				// loop control counter
	register Uint8 carry1, carry2;		// final state bits

	if ((dptr = dta) == 0)	return;		// null pointer
	if (byteCount == 0)		return;		// no data given

	// unpack the carry bits from the state structure
	carry1 = ss.carries & 1;			// first carry bit
	carry2 = (ss.carries >> 1) & 1;	// second carry bit

	// process data 4 bytes at a time until all done
	for (bcount = byteCount; bcount > 0; bcount -= 4) {
		// first pack 4 bytes into a fragment
		fragment = (((((dptr[3] << 8) | dptr[2]) << 8) | dptr[1]) << 8) | dptr[0];
		dptr += 4;						// advance data pointer

		// update the checksum accumulators
		ss.Checksum1.ui32 += fragment;
		ss.Checksum2.ui32 += ~fragment;

		// do the even side compression using T1 as a temporary variable
		T1 = ((fragment >> 8) | (fragment << 24)) ^ ss.Word[0].ui32;
		temp = ss.Word[2].ui32;	// fetch the middle even word
		temp = ((temp >> 8) | (temp << 24)) ^ ss.Word[4].ui32;
		if (carry2 == 0) {			// no previous carry
			T2 = T1 + temp;			// add the two pieces together
			if (T2 < (T1 | temp))	// did overflow happen?
				carry2 = 1;			// yes - remember that
			//else carry2 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T2 = T1 + temp + 1;		// add the pieces plus carry
			if (T2 > (T1 | temp))	// did overflow happen?
				carry2 = 0;			// no - remember that
			//else carry2 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T2: %08X", T1);
		//fprintf (fp_log, " + %08X", temp);
		//fprintf (fp_log, " = %d,%08X", carry2, T2);
		//fprintf (fp_log, "\r\n");

		// do the odd side compression using fragment as a temporary variable
		fragment ^= ss.Word[5].ui32;
		temp = ss.Word[3].ui32;	// fetch the middle odd word
		temp = ((temp << 8) | (temp >> 24)) ^ ss.Word[1].ui32;
		if (carry1 == 0) {			// no previous carry
			T1 = fragment + temp;	// add the two pieces together
			if (T1 < (fragment | temp))	// did overflow happen?
				carry1 = 1;			// yes - remember that
			//else carry1 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T1 = fragment + temp + 1;	// add the pieces plus carry
			if (T1 > (fragment | temp))	// did overflow happen?
				carry1 = 0;			// no - remember that
			//else carry1 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T1: %08X", fragment);
		//fprintf (fp_log, " + %08X", temp);
		//fprintf (fp_log, " = %d,%08X", carry1, T1);
		//fprintf (fp_log, "\r\n");

		// now rotate the compression outputs
		if ((carry1 ^ carry2) != 0) {	// one carry - rotate 7
			T1 = (T1 << 7) | (T1 >> 25);
			T2 = (T2 >> 7) | (T2 << 25);
		} else if (carry1 == 0) {		// no carries - rotate 8
			T1 = (T1 << 8) | (T1 >> 24);
			T2 = (T2 >> 8) | (T2 << 24);
		} else {						// two carries - rotate 6
			T1 = (T1 << 6) | (T1 >> 26);
			T2 = (T2 >> 6) | (T2 << 26);
		}

		// finally do the expansion and summation
		temp = ss.Word[0].ui32;		// word to rotate and wrap around
		temp = (temp >> 7) | (temp << 25);	// do rotation
		ss.Sum[0].ui32 += 
			(ss.Word[0].ui32 = ss.Word[1].ui32 ^ T2);
		ss.Sum[1].ui32 += 
			(ss.Word[1].ui32 = ss.Word[2].ui32 ^ T1);
		ss.Sum[2].ui32 += 
			(ss.Word[2].ui32 = ss.Word[3].ui32 ^ T2);
		ss.Sum[3].ui32 += 
			(ss.Word[3].ui32 = ss.Word[4].ui32 ^ T1);
		ss.Sum[4].ui32 += 
			(ss.Word[4].ui32 = ss.Word[5].ui32 ^ T2);
		if (ss.wordCount == 6)			// 6 words
			ss.Sum[5].ui32 += 
				(ss.Word[5].ui32 = temp ^ T1);
		else {								// 7 or 8 words
			ss.Sum[5].ui32 += 
				(ss.Word[5].ui32 = ss.Word[6].ui32 ^ T1);
			if (ss.wordCount == 7)		// 7 words
				ss.Sum[6].ui32 += 
					(ss.Word[6].ui32 = temp ^ T2);
			else {							// 8 words
				ss.Sum[6].ui32 += 
					(ss.Word[6].ui32 = ss.Word[7].ui32 ^ T2);
				ss.Sum[7].ui32 += 
					(ss.Word[7].ui32 = temp ^ T1);
			}
		}

		if (--ss.roundsToGo == 0)	// reduce space left in current block
			ss.roundsToGo = ss.blockSizeFrags;
	}	// end of round

	ss.carries = (carry2 << 1) | (carry1 & 1);		// save the carry bits
}

//========================================================
// hash engine for 16-bit words, dual compression function
// note that the bytecount is guaranteed to be a multiple of 4
//========================================================
void Blender::doHash16x2 (const BitSequence *dta, Uint32 byteCount) {
	// declare register variables in order of frequency of use
	register Uint16 fragment;			// holds assembled input bytes
	register const BitSequence *dptr;	// fast pointer to data
	register Uint16 T1, T2, temp;		// work resisters
	register Uint32 bcount;				// loop control counter
	register Uint8 carry1, carry2;		// final state bits

	if ((dptr = dta) == 0)	return;		// null pointer
	if (byteCount == 0)		return;		// no data given

	// unpack the carry bits from the state structure
	carry1 = ss.carries & 1;			// first carry bit
	carry2 = (ss.carries >> 1) & 1;	// second carry bit

	// process data 2 bytes at a time until all done
	for (bcount = byteCount; bcount > 0; bcount -= 2) {
		// first pack 2 bytes into a fragment
		fragment = (dptr[1] << 8) | dptr[0];
		dptr += 2;						// advance data pointer

		// update the checksum accumulators
		ss.Checksum1.ui16 += fragment;
		ss.Checksum2.ui16 += ~fragment;

		// do the even side compression using T1 as a temporary variable
		T1 = ((fragment >> 8) | (fragment << 8)) ^ ss.Word[0].ui16;
		temp = ss.Word[2].ui16;	// fetch the middle even word
		temp = ((temp >> 8) | (temp << 8)) ^ ss.Word[4].ui16;
		if (carry2 == 0) {			// no previous carry
			T2 = T1 + temp;			// add the two pieces together
			if (T2 < (T1 | temp))	// did overflow happen?
				carry2 = 1;			// yes - remember that
			//else carry2 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T2 = T1 + temp + 1;		// add the pieces plus carry
			if (T2 > (T1 | temp))	// did overflow happen?
				carry2 = 0;			// no - remember that
			//else carry2 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T2: %04X", T1);
		//fprintf (fp_log, " + %04X", temp);
		//fprintf (fp_log, " = %d,%04X", carry2, T2);
		//fprintf (fp_log, "\r\n");

		// do the odd side compression using fragment as a temporary variable
		fragment ^= ss.Word[5].ui16;
		temp = ss.Word[3].ui16;	// fetch the middle odd word
		temp = ((temp << 8) | (temp >> 8)) ^ ss.Word[1].ui16;
		if (carry1 == 0) {			// no previous carry
			T1 = fragment + temp;	// add the two pieces together
			if (T1 < (fragment | temp))	// did overflow happen?
				carry1 = 1;			// yes - remember that
			//else carry1 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T1 = fragment + temp + 1;	// add the pieces plus carry
			if (T1 > (fragment | temp))	// did overflow happen?
				carry1 = 0;			// no - remember that
			//else carry1 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T1: %04X", fragment);
		//fprintf (fp_log, " + %04X", temp);
		//fprintf (fp_log, " = %d,%04X", carry1, T1);
		//fprintf (fp_log, "\r\n");

		// now rotate the compression outputs
		if ((carry1 ^ carry2) != 0) {	// one carry - rotate 7
			T1 = (T1 << 7) | (T1 >> 9);
			T2 = (T2 >> 7) | (T2 << 9);
		} else if (carry1 == 0) {		// no carries - rotate 8
			T1 = (T1 << 8) | (T1 >> 8);
			T2 = (T2 >> 8) | (T2 << 8);
		} else {						// two carries - rotate 6
			T1 = (T1 << 6) | (T1 >> 10);
			T2 = (T2 >> 6) | (T2 << 10);
		}

		// finally do the expansion and summation
		temp = ss.Word[0].ui16;		// word to rotate and wrap around
		temp = (temp >> 7) | (temp << 9);	// do rotation
		ss.Sum[0].ui16 += 
			(ss.Word[0].ui16 = ss.Word[1].ui16 ^ T2);
		ss.Sum[1].ui16 += 
			(ss.Word[1].ui16 = ss.Word[2].ui16 ^ T1);
		ss.Sum[2].ui16 += 
			(ss.Word[2].ui16 = ss.Word[3].ui16 ^ T2);
		ss.Sum[3].ui16 += 
			(ss.Word[3].ui16 = ss.Word[4].ui16 ^ T1);
		ss.Sum[4].ui16 += 
			(ss.Word[4].ui16 = ss.Word[5].ui16 ^ T2);
		if (ss.wordCount == 6)			// 6 words
			ss.Sum[5].ui16 += 
				(ss.Word[5].ui16 = temp ^ T1);
		else {								// 7 or 8 words
			ss.Sum[5].ui16 += 
				(ss.Word[5].ui16 = ss.Word[6].ui16 ^ T1);
			if (ss.wordCount == 7)		// 7 words
				ss.Sum[6].ui16 += 
					(ss.Word[6].ui16 = temp ^ T2);
			else {							// 8 words
				ss.Sum[6].ui16 += 
					(ss.Word[6].ui16 = ss.Word[7].ui16 ^ T2);
				ss.Sum[7].ui16 += 
					(ss.Word[7].ui16 = temp ^ T1);
			}
		}

		if (--ss.roundsToGo == 0)	// reduce space left in current block
			ss.roundsToGo = ss.blockSizeFrags;
	}	// end of round

	ss.carries = (carry2 << 1) | (carry1 & 1);		// save the carry bits
}


//========================================================
// hash engine for 64-bit words, dual compression function
// note that the bytecount is guaranteed to be a multiple of 8
//========================================================
void Blender::doHash64x2 (const BitSequence *dta, Uint32 byteCount) {
	// declare register variables in order of frequency of use
	register Uint64 fragment;			// holds assembled input bytes
	register const BitSequence *dptr;	// fast pointer to data
	register Uint64 T1, T2, temp;		// work resisters
	register Uint32 bcount;				// loop control counter
	register Uint8 carry1, carry2;		// final state bits

	if ((dptr = dta) == 0)	return;		// null pointer
	if (byteCount == 0)		return;		// no data given

	// unpack the carry bits from the state structure
	carry1 = ss.carries & 1;			// first carry bit
	carry2 = (ss.carries >> 1) & 1;		// second carry bit

	// process data 8 bytes at a time until all done
	for (bcount = byteCount; bcount > 0; bcount -= 8) {
		// first pack 8 bytes into a fragment
		fragment = ((((dptr[7] << 8) | dptr[6]) << 8) | dptr[5]) << 8;
		fragment = ((((fragment | dptr[4]) << 8) | dptr[3]) << 8) | dptr[2];
		fragment = (((fragment << 8) | dptr[1]) << 8) | dptr[0];
		dptr += 8;						// advance data pointer

		// update the checksum accumulators
		ss.Checksum1.ui64 += fragment;
		ss.Checksum2.ui64 += ~fragment;

		// do the even side compression using T1 as a temporary variable
		T1 = ((fragment >> 8) | (fragment << 56)) ^ ss.Word[0].ui64;
		temp = ss.Word[2].ui64;	// fetch the middle even word
		temp = ((temp >> 8) | (temp << 56)) ^ ss.Word[4].ui64;
		if (carry2 == 0) {			// no previous carry
			T2 = T1 + temp;			// add the two pieces together
			if (T2 < (T1 | temp))	// did overflow happen?
				carry2 = 1;			// yes - remember that
			//else carry2 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T2 = T1 + temp + 1;		// add the pieces plus carry
			if (T2 > (T1 | temp))	// did overflow happen?
				carry2 = 0;			// no - remember that
			//else carry2 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T2: "); showHex64 (T1);
		//fprintf (fp_log, " + "); showHex64 (temp);
		//fprintf (fp_log, " = %d,", carry2); showHex64 (T2);
		//fprintf (fp_log, "\r\n");

		// do the odd side compression using fragment as a temporary variable
		fragment ^= ss.Word[5].ui64;
		temp = ss.Word[3].ui64;	// fetch the middle odd word
		temp = ((temp << 8) | (temp >> 56)) ^ ss.Word[1].ui64;
		if (carry1 == 0) {			// no previous carry
			T1 = fragment + temp;	// add the two pieces together
			if (T1 < (fragment | temp))	// did overflow happen?
				carry1 = 1;			// yes - remember that
			//else carry1 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T1 = fragment + temp + 1;	// add the pieces plus carry
			if (T1 > (fragment | temp))	// did overflow happen?
				carry1 = 0;			// no - remember that
			//else carry1 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T1: "); showHex64 (fragment);
		//fprintf (fp_log, " + "); showHex64 (temp);
		//fprintf (fp_log, " = %d,", carry1); showHex64 (T1);
		//fprintf (fp_log, "\r\n");

		// now rotate the compression outputs
		if ((carry1 ^ carry2) != 0) {	// one carry - rotate 7
			T1 = (T1 << 7) | (T1 >> 57);
			T2 = (T2 >> 7) | (T2 << 57);
		} else if (carry1 == 0) {		// no carries - rotate 8
			T1 = (T1 << 8) | (T1 >> 56);
			T2 = (T2 >> 8) | (T2 << 56);
		} else {						// two carries - rotate 6
			T1 = (T1 << 6) | (T1 >> 58);
			T2 = (T2 >> 6) | (T2 << 58);
		}

		// finally do the expansion and summation
		temp = ss.Word[0].ui64;		// word to rotate and wrap around
		temp = (temp >> 7) | (temp << 57);	// do rotation
		ss.Sum[0].ui64 += 
			(ss.Word[0].ui64 = ss.Word[1].ui64 ^ T2);
		ss.Sum[1].ui64 += 
			(ss.Word[1].ui64 = ss.Word[2].ui64 ^ T1);
		ss.Sum[2].ui64 += 
			(ss.Word[2].ui64 = ss.Word[3].ui64 ^ T2);
		ss.Sum[3].ui64 += 
			(ss.Word[3].ui64 = ss.Word[4].ui64 ^ T1);
		ss.Sum[4].ui64 += 
			(ss.Word[4].ui64 = ss.Word[5].ui64 ^ T2);
		if (ss.wordCount == 6)			// 6 words
			ss.Sum[5].ui64 += 
				(ss.Word[5].ui64 = temp ^ T1);
		else {								// 7 or 8 words
			ss.Sum[5].ui64 += 
				(ss.Word[5].ui64 = ss.Word[6].ui64 ^ T1);
			if (ss.wordCount == 7)		// 7 words
				ss.Sum[6].ui64 += 
					(ss.Word[6].ui64 = temp ^ T2);
			else {							// 8 words
				ss.Sum[6].ui64 += 
					(ss.Word[6].ui64 = ss.Word[7].ui64 ^ T2);
				ss.Sum[7].ui64 += 
					(ss.Word[7].ui64 = temp ^ T1);
			}
		}

		if (--ss.roundsToGo == 0)	// reduce space left in current block
			ss.roundsToGo = ss.blockSizeFrags;
	}	// end of round

	ss.carries = (carry2 << 1) | (carry1 & 1);		// save the carry bits
}

//========================================================
// hash engine for 32-bit words, triple compression function
// note that the byte count is guaranteed to be a multiple of 8
//========================================================
void Blender::doHash32x3 (const BitSequence *dta, Uint32 byteCount) {
	// declare register variables in order of frequency of use
	register Uint32 fragment;			// holds assembled input bytes
	register const BitSequence *dptr;	// fast pointer to data
	register Uint32 temp, T1, T2, T3;	// work resisters
	register Uint32 bcount;				// loop control counter
	register Uint8 carry1, carry2, carry3;	// final state bits

	if ((dptr = dta) == 0)	return;		// null pointer
	if (byteCount == 0)		return;		// no data given

	// unpack the carry bits from the state structure
	carry1 = ss.carries & 1;			// first carry bit
	carry2 = (ss.carries >> 1) & 1;		// second carry bit
	carry3 = (ss.carries >> 2) & 1;		// third carry bit

	for (bcount = byteCount; bcount > 0; bcount -= 4) {
		// first pack 4 bytes into a fragment
		fragment = ((((((dptr[3]) << 8) | dptr[2]) << 8) | dptr[1]) << 8) | dptr[0];
		dptr += 4;						// advance data pointer

		// update the checksum accumulators
		ss.Checksum1.ui32 += fragment;
		ss.Checksum2.ui32 += ~fragment;

		// do the right side compression using T1 as a temporary variable
		T1 = ((fragment << 8) | (fragment >> 24)) ^ ss.Word[5].ui32;
		temp = ss.Word[2].ui32;		// fetch the middle word
		temp = ((temp << 8) | (temp >> 24)) ^ ss.Word[8].ui32;
		if (carry3 == 0) {			// no previous carry
			T3 = T1 + temp;			// add the two pieces together
			if (T3 < (T1 | temp))	// did overflow happen?
				carry3 = 1;			// yes - remember that
			//else carry3 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T3 = T1 + temp + 1;		// add the pieces plus carry
			if (T3 > (T1 | temp))	// did overflow happen?
				carry3 = 0;			// no - remember that
			//else carry3 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T3: %8x", T1);
		//fprintf (fp_log, " + %8x", temp);
		//fprintf (fp_log, " = %d,%8x", carry3, T3);
		//fprintf (fp_log, "\r\n");

		// do the left side compression using T1 as a temporary variable
		T1 = ((fragment >> 8) | (fragment << 24)) ^ ss.Word[0].ui32;
		temp = ss.Word[3].ui32;		// fetch the middle word
		temp = ((temp >> 8) | (temp << 24)) ^ ss.Word[6].ui32;
		if (carry2 == 0) {			// no previous carry
			T2 = T1 + temp;			// add the two pieces together
			if (T2 < (T1 | temp))	// did overflow happen?
				carry2 = 1;			// yes - remember that
			//else carry2 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T2 = T1 + temp + 1;		// add the pieces plus carry
			if (T2 > (T1 | temp))	// did overflow happen?
				carry2 = 0;			// no - remember that
			//else carry2 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T2: %8x", T1);
		//fprintf (fp_log, " + %8x", temp);
		//fprintf (fp_log, " = %d,%8x", carry2, T2);
		//fprintf (fp_log, "\r\n");

		// do the center compression using fragment as a temporary variable
		fragment ^= ss.Word[7].ui32;
		temp = ss.Word[4].ui32;			// fetch the middle word
		temp = ((temp << 8) | (temp >> 24)) ^ ss.Word[1].ui32;
		if (carry1 == 0) {				// no previous carry
			T1 = fragment + temp;		// add the two pieces together
			if (T1 < (fragment | temp))	// did overflow happen?
				carry1 = 1;				// yes - remember that
			//else carry1 = 0;			// redundant - it was already 0
		} else {						// previous carry flagged
			T1 = fragment + temp + 1;	// add the pieces plus carry
			if (T1 > (fragment | temp))	// did overflow happen?
				carry1 = 0;				// no - remember that
			//else carry1 = 1;			// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T1: %8x", fragment);
		//fprintf (fp_log, " + %8x", temp);
		//fprintf (fp_log, " = %d,%8x", carry1, T1);
		//fprintf (fp_log, "\r\n");

		// now rotate the compression outputs
		if ((carry1 ^ carry2) != 0) {	// one carry - rotate 7
			T1 = (T1 << 7) | (T1 >> 25);
			T2 = (T2 >> 7) | (T2 << 25);
			if (carry3 == 0)			// one carry - rotate 9
				T3 = (T3 << 9) | (T3 >> 23);
			else						// two carries - rotate 10
				T3 = (T3 << 10) | (T3 >> 22);
		} else if (carry1 == 0) {		// no carries - rotate 8
			T1 = (T1 << 8) | (T1 >> 24);
			T2 = (T2 >> 8) | (T2 << 24);
			if (carry3 == 0)			// no carries - rotate 8
				T3 = (T3 << 8) | (T3 >> 24);
			else						// one carry - rotate 9
				T3 = (T3 << 9) | (T3 >> 23);
		} else {						// two carries - rotate 6
			T1 = (T1 << 6) | (T1 >> 26);
			T2 = (T2 >> 6) | (T2 << 26);
			if (carry3 == 0)			// two carries - rotate 10
				T3 = (T3 << 10) | (T3 >> 22);
			else						// three carries - rotate 11
				T3 = (T3 << 11) | (T3 >> 21);
		}

		// finally do the expansion and summation
		temp = ss.Word[0].ui32;		// word to rotate and wrap around
		temp = (temp >> 7) | (temp << 25);	// do rotation
		ss.Sum[0].ui32 += 
			(ss.Word[0].ui32 = ss.Word[1].ui32 ^ T2);
		ss.Sum[1].ui32 += 
			(ss.Word[1].ui32 = ss.Word[2].ui32 ^ T1);
		ss.Sum[2].ui32 += 
			(ss.Word[2].ui32 = ss.Word[3].ui32 ^ T3);
		ss.Sum[3].ui32 += 
			(ss.Word[3].ui32 = ss.Word[4].ui32 ^ T2);
		ss.Sum[4].ui32 += 
			(ss.Word[4].ui32 = ss.Word[5].ui32 ^ T1);
		ss.Sum[5].ui32 += 
			(ss.Word[5].ui32 = ss.Word[6].ui32 ^ T3);
		ss.Sum[6].ui32 += 
			(ss.Word[6].ui32 = ss.Word[7].ui32 ^ T2);
		ss.Sum[7].ui32 += 
			(ss.Word[7].ui32 = ss.Word[8].ui32 ^ T1);
		if (ss.wordCount == 9)				// 9 words
			ss.Sum[8].ui32 += 
				(ss.Word[8].ui32 = temp ^ T3);
		else {								// 10 words
			ss.Sum[8].ui32 += 
				(ss.Word[8].ui32 = ss.Word[9].ui32 ^ T3);
			ss.Sum[9].ui32 += 
				(ss.Word[9].ui32 = temp ^ T2);
		}

		if (--ss.roundsToGo == 0)	// reduce space left in current block
			ss.roundsToGo = ss.blockSizeFrags;
	}	// end of round

	ss.carries = (carry3 << 2) | (carry2 << 1) | (carry1 & 1);	// save the carry bits
}


//========================================================
// hash engine for 16-bit words, triple compression function
// note that the byte count is guaranteed to be a multiple of 8
//========================================================
void Blender::doHash16x3 (const BitSequence *dta, Uint32 byteCount) {
	// declare register variables in order of frequency of use
	register Uint16 fragment;			// holds assembled input bytes
	register const BitSequence *dptr;	// fast pointer to data
	register Uint16 temp, T1, T2, T3;	// work resisters
	register Uint32 bcount;				// loop control counter
	register Uint8 carry1, carry2, carry3;	// final state bits

	if ((dptr = dta) == 0)	return;		// null pointer
	if (byteCount == 0)		return;		// no data given

	// unpack the carry bits from the state structure
	carry1 = ss.carries & 1;			// first carry bit
	carry2 = (ss.carries >> 1) & 1;		// second carry bit
	carry3 = (ss.carries >> 2) & 1;		// third carry bit

	for (bcount = byteCount; bcount > 0; bcount -= 2) {
		// first pack 2 bytes into a fragment
		fragment = (dptr[1] << 8) | dptr[0];
		dptr += 2;						// advance data pointer

		// update the checksum accumulators
		ss.Checksum1.ui16 += fragment;
		ss.Checksum2.ui16 += ~fragment;

		// do the right side compression using T1 as a temporary variable
		T1 = ((fragment << 8) | (fragment >> 8)) ^ ss.Word[5].ui16;
		temp = ss.Word[2].ui16;		// fetch the middle word
		temp = ((temp << 8) | (temp >> 8)) ^ ss.Word[8].ui16;
		if (carry3 == 0) {			// no previous carry
			T3 = T1 + temp;			// add the two pieces together
			if (T3 < (T1 | temp))	// did overflow happen?
				carry3 = 1;			// yes - remember that
			//else carry3 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T3 = T1 + temp + 1;		// add the pieces plus carry
			if (T3 > (T1 | temp))	// did overflow happen?
				carry3 = 0;			// no - remember that
			//else carry3 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T3: %4x", T1);
		//fprintf (fp_log, " + %4x", temp);
		//fprintf (fp_log, " = %d,%4x", carry3, T3);
		//fprintf (fp_log, "\r\n");

		// do the left side compression using T1 as a temporary variable
		T1 = ((fragment >> 8) | (fragment << 8)) ^ ss.Word[0].ui16;
		temp = ss.Word[3].ui16;		// fetch the middle word
		temp = ((temp >> 8) | (temp << 8)) ^ ss.Word[6].ui16;
		if (carry2 == 0) {			// no previous carry
			T2 = T1 + temp;			// add the two pieces together
			if (T2 < (T1 | temp))	// did overflow happen?
				carry2 = 1;			// yes - remember that
			//else carry2 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T2 = T1 + temp + 1;		// add the pieces plus carry
			if (T2 > (T1 | temp))	// did overflow happen?
				carry2 = 0;			// no - remember that
			//else carry2 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T2: %4x", T1);
		//fprintf (fp_log, " + %4x", temp);
		//fprintf (fp_log, " = %d,%4x", carry2, T2);
		//fprintf (fp_log, "\r\n");

		// do the center compression using fragment as a temporary variable
		fragment ^= ss.Word[7].ui16;
		temp = ss.Word[4].ui16;			// fetch the middle word
		temp = ((temp << 8) | (temp >> 8)) ^ ss.Word[1].ui16;
		if (carry1 == 0) {				// no previous carry
			T1 = fragment + temp;		// add the two pieces together
			if (T1 < (fragment | temp))	// did overflow happen?
				carry1 = 1;				// yes - remember that
			//else carry1 = 0;			// redundant - it was already 0
		} else {						// previous carry flagged
			T1 = fragment + temp + 1;	// add the pieces plus carry
			if (T1 > (fragment | temp))	// did overflow happen?
				carry1 = 0;				// no - remember that
			//else carry1 = 1;			// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T1: %4x", fragment);
		//fprintf (fp_log, " + %4x", temp);
		//fprintf (fp_log, " = %d,%4x", carry1, T1);
		//fprintf (fp_log, "\r\n");

		// now rotate the compression outputs
		if ((carry1 ^ carry2) != 0) {	// one carry - rotate 7
			T1 = (T1 << 7) | (T1 >> 9);
			T2 = (T2 >> 7) | (T2 << 9);
			if (carry3 == 0)			// one carry - rotate 9
				T3 = (T3 << 9) | (T3 >> 7);
			else						// two carries - rotate 10
				T3 = (T3 << 10) | (T3 >> 6);
		} else if (carry1 == 0) {		// no carries - rotate 8
			T1 = (T1 << 8) | (T1 >> 8);
			T2 = (T2 >> 8) | (T2 << 8);
			if (carry3 == 0)			// no carries - rotate 8
				T3 = (T3 << 8) | (T3 >> 8);
			else						// one carry - rotate 9
				T3 = (T3 << 9) | (T3 >> 7);
		} else {						// two carries - rotate 6
			T1 = (T1 << 6) | (T1 >> 10);
			T2 = (T2 >> 6) | (T2 << 10);
			if (carry3 == 0)			// two carries - rotate 10
				T3 = (T3 << 10) | (T3 >> 6);
			else						// three carries - rotate 11
				T3 = (T3 << 11) | (T3 >> 5);
		}

		// finally do the expansion and summation
		temp = ss.Word[0].ui16;		// word to rotate and wrap around
		temp = (temp >> 7) | (temp << 9);	// do rotation
		ss.Sum[0].ui16 += 
			(ss.Word[0].ui16 = ss.Word[1].ui16 ^ T2);
		ss.Sum[1].ui16 += 
			(ss.Word[1].ui16 = ss.Word[2].ui16 ^ T1);
		ss.Sum[2].ui16 += 
			(ss.Word[2].ui16 = ss.Word[3].ui16 ^ T3);
		ss.Sum[3].ui16 += 
			(ss.Word[3].ui16 = ss.Word[4].ui16 ^ T2);
		ss.Sum[4].ui16 += 
			(ss.Word[4].ui16 = ss.Word[5].ui16 ^ T1);
		ss.Sum[5].ui16 += 
			(ss.Word[5].ui16 = ss.Word[6].ui16 ^ T3);
		ss.Sum[6].ui16 += 
			(ss.Word[6].ui16 = ss.Word[7].ui16 ^ T2);
		ss.Sum[7].ui16 += 
			(ss.Word[7].ui16 = ss.Word[8].ui16 ^ T1);
		if (ss.wordCount == 9)				// 9 words
			ss.Sum[8].ui16 += 
				(ss.Word[8].ui16 = temp ^ T3);
		else {								// 10 words
			ss.Sum[8].ui16 += 
				(ss.Word[8].ui16 = ss.Word[9].ui16 ^ T3);
			ss.Sum[9].ui16 += 
				(ss.Word[9].ui16 = temp ^ T2);
		}

		if (--ss.roundsToGo == 0)	// reduce space left in current block
			ss.roundsToGo = ss.blockSizeFrags;
	}	// end of round

	ss.carries = (carry3 << 2) | (carry2 << 1) | (carry1 & 1);	// save the carry bits
}


//========================================================
// hash engine for 64-bit words, triple compression function
// note that the byte count is guaranteed to be a multiple of 8
//========================================================
void Blender::doHash64x3 (const BitSequence *dta, Uint32 byteCount) {
	// declare register variables in order of frequency of use
	register Uint64 fragment;			// holds assembled input bytes
	register const BitSequence *dptr;	// fast pointer to data
	register Uint64 temp, T1, T2, T3;	// work resisters
	register Uint32 bcount;				// loop control counter
	register Uint8 carry1, carry2, carry3;	// final state bits

	if ((dptr = dta) == 0)	return;		// null pointer
	if (byteCount == 0)		return;		// no data given

	// unpack the carry bits from the state structure
	carry1 = ss.carries & 1;			// first carry bit
	carry2 = (ss.carries >> 1) & 1;		// second carry bit
	carry3 = (ss.carries >> 2) & 1;		// third carry bit

	for (bcount = byteCount; bcount > 0; bcount -= 8) {
		// first pack 8 bytes into a fragment
		fragment = ((((dptr[7] << 8) | dptr[6]) << 8) | dptr[5]) << 8;
		fragment = ((((fragment | dptr[4]) << 8) | dptr[3]) << 8) | dptr[2];
		fragment = (((fragment << 8) | dptr[1]) << 8) | dptr[0];
		dptr += 8;						// advance data pointer

		// update the checksum accumulators
		ss.Checksum1.ui64 += fragment;
		ss.Checksum2.ui64 += ~fragment;

		// do the right side compression using T1 as a temporary variable
		T1 = ((fragment << 8) | (fragment >> 56)) ^ ss.Word[5].ui64;
		temp = ss.Word[2].ui64;		// fetch the middle word
		temp = ((temp << 8) | (temp >> 56)) ^ ss.Word[8].ui64;
		if (carry3 == 0) {			// no previous carry
			T3 = T1 + temp;			// add the two pieces together
			if (T3 < (T1 | temp))	// did overflow happen?
				carry3 = 1;			// yes - remember that
			//else carry3 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T3 = T1 + temp + 1;		// add the pieces plus carry
			if (T3 > (T1 | temp))	// did overflow happen?
				carry3 = 0;			// no - remember that
			//else carry3 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T3: "); showHex64 (T1);
		//fprintf (fp_log, " + "); showHex64 (temp);
		//fprintf (fp_log, " = %d,", carry3); showHex64 (T3);
		//fprintf (fp_log, "\r\n");

		// do the left side compression using T1 as a temporary variable
		T1 = ((fragment >> 8) | (fragment << 56)) ^ ss.Word[0].ui64;
		temp = ss.Word[3].ui64;		// fetch the middle word
		temp = ((temp >> 8) | (temp << 56)) ^ ss.Word[6].ui64;
		if (carry2 == 0) {			// no previous carry
			T2 = T1 + temp;			// add the two pieces together
			if (T2 < (T1 | temp))	// did overflow happen?
				carry2 = 1;			// yes - remember that
			//else carry2 = 0;		// redundant - it was already 0
		} else {					// previous carry flagged
			T2 = T1 + temp + 1;		// add the pieces plus carry
			if (T2 > (T1 | temp))	// did overflow happen?
				carry2 = 0;			// no - remember that
			//else carry2 = 1;		// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T2: "); showHex64 (T1);
		//fprintf (fp_log, " + "); showHex64 (temp);
		//fprintf (fp_log, " = %d,", carry2); showHex64 (T2);
		//fprintf (fp_log, "\r\n");

		// do the center compression using fragment as a temporary variable
		fragment ^= ss.Word[7].ui64;
		temp = ss.Word[4].ui64;			// fetch the middle word
		temp = ((temp << 8) | (temp >> 56)) ^ ss.Word[1].ui64;
		if (carry1 == 0) {				// no previous carry
			T1 = fragment + temp;		// add the two pieces together
			if (T1 < (fragment | temp))	// did overflow happen?
				carry1 = 1;				// yes - remember that
			//else carry1 = 0;			// redundant - it was already 0
		} else {						// previous carry flagged
			T1 = fragment + temp + 1;	// add the pieces plus carry
			if (T1 > (fragment | temp))	// did overflow happen?
				carry1 = 0;				// no - remember that
			//else carry1 = 1;			// redundant - it was already 1
		}
		//fprintf (fp_log, "pre T1: "); showHex64 (fragment);
		//fprintf (fp_log, " + "); showHex64 (temp);
		//fprintf (fp_log, " = %d,", carry1); showHex64 (T1);
		//fprintf (fp_log, "\r\n");

		// now rotate the compression outputs
		if ((carry1 ^ carry2) != 0) {	// one carry - rotate 7
			T1 = (T1 << 7) | (T1 >> 57);
			T2 = (T2 >> 7) | (T2 << 57);
			if (carry3 == 0)			// one carry - rotate 9
				T3 = (T3 << 9) | (T3 >> 55);
			else						// two carries - rotate 10
				T3 = (T3 << 10) | (T3 >> 54);
		} else if (carry1 == 0) {		// no carries - rotate 8
			T1 = (T1 << 8) | (T1 >> 56);
			T2 = (T2 >> 8) | (T2 << 56);
			if (carry3 == 0)			// no carries - rotate 8
				T3 = (T3 << 8) | (T3 >> 56);
			else						// one carry - rotate 9
				T3 = (T3 << 9) | (T3 >> 55);
		} else {						// two carries - rotate 6
			T1 = (T1 << 6) | (T1 >> 58);
			T2 = (T2 >> 6) | (T2 << 58);
			if (carry3 == 0)			// two carries - rotate 10
				T3 = (T3 << 10) | (T3 >> 54);
			else						// three carries - rotate 11
				T3 = (T3 << 11) | (T3 >> 53);
		}

		// finally do the expansion and summation
		temp = ss.Word[0].ui64;		// word to rotate and wrap around
		temp = (temp >> 7) | (temp << 57);	// do rotation
		ss.Sum[0].ui64 += 
			(ss.Word[0].ui64 = ss.Word[1].ui64 ^ T2);
		ss.Sum[1].ui64 += 
			(ss.Word[1].ui64 = ss.Word[2].ui64 ^ T1);
		ss.Sum[2].ui64 += 
			(ss.Word[2].ui64 = ss.Word[3].ui64 ^ T3);
		ss.Sum[3].ui64 += 
			(ss.Word[3].ui64 = ss.Word[4].ui64 ^ T2);
		ss.Sum[4].ui64 += 
			(ss.Word[4].ui64 = ss.Word[5].ui64 ^ T1);
		ss.Sum[5].ui64 += 
			(ss.Word[5].ui64 = ss.Word[6].ui64 ^ T3);
		ss.Sum[6].ui64 += 
			(ss.Word[6].ui64 = ss.Word[7].ui64 ^ T2);
		ss.Sum[7].ui64 += 
			(ss.Word[7].ui64 = ss.Word[8].ui64 ^ T1);
		if (ss.wordCount == 9)				// 9 words
			ss.Sum[8].ui64 += 
				(ss.Word[8].ui64 = temp ^ T3);
		else {								// 10 words
			ss.Sum[8].ui64 += 
				(ss.Word[8].ui64 = ss.Word[9].ui64 ^ T3);
			ss.Sum[9].ui64 += 
				(ss.Word[9].ui64 = temp ^ T2);
		}

		if (--ss.roundsToGo == 0)	// reduce space left in current block
			ss.roundsToGo = ss.blockSizeFrags;
	}	// end of round

	ss.carries = (carry3 << 2) | (carry2 << 1) | (carry1 & 1);	// save the carry bits
}


//========================================================
// generic hash engine for all cases < 9 words - runs on all platforms
// First-time readers are advised to skip this routine and look
// at one of the above special-purpose routines instead.
// Note that the bytecount is guaranteed to be a multiple of the fragment size
//========================================================
void Blender::doHashGen (const BitSequence *dta, Uint32 byteCount) {
	register int i, j, temp, temp1, temp2, carry, carry1, carry2;
	register Uint32 bcount;
	register const BitSequence *dptr;
	Uint8 fragment [8], T1result [8], T2result [8], word0rot[8];

	if ((dptr = dta) == 0)	return;		// null pointer
	if (byteCount == 0)		return;		// no data given

	// unpack the carry bits from the state structure
	carry1 = ss.carries & 1;			// first carry bit
	carry2 = (ss.carries >> 1) & 1;		// second carry bit

	for (bcount = byteCount; bcount > 0; bcount -= ss.fragSizeBytes) {
		carry = 0;
		for (i=0; i<ss.fragSizeBytes; ++i) {		// build a fragment
			temp = fragment [i] = *dptr++;			// get next byte
			temp += carry + (int)ss.Checksum1.b[i];	// update checksum1
			carry = temp >> 8;						// ripple carry
			ss.Checksum1.b[i] = (Uint8)temp;
		}

		// update checksum2
		carry = 0;
		for (i=0; i<ss.fragSizeBytes; ++i) {
			temp = ((int)~fragment [i]) & 0xFF;
			temp += carry + (int)ss.Checksum2.b[i];
			carry = temp >> 8;			// ripple carry
			ss.Checksum2.b[i] = (Uint8)temp;
		}

		// perform the compression function a byte slice at a time
		for (i=0; i<ss.fragSizeBytes; ++i) {
			// calculate even side
			if ((j = i + 1) >= ss.fragSizeBytes)	// rotate right 8 bits by indexing
				j = 0;				// wrap around
			temp = (fragment[j] ^ ss.Word[0].b[i]) & 0xFF;
			temp2 = (ss.Word[2].b[j] ^ ss.Word[4].b[i]) & 0xFF;
			temp2 += temp + (carry2 & 1);
			carry2 = temp2 >> 8;			// ripple carry
			T2result [i] = (Uint8)temp2;		// T2 value

			// calculate odd side
			if ((j = i - 1) < 0)			// rotate left 8 bits by indexing
				j = ss.fragSizeBytes - 1;	// top byte index
			temp = (fragment[i] ^ ss.Word[5].b[i]) & 0xFF;
			temp1 = (ss.Word[3].b[j] ^ ss.Word[1].b[i]) & 0xFF;
			temp1 += temp + (carry1 & 1);
			carry1 = temp1 >> 8;			// ripple carry
			T1result [i] = (Uint8)temp1;		// T1 value
		}

		// now rotate T1 left 8 bits
		temp = T1result [ss.fragSizeBytes-1];		// grab the top byte
		for (i=ss.fragSizeBytes-1; i>0; --i)
			T1result [i] = T1result [i-1];		// shuffle up one byte
		T1result [0] = (Uint8)temp;				// put the top byte back

		// and rotate T2 right 8 bits
		temp = T2result [0];
		for (i=1; i<ss.fragSizeBytes; ++i)
			T2result [i-1] = T2result [i];
		T2result [ss.fragSizeBytes-1] = (Uint8)temp;

		if (((carry1 ^ carry2) & 1) != 0) {		// one carry - shift back a bit
			// rotate T1 right one bit
			carry = T1result [0] & 1;
			for (i=0; i<7; ++i)				
				T1result [i] = (Uint8)((T1result [i] >> 1) |
					((T1result [i+1] & 1) << 7));
			T1result [7] = (Uint8)(T1result [7] >> 1);
			i = ss.fragSizeBytes - 1;			// highest byte index
			T1result [i] = (Uint8)((T1result [i] & 0x7F) | (carry << 7));

			// rotate T2 left one bit
			carry = 0;					// no carry in for multi-byte shift
			for (i=0; i<ss.fragSizeBytes; ++i) {
				temp = T2result [i] & 0xFF;
				temp = (temp << 1) | (carry & 1);	// shift subfunction
				carry = temp >> 8;			// save the carry out
				T2result [i] = (Uint8)(temp & 0xFF);	// save the shift result
			}
			T2result [0] |= (Uint8)(carry & 1);		// put back the last carry bit
		}
		else if ((carry1 & 1) != 0) {		// two carries - shift back two bits
			// rotate T1 right two bits
			carry = T1result [0] & 3;
			for (i=0; i<7; ++i)				
				T1result [i] = (Uint8)((T1result [i] >> 2) |
					((T1result [i+1] & 3) << 6));
			T1result [7] = (Uint8)(T1result [7] >> 2);
			i = ss.fragSizeBytes - 1;			// highest byte index
			T1result [i] = (Uint8)((T1result [i] & 0x3F) | (carry << 6));

			// rotate T2 left two bits
			carry = 0;					// no carry in for multi-byte shift
			for (i=0; i<ss.fragSizeBytes; ++i) {
				temp = T2result [i] & 0xFF;
				temp = (temp << 2) | (carry & 3);	// shift subfunction
				carry = temp >> 8;			// save the carry out
				T2result [i] = (Uint8)(temp & 0xFF);	// save the shift result
			}
			T2result [0] |= (Uint8)(carry & 3);		// put back the last carry bits
		}

		// inflate the compression results
		i = 0;
		for (j=ss.fragSizeBytes; j<8; ++j) {	// fill all eight bytes
			T1result [j] = T1result [i];
			T2result [j] = T2result [i];
			++i;
		}

		// rotate word0 right seven bits and save in word0rot
		carry = ss.Word [0].b[0] & 0x7F;
		for (i=0; i<7; ++i)
			word0rot[i] = (Uint8)((ss.Word [0].b[i] >> 7) |
				((ss.Word [0].b[i+1] & 0x7F) << 1));
		word0rot[7] = (Uint8)(ss.Word [0].b[7] >> 7);
		i = ss.wordSizeBytes - 1;				// highest byte index
		word0rot[i] = (Uint8)((word0rot[i] & 1) | (carry << 1));

		// rotate all the words down one place
		for (i=0; i<ss.wordSizeBytes; ++i)
			for (j=1; j<ss.wordCount; ++j)
				ss.Word [j-1].b[i] = ss.Word [j].b[i];
		j = ss.wordCount - 1;				// highest numbered word
		for (i=0; i<ss.wordSizeBytes; ++i)
			ss.Word [j].b[i] = word0rot[i];		// replace word0 at the top

		// finally do the expansion and summation functions
		for (j=0; j<ss.wordCount; ++j) {
			carry = 0;				// no carry in for summation
			for (i=0; i<ss.wordSizeBytes; ++i) {
				temp = ss.Word [j].b[i];
				if ((j & 1) != 0)
					temp ^= T1result [i];		// odd word expansion
				else
					temp ^= T2result [i];		// even word expansion
				ss.Word [j].b[i] = (Uint8)temp;
				temp += carry + (int)ss.Sum[j].b[i];
				carry = temp >> 8;
				ss.Sum[j].b[i] = (Uint8)temp;
			}
		}

		if (--ss.roundsToGo == 0)	// reduce space left in current block
			ss.roundsToGo = ss.blockSizeFrags;
	}	// end of round loop

	ss.carries = (carry2 << 1) | (carry1 & 1);		// save the carry bits
}

//========================================================
// process a chunk of data - an internal call
// the chunk is guaranteed to be an exact number of fragments
//========================================================
void Blender::doUpdate (const BitSequence *dta, Uint32 byteCount) {
	register Uint32 i, j;
	// first deal with filling the first block buffer ready for end fill
	if (ss.firstByteCount < BLENDER_FBSIZE) {
		i = ss.firstByteCount;		// index of first empty byte
		j = BLENDER_FBSIZE - i;					// potential copy length
		if (byteCount < j)				// not enough new data to fill block
			j = byteCount;				// actual copy length
		memcpy (&ss.firstBlock[i], dta, j);	// stash the data
		ss.firstByteCount += (Uint8)j;		// update amount stored
	}
	// now dispatch to the appropriate processing routine
	switch (ss.member) {
	case V384:
	case V448:
	case V512:	doHash64x2 (dta, byteCount);
			break;
	case V576:
	case V640:	doHash64x3 (dta, byteCount);
			break;
	case V192:
	case V224:
	case V256:	doHash32x2 (dta, byteCount);
			break;
	case V288:
	case V320:	doHash32x3 (dta, byteCount);
			break;
	case V96:
	case V112:
	case V128:	doHash16x2 (dta, byteCount);
			break;
	case V144:
	case V160:	doHash16x3 (dta, byteCount);
			break;
	default:	doHashGen (dta, byteCount);		// use generic hash routine
	}
}

//========================================================
// update the hash with the data given - called from user code
//========================================================
int Blender::Update (const BitSequence *dta,
					DataLength databitlen) {
	register int i;
	register DataLength bcount;
	register const BitSequence *dptr = dta;
	register Uint32 j, k;
	memcpy (&ss, &blenderState, sizeof(ss));			// get state in local workspace

	// deal with the length update first
	bcount = ss.sourceDataLength;	// previous length
	ss.sourceDataLength = bcount + databitlen;	// new length
	if (ss.sourceDataLength < (bcount | databitlen))	// overflow
		if (++ss.sourceDataLength2[0] == 0)		// increment higher order count
			if (++ss.sourceDataLength2[1] == 0)	// and the next higher order
				++ss.sourceDataLength2[3];		// and the next one, etc.

	// now deal with moving the data around
	bcount = databitlen >> 3;			// number of complete bytes of data
										// = floor (databitlen / 8)
	if (ss.residueLength != 0) {		// previous partial fragment
		// complete the fragment and process it
		i = ss.residueLength;			// length already used
		k = ss.fragSizeBytes - i;		// length needed to complete fragment
		if ((DataLength)k > bcount)		// if the new addition is tiny
			k = (Uint32)bcount;			// reduce the length
		memcpy (&ss.messageResidue[i], dptr, k);
		dptr += k;						// advance source pointer
		bcount -= k;					// reduce source length
		if ((ss.residueLength += (Uint8)k) >= ss.fragSizeBytes) {
			doUpdate (&ss.messageResidue[0], k+i);	// process it
			ss.residueLength = 0;		// remember nothing remains
		}
	}
	k = ((Uint32)ss.fragSizeBytes) << 24;	// big chunk size in bytes
	while (bcount >= (DataLength)k) {
		doUpdate (dptr, k);				// process a big chunk
		dptr += k;						// advance source pointer
		bcount -= k;					// reduce source length
	}
	j = (Uint32)bcount / ss.fragSizeBytes;		// fragments left
	k = j * ss.fragSizeBytes;			// most of the remainder
	if (k > 0) {
		doUpdate (dptr, k);				// process the small chunk
		dptr += k;						// advance source pointer
		bcount -= k;					// reduce source length
	}
	if ((i = (int)bcount & 7) > 0) {	// save the end for later
		memcpy (&ss.messageResidue[0], dptr, i);
		ss.residueLength = i;			// remember how much saved
		dptr += i;						// advance source pointer
	}

	// now deal with the possible partial byte at the end
	if ((j = (Uint32)databitlen & 7) != 0) {
		k = *dptr;
		if (ss.bitEndianity > 0) {			// big endian
			if ((k & blender_bigendbit[j]) == 0)	// test the last bit
				k |= blender_bigendfill[j];			// fill with ones
			else
				k &= ~blender_bigendfill[j];		// fill with zeros
		} else {							// little endian
			if ((k & blender_littleendbit[j]) == 0)
				k |= blender_littleendfill[j];		// fill with ones
			else
				k &= ~blender_littleendfill[j];		// fill with zeros
		}
		ss.messageResidue[i] = (Uint8)k;	// save the result value
		ss.residueLength += 1;				// update residue byte count
	}
	memcpy (&blenderState, &ss, sizeof(ss));		// save local workspace
	return SUCCESS;
}

//========================================================
// routines to output the final message digest
//========================================================
void Blender::doFinal32 (BitSequence *hashval) {
	register BitSequence *dptr = hashval;
	register Uint32 val;
	register int i, j, k;
	k = ss.wordSizeBytes;				// number of bytes per word
	dptr += (ss.wordCount * k);			// move pointer to the end
	for (i=ss.wordCount-1; i>=0; --i) {
		val = ss.Sum[i].ui32;			// get the result word
		for (j=k; j>0; --j) {
			*--dptr = (Uint8)val;		// output byte
			val >>= 8;					// advance to next byte
		}
	}
}
void Blender::doFinal16 (BitSequence *hashval) {
	register BitSequence *dptr = hashval;
	register Uint16 val;
	register int i, j, k;
	k = ss.wordSizeBytes;				// number of bytes per word
	dptr += (ss.wordCount * k);			// move pointer to the end
	for (i=ss.wordCount-1; i>=0; --i) {
		val = ss.Sum[i].ui16;			// get the result word
		for (j=k; j>0; --j) {
			*--dptr = (Uint8)val;		// output byte
			val >>= 8;					// advance to next byte
		}
	}
}

void Blender::doFinal64 (BitSequence *hashval) {
	register BitSequence *dptr = hashval;
	register Uint64 val;
	register int i, j, k;
	k = ss.wordSizeBytes;				// number of bytes per word
	dptr += (ss.wordCount * k);			// move pointer to the end
	for (i=ss.wordCount-1; i>=0; --i) {
		val = ss.Sum[i].ui64;			// get the result word
		for (j=k; j>0; --j) {
			*--dptr = (Uint8)val;		// output byte
			val >>= 8;					// advance to next byte
		}
	}
}

void Blender::doFinalGen (BitSequence *hashval) {	// general case - array of bytes
	register BitSequence *dptr = hashval;
	register int i, j;
	for (i=0; i<ss.wordCount; ++i)
		for (j=ss.wordSizeBytes-1; j>=0; --j)
			*dptr++ = ss.Sum[i].b[j];		// get final result byte
}

//========================================================
// finalise the hash and output the message digest
//========================================================
int Blender::Final (BitSequence *hashval) {
	register int i, j, k, hwm;
	DataLength datalen;
	int lenlen;					// length of the length
	int spaceleft, spaceneeded, fillsize;
	int filllen;				// amount of fill available
	Uint32 chksum;
	BitSequence buffer[64];	// workspace area
	memcpy (&ss, &blenderState, sizeof(ss));			// get state in local workspace

	// 1. determine the length of the length
	lenlen = 0;				// assume short message (<2**64 bits)
	if ((datalen = ss.sourceDataLength2[2]) != 0)
		lenlen = 3 * sizeof (ss.sourceDataLength);	// 3 x 8 bytes plus
	else if ((datalen = ss.sourceDataLength2[1]) != 0)
		lenlen = 2 * sizeof (ss.sourceDataLength);	// 2 x 8 bytes plus
	else if ((datalen = ss.sourceDataLength2[0]) != 0)
		lenlen = sizeof (ss.sourceDataLength);		// 8 bytes plus
	else
		datalen = ss.sourceDataLength;
	do {					// count bytes used
		lenlen += 1;
		datalen >>= 8;		// remove low order byte
	} while (datalen != 0);
	//printf(" lenlen=%d", lenlen);

	// 2. determine the fill size in bytes
	spaceleft = (ss.roundsToGo - 2) * ss.fragSizeBytes;
	spaceneeded = lenlen + 2 + ss.residueLength;
	while (spaceleft < spaceneeded)		// add a block (or more)
		spaceleft += (ss.blockSizeFrags * ss.fragSizeBytes);
	fillsize = spaceleft + 1 - spaceneeded;
	//printf (" fillsize=%d", fillsize);

	// 3. move message residue to work buffer
	if ((i = (int)ss.residueLength) > 0)
		memcpy (&buffer[0], &ss.messageResidue[0], i);
	//printf (" residue moved");

	// 4. inflate the fill buffer if required and possible
	j = (int)ss.firstByteCount;			// amount of fill available
	if ((j < BLENDER_FBSIZE) && (i > 0)) {		// need to concatenate residue
		k = i;							// potential copy size
		if ((k + j) > BLENDER_FBSIZE)
			k = BLENDER_FBSIZE - j;				// reduce copy amount
		memcpy (&ss.firstBlock[j], &ss.messageResidue[0], k);
		j += k;							// new amount of fill available
	}
	if (j == 0)							// no fill data at all
		j = BLENDER_FBSIZE;						// so use all zeroes
	filllen = j;
	if ((fillsize > j) && ((j << 1) <= BLENDER_FBSIZE)) {
		for (k=j; j+k <= BLENDER_FBSIZE; k+=j)	// replicate fill
			memcpy (&ss.firstBlock[k], &ss.firstBlock[0], j);
		filllen = k;					// updated amount available
	}
	//printf (" filllen=%d", filllen);

	// 5. add fill to work buffer
	j = (sizeof (buffer) - filllen) / ss.fragSizeBytes;	// capacity in fragments
	hwm = j * ss.fragSizeBytes;				// buffer high water mark
	while (fillsize > 0) {
		if (filllen > fillsize)
			filllen = fillsize;				// reduce amount to copy
		memcpy (&buffer[i], &ss.firstBlock, filllen);
		fillsize -= filllen;				// reduce amount to go
		if ((i += filllen) > hwm) {			// advance buffer pointer
			doUpdate (&buffer[0], hwm);		// process chunk
			i -= hwm;						// amount of data left in buffer
			memcpy (&buffer[0], &buffer[hwm], i);
		}
	}
	//printf (" buffer filled");

	// 6. add length to work buffer
	datalen = ss.sourceDataLength;			// first part of length
	j = lenlen;								// bytes to be set
	for (k=0; k<32; ++k) {		// only four elements in this implementation
		buffer[i++] = (Uint8)datalen;		// store low order byte
		if (--j == 0)
			break;							// all relevant data set
		datalen >>= 8;						// remove byte
		if (sizeof (ss.sourceDataLength) > 4) {
			if ((k & 7) == 7)					// last byte of word
				datalen = ss.sourceDataLength2[k >> 3];	// get next word
		} else
			if ((k & 3) == 3)					// last byte of word
				datalen = ss.sourceDataLength2[k >> 2];	// get next word
		if ((i > hwm) && ((i + j + 1) > 64)) {
			doUpdate (&buffer[0], hwm);		// process chunk
			i -= hwm;						// amount of data left in buffer
			memcpy (&buffer[0], &buffer[hwm], i); }
	}
	//printf (" length added");

	// 7. add length of length to work buffer
	buffer[i++] = lenlen;				// store length of the length
			// note that should make an exact multiple of the word length

	// 8. process work buffer
	doUpdate (&buffer[0], i);			// process chunk
	//printf (" update done");

	// 9. move checksums to work buffer - ugly, revise later
	k = ss.fragSizeBytes;
	if (ss.member == Vgen) {		// general case - byte sequence
		for (i=0; i<k; ++i)
			buffer[i] = ~ss.Checksum1.b[i];		// first checksum btes
		for (j=0; j<k; ++j)
			buffer[i++] = ss.Checksum2.b[j];	// second checksum bytes
	} else if (ss.wordSizeBytes >= 4) {		// 64-bit variant
		datalen = ~ss.Checksum1.ui64;		// first checksum
		for (i=j=0; j<k; ++j) {
			buffer[i++] = (Uint8)datalen;	// store low order byte
			datalen >>= 8;					// remove byte
		}
		datalen = ss.Checksum2.ui64;		// second checksum
		for (j=0; j<k; ++j) {
			buffer[i++] = (Uint8)datalen;	// store low order byte
			datalen >>= 8;					// remove byte
		}
	} else if (ss.wordSizeBytes >= 2) {		// 32-bit variant
		chksum = ~ss.Checksum1.ui32;		// first checksum
		for (i=j=0; j<k; ++j) {
			buffer[i++] = (Uint8)chksum;	// store low order byte
			chksum >>= 8;					// remove byte
		}
		chksum = ss.Checksum2.ui32;			// second checksum
		for (j=0; j<k; ++j) {
			buffer[i++] = (Uint8)chksum;	// store low order byte
			chksum >>= 8;					// remove byte
		}
	} else {								// 16-bit variant
		j = ~ss.Checksum1.ui16;				// first checksum
		buffer[0] = j;
		buffer[1] = j >> 8;
		j = ss.Checksum2.ui16;				// second checksum
		buffer[2] = j;
		buffer[3] = j >> 8;
		i = 4;
	}
	//printf (" checksums prepared");

	// 10. process work buffer
	doUpdate (&buffer[0], i);				// process final chunk
	//printf (" checksums processed");

	// 11. output message digest
	if (ss.member == Vgen)					// general byte format
		doFinalGen (hashval);
	else if (ss.wordSizeBytes >= 4)			// 64-bit variants
		doFinal64 (hashval);
	else if (ss.wordSizeBytes >= 2)			// 32-bit variants
		doFinal32 (hashval);
	else									// 16-bit variants
		doFinal16 (hashval);
	//printf (" doFinal finished");

	memcpy (&blenderState, &ss, sizeof(ss));		// save local workspace
	return SUCCESS;
}

//========================================================
// top level all-in-one function call
//========================================================
int Blender::Hash (int hashbitlen, const BitSequence *data,
					DataLength databitlen, BitSequence *hashval) {
	//hashState	state;
	//HashReturn	retval;
	int retval;
	retval = Blender::Init (hashbitlen);
	if (retval == SUCCESS)
		retval = Blender::Update (data, databitlen);
	if (retval == SUCCESS)
		retval = Blender::Final (hashval);
	return retval;
}

Blender::Blender(const int numRounds) {
	if (numRounds == -1) {
		blenderNumRounds = BLENDER_DEFAULT_ROUNDS;
	} else {
		blenderNumRounds = numRounds;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////