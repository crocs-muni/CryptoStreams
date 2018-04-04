/*\
\ / EnRUPT32 and EnRUPT64 in irRUPT stream hashing mode of operation
/ \ Designed and implemented by Sean O'Neil
\ / NIST SHA-3 submission by VEST Corporation
/ \ Released to the public domain by the author on November 1, 2008.
\ /
/ \ #define ENRUPT32_TYPE 0 for a generic unsigned long implementation
\ / #define ENRUPT32_TYPE 1 for x86 MMX intrinsics [Intel, MSVC and GCC] (only GCC supports MMX intrinsics on enrupt_x64)
/ \ #define ENRUPT32_TYPE 2 for a (mostly) unsigned long long implementation
\ /
/ \ #define ENRUPT64_TYPE 0 for a generic unsigned long long implementation
\ / #define ENRUPT64_TYPE 1 for x86 SSE intrinsics [Intel, MSVC and GCC]
/ \ #define ENRUPT64_TYPE 2 for a unsigned long-only implementation
\*/

#ifndef _portEnRUPT_h_
#define _portEnRUPT_h_

#include <limits.h>
#include <memory.h>

#if defined(__INTEL_COMPILER)
	#pragma warning (disable:138 167 177 810)
	#include <ia32intrin.h>
	#include <mmintrin.h>
	#include <xmmintrin.h>
	__m128i _mm_set1_epi64x(__int64 i);
	__int64 _mm_cvtsi128_si64x(__m128i a);
	/*#ifndef u8
		#define u8			unsigned char
	#endif
	#ifndef u16
		#define u16			unsigned short
	#endif
	#ifndef u32
		#define u32			unsigned long
	#endif
	#ifndef u64
		#define u64			unsigned long long
	#endif*/
	#define enrupt_rotl32(x,n)		_lrotl(x,n)
	#define enrupt_rotr32(x,n)		_lrotr(x,n)
	#define enrupt_rotl64(x,n)		_rotl64(x,n)
	#define enrupt_rotr64(x,n)		_rotr64(x,n)
	#define enrupt_bswap32(x)		_bswap(x)
	#define enrupt_bswap64(x)		_bswap64(x)
	#define enrupt_shl64(x,n)		((x)<<(n))
	#define enrupt_shr64(x,n)		((x)>>(n))
	typedef union _enrupt_ir_octet {unsigned long long q;unsigned long d[2];} enrupt_ir_octet;
#elif defined(_MSC_VER)
	#include <stdlib.h>
	#include <intrin.h>
	#pragma intrinsic(_lrotl,_lrotr,__ll_lshift,__ull_rshift,_rotl64,_rotr64,memcpy,memset)
	/*#ifndef u8
		#define u8			unsigned char
	#endif
	#ifndef u16
		#define u16			unsigned short
	#endif
	#ifndef u32
		#define u32			unsigned long
	#endif
	#ifndef u64
		#define u64			unsigned long long
	#endif*/
	#define enrupt_rotl32(x,n)		_lrotl(x,n)
	#define enrupt_rotr32(x,n)		_lrotr(x,n)
	#define enrupt_rotl64(x,n)		_rotl64(x,n)
	#define enrupt_rotr64(x,n)		_rotr64(x,n)
	#define enrupt_bswap32(x)		_byteswap_ulong(x)
	#define enrupt_bswap64(x)		_byteswap_uint64(x)
	#if (SIZE_MAX>ULONG_MAX)
		#define enrupt_shl64(x,n)	((x)<<(n))
		#define enrupt_shr64(x,n)	((x)>>(n))
	#else
		#define enrupt_shl64(x,n)	__ll_lshift(x,n)
		#define enrupt_shr64(x,n)	__ull_rshift(x,n)
	#endif
	typedef union _enrupt_ir_octet {unsigned long long q;unsigned long d[2];} enrupt_ir_octet;
#else
	#include <limits.h>
	#include <stdint.h>
	#include <inttypes.h>
	#include <sys/types.h>
    #include <mmintrin.h>
    #include <xmmintrin.h>
	/*#ifndef u8
		#define u8			uint8_t
	#endif
	#ifndef u16
		#define u16			uint16_t
	#endif
	#ifndef u32
		#define u32			uint32_t
	#endif
	#ifndef u64
		#define u64			uint64_t
	#endif*/
	static unsigned long				enrupt_rotl32 (unsigned long x, unsigned long r) { return (x << (r&31)) | (x >> ((0-r)&31)); }
	static unsigned long				enrupt_rotr32 (unsigned long x, unsigned long r) { return (x >> (r&31)) | (x << ((0-r)&31)); }
	static unsigned long long				enrupt_rotl64 (unsigned long long x, unsigned long r) { return (x << (r&63)) | (x >> ((0-r)&63)); }
	static unsigned long long				enrupt_rotr64 (unsigned long long x, unsigned long r) { return (x >> (r&63)) | (x << ((0-r)&63)); }
	#define enrupt_shl64(x,n)		((x)<<(n))
	#define enrupt_shr64(x,n)		((x)>>(n))
	#define enrupt_bswap32(x)		((enrupt_rotl32((unsigned long)(x)&0xFF00FF00UL,8))|(enrupt_rotr32((unsigned long)(x)&0x00FF00FFUL,8)))
	typedef union _enrupt_ir_octet {unsigned long long q;unsigned long d[2];} enrupt_ir_octet;
	static unsigned long long				enrupt_bswap64 (const unsigned long long x) {enrupt_ir_octet y,z;y.q=x,z.d[1]=enrupt_bswap32(y.d[0]),z.d[0]=enrupt_bswap32(y.d[1]);return z.q;}
#endif

#if !defined(ENRUPT32_TYPE)
	#if defined(i386)||defined(__i386__)||defined(_M_IX86)
		#define	ENRUPT32_TYPE	1		/* 0: generic unsigned long, 1: MMX, 2: via unsigned long long */
	#elif defined(__amd64__)||defined(__x86_64__)||defined(_M_IA64)||defined(_M_X64)
		#define	ENRUPT32_TYPE	0		/* 0: generic unsigned long, 1: MMX (GCC only), 2: via unsigned long long */
	#else								/* everything else */
		#define ENRUPT32_TYPE	0		/* 0: generic unsigned long, 2: via unsigned long long */
	#endif
#endif

#if !defined(ENRUPT64_TYPE)
	#if defined(i386)||defined(__i386__)||defined(_M_IX86)
		#define	ENRUPT64_TYPE	1		/* 0: generic unsigned long long, 1: SSE, 2: via unsigned long */
	#elif defined(__amd64__)||defined(__x86_64__)||defined(_M_IA64)||defined(_M_X64)
		#define	ENRUPT64_TYPE	0		/* 0: generic unsigned long long, 1: SSE, 2: via unsigned long */
	#else								/* everything else */
		#define ENRUPT64_TYPE	0		/* 0: generic unsigned long long, 2: via unsigned long */
	#endif
#endif

#if defined(__BYTE_ORDER)&&(__BYTE_ORDER==4321)||defined(BYTE_ORDER)&&(BYTE_ORDER==4321)||defined(sun)||defined(__sun)||defined(sparc)||defined(__sparc)||defined(__ppc__)
	#define ENRUPT_4321_BYTE_ORDER
#elif defined(__BYTE_ORDER)&&(__BYTE_ORDER==1234)||defined(BYTE_ORDER)&&(BYTE_ORDER==1234)||defined(i386)||defined(__i386__)||defined(__amd64__)||defined(__x86_64__)||defined(__vax__)||defined(__alpha)||defined(__ultrix)||defined(_M_IX86)||defined(_M_IA64)||defined(_M_X64)||defined(_M_ALPHA)
	#define ENRUPT_1234_BYTE_ORDER
#else
	#error Unknown endianness! Please define or disable this error and check at runtime.
#endif

#endif
