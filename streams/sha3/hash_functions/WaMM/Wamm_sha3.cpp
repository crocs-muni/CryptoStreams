#include "Wamm_sha3.h"
extern "C" {
#include "WaMM.h"
#include "BitArray.h"
}

const long gc_WaMM_SIZE_MessageMemoryBlock_Bytes = 0x100000;	/* process data in 1 megabyte chunks whenever possible */
      long gc_WaMM_SIZE_MessageMemoryBlock_Bits  = 0x800000;	/* (gc_WaMM_SIZE_MessageMemoryBlock_Bytes * WaMM_BitsPerBitSequence) */

int WaMM::Init(int hashbitlen)
{
	int retVal = WaMM_FAIL;
	int i, j;

	retVal = InitWaMMEnvironment();
	if (WaMM_SUCCESS == retVal)
	{
		if (IsHashLengthSupported(hashbitlen))
		{
			if (NULL != &wammState)
			{
				/* set the hash length variable of ths structure */
				wammState.hashbitlen = hashbitlen;

				/* set the Vectors to Size to zero */
				retVal = WaMM_SUCCESS;
				if (WaMM_SUCCESS == retVal)
				{
					retVal = InitWaMMVector(wammState.MessageLength);
				}
				if (WaMM_SUCCESS == retVal)
				{
					retVal = InitWaMMVector(wammState.InputVector);
				}
				
				if (WaMM_SUCCESS == retVal)
				{
					/* Set the buffer and data length to a state of: 
					 *		there are no yet-to-be-hashed bits left over from previous calls to Update()
					 */
					wammState.pUnprocessedBitArray = NewBitArray(0);
					if (NULL != wammState.pUnprocessedBitArray)
					{
						/* use the bytes of the WaMM binary operator to initialize the state matrix. */
						for (i = 0; i < WaMM_MatrixDimension; ++i)
						{
							for (j = 0; j < WaMM_MatrixDimension; ++j)
							{
								 wammState.stateMatrix[i][j] = gc_WaMMOperator[i][j];
							}
						}

						/* set the other variables of this structure */
						wammState.BitsProcessed = 0;
						wammState.iCurrRow = 0;

						retVal = WaMM_SUCCESS;
					}
					else
					{
						retVal = _WaMM_ErrorCode;
					}
				}
			}
			else
			{
				retVal = WaMM_ERORR_NULL_STATE_POINTER;
			}
		}
		else
		{
			retVal = WaMM_BAD_HASHLENGTH;
		}
	}

	if (g_bDumpIntermediateStates &&
		(WaMM_SUCCESS == retVal))
	{
		sprintf(_WaMM_ErrorMesssage, "\fInitialized Hash State");
		WaMM_DumpState(&wammState, _WaMM_ErrorMesssage);
	}

	return retVal;
}

int WaMM::Update(const BitSequence *pData, DataLength databitlen)
{
	int retVal = WaMM_FAIL;
	const BitSequence *pNextBlock = pData;
	DataLength llBitsRemaining = databitlen;

	/* a negative bit length is an error */
	if (0 > databitlen)
	{
		retVal = WaMM_ERORR_NEGATVELEN_BITARRAY;
	}
	else if (0 < databitlen)
	{
		if (NULL != &wammState)
		{
			if (IsHashLengthSupported(wammState.hashbitlen))
			{
				if (NULL != pData)
				{
					retVal = WaMM_SUCCESS;
					wammState.BitsProcessed = 0;

					/* process the incoming data in large, but smaller chunks */
					while ((WaMM_SUCCESS == retVal) && 
						   (gc_WaMM_SIZE_MessageMemoryBlock_Bytes <= llBitsRemaining))
					{
						retVal = UpdateOneMemoryBlock(&wammState, pNextBlock, gc_WaMM_SIZE_MessageMemoryBlock_Bits, wammNumRounds);
						pNextBlock += gc_WaMM_SIZE_MessageMemoryBlock_Bytes;
						llBitsRemaining -= (DataLength) gc_WaMM_SIZE_MessageMemoryBlock_Bits;
					}

					if (WaMM_SUCCESS == retVal)
					{
						/* If there is still some incoming data, then process the last chunk */
						if (0 < llBitsRemaining)
						{
							assert(llBitsRemaining <= LONG_MAX);
							retVal = UpdateOneMemoryBlock(&wammState, pNextBlock, (long) llBitsRemaining, wammNumRounds);
						}
					}

					if (WaMM_SUCCESS == retVal)
					{
						retVal = AddToMessageSize(&wammState, wammState.BitsProcessed);
					}
				}
				else
				{
					retVal = WaMM_ERORR_NULL_HASHDATA_POINTER;
				}
			}
			else
			{
				retVal = WaMM_BAD_HASHLENGTH;
			}
		}
		else
		{
			retVal = WaMM_ERORR_NULL_STATE_POINTER;
		}
	}
	else
	{
		/* 
		 * Process any data still awaiting to be hashed.
		 */
		retVal = WaMM_SUCCESS;
	}

	return retVal;
}

int WaMM::Final(BitSequence *pHashval)
{
	int retVal = WaMM_FAIL;
	int iNumHashBytes;
	int iIndex;
	int iPrimeOffset;
	int iPrimeMultiplier;
	int n, r;
	div_t stateIndex;

	if (NULL != &wammState)
	{
		if (NULL != pHashval)
		{
			if (IsHashLengthSupported(wammState.hashbitlen))
			{
				if (g_bDumpIntermediateStates)
				{
					fprintf(g_fileDumpIntermediate, "\fBegin Finalization\n");
				}

				/* process any remaining unprocessed message bits */
				if (0 < wammState.pUnprocessedBitArray->lNumBits)
				{
					if (g_bDumpIntermediateStates)
					{
						fprintf(g_fileDumpIntermediate, "Process Reminder of Message\n");
					}
					retVal = AddToMessageSize(&wammState, wammState.pUnprocessedBitArray->lNumBits);
					if (WaMM_SUCCESS == retVal)
					{
						retVal = PopPartialVectorFromBitArray( wammState.pUnprocessedBitArray, wammState.InputVector);
						if (WaMM_SUCCESS == retVal)
						{
							retVal = WaMMRoundFunction(&wammState, wammNumRounds);
						}
					}
				}
				else
				{
					retVal = WaMM_SUCCESS;
				}
				if (WaMM_SUCCESS == retVal)
				{
					if (g_bDumpIntermediateStates)
					{
						fprintf(g_fileDumpIntermediate, "Process Message length\n");
					}
					/* Process Message length in bits as if it were another block of data */
					memmove(wammState.InputVector, 
						    wammState.MessageLength, 
							WaMM_VectorLength_BitSequences);
					retVal = WaMMRoundFunction(&wammState, wammNumRounds);
					if (WaMM_SUCCESS == retVal)
					{
						/* If the round function from processing the message length did not invoke a squaring
						 * perform a squaring of the StateMatrix to insure the message length and the last bits
						 * of the incoming message are mixed into the state matrix.
						 */
						if (0 != wammState.iCurrRow)
						{
							retVal = WaMMSquare(&wammState, wammNumRounds);
						}
						if (WaMM_SUCCESS == retVal)
						{
							/*
							 * tap the bytes of the state matrix to form the N-byte hash value.
							 */
							iNumHashBytes = wammState.hashbitlen / WaMM_BitsPerByte;
							iIndex = wammState.hashbitlen / WaMM_HashLen_BitLengthModulus;
							iPrimeOffset = gc_TappingPrime[iIndex];
							iPrimeMultiplier = gc_TappingPrime[iIndex - 2];

							if (g_bDumpIntermediateStates)
							{
								fprintf(g_fileDumpIntermediate, "\fTapping Sequence:");
								fprintf(g_fileDumpIntermediate, "Number of bytes in hash value = %d\n", iNumHashBytes);
								fprintf(g_fileDumpIntermediate, "iIndex = floor(%d / %d) = %d\n", wammState.hashbitlen, WaMM_HashLen_BitLengthModulus, iIndex);
								fprintf(g_fileDumpIntermediate, "r = ((%d * n) + %d ) mod %d\n", iPrimeMultiplier, iPrimeOffset, WaMM_NumBytesStateMatrix);
							}

							for(n = 0; n < iNumHashBytes; ++n)
							{
								r  = n*iPrimeMultiplier + iPrimeOffset;
								r %= WaMM_NumBytesStateMatrix;
								stateIndex = div(r, WaMM_MatrixDimension);
								pHashval[n] = wammState.stateMatrix[stateIndex.quot][stateIndex.rem];

								if (g_bDumpIntermediateStates)
								{
									fprintf(g_fileDumpIntermediate, "[n,r] = [%3d, %3d].  HashByte[%3d] = StateMatrix[%2d][%2d] = 0x%2.2X\n", n,r, n, stateIndex.quot, stateIndex.rem, pHashval[n]);
								}
							}

							/* The hashing is done and so is the need for the allocated BitArray */
							if (NULL != wammState.pUnprocessedBitArray);
							{
								/* Dispose of exising Bit Array if Present */
								FreeBitArray(wammState.pUnprocessedBitArray);
							}
							wammState.pUnprocessedBitArray = NULL;
						}
					}
				}
			}
			else
			{
				retVal = WaMM_BAD_HASHLENGTH;
			}
		}
		else
		{
			retVal = WaMM_ERORR_NULL_HASHVALUE_POINTER;
		}
	}
	else
	{
		retVal = WaMM_ERORR_NULL_STATE_POINTER;
	}

	if (g_bDumpIntermediateStates &&
		(WaMM_SUCCESS == retVal))
	{
		sprintf(_WaMM_ErrorMesssage, "\fFinal Hash State");
		WaMM_DumpState(&wammState, _WaMM_ErrorMesssage);
	}

	return retVal;
}

int WaMM::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
	int retVal = WaMM_FAIL;
	//WaMMhashState State;
	int i;

	if (IsHashLengthSupported(hashbitlen))
	{
		if (NULL != hashval)
		{
			if (NULL != data)
			{
				retVal = WaMM::Init(hashbitlen);
				if (WaMM_SUCCESS == retVal)
				{
#if 2 <= WAMM_DEBUGGING_LEVEL
		WaMM_DumpState(&wammState, "\fInitialization");
#endif

					retVal = WaMM::Update(data, databitlen);
					if (WaMM_SUCCESS == retVal)
					{
#if 2 <= WAMM_DEBUGGING_LEVEL
		WaMM_DumpState(&wammState, "\fUpdate");
#endif
						if (WaMM_SUCCESS == retVal)
						{
							retVal = WaMM::Final(hashval);
#if 2 <= WAMM_DEBUGGING_LEVEL
		WaMM_DumpState(&wammState, "\fFinalized");
#endif
						}
					}
				}
			}
			else
			{
				retVal = WaMM_ERORR_NULL_INPUT_POINTER;
			}
		}
		else
		{
			retVal = WaMM_ERORR_NULL_HASHVALUE_POINTER;
		}
	}
	else
	{
		retVal = WaMM_ERORR_NULL_STATE_POINTER;
	}

#if 2 <= WAMM_DEBUGGING_LEVEL
	if (WaMM_SUCCESS == retVal)
	{
		printf("[");
		for(i = 0; i < hashbitlen / WaMM_BitsPerByte; ++i)
		{
			printf("%2.2X ", hashval[i]);
		}
		printf("] %d-Bit HashValue\n", hashbitlen);
	}
#endif
#if 1 <= WAMM_MEMORY_DEBUGGING
		printf("Hash()\tFinished\n\n");
#endif

	return retVal;
}

WaMM::WaMM(const int numRounds) {
	if ((numRounds == -1) || (numRounds > 2)) {
		wammNumRounds = WaMM_NumberOfRounds;
	} else {
		wammNumRounds = numRounds;
	}
}