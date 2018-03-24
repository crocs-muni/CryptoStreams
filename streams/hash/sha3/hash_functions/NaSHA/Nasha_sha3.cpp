#include <string.h>     /* for memcpy() etc.        */
#include <stdio.h>

#include "Nasha_sha3.h"
#include "brg_endian.h"

namespace sha3 {

#pragma warning(disable:4715)

#if defined( _MSC_VER ) && ( _MSC_VER > 800 )
#pragma intrinsic(memcpy)
#endif

#if 0 && defined(_MSC_VER)
#define nasha_rotl32 _lrotl
#define nasha_rotr32 _lrotr
#else
#define nasha_rotl32(x,n)   (((x) << n) | ((x) >> (32 - n)))
#define nasha_rotr32(x,n)   (((x) >> n) | ((x) << (32 - n)))
#endif

#if !defined(nasha_bswap_32)
#define nasha_bswap_32(x) ((nasha_rotr32((x), 24) & 0x00ff00ff) | (nasha_rotr32((x), 8) & 0xff00ff00))
#endif

#if (PLATFORM_BYTE_ORDER != IS_LITTLE_ENDIAN)
#define SWAP_BYTES
#else
#undef  SWAP_BYTES
#endif

#if defined(SWAP_BYTES)
#define nasha_bsw_32(p,n) \
    { int _i = (n); while(_i--) ((nasha_uint_32t*)p)[_i] = nasha_bswap_32(((nasha_uint_32t*)p)[_i]); }
#else
#define nasha_bsw_32(p,n)
#endif

#if !defined(nasha_bswap_64)
#define nasha_bswap_64(x) (((nasha_uint_64t)(nasha_bswap_32((nasha_uint_32t)(x)))) << 32 | nasha_bswap_32((nasha_uint_32t)((x) >> 32)))
#endif

#if defined(SWAP_BYTES)
#define nasha_bsw_64(p,n) \
    { int _i = (n); while(_i--) ((nasha_uint_64t*)p)[_i] = nasha_bswap_64(((nasha_uint_64t*)p)[_i]); }
#else
#define nasha_bsw_64(p,n)
#endif

// for rotating 64 bit words for 32 bits to the left
#define nasha_sw_32(x) (((nasha_uint_64t)((nasha_uint_32t)(x))) << 32 | (nasha_uint_32t)((x) >> 32))
#define nasha_swap_64(p,n) \
    { int _i = (n); while(_i--) ((nasha_uint_64t*)p)[_i] = nasha_sw_32(((nasha_uint_64t*)p)[_i]); }

// Nasha f function from order 2^8
static const nasha_uint_8t nasha_sbox[256] =   {
//0     1     2     3     4     5     6     7     8     9     A     B     C     D     E     F
 0x8c, 0x90, 0xd9, 0xc1, 0x46, 0x63, 0x53, 0xf1, 0x61, 0x32, 0x15, 0x3e, 0x26, 0x9a, 0x97, 0x2e, //0
 0xd8, 0xa0, 0x99, 0x9e, 0xc0, 0x95, 0x67, 0xb7, 0x6d, 0xe0, 0xf3, 0x28, 0x20, 0x86, 0xb6, 0xef, //1
 0x4b, 0x31, 0xb5, 0xd2, 0x13, 0x39, 0x6c, 0xa5, 0x03, 0x3f, 0x4d, 0x34, 0xf9, 0xec, 0x8e, 0x17, //2
 0xc5, 0x25, 0x3c, 0x89, 0xc9, 0x2b, 0x3a, 0xc2, 0x6e, 0xc6, 0xaa, 0x91, 0x49, 0x18, 0x93, 0xde, //3
 0x0d, 0x6f, 0x65, 0xaf, 0x92, 0xa7, 0xf6, 0xa6, 0x40, 0xb9, 0xed, 0xb0, 0xc3, 0xd7, 0x7d, 0x7c, //4
 0x54, 0x59, 0xdf, 0x2f, 0xda, 0xa4, 0x05, 0x94, 0x9b, 0x72, 0x01, 0x74, 0xa9, 0xf7, 0x81, 0xe9, //5
 0x1f, 0xb3, 0xeb, 0xcf, 0xe8, 0x47, 0x52, 0x36, 0xbc, 0x16, 0x29, 0x76, 0x12, 0xfa, 0x9c, 0x8a, //6
 0x5b, 0xa8, 0x43, 0xd1, 0x79, 0x85, 0x42, 0x82, 0xc7, 0xa1, 0x78, 0x4f, 0xe2, 0x35, 0xea, 0xad, //7
 0xdc, 0x0e, 0xd3, 0x2d, 0x6a, 0x5a, 0x44, 0xab, 0xc8, 0xe5, 0x37, 0x0a, 0x6b, 0x51, 0xe3, 0x14, //8
 0xcd, 0x56, 0x4a, 0xd6, 0x08, 0x83, 0xbb, 0x33, 0xe1, 0x30, 0x4e, 0x24, 0x5e, 0xb4, 0x00, 0x48, //9
 0x5f, 0x22, 0x0b, 0x50, 0x3d, 0x80, 0x1a, 0xbf, 0xcc, 0xff, 0x64, 0x87, 0x1b, 0xc4, 0x07, 0xf8, //A
 0x0c, 0xd4, 0xac, 0x02, 0x10, 0x84, 0x7e, 0x69, 0x70, 0x60, 0x55, 0x2a, 0x21, 0x57, 0x23, 0x66, //B
 0x62, 0x73, 0xcb, 0x41, 0x58, 0x71, 0x77, 0x1c, 0x7b, 0x8f, 0x9f, 0x9d, 0xa3, 0xb1, 0x7f, 0x5d, //C
 0xf4, 0x06, 0xae, 0xd5, 0xe6, 0x3b, 0xba, 0xfe, 0x96, 0xe7, 0x0f, 0x45, 0x2c, 0xf0, 0xfc, 0xbd, //D
 0xe4, 0x98, 0xfb, 0xca, 0x11, 0xf5, 0xdd, 0x7a, 0x5c, 0xfd, 0xce, 0x88, 0xd0, 0x68, 0x8d, 0x4c, //E
 0xbe, 0x04, 0x38, 0x1d, 0x1e, 0xf2, 0x27, 0x19, 0xb2, 0x75, 0xa2, 0xee, 0xdb, 0xb8, 0x09, 0x8b }; //F

// f, f'=F1.F2.F3, F32, F64
#define nasha_getQ64(x,y,a1,b1,c1,a2,b2,c2,a3,b3,c3,alpha, beta, gama,A, B, C) \
{	nasha_uint_8t sb, sb1; x.bit64 ^= y.bit64;   y.bit32[1]^=x.bit32[0]^A;	 \
	x.bit32[1]^=B; x.bit32[0]^=C;\
	y.bit16[1]^=x.bit16[3] ^ x.bit16[0]^alpha; \
	x.bit16[1]^=x.bit16[2]^beta; x.bit16[0]^=gama;\
	sb= nasha_sbox[x.b8[0]^c3]; sb1=nasha_sbox[x.b8[1]^b3^c2^sb];\
	y.b8[1]^=x.b8[3]^ x.b8[0] ^ sb1^ a1^ a3^ b2;	 \
	y.b8[0]^=x.b8[2] ^ x.b8[1]^ sb^b3^a2^b1^nasha_sbox[x.b8[0]^sb1^c1^b2^a3];}

//nasha_AE transformation
#define nasha_AE(m, aa1, bb1,k,a1,b1,c1,a2,b2,c2,a3,b3,c3,alpha1, beta1, gama1,A1, B1, C1) \
{   int i;    B64 m1,p; \
    m1.bit64=m; \
	for (i=0;i<k;i++){	p.bit64=aa1[i]+m1.bit64; m1.bit64=aa1[i];  \
	   nasha_getQ64(p,m1,a1,b1,c1,a2,b2,c2,a3,b3,c3,alpha1, beta1, gama1,A1, B1, C1);  bb1[i]= m1.bit64;	}}

//RAE_m transformation
#define nasha_RAE(m, aa1, bb1,k,a1,b1,c1,a2,b2,c2,a3,b3,c3,alpha2, beta2, gama2,A2, B2, C2) \
{   int i;    B64 m1,p;	p.bit64=m; \
	for (i=k;i>=0;i--){	  p.bit64=p.bit64+aa1[i]; m1.bit64=aa1[i];   \
	   nasha_getQ64(m1,p,a1,b1,c1,a2,b2,c2,a3,b3,c3,alpha2, beta2, gama2,A2, B2, C2);   bb1[i]= p.bit64;}}
	   
#if defined(Nasha_224) || defined(Nasha_256)

#define Nasha256_MASK (Nasha256_BLOCK_SIZE - 1)


//LinTr256^16 transformation
#define nasha_LinTr16(x,y) \
{ y[15]=x[3]^x[6]^x[9]^x[15]; y[14]=x[2]^x[5]^x[8]^x[14]; y[13]=x[1]^x[4]^x[7]^x[13]; \
  y[12]=x[0]^x[3]^x[6]^x[12]; y[11]=x[2]^x[5]^x[11]^y[15]; y[10]=x[1]^x[4]^x[10]^y[14]; \
  y[9]=x[0]^x[3]^x[9]^y[13];   y[8]=y[15]^x[2]^x[8]^y[12];   y[7]=y[14]^x[1]^x[7]^y[11]; \
  y[6]=x[0]^x[6]^y[13]^y[10];   y[5]=x[5]^y[9]^y[12]^y[15]; \
  y[4]=x[4]^y[8]^y[11]^y[14];   y[3]=x[3]^y[7]^y[10]^y[13];   y[2]=x[2]^y[6]^y[9]^y[12];  \
  y[1]=x[1]^y[5]^y[8]^y[11];   y[0]=x[0]^y[4]^y[7]^y[10]; }

//Nasa224/256 initialisation
#define nasha_PutX(x,_state)\
{ int k; for (k=0;k<4;k++){		x[k<<2]=_state->M[k<<1]; \
		x[(k<<2)+1]= _state->H[k];		x[(k<<2)+2]= _state->M[(k<<1)+1]; \
		x[(k<<2)+3]= _state->hash[k];}}


/* Compile 64 bytes of hash data into Nasha256 digest value   */

void Nasha::Nasha256_compile(hashState256 *state)
{
	int i;
	nasha_uint_64t x[16],y[16],l1,l2;
	B64 tmp;
	nasha_uint_32t A1, B1, C1, A2, B2, C2;
    nasha_uint_16t alpha1, beta1, gama1, alpha2, beta2, gama2;
	nasha_uint_8t a1,b1,c1,a2,b2,c2,a3,b3,c3;
	
	nasha_PutX(y,state);
	nasha_LinTr16(y,x);
	
	// computing the leaders l1 and l2
	l1=x[0]+x[1];
	l2=x[2]+x[3];
	
	//computing the 8-bite words a1, b1, c1,a2,b2,c2,a3,b3,c3
	tmp.bit64=x[4]+x[5];
	a1=tmp.b8[7]; b1=tmp.b8[6]; c1=tmp.b8[5];
	a2=tmp.b8[4]; b2=tmp.b8[3]; c2=tmp.b8[2];
	a3=tmp.b8[1]; b3=tmp.b8[0]; c3=a1;
	
	//computing the 16-bite words alpha1, beta1, gama1, alpha2, beta2, gama2
	tmp.bit64=x[6]+x[7];
	alpha1=tmp.bit16[3]; beta1=tmp.bit16[2]; gama1=tmp.bit16[1]; alpha2=tmp.bit16[0];
	
	tmp.bit64=x[8]+x[9]; 
	beta2=tmp.bit16[3]; gama2=tmp.bit16[2];
	
	//computing the 32-bite words A1, B1, C1, A2, B2, C2
	tmp.bit64=x[10]+x[11];
	A1=tmp.bit32[1]; B1=tmp.bit32[0];

	tmp.bit64=x[12]+x[13];
	C1=tmp.bit32[1]; A2=tmp.bit32[0];

	tmp.bit64=x[14]+x[15];
	B2=tmp.bit32[1]; C2=tmp.bit32[0];
	nasha_AE(l2,x,y,16,a1,b1,c1,a2,b2,c2,a3,b3,c3,alpha1, beta1, gama1,A1, B1, C1);
	nasha_swap_64(y,16);
	nasha_RAE(l1,y,x,15,a1,b1,c1,a2,b2,c2,a3,b3,c3,alpha2, beta2, gama2,A2, B2, C2);
	/*  Finally compute the hash digest (or/and the hash chain value) */
	for (i=0;i<4;i++){
		state->H[i]=x[(i<<2)+1];
		state->hash[i]=x[(i<<2)+3];
	}
}
/* Nasha256 hash data in an array of bits into hash buffer   */
/* and call the hash_compile function as required.          */

int Nasha::Nasha256_Update(hashState256 *state, const BitSequence *data, DataLength databitlen)
{    DataLength len=databitlen;
     const unsigned char *sp = data;
     if ((state->count[0] += len)< len)
        ++(state->count[1]);
     while (len>=Nasha256_BLOCK_SIZE*8){
           memcpy(((unsigned char *)state->M), sp, Nasha256_BLOCK_SIZE);
           sp+=Nasha256_BLOCK_SIZE; len-=Nasha256_BLOCK_SIZE*8;
           nasha_bsw_64(state->M, 8);
           Nasha256_compile(state);
     }
     memcpy((unsigned char *)state->M, sp, len/8+1);  
     /* put bytes in the buffer in an order in which references to   */
    /* 32-bit words will put bytes with lower addresses into the    */
    /* top of 32 bit words on BOTH big and little endian machines   */
	 nasha_bsw_64(state->M, 8);
	return SUCCESS;
}

/* Nasha256 Final padding and digest calculation  */

int Nasha::Nasha_Final1(hashState256 *state, BitSequence *hashval, const unsigned int hlen)
{   nasha_uint_32t    i = ((nasha_uint_32t)(state->count[0]>>3) & Nasha256_MASK);

    nasha_uint_64t    pom=state->count[1];
    /* we now need to mask valid bytes and add the padding which is */
    /* a single 1 bit and as many zero bits as necessary. */
	state->M[i>>3] &= li_64(fffffffffffffffe)>>(64-state->count[0]%64);
    state->M[i>>3] |= li_64(0000000000000001) << (state->count[0]%64);
    /* we need 17 or more empty positions, one for the padding byte  */
    /* (above) and eight for the length count.  If there is not     */
    /* enough space pad and empty the buffer                        */
    if(i > Nasha256_BLOCK_SIZE - 17)
    {
       	Nasha256_compile(state);
        i = 0;
    }
    else    /* compute a word index for the empty buffer positions  */
        i = (i >> 3)+1;

    while(i < 6) /* and zero pad all but last two positions        */
        state->M[i++] = 0;
    state->M[6] = state->count[1];
	state->M[7] = state->count[0];
	Nasha256_compile(state);

    /* extract the hash value as bytes   */
	for(i = 0; i < hlen; ++i)
        hashval[i] = (unsigned char)(state->hash[i >> 3] >> (8 * (~i & 7))); 
		
	return SUCCESS;
}

#endif

#if defined(Nasha_224)

/* Nasa224 chaining initial vectors */
const nasha_uint_64t nasha_i224H1[4] =
{
    li_64(6a09e667f3bcc908), li_64(bb67ae8584caa73b),
    li_64(3c6ef372fe94f82b), li_64(a54ff53a5f1d36f1)
};
const nasha_uint_64t nasha_i224H2[4] =
{
    li_64(cbbb9d5dc1059ed8), li_64(629a292a367cd507),
    li_64(9159015a3070dd17), li_64(152fecd8f70e5939)
};

int Nasha::Nasha224_Init(hashState *state)
{
	state->uu->hs256->count[0] = state->uu->hs256->count[1] = 0;
    memcpy(state->uu->hs256->H, nasha_i224H1, 32);
	memcpy(state->uu->hs256->hash, nasha_i224H2, 32);
    state->hashbitlen = 224; return SUCCESS;
}

int Nasha::Nasha224_Final(hashState256 *state, BitSequence *hashval)
{
    return Nasha_Final1(state, hashval,Nasha224_DIGEST_SIZE);
}

int Nasha::Nasha224_Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{   hashState  cx[1];

    Nasha224_Init(cx);
    Nasha256_Update(cx->uu->hs256, data,databitlen);
    return Nasha224_Final(cx->uu->hs256,hashval);
}
#endif

#if defined(Nasha_256)

/* Nasa256 chaining initial vectors */
const nasha_uint_64t nasha_i256H1[80] =
{
    li_64(510e527fade682d1), li_64(9b05688c2b3e6c1f),
    li_64(1f83d9abfb41bd6b), li_64(5be0cd19137e2179)
};
const nasha_uint_64t nasha_i256H2[80] =
{
    li_64(67332667ffc00b31), li_64(8eb44a8768581511),
    li_64(db0c2e0d64f98fa7), li_64(47b5481dbefa4fa4)
};
int Nasha::Nasha256_Init(hashState *state)
{
	state->uu->hs256->count[0] = state->uu->hs256->count[1] = 0;
    memcpy(state->uu->hs256->H, nasha_i256H1, 32);
	memcpy(state->uu->hs256->hash, nasha_i256H2, 32);
    state->hashbitlen = 256; return SUCCESS;
}
int Nasha::Nasha256_Final(hashState256 *state, BitSequence *hashval)
{
    return Nasha_Final1(state, hashval,Nasha256_DIGEST_SIZE);
}

int Nasha::Nasha256_Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{   hashState  cx[1];

    Nasha256_Init(cx);
    Nasha256_Update(cx->uu->hs256, data,databitlen);
    return Nasha256_Final(cx->uu->hs256,hashval);
}

#endif

#if defined(Nasha_384) || defined(Nasha_512)

#define Nasha512_MASK (Nasha512_BLOCK_SIZE - 1)

//Nasa384/512 initialisation
#define nasha_PutX_2(x,_state)\
{ int k; for (k=0;k<8;k++){		x[k<<2]=_state->M[k<<1]; \
		x[(k<<2)+1]= _state->H[k];		x[(k<<2)+2]= _state->M[(k<<1)+1]; \
		x[(k<<2)+3]= _state->hash[k];}}

//LinTr512^32 transformation
#define nasha_LinTr32(x,y)\
{y[31]=x[6]^x[14]^x[24]^x[31]; y[30]=x[5]^x[13]^x[23]^x[30]; y[29]=x[4]^x[12]^x[22]^x[29];\
 y[28]=x[3]^x[11]^x[21]^x[28]; y[27]=x[2]^x[10]^x[20]^x[27]; y[26]=x[1]^x[9]^x[19]^x[26];\
 y[25]=x[0]^x[8]^x[18]^x[25]; y[24]=y[31]^x[7]^x[17]^x[24];  y[23]=y[30]^x[6]^x[16]^x[23]; \
 y[22]=y[29]^x[5]^x[15]^x[22]; y[21]=y[28]^x[4]^x[14]^x[21]; y[20]=y[27]^x[3]^x[13]^x[20];\
 y[19]=y[26]^x[2]^x[12]^x[19]; y[18]=y[25]^x[1]^x[11]^x[18]; y[17]=y[24]^x[0]^x[10]^x[17];\
 y[16]=y[23]^y[31]^x[9]^x[16]; y[15]=y[22]^y[30]^x[8]^x[15];\
 y[14]=y[21]^y[29]^x[7]^x[14]; y[13]=y[20]^y[28]^x[6]^x[13]; \
 y[12]=y[19]^y[27]^x[5]^x[12]; y[11]=y[18]^y[26]^x[4]^x[11];\
 y[10]=y[17]^y[25]^x[3]^x[10]; y[9]=y[16]^y[24]^x[2]^x[9];\
 y[8]=y[15]^y[23]^x[1]^x[8]; y[7]=y[14]^y[22]^x[0]^x[7];\
 y[6]=y[13]^y[21]^y[31]^x[6]; y[5]=y[12]^y[20]^y[30]^x[5];\
 y[4]=y[11]^y[19]^y[29]^x[4]; y[3]=y[10]^y[18]^y[28]^x[3];\
 y[2]=y[9]^y[17]^y[27]^x[2];  y[1]=y[8]^y[16]^y[26]^x[1];  y[0]=y[7]^y[15]^y[25]^x[0];}

/* Compile 128 bytes of hash data into Nasha384/512 digest    */
void Nasha::Nasha512_compile(hashState512 *state)
{   int i;
	nasha_uint_64t x[32],y[32],l1,l2;
	B64 tmp;
	nasha_uint_32t A1, B1, C1, A2, B2, C2;
    nasha_uint_16t alpha1, beta1, gama1, alpha2, beta2, gama2;
	nasha_uint_8t a1,b1,c1,a2,b2,c2,a3,b3,c3;
	
	nasha_PutX_2(y,state);
	nasha_LinTr32(y,x);
	
	// computing the leaders l1 and l2
	l1=x[0]+x[1];
	l2=x[2]+x[3];
	
	//computing the 8-bite words a1, b1, c1,a2,b2,c2,a3,b3,c3
	tmp.bit64=x[4]+x[5];
	a1=tmp.b8[7]; b1=tmp.b8[6]; c1=tmp.b8[5];
	a2=tmp.b8[4]; b2=tmp.b8[3]; c2=tmp.b8[2];
	a3=tmp.b8[1]; b3=tmp.b8[0]; c3=a1;
	
	//computing the 16-bite words alpha1, beta1, gama1, alpha2, beta2, gama2
	tmp.bit64=x[6]+x[7];
	alpha1=tmp.bit16[3]; beta1=tmp.bit16[2]; gama1=tmp.bit16[1]; alpha2=tmp.bit16[0];
	
	tmp.bit64=x[8]+x[9]; 
	beta2=tmp.bit16[3]; gama2=tmp.bit16[2];
	
	//computing the 32-bite words A1, B1, C1, A2, B2, C2
	tmp.bit64=x[10]+x[11];
	A1=tmp.bit32[1]; B1=tmp.bit32[0];

	tmp.bit64=x[12]+x[13];
	C1=tmp.bit32[1]; A2=tmp.bit32[0];

	tmp.bit64=x[14]+x[15];
	B2=tmp.bit32[1]; C2=tmp.bit32[0];

	nasha_AE(l2,x,y,32,a1,b1,c1,a2,b2,c2,a3,b3,c3,alpha1, beta1, gama1,A1, B1, C1);
	nasha_swap_64(y,32);
	nasha_RAE(l1,y,x,31,a1,b1,c1,a2,b2,c2,a3,b3,c3,alpha2, beta2, gama2,A2, B2, C2);

	/*  Finally compute the hash digest (or the hash chain value) */
	for (i=0;i<8;i++){
		state->H[i]=x[(i<<2)+1];
		state->hash[i]=x[(i<<2)+3];
	}
}

int Nasha::Nasha512_Update(hashState512 *state, const BitSequence *data, DataLength databitlen)
{    DataLength len=databitlen;
     const unsigned char *sp = data;
     if ((state->count[0] += len)< len)
        ++(state->count[1]);
     while (len>(Nasha512_BLOCK_SIZE*8)){
           memcpy((unsigned char *)state->M, sp, Nasha512_BLOCK_SIZE);
           sp+=Nasha512_BLOCK_SIZE; len-=Nasha512_BLOCK_SIZE*8;
           nasha_bsw_64(state->M, 16);
           Nasha512_compile(state);
     }
    memcpy((unsigned char *)state->M, sp, len/8+1);

	/* put bytes in the buffer in an order in which references to   */
    /* 32-bit words will put bytes with lower addresses into the    */
    /* top of 32 bit words on BOTH big and little endian machines   */
    nasha_bsw_64(state->M, 16);
	return SUCCESS;
}

/* Nasha384/512 Final padding and digest calculation  */

int Nasha::Nasha_Final2(hashState512 *state, BitSequence *hashval,  const unsigned int hlen)
{   nasha_uint_32t    i = ((nasha_uint_32t)(state->count[0]>>3) & Nasha512_MASK);
    nasha_uint_64t    pom=state->count[1];
     
    /* we now need to mask valid bytes and add the padding which is */
    /* a single 1 bit and as many zero bits as necessary. */
    
	state->M[i>>3] &= li_64(fffffffffffffffe)>>(64-state->count[0]%64);
    state->M[i>>3] |= li_64(0000000000000001) << (state->count[0]%64);
    /* we need 17 or more empty positions, one for the padding byte  */
    /* (above) and eight for the length count.  If there is not     */
    /* enough space pad and empty the buffer                        */
    if(i > Nasha512_BLOCK_SIZE - 17)
    {
       	if(i < 120) state->M[15] = 0;
        Nasha512_compile(state);
        i = 0;
    }
    else    /* compute a word index for the empty buffer positions  */
        i = (i >> 3)+1;

    while(i < 14) /* and zero pad all but last two positions        */
        state->M[i++] = 0;
    state->M[14] = state->count[1];
	state->M[15] = state->count[0];

    Nasha512_compile(state);

    /* extract the hash value as bytes in case the hash buffer is   */
    /* misaligned for 32-bit words                                  */
	for(i = 0; i < hlen; ++i)
        hashval[i] = (unsigned char)(state->hash[i >> 3] >> (8 * (~i & 7)));
		
	return SUCCESS;
}

#endif

#if defined(Nasha_384)

/* Nasha384 initialisation data   */

const nasha_uint_64t nasha_i384H1[8] =
{
    li_64(6a09e667f3bcc908), li_64(bb67ae8584caa73b),
    li_64(3c6ef372fe94f82b), li_64(a54ff53a5f1d36f1),
    li_64(510e527fade682d1), li_64(9b05688c2b3e6c1f),
    li_64(1f83d9abfb41bd6b), li_64(5be0cd19137e2179)
};
const nasha_uint_64t nasha_i384H2[8] =
{
    li_64(cbbb9d5dc1059ed8), li_64(629a292a367cd507),
    li_64(9159015a3070dd17), li_64(152fecd8f70e5939),
    li_64(67332667ffc00b31), li_64(8eb44a8768581511),
    li_64(db0c2e0d64f98fa7), li_64(47b5481dbefa4fa4)
};

int Nasha::Nasha384_Init(hashState *state)
{
	state->uu->hs512->count[0] = state->uu->hs512->count[1] = 0;
    memcpy(state->uu->hs512->H, nasha_i384H1, 64);
	memcpy(state->uu->hs512->hash, nasha_i384H2, 64);
    state->hashbitlen = 384; return SUCCESS;
}
int Nasha::Nasha384_Final(hashState512 *state, BitSequence *hashval)
{
    return Nasha_Final2(state, hashval,Nasha384_DIGEST_SIZE);
}

int Nasha::Nasha384_Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{   hashState  cx[1];

    Nasha384_Init(cx);
    Nasha512_Update(cx->uu->hs512, data,databitlen);
    return Nasha384_Final(cx->uu->hs512,hashval);
}
#endif

#if defined(Nasha_512)

/* Nasha512 initialisation data   */

const nasha_uint_64t nasha_i512H1[8] =
{
    li_64(2dd8a09a3c4e3efb), li_64(061a77a060948dcd),
    li_64(8a47ea1880559ce6), li_64(9f22535b264607a8),
    li_64(2547d84e9ccde59d), li_64(9486eb50c7d8037f),
    li_64(c0f905d741c9cb74), li_64(ad0d1e41a985e51e)
};
const nasha_uint_64t nasha_i512H2[8] =
{
    li_64(e07688dc6f166b73), li_64(0c34aa2a315e01d5),
    li_64(c785f4364a0b98f4), li_64(53a8c8ca56e1288c),
    li_64(3c1563a9317c57a1), li_64(77341edad21e9a40),
    li_64(d648813e45121dbb), li_64(4cf768fc7df11b00)
};

int Nasha::Nasha512_Init(hashState *state)
{
	state->uu->hs512->count[0] = state->uu->hs512->count[1] = 0;
    memcpy(state->uu->hs512->H, nasha_i512H1, 64);
	memcpy(state->uu->hs512->hash, nasha_i512H2, 64);
    state->hashbitlen = 512; return SUCCESS;
}
int Nasha::Nasha512_Final(hashState512 *state, BitSequence *hashval)
{
    return Nasha_Final2(state, hashval,Nasha512_DIGEST_SIZE);
}

int Nasha::Nasha512_Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{   hashState  cx[1];

    Nasha512_Init(cx);
    Nasha512_Update(cx->uu->hs512, data,databitlen);
    return Nasha512_Final(cx->uu->hs512,hashval);
}
#endif

#if defined(Nasha_)

#define nasha_state_224(x)  ((x)->uu->hs256)
#define nasha_state_256(x)  ((x)->uu->hs256)
#define nasha_state_384(x)  ((x)->uu->hs512)
#define nasha_state_512(x)  ((x)->uu->hs512)

/* Nasha initialisation */

int Nasha::Init(int hashbitlen)
{
    switch(hashbitlen)
    {
#if defined(Nasha_224)
        case 224:   return Nasha224_Init(&nashaState);
#endif
#if defined(Nasha_256)
        case 256:   return Nasha256_Init(&nashaState);
#endif
#if defined(Nasha_384)
        case 384:   return Nasha384_Init(&nashaState);
#endif
#if defined(Nasha_512)
        case 512:   return Nasha512_Init(&nashaState);
#endif
        default:    return BAD_HASHLEN;
    }
}

int Nasha::Update(const BitSequence *data, DataLength databitlen)
{
    switch(nashaState.hashbitlen)
    {
#if defined(Nasha_224)
        case 224: return Nasha256_Update(nasha_state_224(&nashaState), data, databitlen); 
#endif
#if defined(Nasha_256)
        case 256: return Nasha256_Update(nasha_state_256(&nashaState), data, databitlen);
#endif
#if defined(Nasha_384)
        case 384: return Nasha512_Update(nasha_state_384(&nashaState), data, databitlen); 
#endif
#if defined(Nasha_512)
        case 512: return Nasha512_Update(nasha_state_512(&nashaState), data, databitlen); 
#endif
    }
}

int Nasha::Final(BitSequence *hashval)
{
    switch (nashaState.hashbitlen)
    {
#if defined(Nasha_224)
        case 224: return Nasha_Final1(nasha_state_224(&nashaState), hashval, Nasha224_DIGEST_SIZE); 
#endif
#if defined(Nasha_256)
        case 256: return Nasha_Final1(nasha_state_256(&nashaState), hashval, Nasha256_DIGEST_SIZE); 
#endif
#if defined(Nasha_384)
        case 384: return Nasha_Final2(nasha_state_384(&nashaState), hashval, Nasha384_DIGEST_SIZE); 
#endif
#if defined(Nasha_512)
        case 512: return Nasha_Final2(nasha_state_512(&nashaState), hashval, Nasha512_DIGEST_SIZE); 
#endif
    }
}

int Nasha::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{   //hashState    hs[1];

    if(Nasha::Init(hashbitlen) == SUCCESS)
    {
        Nasha::Update(data, databitlen); Nasha::Final(hashval); return SUCCESS;
    }
    else
       if (hashbitlen != 224 || hashbitlen != 256 || hashbitlen != 384 || hashbitlen!=512)
		   return BAD_HASHLEN;
	   else
		   return FAIL;
}

#endif

} // namespace sha3
