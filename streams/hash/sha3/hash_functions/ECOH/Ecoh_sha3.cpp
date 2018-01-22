#include "Ecoh_sha3.h"

#define ECOH_SHIFT_RIGHT_19(x18,x17,x16,x15,x14,x13,x12,x11,x10,x9,x8,x7,x6,x5,x4,x3,x2,x1,x0) \
(x0)  = (((ecoh_uint)x0)  >> 1) + (((ecoh_uint)x1)  << (ECOH_LIMB_BIT_SIZE-1)); \
(x1)  = (((ecoh_uint)x1)  >> 1) + (((ecoh_uint)x2)  << (ECOH_LIMB_BIT_SIZE-1)); \
(x2)  = (((ecoh_uint)x2)  >> 1) + (((ecoh_uint)x3)  << (ECOH_LIMB_BIT_SIZE-1)); \
(x3)  = (((ecoh_uint)x3)  >> 1) + (((ecoh_uint)x4)  << (ECOH_LIMB_BIT_SIZE-1)); \
(x4)  = (((ecoh_uint)x4)  >> 1) + (((ecoh_uint)x5)  << (ECOH_LIMB_BIT_SIZE-1)); \
(x5)  = (((ecoh_uint)x5)  >> 1) + (((ecoh_uint)x6)  << (ECOH_LIMB_BIT_SIZE-1)); \
(x6)  = (((ecoh_uint)x6)  >> 1) + (((ecoh_uint)x7)  << (ECOH_LIMB_BIT_SIZE-1)); \
(x7)  = (((ecoh_uint)x7)  >> 1) + (((ecoh_uint)x8)  << (ECOH_LIMB_BIT_SIZE-1)); \
(x8)  = (((ecoh_uint)x8)  >> 1) + (((ecoh_uint)x9)  << (ECOH_LIMB_BIT_SIZE-1)); \
(x9)  = (((ecoh_uint)x9)  >> 1) + (((ecoh_uint)x10) << (ECOH_LIMB_BIT_SIZE-1)); \
(x10) = (((ecoh_uint)x10) >> 1) + (((ecoh_uint)x11) << (ECOH_LIMB_BIT_SIZE-1)); \
(x11) = (((ecoh_uint)x11) >> 1) + (((ecoh_uint)x12) << (ECOH_LIMB_BIT_SIZE-1)); \
(x12) = (((ecoh_uint)x12) >> 1) + (((ecoh_uint)x13) << (ECOH_LIMB_BIT_SIZE-1)); \
(x13) = (((ecoh_uint)x13) >> 1) + (((ecoh_uint)x14) << (ECOH_LIMB_BIT_SIZE-1)); \
(x14) = (((ecoh_uint)x14) >> 1) + (((ecoh_uint)x15) << (ECOH_LIMB_BIT_SIZE-1)); \
(x15) = (((ecoh_uint)x15) >> 1) + (((ecoh_uint)x16) << (ECOH_LIMB_BIT_SIZE-1)); \
(x16) = (((ecoh_uint)x16) >> 1) + (((ecoh_uint)x17) << (ECOH_LIMB_BIT_SIZE-1)); \
(x17) = (((ecoh_uint)x17) >> 1) + (((ecoh_uint)x18) << (ECOH_LIMB_BIT_SIZE-1)); \
(x18)          >>= 1;

#define ECOH_SHIFT_LEFT_19(x18,x17,x16,x15,x14,x13,x12,x11,x10,x9,x8,x7,x6,x5,x4,x3,x2,x1,x0) \
(x18) = (((ecoh_uint)x18) << 1) + (((ecoh_uint)x17) >> (ECOH_LIMB_BIT_SIZE-1)); \
(x17) = (((ecoh_uint)x17) << 1) + (((ecoh_uint)x16) >> (ECOH_LIMB_BIT_SIZE-1)); \
(x16) = (((ecoh_uint)x16) << 1) + (((ecoh_uint)x15) >> (ECOH_LIMB_BIT_SIZE-1)); \
(x15) = (((ecoh_uint)x15) << 1) + (((ecoh_uint)x14) >> (ECOH_LIMB_BIT_SIZE-1)); \
(x14) = (((ecoh_uint)x14) << 1) + (((ecoh_uint)x13) >> (ECOH_LIMB_BIT_SIZE-1)); \
(x13) = (((ecoh_uint)x13) << 1) + (((ecoh_uint)x12) >> (ECOH_LIMB_BIT_SIZE-1)); \
(x12) = (((ecoh_uint)x12) << 1) + (((ecoh_uint)x11) >> (ECOH_LIMB_BIT_SIZE-1)); \
(x11) = (((ecoh_uint)x11) << 1) + (((ecoh_uint)x10) >> (ECOH_LIMB_BIT_SIZE-1)); \
(x10) = (((ecoh_uint)x10) << 1) + (((ecoh_uint)x9)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x9)  = (((ecoh_uint)x9)  << 1) + (((ecoh_uint)x8)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x8)  = (((ecoh_uint)x8)  << 1) + (((ecoh_uint)x7)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x7)  = (((ecoh_uint)x7)  << 1) + (((ecoh_uint)x6)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x6)  = (((ecoh_uint)x6)  << 1) + (((ecoh_uint)x5)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x5)  = (((ecoh_uint)x5)  << 1) + (((ecoh_uint)x4)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x4)  = (((ecoh_uint)x4)  << 1) + (((ecoh_uint)x3)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x3)  = (((ecoh_uint)x3)  << 1) + (((ecoh_uint)x2)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x2)  = (((ecoh_uint)x2)  << 1) + (((ecoh_uint)x1)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x1)  = (((ecoh_uint)x1)  << 1) + (((ecoh_uint)x0)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x0)           <<= 1;

#define ECOH_SHIFT_RIGHT_10(x9,x8,x7,x6,x5,x4,x3,x2,x1,x0) \
(x0) = (((ecoh_uint)x0) >> 1) + (((ecoh_uint)x1) << (ECOH_LIMB_BIT_SIZE-1)); \
(x1) = (((ecoh_uint)x1) >> 1) + (((ecoh_uint)x2) << (ECOH_LIMB_BIT_SIZE-1)); \
(x2) = (((ecoh_uint)x2) >> 1) + (((ecoh_uint)x3) << (ECOH_LIMB_BIT_SIZE-1)); \
(x3) = (((ecoh_uint)x3) >> 1) + (((ecoh_uint)x4) << (ECOH_LIMB_BIT_SIZE-1)); \
(x4) = (((ecoh_uint)x4) >> 1) + (((ecoh_uint)x5) << (ECOH_LIMB_BIT_SIZE-1)); \
(x5) = (((ecoh_uint)x5) >> 1) + (((ecoh_uint)x6) << (ECOH_LIMB_BIT_SIZE-1)); \
(x6) = (((ecoh_uint)x6) >> 1) + (((ecoh_uint)x7) << (ECOH_LIMB_BIT_SIZE-1)); \
(x7) = (((ecoh_uint)x7) >> 1) + (((ecoh_uint)x8) << (ECOH_LIMB_BIT_SIZE-1)); \
(x8) = (((ecoh_uint)x8) >> 1) + (((ecoh_uint)x9) << (ECOH_LIMB_BIT_SIZE-1)); \
(x9)         >>= 1;

#define ECOH_SHIFT_LEFT_10(x9,x8,x7,x6,x5,x4,x3,x2,x1,x0) \
(x9)  = (((ecoh_uint)x9)  << 1) + (((ecoh_uint)x8)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x8)  = (((ecoh_uint)x8)  << 1) + (((ecoh_uint)x7)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x7)  = (((ecoh_uint)x7)  << 1) + (((ecoh_uint)x6)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x6)  = (((ecoh_uint)x6)  << 1) + (((ecoh_uint)x5)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x5)  = (((ecoh_uint)x5)  << 1) + (((ecoh_uint)x4)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x4)  = (((ecoh_uint)x4)  << 1) + (((ecoh_uint)x3)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x3)  = (((ecoh_uint)x3)  << 1) + (((ecoh_uint)x2)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x2)  = (((ecoh_uint)x2)  << 1) + (((ecoh_uint)x1)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x1)  = (((ecoh_uint)x1)  << 1) + (((ecoh_uint)x0)  >> (ECOH_LIMB_BIT_SIZE-1)); \
(x0)           <<= 1;
/*****************************************************************************/
/*
/*	external variables required for this file
/*
/*****************************************************************************/

extern "C" ecoh_curve ecoh_sect283r1;
extern "C" ecoh_curve ecoh_sect409r1;
extern "C" ecoh_curve ecoh_sect571r1;

extern "C" unsigned char	ECOH_TBIT[];
extern "C" unsigned char	ECOH_BMASK[];
extern "C" ecoh_uint		ECOH_BIT[];
extern "C" ecoh_uint		ECOH_F2X2[];
extern "C" ecoh_uint		ECOH_F2X22[];
extern "C" ecoh_uint		ECOH_HT283[][ECOH_SECT283_SIZE];
extern "C" ecoh_uint		ECOH_HT409[][ECOH_SECT409_SIZE];
extern "C" ecoh_uint		ECOH_HT571[][ECOH_SECT571_SIZE];
extern "C" ecoh_uint		ECOH_NIB_MASK[];

void	Ecoh::PrintInt2(ecoh_uint sz, ecoh_uint *op)
{	
	ecoh_uint i;
	for(i=sz-1;i!=-1;i--){	printf(ECOH_SZ_PRINTINT, op[i]);	}
}

void	Ecoh::PrintInt(ecoh_uint sz, ecoh_uint *op)
{	
	ecoh_uint i;
	for(i=0;i<sz;i++){	printf(ECOH_SZ_PRINTINT, op[i]);		}
}
/*****************************************************************************/
/*
/*	Some MACROS for setting and adding counters
/*
/*****************************************************************************/
#ifdef _x64_
#define ECOH_SET(OP,UI,LEN)		{	(OP)[0]=(UI); if((LEN)==2){	OP[1]=0;	} }
#define ECOH_SETDL(OP,DL,LEN)	ECOH_SET((OP),(DL),(LEN))
#define ECOH_SETCTR(OP,CTR,LEN)	{	(OP)[0]=(CTR)[0]; if((LEN)==2){	(OP)[1]=(CTR)[1];	} }
#define ECOH_ADD(OP,UI,LEN)		{\
	if((((OP)[0])+=(UI))<(UI)){\
		if((LEN)==2){ if((++((OP)[1]))==0)	return ECOH_FAIL;	}\
		else return ECOH_FAIL;\
	}\
}
#define ECOH_ADDDL(OP,DL,LEN)	( ECOH_ADD((OP),(DL),(LEN))) 

#define ECOH_INCCTR(CTR, LEN)	{ \
	if(++((CTR)[0])==0){	\
		if((LEN)==2){\
			if(++((CTR)[1])==0)	return ECOH_FAIL; \
		}\
		else	return ECOH_FAIL;\
	}\
}
	
#else
#define ECOH_SET(OP,UI,LEN)		{	(OP)[0]=(UI);	(OP)[1]=0;	if((LEN)==4) { (OP)[2]=0; (OP)[3]=0;	} }
#define ECOH_SETDL(OP,DL,LEN)	{	(OP)[0]=(ecoh_uint)((DL)&0xFFFFFFFF);	\
	(OP)[1]=(ecoh_uint)((DL)>>32);	\
	if((LEN)==4)	{ (OP)[2]=0; (OP)[3]=0;	} \
}
#define ECOH_SETCTR(OP,CTR,LEN)	{	ECOH_SETDL((OP), (CTR)[0], 2);	if((LEN)==4)	ECOH_SETDL(&((OP)[2]), ((CTR)[1]), 2);	}
#define ECOH_ADD(OP,UI,LEN)		{\
	if((((OP)[0])+=UI)<(UI)){\
		if((++((OP)[1]))==0){\
			if((LEN)==4){\
				if((++((OP)[2]))==0){\
					if((++((OP)[3]))==0)	return ECOH_FAIL;\
				}}}\
		else	return ECOH_FAIL;\
	}}\

#define ECOH_ADDDL(OP,DL,LEN)	{  \
	ecoh_uint cx = 0;\
	if((((OP)[0])+=(ecoh_uint)((DL)&0xFFFFFFFF))<(ecoh_uint)((DL)&0xFFFFFFFF)){\
		cx = 1;\
		if((((OP)[1])+=cx))  cx = 0;\
	}\
	if((((OP)[1])+=(ecoh_uint)((DL)>>32))<(ecoh_uint)((DL)>>32))	++cx;\
	if((LEN)==4){\
		if((((OP)[2])+=cx)<cx){\
			if((++((OP)[3]))==0)	return ECOH_FAIL;\
		}\
	}\
	else \
		if (cx)	return ECOH_FAIL;\
}
#define ECOH_INCCTR(CTR, LEN)	{ \
	if(++((CTR)[0])==0){	\
		if((LEN)==4){\
			if(++((CTR)[1])==0)	return ECOH_FAIL; \
		}\
		else	return ECOH_FAIL;\
	}\
}
#endif
static const unsigned char ecoh_byterev[256] =  {
     0x00, 0x80,
     0x40, 0xC0,
     0x20, 0xA0,
     0x60, 0xE0,
     0x10, 0x90,
     0x50, 0xD0,
     0x30, 0xB0,
     0x70, 0xF0,
     0x08, 0x88,
     0x48, 0xC8,
     0x28, 0xA8,
     0x68, 0xE8,
     0x18, 0x98,
     0x58, 0xD8,
     0x38, 0xB8,
     0x78, 0xF8,
     0x04, 0x84,
     0x44, 0xC4,
     0x24, 0xA4,
     0x64, 0xE4,
     0x14, 0x94,
     0x54, 0xD4,
     0x34, 0xB4,
     0x74, 0xF4,
     0x0C, 0x8C,
     0x4C, 0xCC,
     0x2C, 0xAC,
     0x6C, 0xEC,
     0x1C, 0x9C,
     0x5C, 0xDC,
     0x3C, 0xBC,
     0x7C, 0xFC,
     0x02, 0x82,
     0x42, 0xC2,
     0x22, 0xA2,
     0x62, 0xE2,
     0x12, 0x92,
     0x52, 0xD2,
     0x32, 0xB2,
     0x72, 0xF2,
     0x0A, 0x8A,
     0x4A, 0xCA,
     0x2A, 0xAA,
     0x6A, 0xEA,
     0x1A, 0x9A,
     0x5A, 0xDA,
     0x3A, 0xBA,
     0x7A, 0xFA,
     0x06, 0x86,
     0x46, 0xC6,
     0x26, 0xA6,
     0x66, 0xE6,
     0x16, 0x96,
     0x56, 0xD6,
     0x36, 0xB6,
     0x76, 0xF6,
     0x0E, 0x8E,
     0x4E, 0xCE,
     0x2E, 0xAE,
     0x6E, 0xEE,
     0x1E, 0x9E,
     0x5E, 0xDE,
     0x3E, 0xBE,
     0x7E, 0xFE,
     0x01, 0x81,
     0x41, 0xC1,
     0x21, 0xA1,
     0x61, 0xE1,
     0x11, 0x91,
     0x51, 0xD1,
     0x31, 0xB1,
     0x71, 0xF1,
     0x09, 0x89,
     0x49, 0xC9,
     0x29, 0xA9,
     0x69, 0xE9,
     0x19, 0x99,
     0x59, 0xD9,
     0x39, 0xB9,
     0x79, 0xF9,
     0x05, 0x85,
     0x45, 0xC5,
     0x25, 0xA5,
     0x65, 0xE5,
     0x15, 0x95,
     0x55, 0xD5,
     0x35, 0xB5,
     0x75, 0xF5,
     0x0D, 0x8D,
     0x4D, 0xCD,
     0x2D, 0xAD,
     0x6D, 0xED,
     0x1D, 0x9D,
     0x5D, 0xDD,
     0x3D, 0xBD,
     0x7D, 0xFD,
     0x03, 0x83,
     0x43, 0xC3,
     0x23, 0xA3,
     0x63, 0xE3,
     0x13, 0x93,
     0x53, 0xD3,
     0x33, 0xB3,
     0x73, 0xF3,
     0x0B, 0x8B,
     0x4B, 0xCB,
     0x2B, 0xAB,
     0x6B, 0xEB,
     0x1B, 0x9B,
     0x5B, 0xDB,
     0x3B, 0xBB,
     0x7B, 0xFB,
     0x07, 0x87,
     0x47, 0xC7,
     0x27, 0xA7,
     0x67, 0xE7,
     0x17, 0x97,
     0x57, 0xD7,
     0x37, 0xB7,
     0x77, 0xF7,
     0x0F, 0x8F,
     0x4F, 0xCF,
     0x2F, 0xAF,
     0x6F, 0xEF,
     0x1F, 0x9F,
     0x5F, 0xDF,
     0x3F, 0xBF,
     0x7F, 0xFF
};
/*****************************************************************************/
/*
/*	START OF ECOH FUNCTIONS AND AUXILARY FUNCTIONS
/*
/*****************************************************************************/
/*
/*	ByteReverse			This function byte reverses limbs for the purpose of
/*						formatting
/*
/*****************************************************************************/
void Ecoh::Unload(unsigned char *rop, ecoh_uint *op, ecoh_uint count)
{
    ecoh_uint	i;
	unsigned char	bval;
	memcpy(rop, (unsigned char*)op, count);
	for(i=0;i<count/2;i++){
		bval = rop[i];
		rop[i] = rop[count-(i+1)];
		rop[count-(i+1)] = bval;
	}
}
/*****************************************************************************/
/*
/*	LimbReverse			This function limb reverses an multi-limb element for
/*						the purpose of formatting
/*
/*****************************************************************************/
void Ecoh::FormatData(ecoh_uint *buffer, ecoh_uint count)
{
	ecoh_uint	value, ctr, i = 0;

	ctr = count;
	
	while((--ctr)>(i)){
		value = buffer[i];
		buffer[i++]=buffer[ctr];
		buffer[ctr] = value;
	}
	while( count-- ){
        value = *buffer;
#ifdef _x64_
        value = ((value&0xFF00FF00FF00FF00L)>>8)|((value&0x00FF00FF00FF00FFL)<<8);
        value =((value&0x0000FFFF0000FFFF)<<16)|((value&0xFFFF0000FFFF0000)>>16);
		*buffer++= (value>>32)|(value<<32);
#else
		value = ((value&0xFF00FF00L)>>8)|((value&0x00FF00FFL)<<8);
        *buffer++ =(value<<16)|(value>>16);
#endif
     }
}
/*****************************************************************************/
/*
/*	BitReverse	This function reverses bits ina an multi-limb element 
/*
/*****************************************************************************/
void Ecoh::BitReverse(ecoh_uint *buffer, ecoh_uint count) 
{
	int i; 
	ecoh_uint reversed, original, temp; 
	char *buf = (char*)buffer; 
	/* do a byte reverse */
	FormatData(buffer, count); 
	/* do a bit reverse in each byte */
	for (i=0; i<count; i++)	{
	/* cmn_wordReverse */
		original = buffer[i]; 
		reversed = ecoh_byterev[original & 0xFF]; 
                original >>= 8;
		temp = ecoh_byterev[original & 0xFF]; 
		reversed += temp << 8; 
                original >>= 8;
		temp = ecoh_byterev[original & 0xFF]; 
                reversed += temp << 16;
                original >>= 8;
		temp = ecoh_byterev[original & 0xFF]; 
                reversed += temp << 24; 
#ifdef _x64_
                original >>= 8;
		temp = ecoh_byterev[original & 0xFF]; 
                reversed += temp << 32; 
                original >>= 8;
		temp = ecoh_byterev[original & 0xFF]; 
                reversed += temp << 40; 
                original >>= 8;
		temp = ecoh_byterev[original & 0xFF]; 
                reversed += temp << 48; 
                original >>= 8;
		temp = ecoh_byterev[original & 0xFF]; 
                reversed += temp << 56; 
		
#endif 

		buffer[i] = reversed; 
	}

}
/*****************************************************************************/
/*
/*	START OF f_{2^m} FUNCTIONS
/*
/*****************************************************************************/
/*
/*	IsInt()		checks to see if int array is = a single int, returns 0 
/*					on false, and 1 on true
/*
/*****************************************************************************/
int		Ecoh::IsInt(ecoh_uint *op, ecoh_uint sz, ecoh_uint ui)
{
	ecoh_uint i;
	if(op[0]!=ui)	return 0;
	for(i=sz-1;i>0;i--){	if(op[i])	return 0;	}
	return 1;
}
/*****************************************************************************/
/*
/*	GetBitLength()		returns the bit length floor(log_2(op))
/*
/*****************************************************************************/
ecoh_uint	Ecoh::GetBitLength(ecoh_uint sz, ecoh_uint *op)
{
	ecoh_sint	i, d;
	ecoh_uint	n;
	if(sz == 0)		return 0;
	d = sz-1;
	while(op[d]==0){	--d;	}
	if(d<0)			return 0;	
	n = op[d];
	i = 0;
	while(n){	n>>=1;	++i;	}
	return (d*(ECOH_LIMB_BIT_SIZE)) + i;
}
/*****************************************************************************/
/*
/*	PolyShiftRight()	This function shifts a polynomial to the right 1 bit
/*						in F_2[z]
/*
/*****************************************************************************/
void	Ecoh::PolyShiftRight(ecoh_uint *rop, ecoh_uint *op, ecoh_uint sz)
{
	ecoh_uint	t1, t2;
	--sz;
	t2 = 0;
	do{
		if(op[sz]&0x01)	t1 = ECOH_TOPBIT;
		else			t1 = 0;
		rop[sz] = (op[sz]>>1)|t2;
		t2 = t1;
	}while(--sz!=(-1));

}
/*****************************************************************************/
/*
/*	PolyAdd()		This function add's to polynomials of the same length in 
/*					F_2[z]
/*
/*****************************************************************************/
void	Ecoh::PolyAdd(ecoh_uint *rop, ecoh_uint *op1, ecoh_uint *op2, ecoh_uint sz)
{
	ecoh_uint i;
	i = -sz;	op1 -=i;	op2 -=i;	rop -=i;
	do{
		rop[i] = op1[i]^op2[i];
	}while(++i!=0);
}
/*****************************************************************************/
/*
/*	PolyModT571()		Performs modular reduction by the polynomial
/*						reduce modulo f(x) = x^571 + x^10 + x^5 + x^2 + 1
/*	
/*						See [1], Algorithm 2.45, page 56
/*
/*****************************************************************************/
void	Ecoh::PolyModT571(ecoh_uint *rop, ecoh_uint *op)
{
	ecoh_uint j, h1;
#ifdef _x64_
	for(j=17;j>8;j--){
		h1 = op[j];
		op[j-9]^=(h1<<5)^(h1<<7)^(h1<<10)^(h1<<15);
		op[j-8]^=(h1>>59)^(h1>>57)^(h1>>54)^(h1>>49);
	}
	h1 = op[8]>>59;
	op[0] ^= h1^(h1<<2)^(h1<<5)^(h1<<10);
	op[8] &=0x07FFFFFFFFFFFFFF;
#else
	for(j=35;j>17;j--){
		h1 = op[j];
		op[j-18]^=(h1<<5)^(h1<<7)^(h1<<10)^(h1<<15);
		op[j-17]^=(h1>>27)^(h1>>25)^(h1>>22)^(h1>>17);
	}
	h1 = op[17]>>27;
	op[0] ^= h1^(h1<<2)^(h1<<5)^(h1<<10);
	op[17] &=0x07FFFFFF;
#endif
	memcpy(rop, op, sizeof(ecoh_uint)*ECOH_SECT571_SIZE);
}
/*****************************************************************************/
/*
/*	PolyMulMod()		modular polynomial multiplication in GF(2)[X]/(f(x))
/*
/*
/*	RESTRICTIONS:		op1, op2 < f(x)
/*
/*****************************************************************************/
void	Ecoh::PolyMulMod(ecoh_uint *rop, ecoh_uint *op1, ecoh_uint *op2, ecoh_uint sz)
#ifdef _x64_
{
    ecoh_uint tempB0,tempB1,tempB2,tempB3,tempB4,tempB5,tempB6,tempB7,tempB8,tempB9;
    ecoh_uint r[18];  /* to store the double size product */
    ecoh_uint mask;
    ecoh_uint* ptr;
    int i;

    tempB0 = op2[0];
    tempB1 = op2[1];
    tempB2 = op2[2];
    tempB3 = op2[3];
    tempB4 = op2[4];
    tempB5 = op2[5];
    tempB6 = op2[6];
    tempB7 = op2[7];
    tempB8 = op2[8];
    tempB9 = (ecoh_uint)0;

    for ( mask = (ecoh_uint) 0; mask < (ecoh_uint) 16; ) {
        r[mask++] = 0;
        r[mask++] = 0;
        r[mask++] = 0;
        r[mask++] = 0;
    }
    r[16] = 0;
    r[17] = 0;

    mask = 1;
    for (;;) 
    {
        i = 8;
        for (;;) 
        {
            ptr = r+i;
            if (op1[i] & mask) {
                ptr[0] ^= tempB0;
                ptr[1] ^= tempB1;
                ptr[2] ^= tempB2;
                ptr[3] ^= tempB3;
                ptr[4] ^= tempB4;
                ptr[5] ^= tempB5;
                ptr[6] ^= tempB6;
                ptr[7] ^= tempB7;
                ptr[8] ^= tempB8;
                ptr[9] ^= tempB9;
            }
            i--;
            /* check condition here for an odd number of loops */
            if (i < 0) break;
            ptr--;
            if (op1[i] & mask) {
                ptr[0] ^= tempB0;
                ptr[1] ^= tempB1;
                ptr[2] ^= tempB2;
                ptr[3] ^= tempB3;
                ptr[4] ^= tempB4;
                ptr[5] ^= tempB5;
                ptr[6] ^= tempB6;
                ptr[7] ^= tempB7;
                ptr[8] ^= tempB8;
                ptr[9] ^= tempB9;
            }
            i--;
            /* check condition here for an even number of loops */
        }

        mask <<= 1;
        if (!mask) break;

        ECOH_SHIFT_LEFT_10(tempB9,tempB8,tempB7,tempB6,tempB5,tempB4,tempB3,tempB2,tempB1,tempB0);
    }

    /* reduce the product to single size and store it in c */
    PolyModT571(rop, r); 
    return;
}
#else
{
    ecoh_uint tempB0,tempB1,tempB2,tempB3,tempB4,tempB5,tempB6,tempB7,tempB8,tempB9,tempB10,tempB11,tempB12,tempB13,tempB14,tempB15,tempB16,tempB17,tempB18;
    ecoh_uint r[36];  /* to store the double size product */
    ecoh_uint mask;
    ecoh_uint* ptr;
    int i;

    tempB0 = op2[0];
    tempB1 = op2[1];
    tempB2 = op2[2];
    tempB3 = op2[3];
    tempB4 = op2[4];
    tempB5 = op2[5];
    tempB6 = op2[6];
    tempB7 = op2[7];
    tempB8 = op2[8];
    tempB9 = op2[9];
    tempB10 = op2[10];
    tempB11 = op2[11];
    tempB12 = op2[12];
    tempB13 = op2[13];
    tempB14 = op2[14];
    tempB15 = op2[15];
    tempB16 = op2[16];
    tempB17 = op2[17];
    tempB18 = (ecoh_uint)0;

    for ( mask = (ecoh_uint) 0; mask < (ecoh_uint) 36; ) {
        r[mask++] = 0;
        r[mask++] = 0;
        r[mask++] = 0;
        r[mask++] = 0;
    }

    mask = 1;
    for (;;) 
    {
        i = 17;
        for (;;) 
        {
            ptr = r+i;
            if (op1[i] & mask) {
                ptr[0] ^= tempB0;
                ptr[1] ^= tempB1;
                ptr[2] ^= tempB2;
                ptr[3] ^= tempB3;
                ptr[4] ^= tempB4;
                ptr[5] ^= tempB5;
                ptr[6] ^= tempB6;
                ptr[7] ^= tempB7;
                ptr[8] ^= tempB8;
                ptr[9] ^= tempB9;
                ptr[10] ^= tempB10;
                ptr[11] ^= tempB11;
                ptr[12] ^= tempB12;
                ptr[13] ^= tempB13;
                ptr[14] ^= tempB14;
                ptr[15] ^= tempB15;
                ptr[16] ^= tempB16;
                ptr[17] ^= tempB17;
                ptr[18] ^= tempB18;
            }
            i--;
            /* check condition here for an odd number of loops */
            ptr--;
            if (op1[i] & mask) {
                ptr[0] ^= tempB0;
                ptr[1] ^= tempB1;
                ptr[2] ^= tempB2;
                ptr[3] ^= tempB3;
                ptr[4] ^= tempB4;
                ptr[5] ^= tempB5;
                ptr[6] ^= tempB6;
                ptr[7] ^= tempB7;
                ptr[8] ^= tempB8;
                ptr[9] ^= tempB9;
                ptr[10] ^= tempB10;
                ptr[11] ^= tempB11;
                ptr[12] ^= tempB12;
                ptr[13] ^= tempB13;
                ptr[14] ^= tempB14;
                ptr[15] ^= tempB15;
                ptr[16] ^= tempB16;
                ptr[17] ^= tempB17;
                ptr[18] ^= tempB18;
            }
            i--;
            /* check condition here for an even number of loops */
            if (i < 0) break;
        }

        mask <<= 1;
        if (!mask) break;

        ECOH_SHIFT_LEFT_19(tempB18,tempB17,tempB16,tempB15,tempB14,tempB13,tempB12,tempB11,tempB10,tempB9,tempB8,tempB7,tempB6,tempB5,tempB4,tempB3,tempB2,tempB1,tempB0);
    }

    /* reduce the product to single size and store it in c */
    PolyModT571(rop, r);
    return;
}
#endif 
/*****************************************************************************/
/*
/*	PolySqrMod()		modular polynomial square in GF(2)[X]/(f(x)) 
/*
/*						performs a modified version of [1], Algorithm 2.39, 
/*						p. 53
/*
/*	RESTRICTIONS:		op < f(x)
/*
/*****************************************************************************/
void	Ecoh::PolySqrMod(ecoh_uint *rop, ecoh_uint *op, ecoh_uint sz)
{
	ecoh_uint	j, out[2*ECOH_MAX_SIZE];
	/*************************************************************************/
	/*
	/*	square polynomial using the ECOH_F2X2 look-up table
	/*
	/*************************************************************************/
	for(j=0;j<sz;j++){
#ifdef _x64_
		out[2*j] = ECOH_F2X2[op[j]&0xFF]|(ECOH_F2X22[(op[j]&0xFF00)>>8]);;
		out[2*j] |= ECOH_F2X22[(op[j]&0xFF0000)>>16]<<16|(ECOH_F2X22[(op[j]&0xFF000000)>>24]<<32);
		out[2*j+1] = ECOH_F2X2[(op[j]&0xFF00000000)>>32]|(ECOH_F2X22[(op[j]&0xFF0000000000)>>40]);;
		out[2*j+1] |= ECOH_F2X22[(op[j]&0xFF000000000000)>>48]<<16|(ECOH_F2X22[(op[j]&0xFF00000000000000)>>56]<<32);
#else
		out[2*j] = ECOH_F2X2[op[j]&0xFF]|(ECOH_F2X22[(op[j]&0xFF00)>>8]);;
		out[2*j+1] = ECOH_F2X2[(op[j]&0xFF0000)>>16]|(ECOH_F2X22[(op[j]&0xFF000000)>>24]);
#endif
	}
        PolyModT571(rop, out);
}	

/*****************************************************************************/
/*
/*	PolyInv()		perform a field inversion in GF(2)[X]/(f(x))
/* 
/*
/*	RESTRICTIONS:		op < f(x)
/*	
/*****************************************************************************/
void	Ecoh::PolyInv(ecoh_uint *rop, ecoh_uint *op, ecoh_uint *fx, ecoh_uint sz)
#ifdef _x64_
{
/* This function requires 4*sbwords+eps of stack storage */

    ecoh_uint temp[18] = {(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0}; /* temp storage */
    int uc = 8;      /* upper comma. */
    int lc = 8;      /* lower comma. */
    int count = 0;
    int i; 
    ecoh_uint w;

    /* upper register (merged) */
    ecoh_uint u9 = (ecoh_uint)0, u8 = (ecoh_uint)0, u7 = (ecoh_uint)0, u6 = (ecoh_uint)0, u5 = (ecoh_uint)0, u4 = (ecoh_uint)0, u3 = (ecoh_uint)0, u2 = (ecoh_uint)0, u1 = (ecoh_uint)0, u0 = (ecoh_uint)0;

    /* lower register (merged) */
    ecoh_uint l9 = (ecoh_uint)0, l8 = (ecoh_uint)0, l7 = (ecoh_uint)0, l6 = (ecoh_uint)0, l5 = (ecoh_uint)0, l4 = (ecoh_uint)0, l3 = (ecoh_uint)0, l2 = (ecoh_uint)0, l1 = (ecoh_uint)0, l0 = (ecoh_uint)0;

    /* multiply element by x^2L */
    temp[sz+8] = op[8];
    temp[sz+7] = op[7];
    temp[sz+6] = op[6];
    temp[sz+5] = op[5];
    temp[sz+4] = op[4];
    temp[sz+3] = op[3];
    temp[sz+2] = op[2];
    temp[sz+1] = op[1];
    temp[sz] = op[0];
    PolyModT571(temp+sz, temp); 
    temp[8] = 0;
    temp[7] = 0;
    temp[6] = 0;
    temp[5] = 0;
    temp[4] = 0;
    temp[3] = 0;
    temp[2] = 0;
    temp[1] = 0;
    temp[0] = 0;
    PolyModT571(temp, temp); 

    /* Final reduce and check to see if the element is zero */
    if ( IsInt(temp, sz, 0)) {
        return;
    }

    u9 = ((ecoh_uint)1) << (ECOH_LIMB_BIT_SIZE - 1);
    l9 = 0;
    /* set the modulus */
    u8 = fx[8]; l8 = temp[8];
    u7 = fx[7]; l7 = temp[7];
    u6 = fx[6]; l6 = temp[6];
    u5 = fx[5]; l5 = temp[5];
    u4 = fx[4]; l4 = temp[4];
    u3 = fx[3]; l3 = temp[3];
    u2 = fx[2]; l2 = temp[2];
    u1 = fx[1]; l1 = temp[1];
    u0 = fx[0]; l0 = temp[0];

    /* We know that the irreducible has a 1 in position 0.
     * Jump into the code that shifts a 1 into position 0 of the lower reg.
     */
    goto AlignLower;

    /* main loop */
    /* select an inner loop based upon the comma positions */
    for (;;)
    {
        /* case upper comma == lower comma */
        if (uc == lc) {
            switch (uc) { /* jump to a specific optimized loop */
                case 8: goto commaU8L8;
                case 7: goto commaU7L7;
                case 6: goto commaU6L6;
                case 5: goto commaU5L5;
                case 4: goto commaU4L4;
                case 3: goto commaU3L3;
                case 2: goto commaU2L2;
                case 1: goto commaU1L1;
                case 0: goto commaU0L0;
            }
        }

        /* case upper comma > lower comma */
        while (uc > lc) {

            switch (uc) { /* xor downto uc+1 */
                 case 0: l1 ^= u1;
                 case 1: l2 ^= u2;
                 case 2: l3 ^= u3;
                 case 3: l4 ^= u4;
                 case 4: l5 ^= u5;
                 case 5: l6 ^= u6;
                 case 6: l7 ^= u7;
                 case 7: l8 ^= u8;
                 case 8: l9 ^= u9;
            }
            switch (lc) { /* xor upto lc */
                 case 8: u8 ^= l8;
                 case 7: u7 ^= l7;
                 case 6: u6 ^= l6;
                 case 5: u5 ^= l5;
                 case 4: u4 ^= l4;
                 case 3: u3 ^= l3;
                 case 2: u2 ^= l2;
                 case 1: u1 ^= l1;
                 case 0: u0 ^= l0;
            }
            /* shift upper reg right until the rightmost bit is one */
            do {
                ECOH_SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
 AlignUpper:
                /* move the comma to the right if we need to */
                switch (uc) {
                    case 8: if  (u8) break; --uc;
                    case 7: if  (u7) break; --uc;
                    case 6: if  (u6) break; --uc;
                    case 5: if  (u5) break; --uc;
                    case 4: if  (u4) break; --uc;
                    case 3: if  (u3) break; --uc;
                    case 2: if  (u2) break; --uc;
                    case 1: if  (u1) break; --uc;
                    case 0: if (!u0) return;
                            if (u0 == (ecoh_uint) 1) {
                                temp[8] = l9;
                                temp[7] = l8;
                                temp[6] = l7;
                                temp[5] = l6;
                                temp[4] = l5;
                                temp[3] = l4;
                                temp[2] = l3;
                                temp[1] = l2;
                                temp[0] = l1;
                                /* use uc as the loop counter */
                                uc = lc;
                                goto doneCommasNotAligned;
                            }
                }
            } while (!(u0 & 1));
        } /* while (uc > lc) */

        /* case upper comma < lower comma */
        while (uc < lc) {

            switch (lc) { /* xor downto lc+1 */
                 case 0: u1 ^= l1;
                 case 1: u2 ^= l2;
                 case 2: u3 ^= l3;
                 case 3: u4 ^= l4;
                 case 4: u5 ^= l5;
                 case 5: u6 ^= l6;
                 case 6: u7 ^= l7;
                 case 7: u8 ^= l8;
                 case 8: u9 ^= l9;
            }
            switch (uc) { /* xor upto uc */
                 case 8: l8 ^= u8;
                 case 7: l7 ^= u7;
                 case 6: l6 ^= u6;
                 case 5: l5 ^= u5;
                 case 4: l4 ^= u4;
                 case 3: l3 ^= u3;
                 case 2: l2 ^= u2;
                 case 1: l1 ^= u1;
                 case 0: l0 ^= u0;
            }
            /* shift lower reg right until the rightmost bit is one */
            do {
                ECOH_SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
 AlignLower:
                /* move the comma to the right if we need to */
                switch (lc) {
                    case 8: if  (l8) break; --lc;
                    case 7: if  (l7) break; --lc;
                    case 6: if  (l6) break; --lc;
                    case 5: if  (l5) break; --lc;
                    case 4: if  (l4) break; --lc;
                    case 3: if  (l3) break; --lc;
                    case 2: if  (l2) break; --lc;
                    case 1: if  (l1) break; --lc;
                    case 0: if (!l0) return;
                            if (l0 == (ecoh_uint) 1) {
                                temp[8] = u9;
                                temp[7] = u8;
                                temp[6] = u7;
                                temp[5] = u6;
                                temp[4] = u5;
                                temp[3] = u4;
                                temp[2] = u3;
                                temp[1] = u2;
                                temp[0] = u1;
                                /* use uc as the loop counter */
                                uc = uc;
                                goto doneCommasNotAligned;
                            }
                }
            } while (!(l0 & 1));
        } /* while (uc < lc) */

    } /* main loop */
 commaU8L8:
    /* uc == lc == 8 */
    for (;;) {
        while (u8 >= l8) {
            l9^=u9;
            u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u8) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u9^=l9;
            l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l8) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u8 <= l8);
    }


 commaU7L7:
    /* uc == lc == 7 */
    for (;;) {
        while (u7 >= l7) {
            l9^=u9;  l8^=u8;
            u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u7) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;
            l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l7) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u7 <= l7);
    }


 commaU6L6:
    /* uc == lc == 6 */
    for (;;) {
        while (u6 >= l6) {
            l9^=u9;  l8^=u8;  l7^=u7;
            u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u6) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;
            l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l6) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u6 <= l6);
    }


 commaU5L5:
    /* uc == lc == 5 */
    for (;;) {
        while (u5 >= l5) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;
            u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u5) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;
            l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l5) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u5 <= l5);
    }


 commaU4L4:
    /* uc == lc == 4 */
    for (;;) {
        while (u4 >= l4) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;
            u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u4) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;
            l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l4) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u4 <= l4);
    }


 commaU3L3:
    /* uc == lc == 3 */
    for (;;) {
        while (u3 >= l3) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;
            u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u3) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;
            l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l3) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u3 <= l3);
    }


 commaU2L2:
    /* uc == lc == 2 */
    for (;;) {
        while (u2 >= l2) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;
            u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u2) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;
            l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l2) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u2 <= l2);
    }


 commaU1L1:
    /* uc == lc == 1 */
    for (;;) {
        while (u1 >= l1) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;
            u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u1) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;
            l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l1) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u1 <= l1);
    }


 commaU0L0:
    /* uc == lc == 0 */
    for (;;) {
        while (u0 >= l0) {
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;
            u0^=l0;
            //if (!u0) return SB_FAIL_NO_INVERSE;
            do {
                ECOH_SHIFT_RIGHT_10(u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            } while (!(u0 & (ecoh_uint) 1));
            if (u0 == (ecoh_uint) 1) goto ULIsUnity;
        }
        do {
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;
            l0^=u0;
            //if (!l0) return SB_FAIL_NO_INVERSE;
            do {
                ECOH_SHIFT_RIGHT_10(l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            } while (!(l0 & (ecoh_uint) 1));
            if (l0 == (ecoh_uint) 1) goto LLIsUnity;
        } while (u0 <= l0);
    }


    /* extract almost inverse from the regs depending on the comma positions  */

 doneCommasNotAligned:
    /* it's possible for the commas to be aligned at 0 here */
    while ( uc > 0 ) {
        uc--;
        temp[uc] = 0;
    }
    goto finish;


 ULIsUnity:
    /* upper left is unity, so result is in the lower right */
    temp[8] = l9;
    temp[7] = l8;
    temp[6] = l7;
    temp[5] = l6;
    temp[4] = l5;
    temp[3] = l4;
    temp[2] = l3;
    temp[1] = l2;
    temp[0] = l1;
    goto finish;

 LLIsUnity:
    /* lower left is unity, so result is in the upper right */
    temp[8] = u9;
    temp[7] = u8;
    temp[6] = u7;
    temp[5] = u6;
    temp[4] = u5;
    temp[3] = u4;
    temp[2] = u3;
    temp[1] = u2;
    temp[0] = u1;

 finish:

    BitReverse(temp, 9); 

    /* multiply almost inverse by x^(2L-C) */
    /* first multiply and reduce in chunks of the word size */
    count = (sz << (ECOH_LIMB_LGBITS_SIZE+1)) - count;
    while ( count >= ECOH_LIMB_BIT_SIZE)
    {
        count -= ECOH_LIMB_BIT_SIZE;
	temp[9] = temp[8];
	temp[8] = temp[7];
	temp[7] = temp[6];
	temp[6] = temp[5];
	temp[5] = temp[4];
	temp[4] = temp[3];
	temp[3] = temp[2];
	temp[2] = temp[1];
	temp[1] = temp[0];

        temp[0] = 0;
        w = temp[9];
        temp[0] ^= (w << 5) ^ (w << 7) ^ (w << 10) ^ (w << 15);
        temp[1] ^= (w >> 59) ^ (w >> 57) ^ (w >> 54) ^ (w >> 49);
    }

    /* multiply by the remaining multiplier */
    temp[9] = 0;
for (i=count; i>0; i--) {
    ECOH_SHIFT_LEFT_10(temp[9],temp[8],temp[7],temp[6],temp[5],temp[4],temp[3],temp[2],temp[1],temp[0]);
}
        w = temp[9];
        temp[0] ^= (w << 5) ^ (w << 7) ^ (w << 10) ^ (w << 15);
        temp[1] ^= (w >> 59) ^ (w >> 57) ^ (w >> 54) ^ (w >> 49);

    temp[9] = 0;
    temp[10] = 0;
    temp[11] = 0;
    temp[12] = 0;
    temp[13] = 0;
    temp[14] = 0;
    temp[15] = 0;
    temp[16] = 0;
    temp[17] = 0;

    PolyModT571(rop, temp);
    return; 
}
#else
{
    ecoh_uint temp[36] = {(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0,(ecoh_uint)0}; /* temp storage */
    int uc = 17;      /* upper comma. */
    int lc = 17;      /* lower comma. */
    int count = 0;
    int i;
    ecoh_uint w;

    /* upper register (merged) */
    ecoh_uint u18 = (ecoh_uint)0, u17 = (ecoh_uint)0, u16 = (ecoh_uint)0, u15 = (ecoh_uint)0, u14 = (ecoh_uint)0, u13 = (ecoh_uint)0, u12 = (ecoh_uint)0, u11 = (ecoh_uint)0, u10 = (ecoh_uint)0, u9 = (ecoh_uint)0, u8 = (ecoh_uint)0, u7 = (ecoh_uint)0, u6 = (ecoh_uint)0, u5 = (ecoh_uint)0, u4 = (ecoh_uint)0, u3 = (ecoh_uint)0, u2 = (ecoh_uint)0, u1 = (ecoh_uint)0, u0 = (ecoh_uint)0;

    /* lower register (merged) */
    ecoh_uint l18 = (ecoh_uint)0, l17 = (ecoh_uint)0, l16 = (ecoh_uint)0, l15 = (ecoh_uint)0, l14 = (ecoh_uint)0, l13 = (ecoh_uint)0, l12 = (ecoh_uint)0, l11 = (ecoh_uint)0, l10 = (ecoh_uint)0, l9 = (ecoh_uint)0, l8 = (ecoh_uint)0, l7 = (ecoh_uint)0, l6 = (ecoh_uint)0, l5 = (ecoh_uint)0, l4 = (ecoh_uint)0, l3 = (ecoh_uint)0, l2 = (ecoh_uint)0, l1 = (ecoh_uint)0, l0 = (ecoh_uint)0;

    /* multiply element by x^2L */
    temp[sz+17] = op[17];
    temp[sz+16] = op[16];
    temp[sz+15] = op[15];
    temp[sz+14] = op[14];
    temp[sz+13] = op[13];
    temp[sz+12] = op[12];
    temp[sz+11] = op[11];
    temp[sz+10] = op[10];
    temp[sz+9]  = op[9];
    temp[sz+8]  = op[8];
    temp[sz+7]  = op[7];
    temp[sz+6]  = op[6];
    temp[sz+5]  = op[5];
    temp[sz+4]  = op[4];
    temp[sz+3]  = op[3];
    temp[sz+2]  = op[2];
    temp[sz+1]  = op[1];
    temp[sz]    = op[0];

    PolyModT571(temp+sz, temp);

    temp[17] = 0;
    temp[16] = 0;
    temp[15] = 0;
    temp[14] = 0;
    temp[13] = 0;
    temp[12] = 0;
    temp[11] = 0;
    temp[10] = 0;
    temp[9]  = 0;
    temp[8]  = 0;
    temp[7]  = 0;
    temp[6]  = 0;
    temp[5]  = 0;
    temp[4]  = 0;
    temp[3]  = 0;
    temp[2]  = 0;
    temp[1]  = 0;
    temp[0]  = 0;

    PolyModT571(temp, temp);

    /* Final reduce and check to see if the element is zero */
    if ( IsInt(temp, sz, 0) ) {
        return;
    }

    u18 = ((ecoh_uint)1) << (ECOH_LIMB_BIT_SIZE - 1);
    l18 = 0;
    /* set the modulus */
    u17 = fx[17]; l17 = temp[17];
    u16 = fx[16]; l16 = temp[16];
    u15 = fx[15]; l15 = temp[15];
    u14 = fx[14]; l14 = temp[14];
    u13 = fx[13]; l13 = temp[13];
    u12 = fx[12]; l12 = temp[12];
    u11 = fx[11]; l11 = temp[11];
    u10 = fx[10]; l10 = temp[10];
    u9 = fx[9]; l9 = temp[9];
    u8 = fx[8]; l8 = temp[8];
    u7 = fx[7]; l7 = temp[7];
    u6 = fx[6]; l6 = temp[6];
    u5 = fx[5]; l5 = temp[5];
    u4 = fx[4]; l4 = temp[4];
    u3 = fx[3]; l3 = temp[3];
    u2 = fx[2]; l2 = temp[2];
    u1 = fx[1]; l1 = temp[1];
    u0 = fx[0]; l0 = temp[0];


    /* We know that the irreducible has a 1 in position 0.
     * Jump into the code that shifts a 1 into position 0 of the lower reg.
     */
    goto AlignLower;

    /* main loop */
    /* select an inner loop based upon the comma positions */
    for (;;)
    {
        /* case upper comma == lower comma */
        if (uc == lc) {
            switch (uc) { /* jump to a specific optimized loop */
                case 17: goto commaU17L17;
                case 16: goto commaU16L16;
                case 15: goto commaU15L15;
                case 14: goto commaU14L14;
                case 13: goto commaU13L13;
                case 12: goto commaU12L12;
                case 11: goto commaU11L11;
                case 10: goto commaU10L10;
                case 9: goto commaU9L9;
                case 8: goto commaU8L8;
                case 7: goto commaU7L7;
                case 6: goto commaU6L6;
                case 5: goto commaU5L5;
                case 4: goto commaU4L4;
                case 3: goto commaU3L3;
                case 2: goto commaU2L2;
                case 1: goto commaU1L1;
                case 0: goto commaU0L0;
            }
        }

        /* case upper comma > lower comma */
        while (uc > lc) {

            switch (uc) { /* xor downto uc+1 */
                 case 0: l1 ^= u1;
                 case 1: l2 ^= u2;
                 case 2: l3 ^= u3;
                 case 3: l4 ^= u4;
                 case 4: l5 ^= u5;
                 case 5: l6 ^= u6;
                 case 6: l7 ^= u7;
                 case 7: l8 ^= u8;
                 case 8: l9 ^= u9;
                 case 9: l10 ^= u10;
                 case 10: l11 ^= u11;
                 case 11: l12 ^= u12;
                 case 12: l13 ^= u13;
                 case 13: l14 ^= u14;
                 case 14: l15 ^= u15;
                 case 15: l16 ^= u16;
                 case 16: l17 ^= u17;
                 case 17: l18 ^= u18;
            }
            switch (lc) { /* xor upto lc */
                 case 17: u17 ^= l17;
                 case 16: u16 ^= l16;
                 case 15: u15 ^= l15;
                 case 14: u14 ^= l14;
                 case 13: u13 ^= l13;
                 case 12: u12 ^= l12;
                 case 11: u11 ^= l11;
                 case 10: u10 ^= l10;
                 case 9: u9 ^= l9;
                 case 8: u8 ^= l8;
                 case 7: u7 ^= l7;
                 case 6: u6 ^= l6;
                 case 5: u5 ^= l5;
                 case 4: u4 ^= l4;
                 case 3: u3 ^= l3;
                 case 2: u2 ^= l2;
                 case 1: u1 ^= l1;
                 case 0: u0 ^= l0;
            }
            /* shift upper reg right until the rightmost bit is one */
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
 AlignUpper:
                /* move the comma to the right if we need to */
                switch (uc) {
                    case 17: if  (u17) break; --uc;
                    case 16: if  (u16) break; --uc;
                    case 15: if  (u15) break; --uc;
                    case 14: if  (u14) break; --uc;
                    case 13: if  (u13) break; --uc;
                    case 12: if  (u12) break; --uc;
                    case 11: if  (u11) break; --uc;
                    case 10: if  (u10) break; --uc;
                    case 9: if  (u9) break; --uc;
                    case 8: if  (u8) break; --uc;
                    case 7: if  (u7) break; --uc;
                    case 6: if  (u6) break; --uc;
                    case 5: if  (u5) break; --uc;
                    case 4: if  (u4) break; --uc;
                    case 3: if  (u3) break; --uc;
                    case 2: if  (u2) break; --uc;
                    case 1: if  (u1) break; --uc;
                    case 0: if (!u0) return;
                            if (u0 == (ecoh_uint) 1) {
                                temp[17] = l18;
                                temp[16] = l17;
                                temp[15] = l16;
                                temp[14] = l15;
                                temp[13] = l14;
                                temp[12] = l13;
                                temp[11] = l12;
                                temp[10] = l11;
                                temp[9] = l10;
                                temp[8] = l9;
                                temp[7] = l8;
                                temp[6] = l7;
                                temp[5] = l6;
                                temp[4] = l5;
                                temp[3] = l4;
                                temp[2] = l3;
                                temp[1] = l2;
                                temp[0] = l1;
                                /* use uc as the loop counter */
                                uc = lc;
                                goto doneCommasNotAligned;
                            }
                }
            } while (!(u0 & 1));
        } /* while (uc > lc) */

        /* case upper comma < lower comma */
        while (uc < lc) {

            switch (lc) { /* xor downto lc+1 */
                 case 0: u1 ^= l1;
                 case 1: u2 ^= l2;
                 case 2: u3 ^= l3;
                 case 3: u4 ^= l4;
                 case 4: u5 ^= l5;
                 case 5: u6 ^= l6;
                 case 6: u7 ^= l7;
                 case 7: u8 ^= l8;
                 case 8: u9 ^= l9;
                 case 9: u10 ^= l10;
                 case 10: u11 ^= l11;
                 case 11: u12 ^= l12;
                 case 12: u13 ^= l13;
                 case 13: u14 ^= l14;
                 case 14: u15 ^= l15;
                 case 15: u16 ^= l16;
                 case 16: u17 ^= l17;
                 case 17: u18 ^= l18;
            }
            switch (uc) { /* xor upto uc */
                 case 17: l17 ^= u17;
                 case 16: l16 ^= u16;
                 case 15: l15 ^= u15;
                 case 14: l14 ^= u14;
                 case 13: l13 ^= u13;
                 case 12: l12 ^= u12;
                 case 11: l11 ^= u11;
                 case 10: l10 ^= u10;
                 case 9: l9 ^= u9;
                 case 8: l8 ^= u8;
                 case 7: l7 ^= u7;
                 case 6: l6 ^= u6;
                 case 5: l5 ^= u5;
                 case 4: l4 ^= u4;
                 case 3: l3 ^= u3;
                 case 2: l2 ^= u2;
                 case 1: l1 ^= u1;
                 case 0: l0 ^= u0;
            }
            /* shift lower reg right until the rightmost bit is one */
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
 AlignLower:
                /* move the comma to the right if we need to */
                switch (lc) {
                    case 17: if  (l17) break; --lc;
                    case 16: if  (l16) break; --lc;
                    case 15: if  (l15) break; --lc;
                    case 14: if  (l14) break; --lc;
                    case 13: if  (l13) break; --lc;
                    case 12: if  (l12) break; --lc;
                    case 11: if  (l11) break; --lc;
                    case 10: if  (l10) break; --lc;
                    case 9: if  (l9) break; --lc;
                    case 8: if  (l8) break; --lc;
                    case 7: if  (l7) break; --lc;
                    case 6: if  (l6) break; --lc;
                    case 5: if  (l5) break; --lc;
                    case 4: if  (l4) break; --lc;
                    case 3: if  (l3) break; --lc;
                    case 2: if  (l2) break; --lc;
                    case 1: if  (l1) break; --lc;
                    case 0: if (!l0) return;
                            if (l0 == (ecoh_uint) 1) {
                                temp[17] = u18;
                                temp[16] = u17;
                                temp[15] = u16;
                                temp[14] = u15;
                                temp[13] = u14;
                                temp[12] = u13;
                                temp[11] = u12;
                                temp[10] = u11;
                                temp[9] = u10;
                                temp[8] = u9;
                                temp[7] = u8;
                                temp[6] = u7;
                                temp[5] = u6;
                                temp[4] = u5;
                                temp[3] = u4;
                                temp[2] = u3;
                                temp[1] = u2;
                                temp[0] = u1;
                                /* use uc as the loop counter */
                                uc = uc;
                                goto doneCommasNotAligned;
                            }
                }
            } while (!(l0 & 1));
        } /* while (uc < lc) */

    } /* main loop */
 commaU17L17:
    /* uc == lc == 17 */
    for (;;) {
        while (u17 >= l17) {
            l18^=u18;
            u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u17) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;
            l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l17) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u17 <= l17);
    }


 commaU16L16:
    /* uc == lc == 16 */
    for (;;) {
        while (u16 >= l16) {
            l18^=u18;  l17^=u17;
            u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u16) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;
            l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l16) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u16 <= l16);
    }


 commaU15L15:
    /* uc == lc == 15 */
    for (;;) {
        while (u15 >= l15) {
            l18^=u18;  l17^=u17;  l16^=u16;
            u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u15) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;
            l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l15) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u15 <= l15);
    }


 commaU14L14:
    /* uc == lc == 14 */
    for (;;) {
        while (u14 >= l14) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;
            u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u14) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;
            l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l14) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u14 <= l14);
    }


 commaU13L13:
    /* uc == lc == 13 */
    for (;;) {
        while (u13 >= l13) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;
            u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u13) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;
            l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l13) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u13 <= l13);
    }


 commaU12L12:
    /* uc == lc == 12 */
    for (;;) {
        while (u12 >= l12) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;
            u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u12) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;
            l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l12) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u12 <= l12);
    }


 commaU11L11:
    /* uc == lc == 11 */
    for (;;) {
        while (u11 >= l11) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;
            u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u11) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;
            l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l11) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u11 <= l11);
    }


 commaU10L10:
    /* uc == lc == 10 */
    for (;;) {
        while (u10 >= l10) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;
            u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u10) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;
            l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l10) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u10 <= l10);
    }


 commaU9L9:
    /* uc == lc == 9 */
    for (;;) {
        while (u9 >= l9) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;
            u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u9) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;
            l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l9) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u9 <= l9);
    }


 commaU8L8:
    /* uc == lc == 8 */
    for (;;) {
        while (u8 >= l8) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;
            u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u8) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;
            l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l8) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u8 <= l8);
    }


 commaU7L7:
    /* uc == lc == 7 */
    for (;;) {
        while (u7 >= l7) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;
            u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u7) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;
            l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l7) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u7 <= l7);
    }


 commaU6L6:
    /* uc == lc == 6 */
    for (;;) {
        while (u6 >= l6) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;
            u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u6) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;
            l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l6) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u6 <= l6);
    }


 commaU5L5:
    /* uc == lc == 5 */
    for (;;) {
        while (u5 >= l5) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;
            u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u5) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;
            l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l5) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u5 <= l5);
    }


 commaU4L4:
    /* uc == lc == 4 */
    for (;;) {
        while (u4 >= l4) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;
            u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u4) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;
            l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l4) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u4 <= l4);
    }


 commaU3L3:
    /* uc == lc == 3 */
    for (;;) {
        while (u3 >= l3) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;
            u3^=l3;  u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u3) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;
            l3^=u3;  l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l3) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u3 <= l3);
    }


 commaU2L2:
    /* uc == lc == 2 */
    for (;;) {
        while (u2 >= l2) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;
            u2^=l2;  u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u2) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;
            l2^=u2;  l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l2) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u2 <= l2);
    }


 commaU1L1:
    /* uc == lc == 1 */
    for (;;) {
        while (u1 >= l1) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;
            u1^=l1;  u0^=l0;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            if (!u1) { --uc; goto AlignUpper; }
            } while (!(u0 & (ecoh_uint) 1));
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;
            l1^=u1;  l0^=u0;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            if (!l1) { --lc; goto AlignLower; }
            } while (!(l0 & (ecoh_uint) 1));
        } while (u1 <= l1);
    }


 commaU0L0:
    /* uc == lc == 0 */
    for (;;) {
        while (u0 >= l0) {
            l18^=u18;  l17^=u17;  l16^=u16;  l15^=u15;  l14^=u14;  l13^=u13;  l12^=u12;  l11^=u11;  l10^=u10;  l9^=u9;  l8^=u8;  l7^=u7;  l6^=u6;  l5^=u5;  l4^=u4;  l3^=u3;  l2^=u2;  l1^=u1;
            u0^=l0;
            //if (!u0) return SB_FAIL_NO_INVERSE;
            do {
                ECOH_SHIFT_RIGHT_19(u18,u17,u16,u15,u14,u13,u12,u11,u10,u9,u8,u7,u6,u5,u4,u3,u2,u1,u0);  ++count;
            } while (!(u0 & (ecoh_uint) 1));
            if (u0 == (ecoh_uint) 1) goto ULIsUnity;
        }
        do {
            u18^=l18;  u17^=l17;  u16^=l16;  u15^=l15;  u14^=l14;  u13^=l13;  u12^=l12;  u11^=l11;  u10^=l10;  u9^=l9;  u8^=l8;  u7^=l7;  u6^=l6;  u5^=l5;  u4^=l4;  u3^=l3;  u2^=l2;  u1^=l1;
            l0^=u0;
            //if (!l0) return SB_FAIL_NO_INVERSE;
            do {
                ECOH_SHIFT_RIGHT_19(l18,l17,l16,l15,l14,l13,l12,l11,l10,l9,l8,l7,l6,l5,l4,l3,l2,l1,l0);  ++count;
            } while (!(l0 & (ecoh_uint) 1));
            if (l0 == (ecoh_uint) 1) goto LLIsUnity;
        } while (u0 <= l0);
    }


    /* extract almost inverse from the regs depending on the comma positions  */

 doneCommasNotAligned:
    /* it's possible for the commas to be aligned at 0 here */
    while ( uc > 0 ) {
        uc--;
        temp[uc] = 0;
    }
    goto finish;


 ULIsUnity:
    /* upper left is unity, so result is in the lower right */
    temp[17] = l18;
    temp[16] = l17;
    temp[15] = l16;
    temp[14] = l15;
    temp[13] = l14;
    temp[12] = l13;
    temp[11] = l12;
    temp[10] = l11;
    temp[9] = l10;
    temp[8] = l9;
    temp[7] = l8;
    temp[6] = l7;
    temp[5] = l6;
    temp[4] = l5;
    temp[3] = l4;
    temp[2] = l3;
    temp[1] = l2;
    temp[0] = l1;
    goto finish;

 LLIsUnity:
    /* lower left is unity, so result is in the upper right */
    temp[17] = u18;
    temp[16] = u17;
    temp[15] = u16;
    temp[14] = u15;
    temp[13] = u14;
    temp[12] = u13;
    temp[11] = u12;
    temp[10] = u11;
    temp[9] = u10;
    temp[8] = u9;
    temp[7] = u8;
    temp[6] = u7;
    temp[5] = u6;
    temp[4] = u5;
    temp[3] = u4;
    temp[2] = u3;
    temp[1] = u2;
    temp[0] = u1;

 finish:

    BitReverse(temp, 18);

    /* multiply almost inverse by x^(2L-C) */
    /* first multiply and reduce in chunks of the word size */
    count = (sz << (ECOH_LIMB_LGBITS_SIZE+1)) - count;
    while ( count >= ECOH_LIMB_BIT_SIZE )
    {
        count -= ECOH_LIMB_BIT_SIZE;
        temp[19] = temp[18];
        temp[18] = temp[17];
        temp[17] = temp[16];
        temp[16] = temp[15];
        temp[15] = temp[14];
        temp[14] = temp[13];
        temp[13] = temp[12];
        temp[12] = temp[11];
        temp[11] = temp[10];
        temp[10] = temp[9];
        temp[9] = temp[8];
        temp[8] = temp[7];
        temp[7] = temp[6];
        temp[6] = temp[5];
        temp[5] = temp[4];
        temp[4] = temp[3];
        temp[3] = temp[2];
        temp[2] = temp[1];
        temp[1] = temp[0];

        temp[0] = 0;
        w = temp[18]; 
        temp[0] ^= (w << 5) ^ (w << 7) ^ (w << 10) ^ (w << 15);
        temp[1] ^= (w >> 27) ^ (w >> 25) ^ (w >> 22) ^ (w >> 17);
    }

    /* multiply by the remaining multiplier */
    temp[18] = 0;
for (i=count; i>0; i--)  {
    ECOH_SHIFT_LEFT_19(temp[18],temp[17],temp[16],temp[15],temp[14],temp[13],temp[12],temp[11],temp[10],temp[9],temp[8],temp[7],temp[6],temp[5],temp[4],temp[3],temp[2],temp[1],temp[0]);
}

        w = temp[18]; 
        temp[0] ^= (w << 5) ^ (w << 7) ^ (w << 10) ^ (w << 15);
        temp[1] ^= (w >> 27) ^ (w >> 25) ^ (w >> 22) ^ (w >> 17);

        temp[18] = 0;
        temp[19] = 0;
        temp[20] = 0;
        temp[21] = 0;
        temp[22] = 0;
        temp[23] = 0;
        temp[24] = 0;
        temp[25] = 0;
        temp[26] = 0;
        temp[27] = 0;
        temp[28] = 0;
        temp[29] = 0;
        temp[30] = 0;
        temp[31] = 0;
        temp[32] = 0;
        temp[33] = 0;
        temp[34] = 0;
        temp[35] = 0;
        PolyModT571(rop,temp);

    return;
}
#endif 
/*
/*	END OF f_{2^m} FUNCTIONS
/*
/*****************************************************************************/

/*****************************************************************************/
/*
/*	START OF ECC FUNCTIONS
/*
/*****************************************************************************/
/*
/*	PointDouble()			This function doubles a ecoh_point represented in
/*							polynomial basis on a ecoh_curve 
/*
/*							See [1] Group Laws for non-super singular curves
/*							p. 81, Point Doubling
/*
/*****************************************************************************/
void Ecoh::PointDouble(ecoh_point *rop, ecoh_point *op, ecoh_curve *T)
{
	ecoh_uint	l[2*ECOH_MAX_SIZE], l2[2*ECOH_MAX_SIZE], px[ECOH_MAX_SIZE];
	
	if(op->x[0]==0){
		if(IsInt(op->x, T->sz, 0)){
			memset(rop->x, 0, sizeof(ecoh_uint)*T->sz);
			memset(rop->y, 0, sizeof(ecoh_uint)*T->sz);
			return;
		}
	}
	PolyInv(l, op->x, T->f, T->sz);			//	l = x^-1
	PolyMulMod(l, l, op->y, T->sz);			//	l = y/x in f2m
	PolyAdd(l, l, op->x, T->sz);			//	l = x + y/x in f2m
	PolySqrMod(l2, l, T->sz);				//	l2 = l^2 in f2m
	l[0]^=T->a;								//	l = l + 1 in f2m
	PolyAdd(px, l2, l, T->sz);				//	px = l^2 + l + a in f2m 
	PolySqrMod(l2, op->x, T->sz);			//	l2 = x^2
	PolyMulMod(l, l, px, T->sz);			//	l = px(l + 1) 
	PolyAdd(rop->y, l2, l, T->sz);			//	py = x^2 + px(l + 1) in f2m
	memcpy(rop->x, px, sizeof(ecoh_uint)*T->sz);
}
/*****************************************************************************/
/*
/*	PointAdd()				add one ecoh_point to another on the ecoh_curve 
/*							See [1] Group Laws for non-super singular curves
/*							p. 81, Point Addition
/*
/*
/*****************************************************************************/
void Ecoh::PointAdd(ecoh_point *rop, ecoh_point *op1, ecoh_point *op2, ecoh_curve *T)
{
	ecoh_uint	l[2*ECOH_MAX_SIZE], l2[2*ECOH_MAX_SIZE], px[ECOH_MAX_SIZE], py[ECOH_MAX_SIZE];
	ecoh_uint	op1x0, op1y0;

	op1x0 = IsInt(op1->x, T->sz, 0);	op1y0 = IsInt(op1->y, T->sz, 0);
	if(op1x0 && op1y0)	{
		memcpy(rop->x, op2->x, sizeof(ecoh_uint)*T->sz);
		memcpy(rop->y, op2->y, sizeof(ecoh_uint)*T->sz);
		return;
	}
	op1x0 = IsInt(op2->x, T->sz, 0);	op1y0 = IsInt(op2->y, T->sz, 0);
	if(op1x0 && op1y0)	{
		memcpy(rop->x, op1->x, sizeof(ecoh_uint)*T->sz);
		memcpy(rop->y, op1->y, sizeof(ecoh_uint)*T->sz);
		return;
	}
	if(memcmp(op1->x, op2->x, sizeof(ecoh_uint)*T->sz)==0){
		if(memcmp(op1->y, op2->y, sizeof(ecoh_uint)*T->sz)==0){
			PointDouble(rop, op1, T);		return;
		}
		else{
			memset(rop->x, 0, sizeof(ecoh_uint)*T->sz);
			memset(rop->y, 0, sizeof(ecoh_uint)*T->sz);
			return;
		}
	}
	PolyAdd(l2, op1->x, op2->x, T->sz);		//	l2 = x1 + x2
	PolyInv(l, l2, T->f, T->sz);			//	l = 1/(x1 + x2)
	PolyAdd(l2, op1->y, op2->y, T->sz);		//	l2 = y1 + y2
	PolyMulMod(l, l, l2, T->sz);			//	l = (y1 + y2)/(x1 + x2)
	PolySqrMod(l2, l, T->sz);				//	l2 = l^2
	PolyAdd(px, l2, l, T->sz);				//	px = l^2 + l in f2m
	PolyAdd(px, px, op1->x, T->sz);			//	px = l^2 + l + x1 in f2m
	PolyAdd(px, px, op2->x, T->sz);			//	px = l^2 + l + x1 + x2 in f2m
	px[0]^=T->a;							//	px = l^2 + l + x1 + x2 + a in f2m
	PolyAdd(l2, px, op1->x, T->sz);			//	l2 = x3 + x1 in f2m
	PolyMulMod(l2, l2, l, T->sz);			//	l2 = l(x3 + x1)
	PolyAdd(py, l2, px, T->sz);				//	py = l(x3 + x1) + x3 in f2m
	PolyAdd(py, py, op1->y, T->sz);			//	py = l(x3 + x1) + x3 + y1 in f2m
	memcpy(rop->x, px, sizeof(ecoh_uint)*T->sz);	
	memcpy(rop->y, py, sizeof(ecoh_uint)*T->sz);
}
/*****************************************************************************/
/*
/*	BasePointMultiply()			This function performs a base ecoh_point scalar
/*								multiply.
/*
/*								See [1], Algorithm 3.41, p. 104.
/*
/*
/*****************************************************************************/
void	Ecoh::BasePointMultiply(ecoh_point *P, ecoh_uint sz, ecoh_uint *k, ecoh_curve *T)
{
	ecoh_uint	i, j, n;
	ecoh_point	A, B;

	n = GetBitLength(sz, k);
	n = (n + 3) >> 2;
	memset(A.x, 0, ECOH_MAX_SIZE*sizeof(ecoh_uint));	memset(A.y, 0, ECOH_MAX_SIZE*sizeof(ecoh_uint));
	memset(B.x, 0, ECOH_MAX_SIZE*sizeof(ecoh_uint));	memset(B.y, 0, ECOH_MAX_SIZE*sizeof(ecoh_uint));

	for(j=15;j>0;j--){
		//
		//	compute Q_j = \sum_{i:Ki = j}, where 
		//		K_i is the ith part base 2^4.
		//
		for(i=0;i<n;i++){

			if(((k[i/ECOH_LIMB_NIB_LENGTH]&ECOH_NIB_MASK[i%ECOH_LIMB_NIB_LENGTH])>>(4*(i%ECOH_LIMB_NIB_LENGTH)))==j){
				PointAdd(&B, &B, &T->G[i], T);			
			}
		}
		PointAdd(&A, &A, &B, T);							
	}
	memcpy(P->x, A.x, sizeof(ecoh_uint)*T->sz);	memcpy(P->y, A.y, sizeof(ecoh_uint)*T->sz);
	return;
}
/*****************************************************************************/
/*
/*	SolveQuadratic()			Solves the quadratic equation for z
/*
/*								z^2 + z = op
/*
/*								Uses the basic version algorithm in [1], 
/*								Algorithm 3.85, p.133
/*
/*	RESTRICTIONS:				trace(op) = 0
/*								op gets destroyed
/*
/*****************************************************************************/
void Ecoh::SolveQuadratic(ecoh_uint *rop, ecoh_uint *op, ecoh_uint sz)
{
	ecoh_uint			i, g[ECOH_MAX_SIZE], b, qsz;
	/*************************************************************************/
	memset(g, 0, sizeof(ecoh_uint)*sz);
	if(sz==ECOH_SECT283_SIZE)	{	qsz = 141;	}
	else if(sz==ECOH_SECT409_SIZE)	{	qsz = 204;	}
	else						{	qsz = 285;	}
	for(i=qsz;i>0;i--){
		if(ECOH_GETBIT(op, 2*i)){
			b = ECOH_BIT[i%ECOH_LIMB_BIT_SIZE];
			op[i/ECOH_LIMB_BIT_SIZE]^= b;
			g[i/ECOH_LIMB_BIT_SIZE]^= b;
		}
	}
	for(i=0;i<qsz;i++){
		if(ECOH_GETBIT(op, 2*i + 1)){	
			switch(sz)
			{
			case ECOH_SECT283_SIZE:	PolyAdd(g, g, ECOH_HT283[i], sz);	break;
			case ECOH_SECT409_SIZE:	PolyAdd(g, g, ECOH_HT409[i], sz);	break;
			case ECOH_SECT571_SIZE:	PolyAdd(g, g, ECOH_HT571[i], sz);	break;
			default:	break;
			}
		}
	}
	memcpy(rop, g, sizeof(ecoh_uint)*sz);
}
/*****************************************************************************/
/*
/*	PointDecompress	This function attempts to decompress a ecoh_point.
/*
/*	it uses a hard coded (top bit of the 8th limb (index 7)) to determine which
/*	ecoh_point on the ecoh_curve it is mapped to.
/*
/*	it assumes the x-coordinate of the passed ecoh_point structure has trace = 1.
/*	trace(rop->x)=1;
/*
/*	See [2], Octet-String-to-Elliptic-Curve-Point Conversion, \S 2.3.4, p.11.
/*
/*****************************************************************************/
ecoh_uint	Ecoh::PointDecompress(ecoh_point *rop, ecoh_curve *T)
{
	ecoh_uint	yp, a[2*ECOH_MAX_SIZE], b[ECOH_MAX_SIZE], loop = 1;
	switch(T->sz)
	{
	case ECOH_SECT283_SIZE:
		yp = ECOH_GETBIT(rop->x, ECOH_YP_BIT_T283);
		break;
	case ECOH_SECT409_SIZE:
		yp = ECOH_GETBIT(rop->x, ECOH_YP_BIT_T409);
		break;
	case ECOH_SECT571_SIZE:
		yp = ECOH_GETBIT(rop->x, ECOH_YP_BIT_T571);
		break;
	default:
		break;
	}
	/*************************************************************************/
	/*
	/*	Verify we are not decompressing the 0 value (should never happen)		
	/*
	/*************************************************************************/
	if(rop->x[0]==0){ if(IsInt(rop->x, T->sz, 0)){	return -1;	}	}
	/*************************************************************************/
	/*
	/*	compute a = x + T->a + T->b*x^{-2} in F_{2^m}
	/*
	/*************************************************************************/
	PolyInv(b, rop->x, T->f, T->sz);			//	a = x^-1
	PolySqrMod(a, b, T->sz);					//	b = x^-2
	PolyMulMod(a, a, T->b, T->sz);				//	a = T->b*(x^-2)
	switch(T->sz)
	{
	case ECOH_SECT283_SIZE:
		if(ECOH_TRACET283(a))	return -1;
		break;
	case ECOH_SECT409_SIZE:
		if(ECOH_TRACET409(a))	return -1;
		break;
	case ECOH_SECT571_SIZE:
		if(ECOH_TRACET571(a))	return -1;
		break;
	default:
		break;
	}
	PolyAdd(a, a, rop->x, T->sz);			//	a = x + T->b*(x^-2)
	a[0]^=T->a;								//	a = x + T->a + T->b*(x^-2)
	SolveQuadratic(b, a, T->sz);			//	solve quadratic z^2 + z = a
	if(yp!=(b[0]&0x01))	{		b[0]^=0x01;	}
	PolyMulMod(a, b, rop->x, T->sz);
	memcpy(rop->y, a, sizeof(ecoh_uint)*T->sz);
	return 0;
}
/*
/*	END OF ECC FUNCTIONS
/*
/*****************************************************************************/

/*****************************************************************************/
/*
/*	EcohHashReturn Init(ecohHashState *state, int hashbitlen);
/*
/*		Initializes a ecohHashState with the intended hash length of this particular 
/*		instantiation.  Additionally, any data independent setup is performed.
/*
/*	Parameters:
/*		
/*		state:		a structure that holds the ecohHashState information
/*	
/*		hashbitlen: an integer value that indicates the length of the hash 
/*					output in bits.
/*
/*	Returns:
/*		
/*		ECOH_SUCCESS				on success
/*	
/*		ECOH_BAD_HASHBITLEN		hashbitlen is invalid (e.g., unknown value)
/*
/*****************************************************************************/
int Ecoh::Init(int hashbitlen)
{
	ecohState.blen = 0;

	ecohState.counter[0] = 0;	ecohState.counter[1] = 0;
	ecohState.mlen[0] = 0;		ecohState.mlen[1] = 0;
	switch(hashbitlen)
	{
	case ECOH224:
		ecohState.type = ECOH224;
		ecohState.DATASIZE = ECOH224_DATASIZE;
		ecohState.DIGESTSIZE = ECOH224_DIGESTSIZE;
		ecohState.clen = ECOH224_CLEN;
		ecohState.T = &ecoh_sect283r1;
		break;
	case ECOH256:
		ecohState.type = ECOH256;
		ecohState.DATASIZE = ECOH256_DATASIZE;
		ecohState.DIGESTSIZE = ECOH256_DIGESTSIZE;
		ecohState.clen = ECOH256_CLEN;
		ecohState.T = &ecoh_sect283r1;
		break;
	case ECOH384:
		ecohState.type = ECOH384;
		ecohState.DATASIZE = ECOH384_DATASIZE;
		ecohState.DIGESTSIZE = ECOH384_DIGESTSIZE;
		ecohState.clen = ECOH384_CLEN;
		ecohState.T = &ecoh_sect409r1;
		break;
	case ECOH512:
		ecohState.type = ECOH512;
		ecohState.DATASIZE = ECOH512_DATASIZE;
		ecohState.DIGESTSIZE = ECOH512_DIGESTSIZE;
		ecohState.clen = ECOH512_CLEN;
		ecohState.T = &ecoh_sect571r1;
		break;
	default:
		return ECOH_BAD_HASHBITLEN;
	}
	memset(ecohState.N, 0, ECOH_MAX_DATASIZE*ECOH_LIMB_SIZE);
	memset(ecohState.Q.x, 0, ECOH_MAX_SIZE*ECOH_LIMB_SIZE);
	memset(ecohState.Q.y, 0, ECOH_MAX_SIZE*ECOH_LIMB_SIZE);
	memset(ecohState.P.x, 0, ECOH_MAX_SIZE*ECOH_LIMB_SIZE);
	memset(ecohState.P.y, 0, ECOH_MAX_SIZE*ECOH_LIMB_SIZE);
	return ECOH_SUCCESS;
}
/*****************************************************************************/
/*
/*	Transform()		internal hashing of the data
/*
/*****************************************************************************/
ecoh_uint	Ecoh::Transform()
{
	ECOH_SET(ecohState.P.x, 1, ecohState.clen);
	ECOH_SETCTR(&ecohState.P.x[ecohState.clen], ecohState.counter, ecohState.clen);
	while(PointDecompress(&ecohState.P, ecohState.T))	{
		ECOH_ADD(ecohState.P.x, 2, ecohState.clen);
	}
	/*************************************************************************/
	/*
	/*	Q = Q + P_i, up counter, zeroize blen
	/*
	/*************************************************************************/
	PointAdd(&ecohState.Q, &ecohState.Q, &ecohState.P, ecohState.T);
	ECOH_INCCTR(ecohState.counter, ecohState.clen);
	ecohState.blen = 0;
	return ECOH_SUCCESS;
}
/*****************************************************************************/
/*
/*	Update()
/*
/*	EcohHashReturn Update(	ecohHashState *state, const EcohBitSequence *data, EcohDataLength 
/*						databitlen);
/*
/*	Process the supplied data.
/*
/*	Parameters:
/*
/*		state:		a structure that holds the ecohHashState information
/*
/*		data:		the data to be hashed
/*
/*		databitlen: the length, in bits, of the data to be hashed
/*
/*	Returns:
/*
/*		ECOH_SUCCESS			on success
/*	
/*		ECOH_FAIL			on failure
/*	
/*		ECOH_BAD_HASHBITLEN	on bad hash bit length
/*
/*****************************************************************************/
int Ecoh::Update(const EcohBitSequence *data, EcohDataLength databitlen)
{
	EcohDataLength	free, used;
	ecoh_uint		datastart;
	unsigned char		*ptr;

	if(databitlen&0x7)			{	return ECOH_BAD_HASHBITLEN;	}
	if(ecohState.type == ECOH512)	{	datastart = ECOH_DATASTART*2;	}
	else						{	datastart = ECOH_DATASTART;		}
	/*************************************************************************/
	/*
	/*	add in the bit count
	/*
	/*************************************************************************/
	ecohState.mlen[0]+=(databitlen);
	if(ecohState.mlen[0]<databitlen)	if((++ecohState.mlen[1])==0)	return ECOH_BAD_HASHBITLEN;	
	databitlen>>=3;
	used = ecohState.blen;
	ptr = ((unsigned char*)(&ecohState.P.x[datastart])) + used;
	/*************************************************************************/
	/*
	/*	handle the case where partial data is in the buffer
	/*
	/*************************************************************************/
	if(used){
		free = ecohState.DATASIZE - used;
		if(databitlen>=free){
			memcpy(ptr, data, (ecoh_uint)free);
			databitlen-=free;
			databitlen+=free;
			FormatData(&ecohState.P.x[datastart], (ecoh_uint)ecohState.DATASIZE/sizeof(ecoh_uint));
			PolyAdd(ecohState.N, ecohState.N, &ecohState.P.x[datastart], ecohState.DATASIZE/sizeof(ecoh_uint));
			if(Transform())	return ECOH_FAIL;
		}
		else{
			memcpy(ptr, data, (ecoh_uint)databitlen);
			ecohState.blen += (ecoh_uint)databitlen;
			return ECOH_SUCCESS;
		}
	}
	/*************************************************************************/
	/*
	/*	loop over the rest of the data, transforming every ECOH_DATASIZE
	/*
	/*************************************************************************/
	while(databitlen>=ecohState.DATASIZE){
		memcpy(&ecohState.P.x[datastart], data, (ecoh_uint)ecohState.DATASIZE);
		FormatData(&ecohState.P.x[datastart], (ecoh_uint)ecohState.DATASIZE/sizeof(ecoh_uint));
		PolyAdd(ecohState.N, ecohState.N, &ecohState.P.x[datastart], ecohState.DATASIZE/sizeof(ecoh_uint));
		if(Transform())	return ECOH_FAIL;
		databitlen-=ecohState.DATASIZE;
		data+=ecohState.DATASIZE;
	}
	/*************************************************************************/
	/*
	/*	Load any remaining data
	/*
	/*************************************************************************/
	if(databitlen>0){
		memcpy(&ecohState.P.x[datastart], data, (ecoh_uint)databitlen);
		ecohState.blen = (ecoh_uint)databitlen;
	}
	return ECOH_SUCCESS;
}
/*****************************************************************************/
/*
/*	Final()
/*
/*		EcohHashReturn Final(ecohHashState *state, EcohBitSequence *hashval);
/*
/*		Perform any post processing and output filtering required and return 
/*		the final hash value.
/*
/*	Parameters:
/*
/*		state:		a structure that holds the ecohHashState information
/*
/*		hashval:	the storage for the final hash value to be returned
/*
/*	Returns:
/*
/*		ECOH_SUCCESS		on success
/*
/*		ECOH_FAIL		otherwise
/*	
/*****************************************************************************/
int Ecoh::Final(EcohBitSequence *hashval)
{
    ecoh_uint		cnt, datastart;
	unsigned char	    *ptr;

	if(ecohState.type == ECOH512)	{	datastart = ECOH_DATASTART*2;	}
	else						{	datastart = ECOH_DATASTART;		}
	
	cnt = ecohState.blen;
	ptr = ((unsigned char*)(&ecohState.P.x[datastart])) + cnt;
    
    *ptr++ = 0x80;

    cnt = ecohState.DATASIZE - 1 - cnt;

    memset(ptr, 0, cnt );
    FormatData(&ecohState.P.x[datastart], (ecoh_uint)ecohState.DATASIZE/sizeof(ecoh_uint));
	PolyAdd(ecohState.N, ecohState.N, &ecohState.P.x[datastart], ecohState.DATASIZE/sizeof(ecoh_uint));
	if(Transform())	return ECOH_FAIL;
    
	memcpy(&ecohState.P.x[datastart], ecohState.N, ecohState.DATASIZE);

	ECOH_SET(ecohState.P.x, 1, ecohState.clen);
	ECOH_SETCTR(&ecohState.P.x[ecohState.clen], ecohState.mlen, ecohState.clen);

	while(PointDecompress(&ecohState.P, ecohState.T))	{
		ECOH_ADD(ecohState.P.x, 2, ecohState.clen);
	}
	/*************************************************************************/
	/*
	/*	Q = Q + P, up counter, zeroize blen
	/*
	/*************************************************************************/
	PointAdd(&ecohState.Q, &ecohState.Q, &ecohState.P, ecohState.T);
	/*************************************************************************/
	/*
	/*	Q = Q + [Q.x/2]G where[Q.x/2] interpreted as an integer and divided by
	/*	2, and G is the base ecoh_point
	/*
	/*************************************************************************/
	PolyShiftRight(ecohState.P.x, ecohState.Q.x, ecohState.T->sz);
	BasePointMultiply(&ecohState.P, ecohState.T->sz, ecohState.P.x, ecohState.T);
	PointAdd(&ecohState.Q, &ecohState.Q, &ecohState.P, ecohState.T);
	PolyShiftRight(ecohState.Q.x, ecohState.Q.x, ecohState.T->sz);
	Unload(hashval, ecohState.Q.x, ecohState.DIGESTSIZE);
	return ECOH_SUCCESS;
}
/*****************************************************************************/
/*	Hash()
/*
/*		EcohHashReturn Hash(int hashbitlen, const EcohBitSequence *data,  EcohDataLength 
/*							databitlen, EcohBitSequence *hashval);
/*
/*
/*		Hash the supplied data and provide the resulting hash value. Set return 
/*		code as appropriate.
/*
/*	Parameters:
/*
/*		hashbitlen:			the length in bits of the desired hash value
/*
/*		data:				the data to be hashed
/*
/*		databitlen:			the length, in bits, of the data to be hashed
/*
/*		hashval:			the resulting hash value of the provided data
/*
/*	Returns:
/*
/*		ECOH_SUCCESS				on success
/*
/*		ECOH_FAIL				arbitrary failure
/*
/*		ECOH_BAD_HASHBITLEN		unknown hashbitlen requested
/*
/*****************************************************************************/
int Ecoh::Hash(int hashbitlen, const EcohBitSequence *data,  EcohDataLength databitlen, EcohBitSequence *hashval)
{
	//ecohHashState	state;
	int	rval;
	EcohDataLength	blen, dblen, cnt;
	unsigned char		*ptr;
	ecoh_uint		datastart;

	rval = Ecoh::Init(hashbitlen);
	if(rval)	{	return rval;	}
	
	if(ecohState.type == ECOH512)	{	datastart = ECOH_DATASTART*2;	}
	else						{	datastart = ECOH_DATASTART;		}
	/*************************************************************************/
	/*
	/*	set mlen, anf the byte length blen, and the databitlength remainder
	/*
	/*************************************************************************/
	ecohState.mlen[0]=(databitlen);
	blen = (databitlen>>3);
	dblen = (databitlen&7);
	/*************************************************************************/
	/*
	/*	loop over the rest of the data, transforming every ECOH_DATASIZE
	/*
	/*************************************************************************/
	while(blen>=ecohState.DATASIZE){
		memcpy(&ecohState.P.x[datastart], data, (ecoh_uint)ecohState.DATASIZE);
		FormatData(&ecohState.P.x[datastart], (ecoh_uint)ecohState.DATASIZE/sizeof(ecoh_uint));
		PolyAdd(ecohState.N, ecohState.N, &ecohState.P.x[datastart], ecohState.DATASIZE/sizeof(ecoh_uint));
		if(Transform())	return ECOH_FAIL;
		blen-=ecohState.DATASIZE;
		data+=ecohState.DATASIZE;
	}
	/*************************************************************************/
	/*
	/*	Load any remaining data
	/*
	/*************************************************************************/
	if(blen>0){
		memcpy(&ecohState.P.x[datastart], data, (ecoh_uint)blen);
		ecohState.blen = blen;
		data+=blen;
	}
	cnt = ecohState.blen;
	ptr = ((unsigned char*)(&ecohState.P.x[datastart])) + cnt;
    if(dblen){
		*ptr++ = ((*data)&ECOH_BMASK[dblen])|ECOH_TBIT[dblen];
	}
	else{
		*ptr++ = 0x80;
	}
    cnt = ecohState.DATASIZE - 1 - cnt;

    
    memset(ptr, 0, cnt );
	FormatData(&ecohState.P.x[datastart], (ecoh_uint)ecohState.DATASIZE/sizeof(ecoh_uint));
	PolyAdd(ecohState.N, ecohState.N, &ecohState.P.x[datastart], ecohState.DATASIZE/sizeof(ecoh_uint));
	if(Transform())	return ECOH_FAIL;


    memcpy(&ecohState.P.x[datastart], ecohState.N, ecohState.DATASIZE);
	ECOH_SET(ecohState.P.x, 1, ecohState.clen);
	ECOH_SETCTR(&ecohState.P.x[ecohState.clen], ecohState.mlen, ecohState.clen);

	while(PointDecompress(&ecohState.P, ecohState.T))	{
		ECOH_ADD(ecohState.P.x, 2, ecohState.clen);
	}
	/*************************************************************************/
	/*
	/*	Q = Q + P, up counter, zeroize blen
	/*
	/*************************************************************************/
	PointAdd(&ecohState.Q, &ecohState.Q, &ecohState.P, ecohState.T);
	/*************************************************************************/
	/*
	/*	Q = Q + [Q.x/2]G where[Q.x/2] interpreted as an integer and divided by
	/*	2, and G is the base ecoh_point
	/*
	/*************************************************************************/
	PolyShiftRight(ecohState.P.x, ecohState.Q.x, ecohState.T->sz);
	BasePointMultiply(&ecohState.P, ecohState.T->sz, ecohState.P.x, ecohState.T);
	PointAdd(&ecohState.Q, &ecohState.Q, &ecohState.P, ecohState.T);
	PolyShiftRight(ecohState.Q.x, ecohState.Q.x, ecohState.T->sz);
	Unload(hashval, ecohState.Q.x, ecohState.DIGESTSIZE);

	return ECOH_SUCCESS;
}