/*
 * WaMM.H ($RCSfile: WaMM.h,v $)
 *
 * Primary Header file for the reference implimentation of the WaMM hash Algorithm
 *
 * The ANSI C compliant way to implement the Microsoft specific directive, #pragma once, is us use
 * the #ifndef/#define/#endif a definition guard
 * The GUID is just to make the pre_compiler directive as unique as possible.
 *
 * Copyright 2008 John Washburn.  All rights reserved until I win the SHA3 contest.

$Header: D:\\RCSArchive\\D\\Programming\\WaMM\\ReferenceImplementation\\WaMM\\WaMM\\WaMM.h,v 1.7 2008-10-19 08:25:49-05 johnwashburn Exp johnwashburn $
$Revision: 1.7 $

$Log: WaMM.h,v $
Revision 1.7  2008-10-19 08:25:49-05  johnwashburn
Daily Checkpointing

Revision 1.6  2008-10-13 18:13:55-05  johnwashburn
Daily Check Point

Revision 1.2  2008-10-08 02:28:26-05  johnwashburn
Created proper pairs of header/source files for the reference and definition of global variables.

Revision 1.1  2008-10-08 02:08:03-05  johnwashburn
Added RCS Header Comment block using RCS Keywords.


 */

#ifndef WaMM_h_4D2DF387_53E7_4231_8F6A_20EF4A08E8F7
#define WaMM_h_4D2DF387_53E7_4231_8F6A_20EF4A08E8F7 1

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <memory.h>
#if defined(__APPLE__)			// EACIRC: manual edit: apple compatibility
#  include <malloc/malloc.h>
#else
#  include <malloc.h>
#endif
#include <time.h>
#include <assert.h>
//#include "SHA3API.h"
#include "WaMMConstants.h"
#include "WaMMOperator.h"
#include "WaMMTappingPrimes.h"
#include "WaMMErrorMessage.h"

/*
 * enumerate the hash lengths supported
 */
typedef enum WaMMhashLength {
	WAMM_192 = 192
,	WAMM_224 = 224
,	WAMM_256 = 256
,	WAMM_288 = 288
,	WAMM_320 = 320
,	WAMM_352 = 352
,	WAMM_384 = 384
,	WAMM_416 = 416
,	WAMM_448 = 448
,	WAMM_480 = 480
,	WAMM_512 = 512
,	WAMM_544 = 544
,	WAMM_576 = 576
,	WAMM_608 = 608
,	WAMM_640 = 640
,	WAMM_672 = 672
,	WAMM_704 = 704
,	WAMM_736 = 736
,	WAMM_768 = 768
,	WAMM_800 = 800
,	WAMM_832 = 832
,	WAMM_864 = 864
,	WAMM_896 = 896
,	WAMM_928 = 928
,	WAMM_960 = 960
,	WAMM_992 = 992
,	WAMM_1024 = 1024
,	WAMM_1056 = 1056
,	WAMM_1088 = 1088
,	WAMM_1120 = 1120
,	WAMM_1152 = 1152
,	WAMM_1184 = 1184
,	WAMM_1216 = 1216
,	WAMM_1248 = 1248
,	WAMM_1280 = 1280
,	WAMM_1312 = 1312
,	WAMM_1344 = 1344
,	WAMM_1376 = 1376
,	WAMM_1408 = 1408
,	WAMM_1440 = 1440
,	WAMM_1472 = 1472
,	WAMM_1504 = 1504
,	WAMM_1536 = 1536
,	WAMM_1568 = 1568
,	WAMM_1600 = 1600
,	WAMM_1632 = 1632
,	WAMM_1664 = 1664
,	WAMM_1696 = 1696
,	WAMM_1728 = 1728
,	WAMM_1760 = 1760
,	WAMM_1792 = 1792
,	WAMM_1824 = 1824
,	WAMM_1856 = 1856
,	WAMM_1888 = 1888
,	WAMM_1920 = 1920
,	WAMM_1952 = 1952
,	WAMM_1984 = 1984
,	WAMM_2016 = 2016
,	WAMM_2048 = 2048
} WaMMhashLength;

WaMMHashReturn InitWaMMEnvironment(void);
WaMMHashReturn WaMM_DumpState(WaMMhashState *pState, char *szMessagePrefix);

BOOL IsHashLengthSupported(int hashbitlen);
WaMMHashReturn InitWaMMVector(WaMM_DataVector pVector);
WaMMHashReturn CopyWaMMVector(WaMM_DataVector pDestinationVector, WaMM_DataVector pSourceVector);
WaMMHashReturn UpdateOneMemoryBlock(WaMMhashState *pState, const BitSequence *pData, long lBitsInBlock, const int rounds);

WaMMHashReturn WaMMRoundFunction(WaMMhashState *pState, const int rounds);
WaMMHashReturn AddToMessageSize(WaMMhashState *pState, DataLength databitlen);
WaMMHashReturn WaMMSquare(WaMMhashState *pState, const int rounds);
WaMMHashReturn WaMMUpdateStateMatrix(WaMMhashState *pState);

/* Set Debugging Levels */
#undef WAMM_DEBUGGING_LEVEL
#undef WAMM_MEMORY_DEBUGGING
#ifdef _DEBUG
	#define WAMM_DEBUGGING_LEVEL 0	/* Debugging levels 0-4. 0 Is off 4 is uber verbose */
	#define WAMM_MEMORY_DEBUGGING 0 /* Debugging levels 0-2. 0 Is off 2 is uber verbose */
#endif /* #ifdef _DEBUG*/

/* Create refernece o Global Variable for dumping intermediate states */
extern BOOL g_bDumpIntermediateStates;
extern FILE *g_fileDumpIntermediate;

#endif /* #ifndef WaMM_h_4D2DF387_53E7_4231_8F6A_20EF4A08E8F7 */