/*
 * BitArray.h ($RCSfile: BitArray.h,v $)
 *
 * Defines an array fo bits of arbirary length.
 *
 * Copyright 2008 John Washburn.  All rights reserved until I win the SHA3 contest.
 
$Header: D:\\RCSArchive\\D\\Programming\\WaMM\\ReferenceImplementation\\WaMM\\WaMM\\BitArray.h,v 1.7 2008-10-19 08:25:49-05 johnwashburn Exp johnwashburn $
$Revision: 1.7 $

$Log: BitArray.h,v $
Revision 1.7  2008-10-19 08:25:49-05  johnwashburn
Daily Checkpointing

Revision 1.6  2008-10-13 18:13:55-05  johnwashburn
Daily Check Point

Revision 1.2  2008-10-08 02:25:41-05  johnwashburn
proved comment header block to better use RCS keywords

Revision 1.1  2008-10-08 02:08:03-05  johnwashburn
Added RCS Header Comment block using RCS Keywords.


*/

#ifndef BitArray_h_FA23D8BC_5624_49e5_94B5_2CF8B826CEB3
#define BitArray_h_FA23D8BC_5624_49e5_94B5_2CF8B826CEB3 1

#include <limits.h>
#include <stddef.h>
#include <memory.h>
#include <string.h>
//#include "SHA3api.h"
#include "WaMMConstants.h"
#include "WaMM.h"

typedef struct BitArray {
	long lNumBits;
	long lNumBytes;
	long lBufferSize;
	BitSequence *pBitArrayData;
	BitSequence *pBuffer;
} BitArray;

/* Helper functions needed to work with BitArrays */
WaMMHashReturn EmptyBitArray(BitArray *pBitArray);
WaMMHashReturn AllocateBuffer(BitArray *pArray, long lNumBitsNeeded);
long CalcBufferSize( long lNumBitsNeeded);

/* functions to initialize and create BitArrays */
BitArray *BitArrayFromBitSequence(long lNumBitsNew, const BitSequence *pInitSequence);
BitArray *NewBitArray(long lNumBitsNew);
BitArray *BitArrayFromBitArray(const BitArray *pSourceBitArray);
WaMMHashReturn CloneBitArray(BitArray *pClonedBitArray, const BitArray *pSourceBitArray);
void FreeBitArray(BitArray *pArray);

/* functions which operate on BitArrays */
WaMMHashReturn ConcatenateBitArray(BitArray *pDestArray, const BitArray *pSourceArray);
BitSequence *GetBitSequence(const BitArray *pDestArray);
BitSequence *ConcatenateTwoBitSequences(const long lBitLen1, const BitSequence *pBitSequence1, const long lBitLen2, const BitSequence *pBitSequence2);
WaMMHashReturn PopWholeVectorFromBitArray(BitArray *pBitArray, WaMM_DataVector UnloadedVector);
WaMMHashReturn PopPartialVectorFromBitArray(BitArray *pBitArray, WaMM_DataVector UnloadedVector);
WaMMHashReturn BitArrayShiftRight(BitArray *pBitArray, long lNumBitsShifted);
WaMMHashReturn BitArrayShiftLeft(BitArray *pBitArray, long lNumBitsShifted);
WaMMHashReturn BitArrayRotateRight(BitArray *pBitArray, long lNumBitsShifted);
WaMMHashReturn BitArrayRotateLeft(BitArray *pBitArray, long lNumBitsShifted);

/* very dangerous function since there is no check on the space in the character buffer 
 * for debuggin only.
 */
void BitArrayToSring(BitArray *pArray, char *szString);

#endif /* #ifndef BitArray_h_FA23D8BC_5624_49e5_94B5_2CF8B826CEB3 */