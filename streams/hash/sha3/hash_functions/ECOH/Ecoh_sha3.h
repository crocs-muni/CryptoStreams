#ifndef ECOH_SHA3_H
#define ECOH_SHA3_H

#include "../../sha3_interface.h"
#include "ecoh.h"

class Ecoh : public SHA3 {

/*****************************************************************************/
/*
/*	API Function Calls as Specified in ANSI C Cryptography API Profile
/*	for SHA-3 Candadite Algorithm Submissions, Rev 5, 11 Febraury 2008.
/*
/*****************************************************************************/
typedef	unsigned char		EcohBitSequence;
typedef unsigned long long	EcohDataLength;
typedef enum { ECOH_SUCCESS = 0, ECOH_FAIL = 1, ECOH_BAD_HASHBITLEN = 2} EcohHashReturn;
/*****************************************************************************/
/*
/*	Platform-specific staet structures
/*
/*****************************************************************************/
typedef struct
{
	ecoh_uint		type;
	ecoh_uint		DATASIZE;
	ecoh_uint		DIGESTSIZE;
	ecoh_uint		blen;
	ecoh_uint		clen;
	EcohDataLength	counter[2];
	EcohDataLength	mlen[2];
	ecoh_uint		N[ECOH_MAX_DATASIZE];
	ecoh_point		P;
	ecoh_point		Q;
	ecoh_curve		*T;
}ecohHashState;

private:
ecohHashState ecohState;

public:
int	Init(int hashbitlen);
int	Update(const EcohBitSequence *data, EcohDataLength databitlen);
int	Final(EcohBitSequence *hashval);
int	Hash(int hashbitlen, const EcohBitSequence *data, EcohDataLength databitlen, EcohBitSequence *hashval);

private:
void	PrintInt2(ecoh_uint sz, ecoh_uint *op);
void	PrintInt(ecoh_uint sz, ecoh_uint *op);
void Unload(unsigned char *rop, ecoh_uint *op, ecoh_uint count);
void FormatData(ecoh_uint *buffer, ecoh_uint count);
void BitReverse(ecoh_uint *buffer, ecoh_uint count);
int		IsInt(ecoh_uint *op, ecoh_uint sz, ecoh_uint ui);
ecoh_uint	GetBitLength(ecoh_uint sz, ecoh_uint *op);
void	PolyShiftRight(ecoh_uint *rop, ecoh_uint *op, ecoh_uint sz);
void	PolyAdd(ecoh_uint *rop, ecoh_uint *op1, ecoh_uint *op2, ecoh_uint sz);
void	PolyModT571(ecoh_uint *rop, ecoh_uint *op);
void	PolyMulMod(ecoh_uint *rop, ecoh_uint *op1, ecoh_uint *op2, ecoh_uint sz);
void	PolySqrMod(ecoh_uint *rop, ecoh_uint *op, ecoh_uint sz);
void	PolyInv(ecoh_uint *rop, ecoh_uint *op, ecoh_uint *fx, ecoh_uint sz);
void PointDouble(ecoh_point *rop, ecoh_point *op, ecoh_curve *T);
void PointAdd(ecoh_point *rop, ecoh_point *op1, ecoh_point *op2, ecoh_curve *T);
void	BasePointMultiply(ecoh_point *P, ecoh_uint sz, ecoh_uint *k, ecoh_curve *T);
void SolveQuadratic(ecoh_uint *rop, ecoh_uint *op, ecoh_uint sz);
ecoh_uint	PointDecompress(ecoh_point *rop, ecoh_curve *T);
ecoh_uint	Transform();

};

#endif