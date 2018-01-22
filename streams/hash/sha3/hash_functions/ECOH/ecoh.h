/* $Id: ecoh.h,v 1.2 2008/10/30 15:58:53 aantipa Exp $ */
/* $Source: /cvs/ca/eng/ecoh/src/Optimized_32bit/ecoh.h,v $ */
/*****************************************************************************/
/*
/*	ecoh.h			Header file for ECOH hash
/*
/*	DATE:			28 August 2008
/*
/*****************************************************************************/
#include <stdio.h>
#include <memory.h>
#include <time.h>

#ifndef _ECOH_H_
#define _ECOH_H_
/*****************************************************************************/
/*
/*	Multi-platform constants and MACROS
/*
/*****************************************************************************/
#define ECOH224_DATASIZE		16
#define ECOH224_DIGESTSIZE		28
#define ECOH256_DATASIZE		16
#define ECOH256_DIGESTSIZE		32
#define ECOH384_DATASIZE		24 
#define ECOH384_DIGESTSIZE		48
#define ECOH512_DATASIZE		32
#define ECOH512_DIGESTSIZE		64

#define ECOH_YP_BIT_T283				255
#define ECOH_YP_BIT_T409				319
#define ECOH_YP_BIT_T571				511

#define	ECOH224					224
#define ECOH256					256
#define	ECOH384					384
#define ECOH512					512

//#define uchar					unsigned char

#define	ECOH_DATASTART				(16/sizeof(ecoh_uint))
#define ECOH_COUNTERSIZE				(8/sizeof(ecoh_uint))

#define ECOH_TRACET409(OP)		((OP)[0]&0x01)
/*****************************************************************************/
/*
/*	Platform specific constants and MACROS
/*
/*****************************************************************************/
#ifdef _x64_

#define ECOH_LIMB_SIZE			8
#define ECOH_LIMB_BIT_SIZE			64
#define ECOH_LIMB_NIB_LENGTH			16
#define ECOH_LIMB_LGBITS_SIZE		6
#define ECOH_TOPBIT				0x8000000000000000

#define ECOH_SECT283_SIZE			5
#define ECOH_SECT409_SIZE			7
#define ECOH_SECT571_SIZE			9
#define ECOH_MAX_SIZE			9


#define ECOH224_CLEN			1
#define ECOH256_CLEN			1
#define ECOH384_CLEN			1
#define ECOH512_CLEN			2

#define ECOH_MAX_DATASIZE			4

#define	ecoh_uint				unsigned long long
#define	ecoh_sint				long long

#define ECOH_TRACET283(OP)		(((OP)[0]^((OP)[4]>>15))&0x01)
#define ECOH_TRACET571(OP)		(((OP)[0]^((OP)[8]>>49)^((OP)[8]>>57))&0x01)
#define ECOH_GETBIT(OP, I)		((((OP)[(I)/64]&(ECOH_BIT[(I)%64]))>>((I)%64)))
#define ECOH_SZ_PRINTINT			"0x%016I64X, "

#else

#define ECOH_LIMB_SIZE			4
#define ECOH_LIMB_BIT_SIZE			32
#define ECOH_LIMB_NIB_LENGTH			8
#define ECOH_LIMB_LGBITS_SIZE		5
#define ECOH_TOPBIT				0x80000000

#define ECOH_SECT283_SIZE			9

#define ECOH_SECT409_SIZE			13
#define ECOH_SECT571_SIZE			18
#define ECOH_MAX_SIZE			18

#define ECOH224_CLEN			2
#define ECOH256_CLEN			2
#define ECOH384_CLEN			2
#define ECOH512_CLEN			4

#define ECOH_MAX_DATASIZE			8

#define ecoh_uint				unsigned int
#define ecoh_sint				signed int

#define ECOH_TRACET283(OP)		(((OP)[0]^((OP)[8]>>15))&0x01) 
#define ECOH_TRACET571(OP)		(((OP)[0]^((OP)[17]>>17)^((OP)[17]>>25))&0x01)
#define ECOH_GETBIT(OP, I)		((((OP)[(I)/32]&(ECOH_BIT[(I)%32]))>>((I)%32)))
#define ECOH_SZ_PRINTINT			"0x%08X, "

#endif
/*****************************************************************************/
/*
/*	Multi-platform data structures
/*
/*****************************************************************************/
typedef struct
{
	ecoh_uint	x[ECOH_MAX_SIZE];
	ecoh_uint	y[ECOH_MAX_SIZE];
}ecoh_point;

typedef struct
{
	ecoh_uint	sz;
	ecoh_uint	a;
	ecoh_uint	*b;
	ecoh_uint	*N;
	ecoh_uint	*f;
	ecoh_point	*G;
}ecoh_curve;

#endif //_ECOH_H_
