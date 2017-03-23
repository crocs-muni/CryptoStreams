

/* Written by Michael E. Kounavis, Commmunications Technology Lab,
   Intel Corporation, 2008
 */

#ifndef VORTEX_MISC_H
#define VORTEX_MISC_H

#include "int_types.h"

#if 0
#define VORTEX_OPTIMIZED_ASSEMBLY
#define VORTEX_OPTIMIZED_64
#endif

#define VORTEX_OPTIMIZED_64_32

#define VORTEX_SUCCESS				0
#define	VORTEX_OTHER_FAILURE		1
#define VORTEX_BAD_HASH_TYPE		2
#define VORTEX_CALL_FAILURE			3
#define VORTEX_INVALID_ROUNDS		4
#define VORTEX_INVALID_LENGTH		5
#define VORTEX_NULL_POINTER_ERROR	6

#define VORTEX_MAX_NUMBER_OF_ROUNDS		15


vortex_uint64_t 
vortex_cmul64(vortex_uint64_t a, 
	   vortex_uint64_t b, 
	   vortex_uint64_t *d);


vortex_uint64_t 
vortex_imul64(vortex_uint64_t a, 
	   vortex_uint64_t b, 
	   vortex_uint64_t *d);


int
vortex_rijndael_128_round(vortex_uint8_t *key,
				   vortex_uint8_t *state);

int
vortex_rijndael_256_round(vortex_uint8_t *key,
				   vortex_uint8_t *state);


int
vortex_varcpy(vortex_uint8_t *dst,
	   vortex_uint8_t *src,
	   vortex_uint32_t length);


int
vortex_varset(vortex_uint8_t *dst,
	   vortex_uint8_t val,
	   vortex_uint32_t length);


int 
vortex_init_hash_256(vortex_uint8_t *hash,
			  vortex_uint8_t *var);


int 
vortex_init_hash_512(vortex_uint8_t *hash,
			  vortex_uint8_t *var);


int
vortex_padd_128(vortex_uint8_t *res,
		 vortex_uint8_t *op1,
		 vortex_uint8_t *op2);


int
vortex_padd_256(vortex_uint8_t *res,
		 vortex_uint8_t *op1,
		 vortex_uint8_t *op2);


int
vortex_pxor_128(vortex_uint8_t *res,
		 vortex_uint8_t *op1,
		 vortex_uint8_t *op2);


int
vortex_pxor_256(vortex_uint8_t *res,
		 vortex_uint8_t *op1,
		 vortex_uint8_t *op2);


int
vortex_sbox_128(vortex_uint8_t *dst,
		 vortex_uint8_t *src);


int
vortex_sbox_256(vortex_uint8_t *dst,
		 vortex_uint8_t *src);


vortex_uint64_t
vortex_math_ceil(double d);



int
vortex_perm_128(vortex_uint8_t *dst,
		 vortex_uint8_t *src);


int
vortex_perm_256(vortex_uint8_t *dst,
		 vortex_uint8_t *src);


int
vortex_mul_128(vortex_uint8_t *res,
		vortex_uint8_t *op1,
		vortex_uint8_t *op2,
		vortex_uint8_t mul_type);


int
vortex_mul_256(vortex_uint8_t *res,
		vortex_uint8_t *op1,
		vortex_uint8_t *op2,
		vortex_uint8_t mul_type);

#endif