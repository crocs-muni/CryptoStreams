#include "Enrupt_sha3.h"
#include "stdio.h"

#if defined(ENRUPT64_TYPE)&&(ENRUPT64_TYPE==1)		/* EnRUPT64 with SSE via compiler intrinsics */

#define enrupt_uw64				__m128i
#define enrupt_x64					((enrupt_uw64 *) state->x)
#define enrupt_p64					((enrupt_uio64 *) state->p)

#define enrupt_ir2_64(R,H)\
(\
	f=_mm_xor_si128(_mm_xor_si128(_mm_xor_si128(_mm_slli_epi64(_mm_shuffle_epi32(enrupt_x64[(R)%H/2+2],0x4E),1),d),r),enrupt_x64[(R+4)%H/2+2]),\
	r=_mm_add_epi64(r,_enrupt_sse_2),\
	f=_mm_xor_si128(_mm_srli_epi64(f,16),_mm_slli_epi64(f,48)),\
	f=_mm_add_epi64(_mm_slli_epi64(f,3),f),\
	enrupt_x64[(R+2)%H/2+2]=_mm_xor_si128(f,enrupt_x64[(R+2)%H/2+2]),\
	d=_mm_xor_si128(_mm_xor_si128(d,enrupt_x64[(R)%H/2+2]),f)\
)
#if defined(__amd64__)||defined(__x86_64__)||defined(_M_IA64)||defined(_M_X64)	/* enrupt_x64 SSE intrinsics are slightly different */
	#define enrupt_uio64				unsigned long long
	#define enrupt_ir8_64(R,W,H,p,n)	(enrupt_ir08(R,W,H),d=_mm_xor_si128(d,_mm_set1_epi64x(enrupt_bswap64((p)[n]))))
	#define enrupt_sw64(hw,n)			((hw)[n]=enrupt_bswap64(_mm_cvtsi128_si64x(_mm_shuffle_epi32(d,0xEE))))
	#define enrupt_irufinish64()		enrupt_x64[0]=d,enrupt_x64[1]=r
	#define enrupt_irfdefine64()
	#define enrupt_irffinish64()
#elif defined(i386)||defined(__i386__)||defined(_M_IX86)
	#define enrupt_uio64				__m64
	#define bswap64mmi(p)		(_mm_shuffle_pi16(_mm_xor_si64(_mm_slli_pi16(p,8),_mm_srli_pi16(p,8)),0x1B))
	#define enrupt_ir8_64(R,W,H,p,n)	(enrupt_ir08(R,W,H),d=_mm_xor_si128(d,_mm_shuffle_epi32(_mm_movpi64_epi64(bswap64mmi((p)[n])),0x4E)))
	#define enrupt_sw64(hw,n)			(o=_mm_movepi64_pi64(_mm_shuffle_epi32(d,0xEE)),(hw)[n]=bswap64mmi(o))
	#define enrupt_irfdefine64()		register enrupt_uio64 o
	#define enrupt_irufinish64()		enrupt_x64[0]=d,enrupt_x64[1]=r,_mm_empty()
	#define enrupt_irffinish64()		_mm_empty()
#else
	#error Unknown processor type. Is it IA32 or IA64?
#endif
#define enrupt_irudefine64()		register enrupt_uw64 d=enrupt_x64[0],r=enrupt_x64[1],f

#ifdef __GNUC__
	static __m128i			_enrupt_sse_2 = {2,2};
#else
	static __m128i			_enrupt_sse_2 = {2,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0};
#endif

#elif defined(ENRUPT64_TYPE)&&(ENRUPT64_TYPE==2)	/* EnRUPT64 with unsigned long */

#define enrupt_uw64				unsigned long
#define enrupt_uio64				unsigned long
#define enrupt_x64					((enrupt_uw64 *) state->x)
#define enrupt_p64					((enrupt_uio64 *) state->p)

#if defined(ENRUPT_1234_BYTE_ORDER)
	#define enrupt_lw64(p,n)		(enrupt_x64[3]^=enrupt_bswap32((p)[(n)*2]),enrupt_x64[2]^=enrupt_bswap32((p)[(n)*2+1]))
	#define enrupt_sw64(hw,n)		((hw)[(n)*2]=enrupt_bswap32(enrupt_x64[3]),(hw)[(n)*2+1]=enrupt_bswap32(enrupt_x64[2]))
#elif defined(ENRUPT_4321_BYTE_ORDER)
	#define enrupt_lw64(p,n)		(enrupt_x64[2]^=(p)[(n)*2],enrupt_x64[3]^=(p)[(n)*2+1])
	#define enrupt_sw64(hw,n)		((hw)[(n)*2]=enrupt_x64[2],(hw)[(n)*2+1]=enrupt_x64[3])
#else
	#error Unknown endianness! Please define.
#endif
#define enrupt_ir1_64(R,H)\
(\
	f1=((enrupt_x64[(R^1)%H*2+9]<<1)+(enrupt_x64[(R^1)%H*2+8]>>31))^enrupt_x64[(R+4)%H*2+9]^enrupt_x64[((R)&1)*2+1]^enrupt_x64[5],\
	f0= (enrupt_x64[(R^1)%H*2+8]<<1)^enrupt_x64[(R+4)%H*2+8]^enrupt_x64[((R)&1)*2]^enrupt_x64[4],\
	t=(f0>>16)+(f1<<16),f1=(f1>>16)+(f0<<16),f0=t*9,f1+=(f1<<3)+(t>>29)+(f0<t),\
	enrupt_x64[5]+=!(++enrupt_x64[4]),\
	enrupt_x64[(R+2)%H*2+8]^=f0,enrupt_x64[(R+2)%H*2+9]^=f1,\
	enrupt_x64[((R)&1)*2]^=enrupt_x64[(R)%H*2+8],enrupt_x64[((R)&1)*2+1]^=enrupt_x64[(R)%H*2+9],\
	enrupt_x64[((R)&1)*2]^=f0,enrupt_x64[((R)&1)*2+1]^=f1\
)
#define enrupt_ir8_64(R,W,H,p,n)	(enrupt_ir08(R,W,H),enrupt_lw64(p,n))
#define enrupt_ir2_64(R,H)			(enrupt_ir1_64(R,H),enrupt_ir1_64(R+1,H))
#define enrupt_irudefine64()		register enrupt_uw64 f0, f1, t
#define enrupt_irfdefine64()
#define enrupt_irufinish64()
#define enrupt_irffinish64()

#else						/* EnRUPT64 with unsigned long long, a generic implementation */

#define enrupt_uw64				unsigned long long
#define enrupt_uio64				unsigned long long
#define enrupt_x64					((enrupt_uw64 *) state->x)
#define enrupt_p64					((enrupt_uio64 *) state->p)

#if defined(ENRUPT_1234_BYTE_ORDER)
	#define enrupt_lw64(p,n)		(enrupt_x64[1]^=enrupt_bswap64((p)[n]))
	#define enrupt_sw64(hw,n)		((hw)[n]=enrupt_bswap64(enrupt_x64[1]))
#elif defined(ENRUPT_4321_BYTE_ORDER)
	#define enrupt_lw64(p,n)		(enrupt_x64[1]^=(p)[n])
	#define enrupt_sw64(hw,n)		((hw)[n]=enrupt_x64[1])
#else
	#error Unknown endianness! Please define.
#endif

#define enrupt_ir1_64(R,H)\
(\
	f=enrupt_shl64(enrupt_x64[(R^1)%H+4],1),\
	f^=enrupt_x64[(R+4)%H+4],\
	f^=enrupt_x64[2],\
	f^=enrupt_x64[(R)&1],\
	f=enrupt_rotr64(f,16),\
	enrupt_x64[2]++,\
	f+=enrupt_shl64(f,3),\
	enrupt_x64[(R+2)%H+4]^=f,\
	enrupt_x64[(R)&1]^=enrupt_x64[(R)%H+4],\
	enrupt_x64[(R)&1]^=f\
)
#define enrupt_ir8_64(R,W,H,p,n)	(enrupt_ir08(R,W,H),enrupt_lw64(p,n))
#define enrupt_ir2_64(R,H)			(enrupt_ir1_64(R,H),enrupt_ir1_64(R+1,H))
#define enrupt_irudefine64()		register enrupt_uw64 f
#define enrupt_irfdefine64()
#define enrupt_irufinish64()
#define enrupt_irffinish64()

#endif

#if defined(ENRUPT32_TYPE)&&(ENRUPT32_TYPE==1)		/* EnRUPT32 with MMX via compiler intrinsics (on enrupt_x64: GCC only, not yet supported by MSVC or Intel compiler) */

#define enrupt_uw32				__m64
#define enrupt_uio32				unsigned long
#define enrupt_x32					((enrupt_uw32 *) state->x)
#define enrupt_p32					((enrupt_uio32 *) state->p)

#define enrupt_ir2_32(R,H)\
(\
	f=_mm_xor_si64(_mm_xor_si64(_mm_xor_si64(_mm_slli_pi32(_mm_shuffle_pi16(enrupt_x32[(R)%H/2+2],0x4E),1),d),r),enrupt_x32[(R+4)%H/2+2]),\
	r=_mm_add_pi32(r,_enrupt_mmx_2),\
	f=_mm_xor_si64(_mm_srli_pi32(f,8),_mm_slli_pi32(f,24)),\
	f=_mm_add_pi32(_mm_slli_pi32(f,3),f),\
	enrupt_x32[(R+2)%H/2+2]=_mm_xor_si64(f,enrupt_x32[(R+2)%H/2+2]),\
	d=_mm_xor_si64(_mm_xor_si64(d,enrupt_x32[(R)%H/2+2]),f)\
)
#define enrupt_ir8_32(R,W,H,p,n)	(enrupt_ir08(R,W,H),d=_mm_xor_si64(d,_mm_shuffle_pi16(_mm_cvtsi32_si64(enrupt_bswap32((p)[n])),0x4E)))
#define enrupt_sw32(hw,n)			((hw)[n]=enrupt_bswap32(_mm_cvtsi64_si32(_mm_shuffle_pi16(d,0xEE))))
#define enrupt_irudefine32()		register enrupt_uw32 d=enrupt_x32[0],r=enrupt_x32[1],f
#define enrupt_irufinish32()		enrupt_x32[0]=d,enrupt_x32[1]=r,_mm_empty()
#define enrupt_irfdefine32()
#define enrupt_irffinish32()		_mm_empty()

#if !defined(__GNUC__)||!defined(__STDC__)||defined(__APPLE_CC__)&&(__APPLE_CC__>1)
	static __m64			_enrupt_mmx_2 = {0x200000002ULL};	/* MSVC, Intel Compiler and Apple GCC */
#else
	static __m64			_enrupt_mmx_2 = {2,2};		/* Most versions of GCC require two 32-bit constants here. Watch out! */
#endif

#elif defined(ENRUPT32_TYPE)&&(ENRUPT32_TYPE==2)	/* EnRUPT32 with (mostly) unsigned long long */

#define enrupt_uw32				unsigned long long
#define enrupt_uio32				unsigned long
#define enrupt_x32					((enrupt_uw32 *) state->x)
#define enrupt_p32					((enrupt_uio32 *) state->p)

#define enrupt_ir8_32(R,W,H,p,n)	(enrupt_ir08(R,W,H),d.d[1]^=enrupt_bswap32((p)[n]))
#define enrupt_sw32(hw,n)			((hw)[n]=enrupt_bswap32(d.d[1]))
#define enrupt_ir2_32(R,H)\
(\
	f.q=(enrupt_rotr64(enrupt_x32[(R)%H/2+2],31)&0xFFFFFFFEFFFFFFFEULL)^d.q^r.q^enrupt_x32[(R+4)%H/2+2],\
	r.d[0]+=2,r.d[1]+=2,\
	f.d[0]=enrupt_rotr32(f.d[0],8),f.d[1]=enrupt_rotr32(f.d[1],8),\
	f.d[0]+=f.d[0]<<3,f.d[1]+=f.d[1]<<3,\
	enrupt_x32[(R+2)%H/2+2]^=f.q,\
	d.q^=f.q^enrupt_x32[(R)%H/2+2]\
)
#define enrupt_irudefine32()		register enrupt_ir_octet d,r,f;d.q=enrupt_x32[0],r.q=enrupt_x32[1]
#define enrupt_irfdefine32()
#define enrupt_irufinish32()		enrupt_x32[0]=d.q,enrupt_x32[1]=r.q
#define enrupt_irffinish32()

#else						/* EnRUPT32 with unsigned long, a generic implementation */

#define enrupt_uw32				unsigned long
#define enrupt_uio32				unsigned long
#define enrupt_x32					((enrupt_uw32 *) state->x)
#define enrupt_p32					((enrupt_uio32 *) state->p)

#if defined(ENRUPT_1234_BYTE_ORDER)
	#define enrupt_lw32(p,n)		(enrupt_x32[1]^=enrupt_bswap32((p)[n]))
	#define enrupt_sw32(hw,n)		((hw)[n]=enrupt_bswap32(enrupt_x32[1]))
#elif defined(ENRUPT_4321_BYTE_ORDER)
	#define enrupt_lw32(p,n)		(enrupt_x32[1]^=(p)[n])
	#define enrupt_sw32(hw,n)		((hw)[n]=enrupt_x32[1])
#else
	#error Unknown endianness! Please define.
#endif
#define enrupt_ir2_32(R,H)\
(\
	f0=enrupt_x32[(R+1)%H+4]<<1,	f1=enrupt_x32[(R)%H+4]<<1,\
	f0^=enrupt_x32[(R+4)%H+4],		f1^=enrupt_x32[(R+5)%H+4],\
	f0^=enrupt_x32[2],				f1^=enrupt_x32[3],\
	f0^=enrupt_x32[0],				f1^=enrupt_x32[1],\
	f0=enrupt_rotr32(f0,8),		f1=enrupt_rotr32(f1,8),\
	enrupt_x32[2]+=2,				enrupt_x32[3]+=2,\
	f0+=f0<<3,				f1+=f1<<3,\
	enrupt_x32[(R+2)%H+4]^=f0,		enrupt_x32[(R+3)%H+4]^=f1,\
	enrupt_x32[0]^=enrupt_x32[(R)%H+4],	enrupt_x32[1]^=enrupt_x32[(R+1)%H+4],\
	enrupt_x32[0]^=f0,				enrupt_x32[1]^=f1\
)
#define enrupt_ir8_32(R,W,H,p,n)	(enrupt_ir08(R,W,H),enrupt_lw32(p,n))
#define enrupt_irudefine32()		register enrupt_uw32 f0, f1
#define enrupt_irfdefine32()
#define enrupt_irufinish32()
#define enrupt_irffinish32()

#endif

#define enrupt_ir08(R,W,H)			(enrupt_ir2_##W(R,H),enrupt_ir2_##W(R+2,H),enrupt_ir2_##W(R+4,H),enrupt_ir2_##W(R+6,H))
#define enrupt_ir8(R,W,H,p,n)		(enrupt_ir8_##W(R,W,H,p,n))

#define EnRUPTxH_8(p,W)		(enrupt_ir8(0,W, 8,p,0),enrupt_ir8(8,W, 8,p,1),enrupt_ir8(16,W, 8,p,2),enrupt_ir8(24,W, 8,p,3))
#define EnRUPTxH_10(p,W)	(enrupt_ir8(0,W,10,p,0),enrupt_ir8(8,W,10,p,1),enrupt_ir8(16,W,10,p,2),enrupt_ir8(24,W,10,p,3),enrupt_ir8(32,W,10,p,4))
#define EnRUPTxH_12(p,W)	(enrupt_ir8(0,W,12,p,0),enrupt_ir8(8,W,12,p,1),enrupt_ir8(16,W,12,p,2),enrupt_ir8(24,W,12,p,3),enrupt_ir8(32,W,12,p,4),enrupt_ir8(40,W,12,p,5))
#define EnRUPTxH_14(p,W)	(enrupt_ir8(0,W,14,p,0),enrupt_ir8(8,W,14,p,1),enrupt_ir8(16,W,14,p,2),enrupt_ir8(24,W,14,p,3),enrupt_ir8(32,W,14,p,4),enrupt_ir8(40,W,14,p,5),enrupt_ir8(48,W,14,p,6))
#define EnRUPTxH_16(p,W)	(enrupt_ir8(0,W,16,p,0),enrupt_ir8(8,W,16,p,1),enrupt_ir8(16,W,16,p,2),enrupt_ir8(24,W,16,p,3),enrupt_ir8(32,W,16,p,4),enrupt_ir8(40,W,16,p,5),enrupt_ir8(48,W,16,p,6),enrupt_ir8(56,W,16,p,7))
#define EnRUPT0x1_8(W)		enrupt_ir08(0,W,8)
#define EnRUPT0x1_10(W)		((iri==0)?(enrupt_ir08(0,W,10),iri=8):(iri==2)?(enrupt_ir08(2,W,10),iri= 0):(iri==4)?(enrupt_ir08(4,W,10),iri= 2):(iri==6)?(enrupt_ir08(6,W,10),iri=4):(iri==8)?(enrupt_ir08(8,W,10),iri=6))
#define EnRUPT0x1_12(W)		((iri==0)?(enrupt_ir08(0,W,12),iri=8):(iri==2)?(enrupt_ir08(2,W,12),iri=10):(iri==4)?(enrupt_ir08(4,W,12),iri= 0):(iri==6)?(enrupt_ir08(6,W,12),iri=2):(iri==8)?(enrupt_ir08(8,W,12),iri=4):(enrupt_ir08(10,W,12),iri=6))
#define EnRUPT0x1_14(W)		((iri==0)?(enrupt_ir08(0,W,14),iri=8):(iri==2)?(enrupt_ir08(2,W,14),iri=10):(iri==4)?(enrupt_ir08(4,W,14),iri=12):(iri==6)?(enrupt_ir08(6,W,14),iri=0):(iri==8)?(enrupt_ir08(8,W,14),iri=2):(iri==10)?(enrupt_ir08(10,W,14),iri=4):(enrupt_ir08(12,W,14),iri=6))
#define EnRUPT0x1_16(W)		((iri==0)?(enrupt_ir08(0,W,16),iri=8):(enrupt_ir08(8,W,16),iri=0))
#define EnRUPT0xH_8(h,W)	(enrupt_ir8(0,W, 8,h,0),enrupt_ir08(8,W, 8),enrupt_ir08(16,W, 8),enrupt_ir08(24,W, 8))
#define EnRUPT0xH_10(h,W)	(enrupt_ir8(0,W,10,h,0),enrupt_ir08(8,W,10),enrupt_ir08(16,W,10),enrupt_ir08(24,W,10),enrupt_ir08(32,W,10))
#define EnRUPT0xH_12(h,W)	(enrupt_ir8(0,W,12,h,0),enrupt_ir08(8,W,12),enrupt_ir08(16,W,12),enrupt_ir08(24,W,12),enrupt_ir08(32,W,12),enrupt_ir08(40,W,12))
#define EnRUPT0xH_14(h,W)	(enrupt_ir8(0,W,14,h,0),enrupt_ir08(8,W,14),enrupt_ir08(16,W,14),enrupt_ir08(24,W,14),enrupt_ir08(32,W,14),enrupt_ir08(40,W,14),enrupt_ir08(48,W,14))
#define EnRUPT0xH_16(h,W)	(enrupt_ir8(0,W,16,h,0),enrupt_ir08(8,W,16),enrupt_ir08(16,W,16),enrupt_ir08(24,W,16),enrupt_ir08(32,W,16),enrupt_ir08(40,W,16),enrupt_ir08(48,W,16),enrupt_ir08(56,W,16))

#define define_EnRUPT_Update(W,h,H)\
int EnRUPTu##W##_##h (enruptHashState *state, const EnruptBitSequence *data, EnruptDataLength databitlen)\
{\
	size_t				i = H*W/2-state->n;\
	enrupt_irudefine##W();\
	\
	if (state->n&7)\
	{\
		enrupt_irufinish##W();\
		return ENRUPT_FAIL;\
	}\
	if (databitlen < i)\
	{\
		memcpy (state->p+(state->n>>3),data,(databitlen+7)>>3);\
		state->n += (int)databitlen;\
		enrupt_irufinish##W();\
		return ENRUPT_SUCCESS;\
	}\
	if (state->n)\
	{\
		memcpy (state->p+(state->n>>3),data,i>>3);\
		EnRUPTxH_##H (enrupt_p##W, W);\
		databitlen -= i;\
		state->n = 0;\
	}\
	else i = 0;\
	for (; databitlen >= H*W/2; databitlen -= H*W/2, i += H*W/2)\
	{\
		EnRUPTxH_##H ((enrupt_uio##W *)(data+(i>>3)), W);\
	}\
	enrupt_irufinish##W();\
	if ((int)databitlen)\
	{\
		memcpy (state->p, data+(i>>3), (databitlen+7)>>3);\
		state->n = (int)databitlen;\
	}\
	return ENRUPT_SUCCESS;\
}

#define define_EnRUPT_Final(W,h,H)\
int EnRUPTf##W##_##h (enruptHashState *state, EnruptBitSequence *hashval)\
{\
	register int		i = state->n>>3, j = (state->n&7)^7, iri = 0;\
	\
	enrupt_irudefine##W();\
	enrupt_irfdefine##W();\
	\
	if (state->n < 0)\
	{\
		enrupt_irffinish##W();\
		return ENRUPT_FAIL;\
	}\
	state->p[i] &= -1 << j;\
	state->p[i] |=  1 << j;\
	memset (state->p+i+1,0,(H+4)*W/16-i-1);\
	state->p[(i+W/8)|(W/8-1)] = h;\
	state->p[((i+W/8)|(W/8-1))-1] = h>>8;\
	EnRUPTxH_##H (enrupt_p##W, W);\
	EnRUPT0xH_##H ((enrupt_uio##W *)(state->p+H*W/16),W);\
	for (i = state->n/W+2; i; i--) { EnRUPT0x1_##H (W); }\
	for (i = 0; i < h/W; i++) { EnRUPT0x1_##H (W); enrupt_sw##W ((enrupt_uio##W *)hashval,i); }\
	if (h%W) { unsigned char lasth[W/8]; EnRUPT0x1_##H (W); enrupt_sw##W ((enrupt_uio##W *)lasth,0); memcpy (hashval+(h-h%W)/8, lasth, (h%W+7)/8); }\
	enrupt_irffinish##W();\
	state->n = -1;\
	return ENRUPT_SUCCESS;\
}

/*	Include only the sizes you need or it may take too long to compile. */

/*	define_EnRUPT_Update(32,  64, 4)	define_EnRUPT_Final(32,  64, 4)	*/
/*	define_EnRUPT_Update(32,  96, 6)	define_EnRUPT_Final(32,  96, 6)	*/
/*	define_EnRUPT_Update(32, 128, 8)	define_EnRUPT_Final(32, 128, 8)	*/
/*	define_EnRUPT_Update(32, 160,10)	define_EnRUPT_Final(32, 160,10)	*/
/*	define_EnRUPT_Update(32, 192,12)	define_EnRUPT_Final(32, 192,12)	*/
/*	define_EnRUPT_Update(32, 224,14)	define_EnRUPT_Final(32, 224,14)	*/
/*	define_EnRUPT_Update(32, 256,16)	define_EnRUPT_Final(32, 256,16)	*/
/*	define_EnRUPT_Update(32, 288,18)	define_EnRUPT_Final(32, 288,18)	*/
/*	define_EnRUPT_Update(32, 320,20)	define_EnRUPT_Final(32, 320,20)	*/
/*	define_EnRUPT_Update(32, 352,22)	define_EnRUPT_Final(32, 352,22)	*/
/*	define_EnRUPT_Update(32, 384,24)	define_EnRUPT_Final(32, 384,24)	*/
/*	define_EnRUPT_Update(32, 416,26)	define_EnRUPT_Final(32, 416,26)	*/
/*	define_EnRUPT_Update(32, 448,28)	define_EnRUPT_Final(32, 448,28)	*/
/*	define_EnRUPT_Update(32, 480,30)	define_EnRUPT_Final(32, 480,30)	*/
/*	define_EnRUPT_Update(32, 512,32)	define_EnRUPT_Final(32, 512,32)	*/
/*	define_EnRUPT_Update(32, 544,34)	define_EnRUPT_Final(32, 544,34)	*/
/*	define_EnRUPT_Update(32, 576,36)	define_EnRUPT_Final(32, 576,36)	*/
/*	define_EnRUPT_Update(32, 608,38)	define_EnRUPT_Final(32, 608,38)	*/
/*	define_EnRUPT_Update(32, 640,40)	define_EnRUPT_Final(32, 640,40)	*/
/*	define_EnRUPT_Update(32, 672,42)	define_EnRUPT_Final(32, 672,42)	*/
/*	define_EnRUPT_Update(32, 704,44)	define_EnRUPT_Final(32, 704,44)	*/
/*	define_EnRUPT_Update(32, 736,46)	define_EnRUPT_Final(32, 736,46)	*/
/*	define_EnRUPT_Update(32, 768,48)	define_EnRUPT_Final(32, 768,48)	*/

/*	define_EnRUPT_Update(64, 128, 4)	define_EnRUPT_Final(64, 128, 4)	*/
/*	define_EnRUPT_Update(64, 160, 6)	define_EnRUPT_Final(64, 160, 6)	*/
/*	define_EnRUPT_Update(64, 192, 6)	define_EnRUPT_Final(64, 192, 6)	*/
/*	define_EnRUPT_Update(64, 224, 8)*/	define_EnRUPT_Final(64, 224, 8)
	define_EnRUPT_Update(64, 256, 8)	define_EnRUPT_Final(64, 256, 8)
/*	define_EnRUPT_Update(64, 320,10)	define_EnRUPT_Final(64, 320,10)	*/
	define_EnRUPT_Update(64, 384,12)	define_EnRUPT_Final(64, 384,12)
/*	define_EnRUPT_Update(64, 448,14)	define_EnRUPT_Final(64, 448,14)	*/
	define_EnRUPT_Update(64, 512,16)	define_EnRUPT_Final(64, 512,16)
/*	define_EnRUPT_Update(64, 576,18)	define_EnRUPT_Final(64, 576,18)	*/
/*	define_EnRUPT_Update(64, 640,20)	define_EnRUPT_Final(64, 640,20)	*/
/*	define_EnRUPT_Update(64, 704,22)	define_EnRUPT_Final(64, 704,22)	*/
/*	define_EnRUPT_Update(64, 768,24)	define_EnRUPT_Final(64, 768,24)	*/
/*	define_EnRUPT_Update(64, 832,26)	define_EnRUPT_Final(64, 832,26)	*/
/*	define_EnRUPT_Update(64, 896,28)	define_EnRUPT_Final(64, 896,28)	*/
/*	define_EnRUPT_Update(64, 960,30)	define_EnRUPT_Final(64, 960,30)	*/
/*	define_EnRUPT_Update(64,1024,32)	define_EnRUPT_Final(64,1024,32)	*/
/*	define_EnRUPT_Update(64,1088,34)	define_EnRUPT_Final(64,1088,34)	*/
/*	define_EnRUPT_Update(64,1152,36)	define_EnRUPT_Final(64,1152,36)	*/
/*	define_EnRUPT_Update(64,1216,38)	define_EnRUPT_Final(64,1216,38)	*/
/*	define_EnRUPT_Update(64,1280,40)	define_EnRUPT_Final(64,1280,40)	*/
/*	define_EnRUPT_Update(64,1344,42)	define_EnRUPT_Final(64,1344,42)	*/
/*	define_EnRUPT_Update(64,1408,44)	define_EnRUPT_Final(64,1408,44)	*/
/*	define_EnRUPT_Update(64,1472,46)	define_EnRUPT_Final(64,1472,46)	*/
/*	define_EnRUPT_Update(64,1536,48)	define_EnRUPT_Final(64,1536,48)	*/

#define EnRUPT_Init(s)\
{\
	memset (s, 0, sizeof(*s));\
	(s)->hashbitlen = hashbitlen;\
	switch (hashbitlen)\
	{\
	case 224: (s)->u=(enrupt_iru*)&EnRUPTu64_256,(s)->f=(enrupt_irf*)&EnRUPTf64_224;((unsigned long*)(s)->x)[6]++; break;\
	case 256: (s)->u=(enrupt_iru*)&EnRUPTu64_256,(s)->f=(enrupt_irf*)&EnRUPTf64_256;((unsigned long*)(s)->x)[6]++; break;\
	case 384: (s)->u=(enrupt_iru*)&EnRUPTu64_384,(s)->f=(enrupt_irf*)&EnRUPTf64_384;((unsigned long*)(s)->x)[6]++; break;\
	case 512: (s)->u=(enrupt_iru*)&EnRUPTu64_512,(s)->f=(enrupt_irf*)&EnRUPTf64_512;((unsigned long*)(s)->x)[6]++; break;\
	default: return ENRUPT_BAD_HASHBITLEN;\
	}\
}

int Enrupt::Init (int hashbitlen) { EnRUPT_Init(&enruptState) return ENRUPT_SUCCESS; }
int Enrupt::Update (const EnruptBitSequence *data, EnruptDataLength databitlen) { return enruptState.u (&enruptState, data, databitlen); }
int Enrupt::Final (EnruptBitSequence *hashval) { return enruptState.f (&enruptState, hashval); }
int Enrupt::Hash (int hashbitlen, const EnruptBitSequence *data, EnruptDataLength databitlen, EnruptBitSequence *hashval) { /*enruptHashState state;*/ EnRUPT_Init(&enruptState) enruptState.u (&enruptState, data, databitlen); return enruptState.f (&enruptState, hashval); }