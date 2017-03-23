/*
 * BitArray.c ($RCSfile: BitArray.c,v $)
 *
 * Operations for bit arrays of indefinite length.
 *
 * Copyright 2008 John Washburn.  All rights reserved until I win the SHA3 contest.
 *
$Header: D:\\RCSArchive\\D\\Programming\\WaMM\\ReferenceImplementation\\WaMM\\WaMM\\BitArray.c,v 1.9 2008-10-19 08:25:49-05 johnwashburn Exp johnwashburn $

$Revision: 1.9 $

$Log: BitArray.c,v $
Revision 1.9  2008-10-19 08:25:49-05  johnwashburn
Daily Checkpointing

Revision 1.8  2008-10-13 18:13:54-05  johnwashburn
Daily Check Point

Revision 1.4  2008-10-08 02:25:24-05  johnwashburn
proved comment header block to better use RCS keywords

Revision 1.3  2008-10-08 02:07:54-05  johnwashburn
Added RCS Header Comment block using RCS Keywords.


 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <memory.h>
#include "BitArray.h"
#include "WaMMErrorMessage.h"

WaMMHashReturn InitStruct(BitArray *pArray);

#if 8 == CHAR_BIT
	static BitSequence gc_MaskSetBit[CHAR_BIT] = {
		0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01
	};
	static BitSequence gc_MaskClearBit[CHAR_BIT] = {
		0x7F, 0xBF, 0xDF, 0xEF, 0xF7, 0xFB, 0xFD, 0xFE 
	};
	static BitSequence gc_MaskMSB_SET[CHAR_BIT+1] = {
		0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF
	};
	static BitSequence gc_MaskLSB_SET[CHAR_BIT+1] = {
		0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0xFF
	};
	static BitSequence gc_MaskMSB_CLEAR[CHAR_BIT+1] = {
		0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00
	};
	static BitSequence gc_MaskLSB_CLEAR[CHAR_BIT+1] = {
		0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x00
	};
	static const char g_BitSequenceFormatString[] = "%2.2X";
#endif /* #if 8 == WaMM_BitsPerBitSequence */
#if 16 == CHAR_BIT
	static BitSequence g_MaskSetBit[CHAR_BIT] = {
		0x8000, 0x4000, 0x2000, 0x1000, 0x0800, 0x0400, 0x0200, 0x0100, 
		0x0080, 0x0040, 0x0020, 0x0010, 0x0008, 0x0004, 0x0002, 0x0001
	};
	static BitSequence g_MaskClearBit[CHAR_BIT] = {
		0x7FFF, 0xBFFF, 0xDFFF, 0xEFFF, 0xF7FF, 0xFBFF, 0xFDFF, 0xFEFF, 
		0xFF7F, 0xFFBF, 0xFFDF, 0xFFEF, 0xFFF7, 0xFFFB, 0xFFFD, 0xFFFE
	};
	static BitSequence gc_MaskMSB_SET[CHAR_BIT+1] = {
		0x0000, 0x8000, 0xC000, 0xE000, 0xF000, 0xF800, 0xFC00, 0xFE00, 0xFF00, 
			0xFF80, 0xFFC0, 0xFFE0, 0xFFF0, 0xFFF8, 0xFFFC, 0xFFFE, 0xFFFF
	};
	static BitSequence gc_MaskLSB_SET[CHAR_BIT+1] = {
		0x0000, 0x0001, 0x0003, 0x0007, 0x000F, 0x001F, 0x003F, 0x007F, 0x00FF, 
			0x01FF, 0x03FF, 0x07FF, 0x0FFF, 0x1FFF, 0x3FFF, 0x7FFF, 0xFFFF

	};
	static BitSequence gc_MaskMSB_CLEAR[CHAR_BIT+1] = {
		0xFFFF, 0x7FFF, 0x3FFF, 0x1FFF, 0x0FFF, 0x07FF, 0x03FF, 0x01FF, 0x00FF, 
			0x007F, 0x003F, 0x001F, 0x000F, 0x0007, 0x0003, 0x0001, 0x0000
	};
	static BitSequence gc_MaskLSB_CLEAR[CHAR_BIT+1] = {
		0xFFFF, 0xFFFE, 0xFFFC, 0xFFF8, 0xFFF0, 0xFFE0, 0xFFC0, 0xFF80, 0xFF00, 
			0xFE00, 0xFC00, 0xF800, 0xF000, 0xE000, 0xC000, 0x8000, 0x0000
	};

	static const char g_BitSequenceFormatString[] = "%4.4X";
#endif /* #if 16 == CHAR_BIT */
#if 32 == CHAR_BIT
	static BitSequence g_MaskSetBit[CHAR_BIT] = {
		0x80000000, 0x40000000, 0x20000000, 0x10000000, 0x08000000, 0x04000000, 0x02000000, 0x01000000, 
		0x00800000, 0x00400000, 0x00200000, 0x00100000, 0x00080000, 0x00040000, 0x00020000, 0x00010000, 
		0x00008000, 0x00004000, 0x00002000, 0x00001000, 0x00000800, 0x00000400, 0x00000200, 0x00000100, 
		0x00000080, 0x00000040, 0x00000020, 0x00000010, 0x00000008, 0x00000004, 0x00000002, 0x00000001
	};
	static BitSequence g_MaskClearBit[CHAR_BIT] = {
		0x7FFFFFFF, 0xBFFFFFFF, 0xDFFFFFFF, 0xEFFFFFFF, 0xF7FFFFFF, 0xFBFFFFFF, 0xFDFFFFFF, 0xFEFFFFFF, 
		0xFF7FFFFF, 0xFFBFFFFF, 0xFFDFFFFF, 0xFFEFFFFF, 0xFFF7FFFF, 0xFFFBFFFF, 0xFFFDFFFF, 0xFFFEFFFF, 
		0xFFFF7FFF, 0xFFFFBFFF, 0xFFFFDFFF, 0xFFFFEFFF, 0xFFFFF7FF, 0xFFFFFBFF, 0xFFFFFDFF, 0xFFFFFEFF, 
		0xFFFFFF7F, 0xFFFFFFBF, 0xFFFFFFDF, 0xFFFFFFEF, 0xFFFFFFF7, 0xFFFFFFFB, 0xFFFFFFFD, 0xFFFFFFFE
	};
	static BitSequence gc_MaskMSB_SET[CHAR_BIT+1] = {
		0x00000000, 0x80000000, 0xC0000000, 0xE0000000, 0xF0000000, 0xF8000000, 0xFC000000, 0xFE000000, 0xFF000000, 
			    0xFF800000, 0xFFC00000, 0xFFE00000, 0xFFF00000, 0xFFF80000, 0xFFFC0000, 0xFFFE0000, 0xFFFF0000, 
			    0xFFFF8000, 0xFFFFC000, 0xFFFFE000, 0xFFFFF000, 0xFFFFF800, 0xFFFFFC00, 0xFFFFFE00, 0xFFFFFF00, 
			    0xFFFFFF80, 0xFFFFFFC0, 0xFFFFFFE0, 0xFFFFFFF0, 0xFFFFFFF8, 0xFFFFFFFC, 0xFFFFFFFE, 0xFFFFFFFF
	};
	static BitSequence gc_MaskLSB_SET[CHAR_BIT+1] = {
		0x00000000, 0x00000001, 0x00000003, 0x00000007, 0x0000000F, 0x0000001F, 0x0000003F, 0x0000007F, 0x000000FF, 
			    0x000001FF, 0x000003FF, 0x000007FF, 0x00000FFF, 0x00001FFF, 0x00003FFF, 0x00007FFF, 0x0000FFFF, 
			    0x0001FFFF, 0x0003FFFF, 0x0007FFFF, 0x000FFFFF, 0x001FFFFF, 0x003FFFFF, 0x007FFFFF, 0x00FFFFFF, 
			    0x01FFFFFF, 0x03FFFFFF, 0x07FFFFFF, 0x0FFFFFFF, 0x1FFFFFFF, 0x3FFFFFFF, 0x7FFFFFFF, 0xFFFFFFFF
	};
	static BitSequence gc_MaskMSB_CLEAR[CHAR_BIT+1] = {
		0xFFFFFFFF, 0x7FFFFFFF, 0x3FFFFFFF, 0x1FFFFFFF, 0x0FFFFFFF, 0x07FFFFFF, 0x03FFFFFF, 0x01FFFFFF, 0x00FFFFFF, 
			    0x007FFFFF, 0x003FFFFF, 0x001FFFFF, 0x000FFFFF, 0x0007FFFF, 0x0003FFFF, 0x0001FFFF, 0x0000FFFF, 
			    0x00007FFF, 0x00003FFF, 0x00001FFF, 0x00000FFF, 0x000007FF, 0x000003FF, 0x000001FF, 0x000000FF, 
			    0x0000007F, 0x0000003F, 0x0000001F, 0x0000000F, 0x00000007, 0x00000003, 0x00000001, 0x00000000
	};
	static BitSequence gc_MaskLSB_CLEAR[CHAR_BIT+1] = {
		0xFFFFFFFF, 0xFFFFFFFE, 0xFFFFFFFC, 0xFFFFFFF8, 0xFFFFFFF0, 0xFFFFFFE0, 0xFFFFFFC0, 0xFFFFFF80, 0xFFFFFF00, 
			    0xFFFFFE00, 0xFFFFFC00, 0xFFFFF800, 0xFFFFF000, 0xFFFFE000, 0xFFFFC000, 0xFFFF8000, 0xFFFF0000, 
			    0xFFFE0000, 0xFFFC0000, 0xFFF80000, 0xFFF00000, 0xFFE00000, 0xFFC00000, 0xFF800000, 0xFF000000, 
			    0xFE000000, 0xFC000000, 0xF8000000, 0xF0000000, 0xE0000000, 0xC0000000, 0x80000000, 0x00000000
	};

	static const char g_BitSequenceFormatString[] = "%8.8lX";
#endif /* #if 32 == CHAR_BIT */

const long gc_lMinBufferSize = 0x1000;
const long gc_lBufferIncrement = 0x1000;

#define BYTES_NEEDED(a) (((a) + CHAR_BIT -1) / WaMM_BitsPerBitSequence)


WaMMHashReturn InitStruct(BitArray *pArray)
{
	WaMMHashReturn bSuccess = WaMM_FAIL;
	if (NULL != pArray)
	{
		pArray->lNumBits = 0;
		pArray->lNumBytes = 0;
		pArray->lBufferSize = 0;
		pArray->pBitArrayData = NULL;
		pArray->pBuffer = NULL;
	}

	return bSuccess;
}

void FreeBitArray(BitArray *pArray)
{
	if (NULL != pArray)
	{
		if (NULL != pArray->pBuffer)
		{
#if 1 <= WAMM_MEMORY_DEBUGGING
		printf("FreeBitArray()\t BitBuffer freed at\t0x%16.16llX\n", (unsigned long long) pArray->pBuffer);
#endif
			free(pArray->pBuffer);
		}
#if 1 <= WAMM_MEMORY_DEBUGGING
		printf("FreeBitArray()\t BitArray freed at\t0x%16.16llX\n", (unsigned long long) pArray);
#endif
		free(pArray);
	}
}

BitArray *BitArrayFromBitSequence(long lNumBitsNew, const BitSequence *pInitSequence)
{
	BitArray *pNewBitArray = NewBitArray(lNumBitsNew);
	div_t ByteCounts;
	BitSequence SourceBitSeq;
	BitSequence bitVal;
	BitSequence *pDestBitSeq;
	int iBitPos;

	if (NULL != pNewBitArray)	/* malloc went bad */
	{
		if (0 < lNumBitsNew)	/* If there is no sequence to transfer then the empty BitArray is the proper result */
		{
			if (NULL != pInitSequence)
			{
				ByteCounts = div(lNumBitsNew, WaMM_BitsPerBitSequence);
				if (0 < ByteCounts.quot)
				{
					memmove(pNewBitArray->pBitArrayData, pInitSequence, ByteCounts.quot);
				}

				/* transfer odd bits from Source to Destination if needed */
				if (0 < ByteCounts.rem)
				{
					pDestBitSeq  = &(pNewBitArray->pBitArrayData[ByteCounts.quot]);
					SourceBitSeq = pInitSequence[ByteCounts.quot];
					*pDestBitSeq = 0;
					for(iBitPos = 0; iBitPos < ByteCounts.rem; ++iBitPos)
					{
						bitVal = SourceBitSeq & gc_MaskSetBit[iBitPos];
						if (bitVal)
						{
							*pDestBitSeq |= gc_MaskSetBit[iBitPos];
						}
					}
				}
			}
			else
			{
				_WaMM_ErrorCode = WaMM_ERORR_ERORR_NULL_BITSEQUENCE;
				FreeBitArray(pNewBitArray);
				pNewBitArray = NULL;
			}
		}
	}
	else
	{
		; /* NULL Statement.  The value of _WaMM_ErrorCode was set by call to NewBitArray () */
	}

	return pNewBitArray;
}

long CalcBufferSize( long lNumBitsNeeded)
{
	long lBufferSize = gc_lMinBufferSize;

	if ( 0 < lNumBitsNeeded)
	{
		lBufferSize = (BYTES_NEEDED(lNumBitsNeeded) / gc_lBufferIncrement) + 1;	/* num blocks */
		lBufferSize *= gc_lBufferIncrement;
		if (lBufferSize < gc_lMinBufferSize)
		{
			lBufferSize = gc_lMinBufferSize;
		}
	}

	return lBufferSize;
}

WaMMHashReturn AllocateBuffer(BitArray *pArray, long lNumBitsNeeded)
{
	WaMMHashReturn retVal = WaMM_FAIL;

	if (NULL != pArray)
	{
		/* initialize the buffer */
		if (NULL != pArray->pBuffer)
		{
#if 1 <= WAMM_MEMORY_DEBUGGING
		printf("AllocatedBuffer()\t BitArray buffer freed at\t0x%16.16llX\n", (unsigned long long) pArray->pBuffer);
#endif
			free(pArray->pBuffer);
		}
		pArray->pBuffer = NULL;
		pArray->lBufferSize = 0;
		
		if (0 <= lNumBitsNeeded)
		{
			/* how big should the allocated buffer be */
			pArray->lBufferSize = CalcBufferSize(lNumBitsNeeded);
// EACIRC: manual edit:   pArray->pBuffer = malloc(pArray->lBufferSize); ->  pArray->pBuffer = (BitSequence*) malloc(pArray->lBufferSize);
            pArray->pBuffer = (BitSequence*) malloc(pArray->lBufferSize);
			if ( NULL != pArray->pBuffer)
			{
#if 1 <= WAMM_MEMORY_DEBUGGING
		printf("AllocatedBuffer()\t BitArray buffer Allocated at\t0x%16.16llX\n", (unsigned long long) pArray->pBuffer);
#endif
				memset(pArray->pBuffer, 0, pArray->lBufferSize);
				retVal = WaMM_SUCCESS;
			}
			else
			{
#if 1 <= WAMM_MEMORY_DEBUGGING
		printf("NewBitArray()\t Buffer allocation failed.\n");
#endif
				pArray->lBufferSize = 0;
				retVal = WaMM_ERORR_MALLOC_BITARRAY;
			}
		}
		else
		{
			retVal = WaMM_ERORR_NEGATVELEN_BITARRAY;
		}
	}
	else
	{
		retVal = WaMM_ERORR_NULL_BITARRAY;
	}

	return retVal;

}

BitArray *NewBitArray(long lNumBitsNew)
{
	BitArray *pNewBitArray = NULL;
	WaMMHashReturn retVal = WaMM_FAIL;

	if (0 <= lNumBitsNew)
	{
// EACIRC: manual edit: pNewBitArray = malloc(sizeof(BitArray)); -> pNewBitArray = (BitArray*) malloc(sizeof(BitArray));
        pNewBitArray = (BitArray*) malloc(sizeof(BitArray));
		if (NULL != pNewBitArray)
		{
			InitStruct(pNewBitArray);

			retVal = AllocateBuffer(pNewBitArray, lNumBitsNew);
			if (WaMM_SUCCESS == retVal)
			{
				pNewBitArray->pBitArrayData = pNewBitArray->pBuffer;
				pNewBitArray->lNumBits = lNumBitsNew;
				pNewBitArray->lNumBytes = BYTES_NEEDED(lNumBitsNew);
			}
			else
			{
#if 1 <= WAMM_MEMORY_DEBUGGING
		printf("NewBitArray()\t Buffer allocation failed. BitArray freed at\t0x%16.16llX\n", (unsigned long long) pNewBitArray);
#endif
				retVal = _WaMM_ErrorCode = WaMM_ERORR_MALLOC_BITARRAY;
				free(pNewBitArray);
				pNewBitArray = NULL;
			}
		}
		else
		{
			retVal = WaMM_ERORR_MALLOC_BITARRAY;
		}
	}
	else
	{
		retVal = WaMM_ERORR_NEGATVELEN_BITARRAY;
	}

	/* since this function does not return an error code. Store the error code in a global variable */
	_WaMM_ErrorCode = retVal;

	return pNewBitArray;
}

BitArray *BitArrayFromBitArray(const BitArray *pSourceBitArray)
{
	BitArray *pClonedBitArray = NULL;
	div_t ByteCounts;

	if (NULL != pSourceBitArray)
	{
		pClonedBitArray = NewBitArray(pSourceBitArray->lNumBits);
		if (NULL != pClonedBitArray)
		{
			if (0 < pSourceBitArray->lNumBits)
			{
				memmove(pClonedBitArray->pBitArrayData, pSourceBitArray->pBitArrayData, pSourceBitArray->lNumBytes);
				ByteCounts = div(pClonedBitArray->lNumBits, WaMM_BitsPerBitSequence);
				if (0 < ByteCounts.rem)
				{
					pClonedBitArray->pBitArrayData[pClonedBitArray->lNumBytes-1] &= gc_MaskMSB_SET[ByteCounts.rem];
				}
			}
		}
		else
		{
			; /* NULL statement.  Error code set by call to NewBitArray() */
		}
	}
	else
	{
		_WaMM_ErrorCode = WaMM_ERORR_NULL_BITARRAY;
	}

	return pClonedBitArray;
}

WaMMHashReturn CloneBitArray(BitArray *pClonedBitArray, const BitArray *pSourceBitArray)
{
	WaMMHashReturn retVal = WaMM_FAIL;

	if ((NULL != pClonedBitArray) &&
		(NULL != pSourceBitArray))
	{
		if (pSourceBitArray != pClonedBitArray)
		{
			/* Decide if Reallocation of the destination data buffer is needed */
			retVal = WaMM_SUCCESS;
			if (pClonedBitArray->lBufferSize < pSourceBitArray->lNumBytes)
			{
				retVal = AllocateBuffer(pClonedBitArray, pSourceBitArray->lNumBits);
				if (WaMM_SUCCESS == retVal)
				{
#if 1 <= WAMM_MEMORY_DEBUGGING
					printf("CloneBitArray()\t BitArray Buffer realloc. Buffer allocated at \t0x%16.16llX\n", (unsigned long long) pClonedBitArray->pBuffer);
#endif
				}
				else
				{
					InitStruct(pClonedBitArray);
					retVal = WaMM_ERORR_REALLOC_BITARRAY;
				}
			}

			/* if all is still good with the destination, then copy data from source */
			if (WaMM_SUCCESS == retVal)
			{
				pClonedBitArray->lNumBits  = pSourceBitArray->lNumBits;
				pClonedBitArray->lNumBytes = pSourceBitArray->lNumBytes;
				pClonedBitArray->pBitArrayData = pClonedBitArray->pBuffer;
				memset(pClonedBitArray->pBuffer, 0, pClonedBitArray->lBufferSize);
				memmove(pClonedBitArray->pBitArrayData, pSourceBitArray->pBitArrayData, pClonedBitArray->lNumBytes);
			}
		}
		else
		{
			retVal = WaMM_ERORR_NULL_ALIAS_ERROR;
		}
	}
	else
	{
		retVal = WaMM_ERORR_NULL_BITARRAY;
	}

	return retVal;
}


/* very dangerous function since ther is no check on the spac in the character buffer */
void BitArrayToSring(BitArray *pBitArray, char *szString)
{
	int iNumBytes;
	size_t iNumFormattedChars;
	BitSequence *pBits;
	char *pCurrChar = szString;

	if (NULL != szString)
	{
		if (NULL != pBitArray)
		{
			sprintf(pCurrChar, "(%d-[", pBitArray->lNumBits);
			iNumFormattedChars = strlen(pCurrChar);
			pCurrChar += iNumFormattedChars;

			pBits = pBitArray->pBitArrayData;
			for(iNumBytes = pBitArray->lNumBytes; 0 < iNumBytes; --iNumBytes)
			{
				sprintf(pCurrChar, g_BitSequenceFormatString, *pBits);
				iNumFormattedChars = strlen(pCurrChar);
				pCurrChar += iNumFormattedChars;
				++pBits;
			}

			sprintf(pCurrChar, "])");
			iNumFormattedChars = strlen(pCurrChar);
			pCurrChar += iNumFormattedChars;
		}
		else
		{
			sprintf(pCurrChar, "[NULL]");
		}
		*pCurrChar = (char) 0;
	}
}

WaMMHashReturn ConcatenateBitArray(BitArray *pDestArray, const BitArray *pSourceArray)
{
	WaMMHashReturn bSuccess = WaMM_FAIL;
	long lOddDestBits;
	long lOddSourceBits;
	long lNewBitArrayLength;
	long lNewBytesNeeded;
	long lNewBufferSize;
	long lMemOffset;
	long lBytesRemaining;
	long lBytesToConcatenate;
	BitSequence currentSourceByte;
	BitSequence *pNewBuffer;
	BitSequence *pConcatenationPoint;
	BitSequence *pStartByteOfSourceBuffer;
	BitSequence *pEndByteOfDestinationBuffer;

	if (NULL != pSourceArray)
	{
		if (0 < pSourceArray->lNumBits)
		{
			/* if the destination is empty then concatation is actually cloning */
			if (0 < pDestArray->lNumBits)
			{
				if ((NULL != pDestArray) &&
					(NULL != pSourceArray->pBitArrayData ))
				{
					lMemOffset = pDestArray->pBitArrayData - pDestArray->pBuffer;
					lBytesRemaining = pDestArray->lBufferSize - lMemOffset;
					lNewBitArrayLength = pDestArray->lNumBits + pSourceArray->lNumBits;
					lNewBytesNeeded = BYTES_NEEDED(lNewBitArrayLength);

					/* see if Re-Allocation is needed */
					if (lBytesRemaining < lNewBytesNeeded)
                    {
						lNewBufferSize = CalcBufferSize(lNewBitArrayLength);
// EACIRC: manual edit: pNewBuffer = malloc(lNewBufferSize); -> pNewBuffer = (BitSequence*) malloc(lNewBufferSize);
                        pNewBuffer = (BitSequence*) malloc(lNewBufferSize);

	#if 1 <= WAMM_MEMORY_DEBUGGING
			printf("ConcatenateBitArray()\t BitArray Buffer realloc'd from\t0x%16.16llX to \t0x%16.16llX\n", (unsigned long long) pDestArray->pBuffer, (unsigned long long) pNewBuffer);
	#endif
						if (NULL != pNewBuffer)
						{
							if (pNewBuffer != pDestArray->pBuffer)
							{
								memset(pNewBuffer, 0, lNewBufferSize);
								memmove(pNewBuffer, pDestArray->pBitArrayData, pDestArray->lNumBytes);
								if (NULL != pDestArray->pBuffer)
								{
	#if 1 <= WAMM_MEMORY_DEBUGGING
			printf("ConcatenateBitArray()\t BitArray Buffer freed at \t0x%16.16llX\n", (unsigned long long) pDestArray->pBuffer);
	#endif
									free(pDestArray->pBuffer);
								}

								/* Use the newly reallocated buffer in place of the old buffer 
								 * and updata the data pointer.
								 */
								pDestArray->pBuffer = pNewBuffer;
								pDestArray->pBitArrayData = &(pDestArray->pBuffer[lMemOffset]);
							}
						}
						else
						{
							if (NULL != pDestArray->pBuffer)
							{
	#if 1 <= WAMM_MEMORY_DEBUGGING
			printf("ConcatenateBitArray()\t BitArray Buffer realloc'd failed.  Old bit buffer freed at\t0x%16.16llX\n", (unsigned long long) pDestArray->pBuffer);
	#endif
								free(pDestArray->pBuffer);
							}
							InitStruct(pDestArray);

							bSuccess = WaMM_ERORR_REALLOC_BITARRAY;
						}
					}

					/* The destination buffer can only be NULL if the call to realloc() failed.
					 * begin the actual process of concatenating the Source bit array to the end 
					 * of the Destination bit array.
					 */
					if (NULL != pDestArray->pBitArrayData)
					{
						/* does the bit array in the destination end on an even Byte Boundary? */
						lOddDestBits = pDestArray->lNumBits % WaMM_BitsPerBitSequence;
						if (0 < lOddDestBits)
						{
							/* Have to concatenate via shift and masking methods */
							lOddSourceBits = WaMM_BitsPerBitSequence - lOddDestBits;

							pStartByteOfSourceBuffer = pSourceArray->pBitArrayData;
							pEndByteOfDestinationBuffer = &(pDestArray->pBitArrayData[pDestArray->lNumBytes-1]);
							
							for( lBytesToConcatenate = pSourceArray->lNumBytes; 0 < lBytesToConcatenate; --lBytesToConcatenate)
							{
								currentSourceByte = *pStartByteOfSourceBuffer;

								*pEndByteOfDestinationBuffer &= gc_MaskMSB_SET[lOddDestBits];
								*pEndByteOfDestinationBuffer |= currentSourceByte >> lOddDestBits;
								++pEndByteOfDestinationBuffer;

								currentSourceByte <<= lOddSourceBits;
								*pEndByteOfDestinationBuffer = currentSourceByte;

								++pStartByteOfSourceBuffer;
							}
						}
						else
						{
							/* Can concatenate via memmove because the end of the destination and the start of the source
							 * Bit arrays are on proper memory boundaries.
							*/
							pConcatenationPoint = &(pDestArray->pBitArrayData[pDestArray->lNumBytes]);
							memmove(pConcatenationPoint, pSourceArray->pBitArrayData, pSourceArray->lNumBytes);
						}

						pDestArray->lNumBits = lNewBitArrayLength;
						pDestArray->lNumBytes = lNewBytesNeeded;
						bSuccess = WaMM_SUCCESS;
					}
					else
					{
						; /* NULL statemennt.  The only way to get here is if the call to realloc has failed.
						   * if that is the case, then the return value is properly set and it is time to return.
						   */
					}
				}
 				else
				{
					/* data pointers are invalid */
					bSuccess = WaMM_ERORR_NULL_BITARRAY;
				}
			}
			else
			{
				bSuccess = CloneBitArray(pDestArray, pSourceArray);
			}
		}
		else
		{
			/* if the bit array to be concantenated is empty then there is 
			 * nothing to do and the destination BitArray is unchanged 
			 */
			bSuccess = WaMM_SUCCESS;
		}
	}
	else
	{
		/* if the bit array to be concantenated is empty then there is 
		 * nothing to do and the destination BitArray is unchanged 
		 */
		bSuccess = WaMM_SUCCESS;
	}

	return bSuccess;
}
BitSequence *GetBitSequence(const BitArray *pDestArray)
{
	BitSequence *pCopyOfBitSequence = NULL;
	long lNumBytes;

	if (NULL != pDestArray)
	{
		lNumBytes = pDestArray->lNumBytes;
		if (0 <= lNumBytes)
		{
			if (0 == lNumBytes)
			{
				++lNumBytes;
			}

// EACIRC: manual edit: pCopyOfBitSequence = malloc(lNumBytes); -> pCopyOfBitSequence = (BitSequence*) malloc(lNumBytes);
            pCopyOfBitSequence = (BitSequence*) malloc(lNumBytes);
			if (NULL != pCopyOfBitSequence)
			{
#if 1 <= WAMM_MEMORY_DEBUGGING
		printf("GetBitSequence()\t BitSequence Buffer allocated at\t0x%16.16llX to \t0x%16.16llX\n", ((unsigned long long) pCopyOfBitSequence));
#endif
				memset(pCopyOfBitSequence, 0, lNumBytes);
				if (NULL != pDestArray->pBitArrayData)
				{
					memmove(pCopyOfBitSequence, pDestArray->pBitArrayData, lNumBytes);
				}
			}
			else
			{
				_WaMM_ErrorCode = WaMM_ERORR_MALLOC_BITARRAY;
			}
		}
		else
		{
			/* this should never happen */
			_WaMM_ErrorCode = WaMM_ERORR_NEGATVELEN_BITARRAY;
		}
	}
	else
	{
		_WaMM_ErrorCode = WaMM_ERORR_NULL_BITARRAY;
	}
	return pCopyOfBitSequence;
}

BitSequence *ConcatenateTwoBitSequences(const long lBitLen1, const BitSequence *pBitSequence1, const long lBitLen2, const BitSequence *pBitSequence2)
{
	BitSequence *pReturnSequence = NULL;
	WaMMHashReturn hrResult;
	BitArray *pBitArray[2];

	pBitArray[0] = BitArrayFromBitSequence(lBitLen1, pBitSequence1);
	pBitArray[1] = BitArrayFromBitSequence(lBitLen2, pBitSequence2);
	if ((NULL != pBitArray[0]) &&
		(NULL != pBitArray[1]))
	{
		hrResult = ConcatenateBitArray(pBitArray[0], pBitArray[1]);
		if (WaMM_SUCCESS == hrResult)
		{
			pReturnSequence = GetBitSequence(pBitArray[0]);
		}
	}
	else
	{
		_WaMM_ErrorCode = WaMM_ERORR_MALLOC_BITARRAY;
	}

	if (NULL != pBitArray[0])
	{
		FreeBitArray(pBitArray[0]);
		pBitArray[1] = NULL;
	}
	if (NULL != pBitArray[1])
	{
		FreeBitArray(pBitArray[1]);
		pBitArray[1] = NULL;
	}

	return pReturnSequence;
}

WaMMHashReturn EmptyBitArray(BitArray *pBitArray)
{
	WaMMHashReturn retVal = WaMM_FAIL;

	if (NULL != pBitArray)
	{
		/* since all the data in the BitArray has been transfered to the DataVector, set the BitArray to empty 
		 */
		pBitArray->lNumBits = 0;
		pBitArray->lNumBytes = 0;
		pBitArray->pBitArrayData = pBitArray->pBuffer;
		memset(pBitArray->pBuffer, 0, pBitArray->lBufferSize);

		retVal = WaMM_SUCCESS;
	}
	else
	{
		retVal = WaMM_ERORR_NULL_BITARRAY;
	}

	return retVal;
}

WaMMHashReturn PopWholeVectorFromBitArray(BitArray *pBitArray, WaMM_DataVector UnloadedVector)
{
	WaMMHashReturn retVal = WaMM_FAIL;

	if (NULL != pBitArray)
	{
		InitWaMMVector(UnloadedVector);

		/* is the bit sequence contains at least one full DataVector */
		if (WaMM_VectorLength_Bits <= pBitArray->lNumBits)
		{
			memmove(UnloadedVector, pBitArray->pBitArrayData, WaMM_VectorLength_Bytes);

			pBitArray->lNumBits  -= WaMM_VectorLength_Bits;
			pBitArray->lNumBytes -= WaMM_VectorLength_Bytes;
			pBitArray->pBitArrayData += WaMM_VectorLength_BitSequences;

			if (0 < pBitArray->lNumBits)
			{
				retVal = WaMM_SUCCESS;
			}
			else
			{				
				retVal = EmptyBitArray(pBitArray);
			}
		}
		else
		{
			retVal = WaMM_WARNING_BITSEQUENCE_TOO_SMALL;
		}
	}
	else
	{
		retVal = WaMM_ERORR_NULL_BITARRAY;
	}
	return retVal;
}

WaMMHashReturn PopPartialVectorFromBitArray(BitArray *pBitArray, WaMM_DataVector UnloadedVector)
{
	WaMMHashReturn retVal = WaMM_FAIL;
	int iOddBits;

	if (NULL != pBitArray)
	{
		InitWaMMVector(UnloadedVector);

		/* is the bit sequence longer than one DataVector */
		if (WaMM_VectorLength_Bits > pBitArray->lNumBits)
		{
			if (0 < pBitArray->lNumBits)
			{
				/* If the bit array does not end on a byte boundary, then clean up the last bits in the last byte. */
				iOddBits = pBitArray->lNumBits % WaMM_BitsPerByte; 
				if (0 < iOddBits)
				{
					pBitArray->pBitArrayData[pBitArray->lNumBytes-1] &= gc_MaskMSB_SET[iOddBits];
				}
				memmove(UnloadedVector, pBitArray->pBitArrayData, pBitArray->lNumBytes);
			}

			/* everything is in the DataVector. Set this to the empty bit Array*/
			retVal = EmptyBitArray(pBitArray);
		}
		else
		{
			retVal = WaMM_WARNING_BITSEQUENCE_TOO_LARGE;
		}
	}
	else
	{
		retVal = WaMM_ERORR_NULL_BITARRAY;
	}
	return retVal;
}

WaMMHashReturn BitArrayShiftLeft(BitArray *pBitArray, long lNumBitsShifted)
{
	WaMMHashReturn retVal = WaMM_FAIL;
	div_t divQuotient;
	long lByteIndex;
	long lBitsRight;
	long lBytesToCopy;
	BitSequence *pStartPoint;
	BitSequence CurrByte;
	BitSequence NextByte;

	if (0 > lNumBitsShifted)
	{
		retVal = BitArrayShiftRight(pBitArray, -lNumBitsShifted);
	}
	else
	{
		if (0 < lNumBitsShifted)
		{
			if (NULL != pBitArray)
			{
				/* If shifting past all the data, the result is a BitArray of zeros */
				if (pBitArray->lNumBits <= lNumBitsShifted)
				{
					memset(pBitArray->pBitArrayData, 0, pBitArray->lNumBytes);
					retVal = WaMM_SUCCESS;
				}
				else
				{
					/* Shift Whole Bytes if possible */
					divQuotient = div(lNumBitsShifted, WaMM_BitsPerBitSequence);
					lBytesToCopy = pBitArray->lNumBytes - divQuotient.quot;
					if ( 0 < divQuotient.quot)
					{
						/* use memmove() to do the shifting */
						pStartPoint = &(pBitArray->pBitArrayData[divQuotient.quot]);
						memmove(pBitArray->pBitArrayData, 
								pStartPoint,
								lBytesToCopy);

						/* use memset() fill the right bits with zeroes*/
						pStartPoint = &(pBitArray->pBitArrayData[lBytesToCopy]);
						memset(pStartPoint, 0, divQuotient.quot);
					}

					/* if odd bits need to be shifted, then shift the needed bits from byte to by via shift and mask*/
					if (0 < divQuotient.rem)
					{
						/* shift in the most significant bits of the next byte into the least significant bits of the current byte. */
						lBitsRight = WaMM_BitsPerBitSequence - divQuotient.rem;
						for(lByteIndex = 0; lByteIndex < lBytesToCopy-1; ++lByteIndex)
						{
							CurrByte = pBitArray->pBitArrayData[lByteIndex];
							NextByte = pBitArray->pBitArrayData[lByteIndex+1];

							CurrByte <<= divQuotient.rem;
							NextByte >>= lBitsRight;
							NextByte &= gc_MaskLSB_SET[divQuotient.rem];
							CurrByte |= NextByte;

							pBitArray->pBitArrayData[lByteIndex] = CurrByte;
						}
						pBitArray->pBitArrayData[lByteIndex] <<= divQuotient.rem;
						pBitArray->pBitArrayData[lByteIndex] &= gc_MaskMSB_SET[lBitsRight];
					}
					retVal = WaMM_SUCCESS;
				}
			}
			else
			{
				retVal = WaMM_ERORR_NULL_BITARRAY;
			}
		}
		else
		{
			/* nothing to to. So we are done. */
			retVal = WaMM_SUCCESS;
		}
	}
	return retVal;
}

WaMMHashReturn BitArrayShiftRight(BitArray *pBitArray, long lNumBitsShifted)
{
	WaMMHashReturn retVal = WaMM_FAIL;
	div_t divQuotient;
	long lByteIndex;
	long lBitsLeft;
	long lBytesToCopy;
	BitSequence *pStartPoint;
	BitSequence CurrByte;
	BitSequence PrevByte;

	if (0 > lNumBitsShifted)
	{
		retVal = BitArrayShiftRight(pBitArray, -lNumBitsShifted);
	}
	else
	{
		if (0 < lNumBitsShifted)
		{
			if (NULL != pBitArray)
			{
				/* If shifting past all the data, the result is a BitArray of zeros */
				if (pBitArray->lNumBits <= lNumBitsShifted)
				{
					memset(pBitArray->pBitArrayData, 0, pBitArray->lNumBytes);
					retVal = WaMM_SUCCESS;
				}
				else
				{
					/* Shift Whole Bytes if possible */
					divQuotient = div(lNumBitsShifted, WaMM_BitsPerBitSequence);
					lBytesToCopy = pBitArray->lNumBytes - divQuotient.quot;
					if ( 0 < divQuotient.quot)
					{
						/* use memmove() to do the shifting */
						pStartPoint = &(pBitArray->pBitArrayData[divQuotient.quot]);
						memmove(pStartPoint,
							   pBitArray->pBitArrayData,
							   lBytesToCopy);
						/* use memset() fill the right bits with zeroes*/
						memset(pBitArray->pBitArrayData, 0, divQuotient.quot);
					}
					if (0 < divQuotient.rem)
					{
						/* shift in the Least significant bits of the previous byte into the Most significant bits of the current byte. */
						lBitsLeft = WaMM_BitsPerBitSequence - divQuotient.rem;
						for(lByteIndex = pBitArray->lNumBytes - 1; pBitArray->lNumBytes - lBytesToCopy < lByteIndex; --lByteIndex)
						{
							CurrByte = pBitArray->pBitArrayData[lByteIndex];
							PrevByte = pBitArray->pBitArrayData[lByteIndex-1];

							CurrByte >>= divQuotient.rem;
							PrevByte <<= lBitsLeft;
							PrevByte &= gc_MaskMSB_SET[divQuotient.rem];
							CurrByte |= PrevByte;

							pBitArray->pBitArrayData[lByteIndex] = CurrByte;
						}
						pBitArray->pBitArrayData[lByteIndex] >>= divQuotient.rem;
						pBitArray->pBitArrayData[lByteIndex] &= gc_MaskLSB_SET[lBitsLeft];
					}
					retVal = WaMM_SUCCESS;
				}
			}
			else
			{
				retVal = WaMM_ERORR_NULL_BITARRAY;
			}
		}
		else
		{
			/* nothing to to. So we are done. */
			retVal = WaMM_SUCCESS;
		}
	}

	return retVal;
}



