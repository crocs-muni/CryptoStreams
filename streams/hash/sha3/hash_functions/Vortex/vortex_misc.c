
/* Written by Michael E. Kounavis, Commmunications Technology Lab,
   Intel Corporation, 2008
 */

#include "int_types.h"
#include "vortex_misc.h"

extern vortex_uint32_t vortex_enc_table_0[256];
extern vortex_uint32_t vortex_enc_table_1[256];
extern vortex_uint32_t vortex_enc_table_2[256];
extern vortex_uint32_t vortex_enc_table_3[256];
extern vortex_uint8_t  vortex_rijndael_sbox[256];

#ifdef VORTEX_OPTIMIZED_64
#ifdef _MSC_VER
unsigned __int64 _umul128( 
unsigned __int64 Multiplier, 
unsigned __int64 Multiplicand, 
unsigned __int64 *HighProduct 
);
#pragma intrinsic(_umul128)
#endif //_MSC_VER
#endif //VORTEX_OPTIMIZED_64

/* Note: The following algorithm performs 64-bit carry-less multiplication. 
   The concept is obtained from the code which is part from the OpenSSLg distribution.
   The algorithm has been re-implemented by Michael Kounavis */

vortex_uint64_t 
vortex_cmul64(vortex_uint64_t a, 
	   vortex_uint64_t b, 
	   vortex_uint64_t *d)
{
	vortex_uint64_t high, low, slice;
	vortex_uint64_t table[16], top_3_bits = a >> 61;
	vortex_uint64_t d_1, d_2, d_4, d_8;

	d_1 = a & (vortex_uint64_t)(0x1fffffffffffffff); 
	d_2 = d_1 << 1; 
	d_4 = d_2 << 1; 
	d_8 = d_4 << 1;

	table[ 0] = 0;     
	table[ 1] = d_1;       
	table[ 2] = d_2;       
	table[ 3] = d_1^d_2;
	table[ 4] = d_4;    
	table[ 5] = d_1^d_4;    
	table[ 6] = d_2^d_4;    
	table[ 7] = d_1^d_2^d_4;
	table[ 8] = d_8;    
	table[ 9] = d_1^d_8;    
	table[10] = d_2^d_8;    
	table[11] = d_1^d_2^d_8;
	table[12] = d_4^d_8; 
	table[13] = d_1^d_4^d_8; 
	table[14] = d_2^d_4^d_8; 
	table[15] = d_1^d_2^d_4^d_8;

	slice = table[b & 0xf]; 
	low = slice;
	slice = table[b >> 4 & 0xf]; 
	low ^= slice <<  4; 
	high  = slice >> 60;
	slice = table[b >> 8 & 0xf]; 
	low ^= slice <<  8; 
	high ^= slice >> 56;
	slice = table[b >> 12 & 0xf]; 
	low ^= slice << 12; 
	high ^= slice >> 52;
	slice = table[b >> 16 & 0xf]; 
	low ^= slice << 16; 
	high ^= slice >> 48;
	slice = table[b >> 20 & 0xf]; 
	low ^= slice << 20; 
	high ^= slice >> 44;
	slice = table[b >> 24 & 0xf]; 
	low ^= slice << 24; 
	high ^= slice >> 40;
	slice = table[b >> 28 & 0xf]; 
	low ^= slice << 28; 
	high ^= slice >> 36;
	slice = table[b >> 32 & 0xf]; 
	low ^= slice << 32; 
	high ^= slice >> 32;
	slice = table[b >> 36 & 0xf]; 
	low ^= slice << 36; 
	high ^= slice >> 28;
	slice = table[b >> 40 & 0xf]; 
	low ^= slice << 40; 
	high ^= slice >> 24;
	slice = table[b >> 44 & 0xf]; 
	low ^= slice << 44; 
	high ^= slice >> 20;
	slice = table[b >> 48 & 0xf]; 
	low ^= slice << 48; 
	high ^= slice >> 16;
	slice = table[b >> 52 & 0xf]; 
	low ^= slice << 52; 
	high ^= slice >> 12;
	slice = table[b >> 56 & 0xf]; 
	low ^= slice << 56; 
	high ^= slice >>  8;
	slice = table[b >> 60]; 
	low ^= slice << 60; 
	high ^= slice >>  4;

	if (top_3_bits & 1) 
	{ 
		low ^= b << 61; 
		high ^= b >> 3; 
	} 
	if (top_3_bits & 2) 
	{ 
		low ^= b << 62; 
		high ^= b >> 2; 
	} 
	if (top_3_bits & 4) 
	{ 
		low ^= b << 63; 
		high ^= b >> 1; 
	} 
	*d = high; 
	return low;
};

vortex_uint64_t 
vortex_imul64(vortex_uint64_t a, 
	   vortex_uint64_t b, 
	   vortex_uint64_t *d)
{
	vortex_uint64_t l, h, c;
	vortex_uint32_t a0, b0, a1, b1;
	vortex_uint64_t p0, p1, p2, p3;

	a0 = (vortex_uint32_t)(a & (vortex_uint64_t)0xffffffff);
	a1 = (vortex_uint32_t)((a>>32) & (vortex_uint64_t)0xffffffff);
	b0 = (vortex_uint32_t)(b & (vortex_uint64_t)0xffffffff);
	b1 = (vortex_uint32_t)((b>>32) & (vortex_uint64_t)0xffffffff);

	p0 = (vortex_uint64_t)a0 * (vortex_uint64_t)b0;
	p1 = (vortex_uint64_t)a1 * (vortex_uint64_t)b0;
	p2 = (vortex_uint64_t)a0 * (vortex_uint64_t)b1;
	p3 = (vortex_uint64_t)a1 * (vortex_uint64_t)b1;

	c = 0;
	l = p0 + (p1 << 32);
	if(l  < p0)
		c++;
	l += (p2 << 32);
	if(l  < (p2 << 32))
		c++;
	h = (p1 >> 32) + (p2 >> 32) + p3 + c;
	*d = h;
	return l;
}

/* For an AES round we currently use a 4K table lookup implementation.
   Future procesor releases will support processor instructions for AES
   round computation
 */
   

int
vortex_rijndael_128_round(vortex_uint8_t *key,
				   vortex_uint8_t *state)
{
	vortex_uint32_t s0, s1, s2, s3, t0, t1, t2, t3;

	s0 = *(vortex_uint32_t *)&state[0];
	s1 = *(vortex_uint32_t *)&state[4];
	s2 = *(vortex_uint32_t *)&state[8];
	s3 = *(vortex_uint32_t *)&state[12];

	t0 = vortex_enc_table_0[s0 & 0xff] ^ 
		 vortex_enc_table_1[(s1 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s2 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s3 >> 24) & 0xff];
	t1 = vortex_enc_table_0[s1 & 0xff] ^ 
		 vortex_enc_table_1[(s2 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s3 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s0 >> 24) & 0xff];	
	t2 = vortex_enc_table_0[s2 & 0xff] ^ 
		 vortex_enc_table_1[(s3 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s0 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s1 >> 24) & 0xff] ;
	t3 = vortex_enc_table_0[s3 & 0xff] ^ 
		 vortex_enc_table_1[(s0 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s1 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s2 >> 24) & 0xff];

	t0 ^= *(vortex_uint32_t *)&key[0];
	t1 ^= *(vortex_uint32_t *)&key[4];
	t2 ^= *(vortex_uint32_t *)&key[8];
	t3 ^= *(vortex_uint32_t *)&key[12];

	*(vortex_uint32_t *)&state[0]  = t0;
	*(vortex_uint32_t *)&state[4]  = t1;
	*(vortex_uint32_t *)&state[8]  = t2;
	*(vortex_uint32_t *)&state[12] = t3;
	return VORTEX_SUCCESS;

}


int
vortex_rijndael_256_round(vortex_uint8_t *key,
				   vortex_uint8_t *state)
{
	vortex_uint32_t s0, s1, s2, s3, s4, s5, s6, s7, t0, t1, t2, t3, t4, t5, t6, t7;

	s0 = *(vortex_uint32_t *)&state[0];
	s1 = *(vortex_uint32_t *)&state[4];
	s2 = *(vortex_uint32_t *)&state[8];
	s3 = *(vortex_uint32_t *)&state[12];
	s4 = *(vortex_uint32_t *)&state[16];
	s5 = *(vortex_uint32_t *)&state[20];
	s6 = *(vortex_uint32_t *)&state[24];
	s7 = *(vortex_uint32_t *)&state[28];

	t0 = vortex_enc_table_0[s0 & 0xff] ^ 
		 vortex_enc_table_1[(s1 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s2 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s3 >> 24) & 0xff];
	t1 = vortex_enc_table_0[s1 & 0xff] ^ 
		 vortex_enc_table_1[(s2 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s3 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s4 >> 24) & 0xff];	
	t2 = vortex_enc_table_0[s2 & 0xff] ^ 
		 vortex_enc_table_1[(s3 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s4 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s5 >> 24) & 0xff] ;
	t3 = vortex_enc_table_0[s3 & 0xff] ^ 
		 vortex_enc_table_1[(s4 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s5 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s6 >> 24) & 0xff];
	t4 = vortex_enc_table_0[s4 & 0xff] ^ 
		 vortex_enc_table_1[(s5 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s6 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s7 >> 24) & 0xff];
	t5 = vortex_enc_table_0[s5 & 0xff] ^ 
		 vortex_enc_table_1[(s6 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s7 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s0 >> 24) & 0xff];	
	t6 = vortex_enc_table_0[s6 & 0xff] ^ 
		 vortex_enc_table_1[(s7 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s0 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s1 >> 24) & 0xff] ;
	t7 = vortex_enc_table_0[s7 & 0xff] ^ 
		 vortex_enc_table_1[(s0 >> 8) & 0xff] ^ 
		 vortex_enc_table_2[(s1 >> 16) & 0xff] ^ 
		 vortex_enc_table_3[(s2 >> 24) & 0xff];

	t0 ^= *(vortex_uint32_t *)&key[0];
	t1 ^= *(vortex_uint32_t *)&key[4];
	t2 ^= *(vortex_uint32_t *)&key[8];
	t3 ^= *(vortex_uint32_t *)&key[12];
	t4 ^= *(vortex_uint32_t *)&key[16];
	t5 ^= *(vortex_uint32_t *)&key[20];
	t6 ^= *(vortex_uint32_t *)&key[24];
	t7 ^= *(vortex_uint32_t *)&key[28];

	*(vortex_uint32_t *)&state[0]  = t0;
	*(vortex_uint32_t *)&state[4]  = t1;
	*(vortex_uint32_t *)&state[8]  = t2;
	*(vortex_uint32_t *)&state[12] = t3;
	*(vortex_uint32_t *)&state[16] = t4;
	*(vortex_uint32_t *)&state[20] = t5;
	*(vortex_uint32_t *)&state[24] = t6;
	*(vortex_uint32_t *)&state[28] = t7;

	return VORTEX_SUCCESS;

}


int
vortex_varcpy(vortex_uint8_t *dst,
	   vortex_uint8_t *src,
	   vortex_uint32_t length)
{
	vortex_uint32_t i;

	for(i=0; i < length; i++)
		dst[i] = src[i];
	return VORTEX_SUCCESS;
}


int
vortex_varset(vortex_uint8_t *dst,
	   vortex_uint8_t val,
	   vortex_uint32_t length)
{
	vortex_uint32_t i;

	for(i=0; i < length; i++)
		dst[i] = val;
	return VORTEX_SUCCESS;
}


int 
vortex_init_hash_256(vortex_uint8_t *hash,
			  vortex_uint8_t *var)
{
	return vortex_varcpy(hash, var, 32);
}

int 
vortex_init_hash_512(vortex_uint8_t *hash,
			  vortex_uint8_t *var)
{
	return vortex_varcpy(hash, var, 64);
}


int
vortex_padd_128(vortex_uint8_t *res,
		 vortex_uint8_t *op1,
		 vortex_uint8_t *op2)
{
	*(vortex_uint64_t *)&res[0] = *(vortex_uint64_t *)&op1[0] + *(vortex_uint64_t *)&op2[0];
	*(vortex_uint64_t *)&res[8] = *(vortex_uint64_t *)&op1[8] + *(vortex_uint64_t *)&op2[8];
	
	return VORTEX_SUCCESS;
}


int
vortex_padd_256(vortex_uint8_t *res,
		 vortex_uint8_t *op1,
		 vortex_uint8_t *op2)
{
	*(vortex_uint64_t *)&res[0]  = *(vortex_uint64_t *)&op1[0]  + *(vortex_uint64_t *)&op2[0];
	*(vortex_uint64_t *)&res[8]  = *(vortex_uint64_t *)&op1[8]  + *(vortex_uint64_t *)&op2[8];
	*(vortex_uint64_t *)&res[16] = *(vortex_uint64_t *)&op1[16] + *(vortex_uint64_t *)&op2[16];
	*(vortex_uint64_t *)&res[24] = *(vortex_uint64_t *)&op1[24] + *(vortex_uint64_t *)&op2[24];

	return VORTEX_SUCCESS;
}


int
vortex_pxor_128(vortex_uint8_t *res,
		 vortex_uint8_t *op1,
		 vortex_uint8_t *op2)
{
	*(vortex_uint64_t *)&res[0] = *(vortex_uint64_t *)&op1[0] ^ *(vortex_uint64_t *)&op2[0];
	*(vortex_uint64_t *)&res[8] = *(vortex_uint64_t *)&op1[8] ^ *(vortex_uint64_t *)&op2[8];
	
	return VORTEX_SUCCESS;
}


int
vortex_pxor_256(vortex_uint8_t *res,
		 vortex_uint8_t *op1,
		 vortex_uint8_t *op2)
{
	*(vortex_uint64_t *)&res[0]  = *(vortex_uint64_t *)&op1[0]  ^ *(vortex_uint64_t *)&op2[0];
	*(vortex_uint64_t *)&res[8]  = *(vortex_uint64_t *)&op1[8]  ^ *(vortex_uint64_t *)&op2[8];
	*(vortex_uint64_t *)&res[16] = *(vortex_uint64_t *)&op1[16] ^ *(vortex_uint64_t *)&op2[16];
	*(vortex_uint64_t *)&res[24] = *(vortex_uint64_t *)&op1[24] ^ *(vortex_uint64_t *)&op2[24];

	return VORTEX_SUCCESS;
}


int
vortex_sbox_128(vortex_uint8_t *dst,
		 vortex_uint8_t *src)
{
	int i;
	for(i=0; i < 16; i++)
		dst[i] = vortex_rijndael_sbox[src[i]];
	return VORTEX_SUCCESS;
}


int
vortex_sbox_256(vortex_uint8_t *dst,
		 vortex_uint8_t *src)
{
	int i;
	for(i=0; i < 32; i++)
		dst[i] = vortex_rijndael_sbox[src[i]];
	return VORTEX_SUCCESS;
}


vortex_uint64_t
vortex_math_ceil(double d)
{
	vortex_uint64_t v;
	double t;
	v = (vortex_uint64_t)d;
	t = (double)v;
	if(d != t)
		return v+1;
	else
		return v;
}


int
vortex_perm_128(vortex_uint8_t *dst,
		 vortex_uint8_t *src)
{
	int i;
	for(i=0; i < 16; i++)
		dst[i] = src[i];
	return VORTEX_SUCCESS;
}


int
vortex_perm_256(vortex_uint8_t *dst,
		 vortex_uint8_t *src)
{
	int i;
	for(i=0; i < 32; i++)
		dst[i] = src[i];
	return VORTEX_SUCCESS;
}


int
vortex_mul_128(vortex_uint8_t *res,
		vortex_uint8_t *op1,
		vortex_uint8_t *op2,
		vortex_uint8_t mul_type)
{
	vortex_uint64_t a, b; 
	vortex_uint64_t c0, c1;

	a = *(vortex_uint64_t *)&op1[0];
	b = *(vortex_uint64_t *)&op2[0];
	
	switch(mul_type)
	{
		case 0:
			c0 = vortex_cmul64(a, b, &c1);
			break;
		case 1:
#ifdef VORTEX_OPTIMIZED_64
			c0 = _umul128(a, b, &c1);
#else
			c0 = vortex_imul64(a, b, &c1);
#endif
			break;
		default:
			break;
	}
	*(vortex_uint64_t *)&res[0]  = c0;
	*(vortex_uint64_t *)&res[8]  = c1;
	return VORTEX_SUCCESS;	
}


int
vortex_mul_256(vortex_uint8_t *res,
		vortex_uint8_t *op1,
		vortex_uint8_t *op2,
		vortex_uint8_t mul_type)
{
	vortex_uint64_t a0, b0, a1, b1, c; 
	vortex_uint64_t p0_0, p0_1, p1_0, p1_1, p2_0, p2_1, p3_0, p3_1;
	vortex_uint64_t c0, c1, c2, c3;

	a0 = *(vortex_uint64_t *)&op1[0];
	a1 = *(vortex_uint64_t *)&op1[8];
	b0 = *(vortex_uint64_t *)&op2[0];
	b1 = *(vortex_uint64_t *)&op2[8];

	switch(mul_type)
	{
		case 0:
			/* Carry-less Karatsuba */
#ifdef VORTEX_OPTIMIZED_64_32
			p0_0 = vortex_cmul64(a0, b0, &p0_1);
			p1_0 = vortex_cmul64(a1^a0, b1^b0, &p1_1);
			p3_0 = vortex_cmul64(a1, b1, &p3_1);
			c0 = p0_0;
			c1 = p0_1 ^ p1_0 ^ p0_0 ^ p3_0;
			c2 = p1_1 ^ p0_1 ^ p3_1 ^ p3_0;
			c3 = p3_1;
#else
			p0_0 = vortex_cmul64(a0, b0, &p0_1);
			p1_0 = vortex_cmul64(a1, b0, &p1_1);
			p2_0 = vortex_cmul64(a0, b1, &p2_1);
			p3_0 = vortex_cmul64(a1, b1, &p3_1);
			c0 = p0_0;
			c1 = p0_1 ^ p1_0 ^ p2_0;
			c2 = p1_1 ^ p2_1 ^ p3_0;
			c3 = p3_1;
#endif
			break;
		case 1:
#ifdef VORTEX_OPTIMIZED_64
			p0_0 = _umul128(a0, b0, &p0_1);
			p1_0 = _umul128(a1, b0, &p1_1);
			p2_0 = _umul128(a0, b1, &p2_1);
			p3_0 = _umul128(a1, b1, &p3_1);
#else
			p0_0 = vortex_imul64(a0, b0, &p0_1);
			p1_0 = vortex_imul64(a1, b0, &p1_1);
			p2_0 = vortex_imul64(a0, b1, &p2_1);
			p3_0 = vortex_imul64(a1, b1, &p3_1);
#endif
			c = 0;
			c0 = p0_0;
			c1 = p0_1 + p1_0;
			if(c1 < p0_1)
				c++;
			c1 += p2_0;
			if(c1 < p2_0)
				c++;
			c2 = p3_0 + c;
			if(c2 < p3_0)
				c = 1;
			else
				c = 0;
			c2 += p1_1;
			if(c2 < p1_1)
				c++;
			c2 += p2_1;
			if(c2 < p2_1)
				c++;
			c3 = p3_1 + c;
			break;
		default:
			break;
	}
	*(vortex_uint64_t *)&res[0]  = c0;
	*(vortex_uint64_t *)&res[8]  = c1;
	*(vortex_uint64_t *)&res[16] = c2;
	*(vortex_uint64_t *)&res[24] = c3;
	return VORTEX_SUCCESS;	
}

