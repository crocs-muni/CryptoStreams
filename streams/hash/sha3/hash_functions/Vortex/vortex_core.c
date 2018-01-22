
/* Written by Michael E. Kounavis, Commmunications Technology Lab,
   Intel Corporation, 2008
 */


#include "int_types.h"
#include "stdio.h"
#include "stdlib.h"
#include "vortex_misc.h"
#include "vortex_core.h"

#ifdef VORTEX_OPTIMIZED_ASSEMBLY

void vortex_256_asm(unsigned char *hash, unsigned char *data, unsigned char *rcon, int length);
void vortex_512_asm(unsigned char *hash, unsigned char *data, unsigned char *rcon, int length);

#endif

/* The following global variables represent the tunable parameters of the vortex algorithm
 */

vortex_uint8_t vortex_number_of_rounds_g;			/* The number of rounds each modified AES block is executed */
vortex_uint8_t vortex_mul_type_g;					/* The type of multiplication used: 0 carryless, 1 integer */
vortex_uint8_t vortex_degree_of_diffusion_g;		/* The number of times diffusion happens in the vortex last sub-block */
vortex_uint8_t *vortex_a0_b0_32_g;				/* The initial value of the chaining variable for vortex 224 and vortex 256 */
vortex_uint8_t *vortex_ta_tb_32_g;				/* The final tweak value for vortex 224 and vortex 256 */
vortex_uint8_t *vortex_a0_b0_64_g;				/* The initial value of the chaining variable for vortex 384 and vortex 512 */
vortex_uint8_t *vortex_ta_tb_64_g;				/* The final tweak value for vortex 384 and vortex 512 */
vortex_uint8_t *vortex_rcon_256_g[VORTEX_MAX_NUMBER_OF_ROUNDS];		/* The round constants used in the key schedule algorithm */
vortex_uint8_t *vortex_rcon_512_g[VORTEX_MAX_NUMBER_OF_ROUNDS];


/*	Routine for deriving a vortex hash value. It uses the parameters assigned to the 
	global variables above */


int
vortex(vortex_uint8_t *data, 
			   vortex_uint64_t *length, 
			   vortex_uint8_t *hash, 
			   vortex_uint32_t type)
{
	vortex_uint8_t number_of_rounds;
	vortex_uint8_t mul_type;
	vortex_uint8_t a0_b0[64];
	vortex_uint8_t ta_tb[64];
	vortex_uint8_t degree_of_diffusion;

	if ((!data) || (!hash) || (!length))
	{
		perror("vortex(): null pointer passed\n");
		return VORTEX_NULL_POINTER_ERROR;
	}
	number_of_rounds = vortex_number_of_rounds_g;
	mul_type = vortex_mul_type_g;
	degree_of_diffusion = vortex_degree_of_diffusion_g;

	switch(type)
	{
		case 224: 
		case 256:
			vortex_varcpy(a0_b0, vortex_a0_b0_32_g, 32);
			vortex_varcpy(ta_tb, vortex_ta_tb_32_g, 32);
			break;
		case 384: 
		case 512:
			vortex_varcpy(a0_b0, vortex_a0_b0_64_g, 64);
			vortex_varcpy(ta_tb, vortex_ta_tb_64_g, 64);
			break;
		default:
			perror("vortex(): bad hash type\n");
			return VORTEX_BAD_HASH_TYPE;
	}

	return tunable_vortex(
		data, 
		length, 
		hash,
		type,
		number_of_rounds, 
		mul_type, 
		a0_b0, 
		ta_tb,
		degree_of_diffusion);
}

/*	Generic routine for deriving a vortex hash value. It accepts parameters as input operands.
	This is used by the Hash() function of the NIST cryptographic API */

int
tunable_vortex(vortex_uint8_t  *data, 
			   vortex_uint64_t *length, 
			   vortex_uint8_t  *hash, 
			   vortex_uint32_t type,
			   vortex_uint8_t  number_of_rounds,
			   vortex_uint8_t  mul_type,
			   vortex_uint8_t  *a0_b0,
			   vortex_uint8_t  *ta_tb,
			   vortex_uint8_t  degree_of_diffusion)
{

	if ((!data) || (!hash) || (!length) || (!a0_b0) || (!ta_tb))
	{
		perror("tunable_vortex(): null pointer passed\n");
		return VORTEX_NULL_POINTER_ERROR;
	}

	switch(type)
	{
		case 224:
			return vortex_224(
				data, 
				length, 
				hash, 
				number_of_rounds, 
				mul_type, 
				a0_b0, 
				ta_tb,
				degree_of_diffusion);
		case 256:
			return vortex_256(
				data, 
				length, 
				hash, 
				number_of_rounds, 
				mul_type, 
				a0_b0, 
				ta_tb,
				degree_of_diffusion);

		case 384:
			return vortex_384(
				data, 
				length, 
				hash, 
				number_of_rounds, 
				mul_type, 
				a0_b0, 
				ta_tb,
				degree_of_diffusion);

		case 512:
			return vortex_512(
				data, 
				length, 
				hash, 
				number_of_rounds, 
				mul_type, 
				a0_b0, 
				ta_tb,
				degree_of_diffusion);
		default:
			perror("tunable_vortex(): bad hash type\n");
			return VORTEX_BAD_HASH_TYPE;
	}
}


int
vortex_224(vortex_uint8_t  *data, 
		   vortex_uint64_t *length, 
		   vortex_uint8_t  *hash, 
		   vortex_uint8_t  number_of_rounds,
		   vortex_uint8_t  mul_type,
		   vortex_uint8_t  *a0_b0,
		   vortex_uint8_t  *ta_tb,
		   vortex_uint8_t  degree_of_diffusion)
{

	vortex_uint8_t hash256[32];	

	if( vortex_256(
		data, 
		length, 
		hash256, 
		number_of_rounds, 
		mul_type, 
		a0_b0, 
		ta_tb,
		degree_of_diffusion) != VORTEX_SUCCESS)
	{
		perror("vortex_224(): vortex_256 call failed\n");
		return VORTEX_CALL_FAILURE;
	}
	/* The 224-bit hash derives from the 256-bit by truncation */
	vortex_varcpy(hash, hash256, 28);	
	return VORTEX_SUCCESS;
}


int
vortex_384(vortex_uint8_t  *data, 
		   vortex_uint64_t *length, 
		   vortex_uint8_t  *hash, 
		   vortex_uint8_t  number_of_rounds,
		   vortex_uint8_t  mul_type,
		   vortex_uint8_t  *a0_b0,
		   vortex_uint8_t  *ta_tb,
		   vortex_uint8_t  degree_of_diffusion)
{

	vortex_uint8_t hash512[64];	
	
	if( vortex_512(
		data, 
		length, 
		hash512, 
		number_of_rounds, 
		mul_type, 
		a0_b0, 
		ta_tb,
		degree_of_diffusion) != VORTEX_SUCCESS)
	{
		perror("vortex_384(): vortex_512 call failed\n");
		return VORTEX_CALL_FAILURE;
	}
	/* The 384-bit hash derives from the 512-bit by truncation */
	vortex_varcpy(hash, hash512, 48);
	return VORTEX_SUCCESS;
}


int
vortex_256(vortex_uint8_t  *data, 
		   vortex_uint64_t *length_pointer, 
		   vortex_uint8_t  *hash, 
		   vortex_uint8_t  number_of_rounds,
		   vortex_uint8_t  mul_type,
		   vortex_uint8_t  *a0_b0,
		   vortex_uint8_t  *ta_tb,
		   vortex_uint8_t  degree_of_diffusion)
{
	vortex_uint64_t length;
	vortex_uint32_t number_of_zeros;
	vortex_uint64_t blocks_no_malloc;
	vortex_uint32_t blocks_malloc;
	vortex_uint8_t	 last_block[64];
	vortex_uint8_t  last_bytes;
	vortex_uint8_t  last_byte_index;
	vortex_uint64_t i;

	length = length_pointer[0];
	vortex_varset(last_block, 0, 64);

	/* number_of_zeros = the number of bits equal to 0 required for padding */

	if (length <= 191)
		number_of_zeros = (vortex_uint32_t)(191-length);
	else
		number_of_zeros = (vortex_uint32_t)(512 - ((length-191) % 512));
	
	/* blocks_no_malloc = vortex blocks for which no additional memory allocations need to be made;
	   We assume that the input buffer may not always contain enough bytes for the padding
	 */
	
	blocks_no_malloc = length/512;
	blocks_malloc = (vortex_uint32_t)((length + number_of_zeros + 65)/512 - blocks_no_malloc);
	
	vortex_init_hash_256(hash, a0_b0);	

	/* NOTE: The optimized assembly routine below uses stand-ins for future processor instructions for AES round
	   and Galois Field multiplication. It is used for measuring expected performance from processors coming out
	   at the the 2009-2010 time frame. Also the code uses the default parameters of the number_of_rounds=3
	   and mul_type = 0
	 */

#ifdef VORTEX_OPTIMIZED_ASSEMBLY
	vortex_256_asm((unsigned char *)hash, (unsigned char *)data, (unsigned char *)vortex_rcon_256_g, (int)blocks_no_malloc);
#else
	for(i=0; i< blocks_no_malloc; i++)
		vortex_256_block(hash, &data[i*64], number_of_rounds, mul_type);
#endif

	last_bytes = (vortex_uint8_t)vortex_math_ceil((length%512)/8.0);
	
	if(!(length%8))
		last_byte_index = last_bytes;
	else
		last_byte_index = last_bytes-1;
	
	for(i=0; i< blocks_malloc; i++)
	{
		/*do the padding in case blocks_malloc = 1 */
		vortex_varcpy(last_block, &data[blocks_no_malloc*64], last_bytes);	
		last_block[last_byte_index] |= 1 << (length%8);
		vortex_256_block(hash, last_block, number_of_rounds, mul_type);
	}
	
	vortex_varset(last_block, 0, 64);

	/* Copy the current hash to the last block according to the 
	   Enveloped Merkle Damgard construction
	 */

	vortex_varcpy(last_block, hash, 32);
	if(!blocks_malloc)
	{
		/* do the padding in case blocks_malloc = 0 */
		vortex_varcpy(&last_block[32], &data[blocks_no_malloc*64], last_bytes);	
		last_block[last_byte_index+32] |= 1 << (length%8);
	}
	*(vortex_uint64_t *)&last_block[56] = length;
	vortex_varcpy(hash, ta_tb, 32);
	last_vortex_256_block(hash, last_block, number_of_rounds, mul_type, degree_of_diffusion);
	return VORTEX_SUCCESS;
}

int
vortex_256_block(vortex_uint8_t *hash,
				 vortex_uint8_t *data,
				 vortex_uint8_t number_of_rounds, 
				 vortex_uint8_t mul_type)
{
	vortex_256_sub_block(hash, data, number_of_rounds, mul_type);
	vortex_256_sub_block(hash, &data[32], number_of_rounds, mul_type);
	return VORTEX_SUCCESS;
}

int
last_vortex_256_block(vortex_uint8_t *hash,
					  vortex_uint8_t *data,
					  vortex_uint8_t number_of_rounds, 
					  vortex_uint8_t mul_type,
					  vortex_uint8_t degree_of_diffusion)
{
	vortex_256_sub_block(hash, data, number_of_rounds, mul_type);
	last_vortex_256_sub_block(hash, &data[32], number_of_rounds, mul_type, degree_of_diffusion);
	return VORTEX_SUCCESS;
}

int
vortex_256_sub_block(vortex_uint8_t *hash,
					 vortex_uint8_t *data,
					 vortex_uint8_t number_of_rounds, 
					 vortex_uint8_t mul_type)

{
	vortex_uint8_t i, j;
	vortex_uint8_t a[16], b[16];
	vortex_uint8_t key_a[16], key_b[16];
	vortex_uint8_t state_a[16], state_b[16];

	if(number_of_rounds > VORTEX_MAX_NUMBER_OF_ROUNDS)
	{
		perror("vortex_256_sub_block(): number of rounds exceeds limit\n");
		return VORTEX_INVALID_ROUNDS;
	}

	vortex_varcpy(a, hash, 16);
	vortex_varcpy(b, &hash[16], 16);

	for(i=0; i < 2; i++)
	{
		vortex_varcpy(key_a, a, 16);
		vortex_varcpy(key_b, b, 16);
		vortex_varcpy(state_a, &data[i*16], 16);
		vortex_varcpy(state_b, &data[i*16], 16);

		for(j=0; j < number_of_rounds; j++)
		{	
			/* prepare the key schedule for the modified AES transformation */
			vortex_padd_128(key_a, key_a, vortex_rcon_256_g[j]);
			vortex_sbox_128(key_a, key_a);
			vortex_perm_128(key_a, key_a);
			
			/* call the AES round routine */
			vortex_rijndael_128_round(key_a, state_a);

			vortex_padd_128(key_b, key_b, vortex_rcon_256_g[j]);
			vortex_sbox_128(key_b, key_b);
			vortex_perm_128(key_b, key_b);
			vortex_rijndael_128_round(key_b, state_b);
		}
		/* Matyas-Meyer-Oseas */
		vortex_pxor_128(a, state_a, &data[i*16]);
		vortex_pxor_128(b, state_b, &data[i*16]);
		
		/* The Merging function of Vortex */
		vortex_vma_256(a, b, mul_type);
	}
	vortex_varcpy(hash, a, 16);
	vortex_varcpy(&hash[16], b, 16);
	return VORTEX_SUCCESS;
}

/* The routine below is the same as vortex_sub_block but the last stage of AES rounds and 
   merging is repeated degree_of_diffusion times to strengthen the processing of the 
   last block 
 */ 	

int
last_vortex_256_sub_block(vortex_uint8_t *hash,
						  vortex_uint8_t *data,
						  vortex_uint8_t number_of_rounds, 
						  vortex_uint8_t mul_type,
						  vortex_uint8_t degree_of_diffusion)

{
	vortex_uint8_t i, j;
	vortex_uint8_t a[16], b[16];
	vortex_uint8_t key_a[16], key_b[16];
	vortex_uint8_t state_a[16], state_b[16];

	if(number_of_rounds > VORTEX_MAX_NUMBER_OF_ROUNDS)
	{
		perror("vortex_256_sub_block(): number of rounds exceeds limit\n");
		return VORTEX_INVALID_ROUNDS;
	}

	vortex_varcpy(a, hash, 16);
	vortex_varcpy(b, &hash[16], 16);

	vortex_varcpy(key_a, a, 16);
	vortex_varcpy(key_b, b, 16);
	vortex_varcpy(state_a, &data[0], 16);
	vortex_varcpy(state_b, &data[0], 16);

	for(j=0; j < number_of_rounds; j++)
	{	
		vortex_padd_128(key_a, key_a, vortex_rcon_256_g[j]);
		vortex_sbox_128(key_a, key_a);
		vortex_perm_128(key_a, key_a);
		vortex_rijndael_128_round(key_a, state_a);

		vortex_padd_128(key_b, key_b, vortex_rcon_256_g[j]);
		vortex_sbox_128(key_b, key_b);
		vortex_perm_128(key_b, key_b);
		vortex_rijndael_128_round(key_b, state_b);
	}
	vortex_pxor_128(a, state_a, &data[0]);
	vortex_pxor_128(b, state_b, &data[0]);
	vortex_vma_256(a, b, mul_type);

	for(i=0; i < degree_of_diffusion; i++)
	{
		vortex_varcpy(key_a, a, 16);
		vortex_varcpy(key_b, b, 16);
		vortex_varcpy(state_a, &data[16], 16);
		vortex_varcpy(state_b, &data[16], 16);

		for(j=0; j < number_of_rounds; j++)
		{	
			vortex_padd_128(key_a, key_a, vortex_rcon_256_g[j]);
			vortex_sbox_128(key_a, key_a);
			vortex_perm_128(key_a, key_a);
			vortex_rijndael_128_round(key_a, state_a);

			vortex_padd_128(key_b, key_b, vortex_rcon_256_g[j]);
			vortex_sbox_128(key_b, key_b);
			vortex_perm_128(key_b, key_b);
			vortex_rijndael_128_round(key_b, state_b);
		}
		vortex_pxor_128(a, state_a, &data[16]);
		vortex_pxor_128(b, state_b, &data[16]);
		vortex_vma_256(a, b, mul_type);
	}
	vortex_varcpy(hash, a, 16);
	vortex_varcpy(&hash[16], b, 16);
	return VORTEX_SUCCESS;
}

/* The merging function of vortex */

int
vortex_vma_256(vortex_uint8_t *a,
		vortex_uint8_t *b,
		vortex_uint8_t mul_type)
{
	vortex_uint8_t inner_product[16];
	vortex_uint8_t outer_product[16];

	vortex_mul_128(inner_product,
			&a[8],
			&b[0],
			mul_type);
	vortex_mul_128(outer_product,
			&a[0],
			&b[8],
			mul_type);

	*(vortex_uint64_t *)&a[0] ^=  *(vortex_uint64_t *)&inner_product[0];
	*(vortex_uint64_t *)&a[8] ^=  *(vortex_uint64_t *)&outer_product[8];
	*(vortex_uint64_t *)&b[0] +=  *(vortex_uint64_t *)&outer_product[0];
	*(vortex_uint64_t *)&b[8] +=  *(vortex_uint64_t *)&inner_product[8];
	return VORTEX_SUCCESS;
}

/* The code structure of vortex 512 is identical to that of vortex 256. For comments see the
   code above.
 */

int
vortex_512(vortex_uint8_t  *data, 
		   vortex_uint64_t *length_pointer, 
		   vortex_uint8_t  *hash, 
		   vortex_uint8_t  number_of_rounds,
		   vortex_uint8_t  mul_type,
		   vortex_uint8_t  *a0_b0,
		   vortex_uint8_t  *ta_tb,
		   vortex_uint8_t  degree_of_diffusion)
{
	vortex_uint64_t length;
	vortex_uint32_t number_of_zeros;
	vortex_uint64_t blocks_no_malloc;
	vortex_uint32_t blocks_malloc;
	vortex_uint8_t	 last_block[128];
	vortex_uint8_t  last_bytes;
	vortex_uint8_t  last_byte_index;
	vortex_uint64_t i;

	if(length_pointer[1])
	{
		perror("vortex_512(): vortex_512 length not supported by the implementation\n");
		return VORTEX_INVALID_LENGTH;
	}
	length = length_pointer[0];
	vortex_varset(last_block, 0, 128);
	if (length <= 383)
		number_of_zeros = (vortex_uint32_t)(383-length);
	else
		number_of_zeros = (vortex_uint32_t)(1024 - ((length-383) % 1024));
	blocks_no_malloc = length/1024;
	blocks_malloc = (vortex_uint32_t)((length + number_of_zeros + 129)/1024 - blocks_no_malloc);
	
	vortex_init_hash_512(hash, a0_b0);	
#ifdef VORTEX_OPTIMIZED_ASSEMBLY
	vortex_512_asm((unsigned char *)hash, (unsigned char *)data, (unsigned char *)vortex_rcon_512_g, (int)blocks_no_malloc);
#else
	for(i=0; i< blocks_no_malloc; i++)
		vortex_512_block(hash, &data[i*128], number_of_rounds, mul_type);
#endif

	last_bytes = (vortex_uint8_t)vortex_math_ceil((length%1024)/8.0);
	
	if(!(length%8))
		last_byte_index = last_bytes;
	else
		last_byte_index = last_bytes-1;
	
	for(i=0; i< blocks_malloc; i++)
	{
		vortex_varcpy(last_block, &data[blocks_no_malloc*128], last_bytes);	
		last_block[last_byte_index] |= 1 << (length%8);
		vortex_512_block(hash, last_block, number_of_rounds, mul_type);
	}
	
	vortex_varset(last_block, 0, 128);
	vortex_varcpy(last_block, hash, 64);
	if(!blocks_malloc)
	{
		vortex_varcpy(&last_block[64], &data[blocks_no_malloc*128], last_bytes);	
		last_block[last_byte_index+64] |= 1 << (length%8);
	}
	*(vortex_uint64_t *)&last_block[120] = length;
	vortex_varcpy(hash, ta_tb, 64);
	last_vortex_512_block(hash, last_block, number_of_rounds, mul_type, degree_of_diffusion);
	return VORTEX_SUCCESS;
}

int
vortex_512_block(vortex_uint8_t *hash,
				 vortex_uint8_t *data,
				 vortex_uint8_t number_of_rounds, 
				 vortex_uint8_t mul_type)
{
	vortex_512_sub_block(hash, data, number_of_rounds, mul_type);
	vortex_512_sub_block(hash, &data[64], number_of_rounds, mul_type);
	return VORTEX_SUCCESS;
}

int
last_vortex_512_block(vortex_uint8_t *hash,
					  vortex_uint8_t *data,
					  vortex_uint8_t number_of_rounds, 
					  vortex_uint8_t mul_type,
					  vortex_uint8_t degree_of_diffusion)
{
	vortex_512_sub_block(hash, data, number_of_rounds, mul_type);
	last_vortex_512_sub_block(hash, &data[64], number_of_rounds, mul_type, degree_of_diffusion);
	return VORTEX_SUCCESS;
}

int
vortex_512_sub_block(vortex_uint8_t *hash,
					 vortex_uint8_t *data,
					 vortex_uint8_t number_of_rounds, 
					 vortex_uint8_t mul_type)

{
	vortex_uint8_t i, j;
	vortex_uint8_t a[32], b[32];
	vortex_uint8_t key_a[32], key_b[32];
	vortex_uint8_t state_a[32], state_b[32];

	if(number_of_rounds > VORTEX_MAX_NUMBER_OF_ROUNDS)
	{
		perror("vortex_512_sub_block(): number of rounds exceeds limit\n");
		return VORTEX_INVALID_ROUNDS;
	}

	vortex_varcpy(a, hash, 32);
	vortex_varcpy(b, &hash[32], 32);

	for(i=0; i < 2; i++)
	{
		vortex_varcpy(key_a, a, 32);
		vortex_varcpy(key_b, b, 32);
		vortex_varcpy(state_a, &data[i*32], 32);
		vortex_varcpy(state_b, &data[i*32], 32);

		for(j=0; j < number_of_rounds; j++)
		{	
			vortex_padd_256(key_a, key_a, vortex_rcon_512_g[j]);
			vortex_sbox_256(key_a, key_a);
			vortex_perm_256(key_a, key_a);
			vortex_rijndael_256_round(key_a, state_a);

			vortex_padd_256(key_b, key_b, vortex_rcon_512_g[j]);
			vortex_sbox_256(key_b, key_b);
			vortex_perm_256(key_b, key_b);
			vortex_rijndael_256_round(key_b, state_b);
		}
		vortex_pxor_256(a, state_a, &data[i*32]);
		vortex_pxor_256(b, state_b, &data[i*32]);
		vortex_vma_512(a, b, mul_type);
	}
	vortex_varcpy(hash, a, 32);
	vortex_varcpy(&hash[32], b, 32);
	return VORTEX_SUCCESS;
}

int
last_vortex_512_sub_block(vortex_uint8_t *hash,
						  vortex_uint8_t *data,
						  vortex_uint8_t number_of_rounds, 
						  vortex_uint8_t mul_type,
						  vortex_uint8_t degree_of_diffusion)

{
	vortex_uint8_t i, j;
	vortex_uint8_t a[32], b[32];
	vortex_uint8_t key_a[32], key_b[32];
	vortex_uint8_t state_a[32], state_b[32];

	if(number_of_rounds > VORTEX_MAX_NUMBER_OF_ROUNDS)
	{
		perror("vortex_512_sub_block(): number of rounds exceeds limit\n");
		return VORTEX_INVALID_ROUNDS;
	}

	vortex_varcpy(a, hash, 32);
	vortex_varcpy(b, &hash[32], 32);

	vortex_varcpy(key_a, a, 32);
	vortex_varcpy(key_b, b, 32);
	vortex_varcpy(state_a, &data[0], 32);
	vortex_varcpy(state_b, &data[0], 32);

	for(j=0; j < number_of_rounds; j++)
	{	
		vortex_padd_256(key_a, key_a, vortex_rcon_512_g[j]);
		vortex_sbox_256(key_a, key_a);
		vortex_perm_256(key_a, key_a);
		vortex_rijndael_256_round(key_a, state_a);

		vortex_padd_256(key_b, key_b, vortex_rcon_512_g[j]);
		vortex_sbox_256(key_b, key_b);
		vortex_perm_256(key_b, key_b);
		vortex_rijndael_256_round(key_b, state_b);
	}
	vortex_pxor_256(a, state_a, &data[0]);
	vortex_pxor_256(b, state_b, &data[0]);
	vortex_vma_512(a, b, mul_type);

	for(i=0; i < degree_of_diffusion; i++)
	{
		vortex_varcpy(key_a, a, 32);
		vortex_varcpy(key_b, b, 32);
		vortex_varcpy(state_a, &data[32], 32);
		vortex_varcpy(state_b, &data[32], 32);

		for(j=0; j < number_of_rounds; j++)
		{	
			vortex_padd_256(key_a, key_a, vortex_rcon_512_g[j]);
			vortex_sbox_256(key_a, key_a);
			vortex_perm_256(key_a, key_a);
			vortex_rijndael_256_round(key_a, state_a);

			vortex_padd_256(key_b, key_b, vortex_rcon_512_g[j]);
			vortex_sbox_256(key_b, key_b);
			vortex_perm_256(key_b, key_b);
			vortex_rijndael_256_round(key_b, state_b);
		}
		vortex_pxor_256(a, state_a, &data[32]);
		vortex_pxor_256(b, state_b, &data[32]);
		vortex_vma_512(a, b, mul_type);
	}
	vortex_varcpy(hash, a, 32);
	vortex_varcpy(&hash[32], b, 32);
	return VORTEX_SUCCESS;
}


int
vortex_vma_512(vortex_uint8_t *a,
		vortex_uint8_t *b,
		vortex_uint8_t mul_type)
{
	vortex_uint8_t inner_product[32];
	vortex_uint8_t outer_product[32];

	vortex_mul_256(inner_product,
			&a[16],
			&b[0],
			mul_type);
	vortex_mul_256(outer_product,
			&a[0],
			&b[16],
			mul_type);

	*(vortex_uint64_t *)&a[0]  ^=  *(vortex_uint64_t *)&inner_product[0];
	*(vortex_uint64_t *)&a[8]  ^=  *(vortex_uint64_t *)&inner_product[8];	
	*(vortex_uint64_t *)&a[16] ^=  *(vortex_uint64_t *)&outer_product[16];
	*(vortex_uint64_t *)&a[24] ^=  *(vortex_uint64_t *)&outer_product[24];
	*(vortex_uint64_t *)&b[0]  +=  *(vortex_uint64_t *)&outer_product[0];
	*(vortex_uint64_t *)&b[8]  +=  *(vortex_uint64_t *)&outer_product[8];
	*(vortex_uint64_t *)&b[16] +=  *(vortex_uint64_t *)&inner_product[16];
	*(vortex_uint64_t *)&b[24] +=  *(vortex_uint64_t *)&inner_product[24];

	return VORTEX_SUCCESS;
}

int
vortex_generate_constants(void)
{
	int i, j;
	vortex_uint64_t p, x; 
	vortex_a0_b0_32_g = (vortex_uint8_t *)malloc(32);
	vortex_a0_b0_64_g = (vortex_uint8_t *)malloc(64);
	vortex_ta_tb_32_g = (vortex_uint8_t *)malloc(32);
	vortex_ta_tb_64_g = (vortex_uint8_t *)malloc(64);

	for(i=0; i < VORTEX_MAX_NUMBER_OF_ROUNDS; i++)
	{
		vortex_rcon_256_g[i] = (vortex_uint8_t *)malloc(16);
		vortex_rcon_512_g[i] = (vortex_uint8_t *)malloc(32);
	}

	p = 4294967291;
	x = 1414213562;
	x = (x*x-x) % p;

	for(i=0; i < 8; i++)
	{
		((vortex_uint32_t *)vortex_a0_b0_32_g)[i] = (vortex_uint32_t)x;
		x = (x*x-x) % p;
	}
	for(i=0; i < VORTEX_MAX_NUMBER_OF_ROUNDS; i++)
		for(j=0; j < 4; j++)
		{
			((vortex_uint32_t *)(vortex_rcon_256_g[i]))[j] = (vortex_uint32_t)x;
			x = (x*x-x) % p;
		}
	for(i=0; i < 16; i++)
	{
		((vortex_uint32_t *)vortex_a0_b0_64_g)[i] = (vortex_uint32_t)x;
		x = (x*x-x) % p;
	}
	for(i=0; i < VORTEX_MAX_NUMBER_OF_ROUNDS; i++)
		for(j=0; j < 8; j++)
		{
			((vortex_uint32_t *)(vortex_rcon_512_g[i]))[j] = (vortex_uint32_t)x;
			x = (x*x-x) % p;
		}
	for(i=0; i < 8; i++)
	{
		((vortex_uint32_t *)vortex_ta_tb_32_g)[i] = (vortex_uint32_t)x;
		x = (x*x-x) % p;
	}
	for(i=0; i < 16; i++)
	{
		((vortex_uint32_t *)vortex_ta_tb_64_g)[i] = (vortex_uint32_t)x;
		x = (x*x-x) % p;
	}
	return VORTEX_SUCCESS;
}

int
vortex_generate_random_tweak_values(void)
{
	int i; 
	vortex_ta_tb_32_g = (vortex_uint8_t *)malloc(32);
	vortex_ta_tb_64_g = (vortex_uint8_t *)malloc(64);

	for(i=0; i < 8; i++)
		((vortex_uint32_t *)vortex_ta_tb_32_g)[i] = 
			(vortex_uint32_t)((double)(1<<31)*((double)rand())/((double)RAND_MAX));

	for(i=0; i < 16; i++)
		((vortex_uint32_t *)vortex_ta_tb_64_g)[i] = 
			(vortex_uint32_t)((double)(1<<31)*((double)rand())/((double)RAND_MAX));

	return VORTEX_SUCCESS;
}

int
vortex_set_default_parameters(void)
{
	vortex_number_of_rounds_g = 10;
	vortex_mul_type_g = 1;
	vortex_degree_of_diffusion_g = 5;
	return VORTEX_SUCCESS;
}

