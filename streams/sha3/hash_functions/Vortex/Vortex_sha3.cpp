#if defined(__APPLE__)			// EACIRC: manual edit: apple compatibility
#  include <malloc/malloc.h>
#  include <stdlib.h>
#else
#  include <malloc.h>
#endif
#include <stdio.h>
#include "Vortex_sha3.h"
extern "C" {
#include "vortex_core.h"
#include "vortex_misc.h"

vortex_uint8_t vortex_number_of_rounds_g;
vortex_uint8_t vortex_mul_type_g;
vortex_uint8_t vortex_degree_of_diffusion_g;
vortex_uint8_t *vortex_a0_b0_32_g;
vortex_uint8_t *vortex_ta_tb_32_g;
vortex_uint8_t *vortex_a0_b0_64_g;
vortex_uint8_t *vortex_ta_tb_64_g;
vortex_uint8_t *vortex_rcon_256_g[VORTEX_MAX_NUMBER_OF_ROUNDS];
vortex_uint8_t *vortex_rcon_512_g[VORTEX_MAX_NUMBER_OF_ROUNDS];
}

#pragma warning( disable : 4013 4312)

int				vortex_iteration_g = 0;
BitSequence		*vortex_buf_g;


vortex_uint8_t
Vortex::reflect(vortex_uint8_t val)
{
	vortex_uint8_t r = 0, b[8];
	int i;
	for(i=0; i < 8; i++)
		b[i] = (val >> i) & 1;
	for(i=0; i < 8; i++)
		r |= (b[7-i] << i);
	return r;
}

#ifdef VORTEX_HAMMING_W_ANALYSIS
vortex_uint8_t
Vortex::get_hamming_w(vortex_uint8_t val)
{
	vortex_uint8_t r = 0;
	int i;
	for(i=0; i < 8; i++)
		r += ((val >> i) & 1);
	return r;
}

vortex_uint32_t
Vortex::compute_hamming_weight(BitSequence *data, 
					  int length)
{
	vortex_uint32_t r=0, i, bytelen = (length+7)/8;
	for(i=0; i < bytelen; i++)
		r += get_hamming_w(data[i]);
	return r;
	
}

void
Vortex::print_hamming_weight(BitSequence *data, 
					 int length)
{
	int r=0, i, bytelen = (length+7)/8;
	for(i=0; i < bytelen; i++)
		r += get_hamming_w(data[i]);
	printf("Hamming Weight %d found as output of vortex %d\n", r, length);
}
#endif

/* reflects the bits inside each byte according to the NIST specified format */

int
Vortex::set_format(BitSequence *data, 
		   DataLength databitlen)
{
	vortex_uint64_t bytelen = (databitlen+7)/8;
	vortex_uint64_t i;
	for(i=0; i < bytelen; i++)
		data[i] = reflect(data[i]);
	return SUCCESS;
}

/* Note: the only variable of the hash state which is not set by the Init() is the padding length. This is
   because padding-length is message-dependent. Vortex-specific parameter values are obtained from the 
   global variables defined in vortex_core.c
 */

int Vortex::Init(int hashbitlen)
{
	vortex_set_default_parameters();
	vortex_number_of_rounds_g = vortexNumRounds;
	vortex_generate_constants();
	if(!(&vortexState))
	{
		perror("Init(): null pointer passed\n");
		return NULL_POINTER_ERROR;
	}
	vortexState.hashbitlen = hashbitlen;
	vortexState.number_of_rounds = vortex_number_of_rounds_g;
	vortexState.mul_type = vortex_mul_type_g;
	vortexState.degree_of_diffusion = vortex_degree_of_diffusion_g;
	vortexState.padding_length = 0;

	switch(hashbitlen)
	{
		case 224: 
		case 256:
			vortexState.hash  = (vortex_uint8_t *)malloc(32);
			vortexState.a0_b0 = (vortex_uint8_t *)malloc(32);
			vortexState.ta_tb = (vortex_uint8_t *)malloc(32);
			vortex_varcpy(vortexState.hash,  vortex_a0_b0_32_g, 32);
			vortex_varcpy(vortexState.a0_b0, vortex_a0_b0_32_g, 32);
			vortex_varcpy(vortexState.ta_tb, vortex_ta_tb_32_g, 32);
			break;
		case 384: 
		case 512:
			vortexState.hash  = (vortex_uint8_t *)malloc(64);
			vortexState.a0_b0 = (vortex_uint8_t *)malloc(64);
			vortexState.ta_tb = (vortex_uint8_t *)malloc(64);
			vortex_varcpy(vortexState.hash,  vortex_a0_b0_64_g, 64);
			vortex_varcpy(vortexState.a0_b0, vortex_a0_b0_64_g, 64);
			vortex_varcpy(vortexState.ta_tb, vortex_ta_tb_64_g, 64);
			break;
		default:
			perror("Init(): bad hash type\n");
			return BAD_HASHBITLEN;
	}
	return SUCCESS;
}

/* Update() assumes that the data length passed is at least equal to the block size 
   of the vortex algorithm invoked. The block size is 512 bits for vortex 224 and
   vortex 256 and 1024 bits for vortex 384 and vortex 512.
 */

int Vortex::Update(const BitSequence *data, 
				  DataLength databitlen)
{
#ifndef VORTEX_OPTIMIZED_ASSEMBLY
	int i;
#endif
	vortex_uint64_t blocks_no_malloc;
	if((!(&vortexState)) || (!data))
	{
		perror("Update(): null pointer passed\n");
		return NULL_POINTER_ERROR;
	}

	if(((vortexState.hashbitlen == 384) || (vortexState.hashbitlen == 512)) &&
		(databitlen == 512) &&
		(vortex_iteration_g == 0))
	{
		vortex_buf_g = (BitSequence *)malloc(128);
		vortex_varcpy(vortex_buf_g, (vortex_uint8_t *)data, (vortex_uint32_t)64);
		vortexState.padding_length += databitlen;
		vortex_iteration_g = 1;
		return SUCCESS;
	}
	else if (((vortexState.hashbitlen == 384) || (vortexState.hashbitlen == 512)) &&
			(databitlen == 512) &&
			(vortex_iteration_g == 1))
	{
		vortex_varcpy(vortex_buf_g+64, (vortex_uint8_t *)data, (vortex_uint32_t)64);
		set_format(vortex_buf_g, 1024);
		vortex_iteration_g = 0;
	}
	else
	{
		vortex_buf_g = (BitSequence *)malloc((databitlen+7)/8);
		vortex_varcpy(vortex_buf_g, (vortex_uint8_t *)data, (vortex_uint32_t)((databitlen+7)/8));
		set_format(vortex_buf_g, databitlen);
	}
	switch(vortexState.hashbitlen)
	{
		case 224: 
		case 256:
			/* update processes multiples of the block size neglecting the remaining bits 
			   as specified by NIST */
			blocks_no_malloc = databitlen/512;
#ifdef VORTEX_OPTIMIZED_ASSEMBLY
		vortex_256_asm((unsigned char *)vortexState.hash, (unsigned char *)vortex_buf_g, (unsigned char *)vortex_rcon_256_g, (int)blocks_no_malloc);
#else
		for(i=0; i< blocks_no_malloc; i++)
			vortex_256_block(vortexState.hash, &vortex_buf_g[i*64], vortexState.number_of_rounds, vortexState.mul_type);
#endif
			/* 
			   change made to the implementation of the API 
			   so that genKAT.c does not require any changes
			*/
			vortexState.padding_length += databitlen;
			break;
		case 384: 
		case 512:
			if(databitlen == 512)
				blocks_no_malloc = 1;
			else
				blocks_no_malloc = databitlen/1024;
#ifdef VORTEX_OPTIMIZED_ASSEMBLY
		vortex_512_asm((unsigned char *)vortexState.hash, (unsigned char *)vortex_buf_g, (unsigned char *)vortex_rcon_512_g, (int)blocks_no_malloc);
#else
			for(i=0; i< blocks_no_malloc; i++)
				vortex_512_block(vortexState.hash, &vortex_buf_g[i*128], vortexState.number_of_rounds, vortexState.mul_type);
#endif
			/* 
			   change made to the implementation of the API 
			   so that genKAT.c does not require any changes
			*/
			vortexState.padding_length += databitlen;
			break;
		default:
			perror("Init(): bad hash type\n");
			return BAD_HASHBITLEN;
	}
	free(vortex_buf_g);
	return SUCCESS;
}

/* Final() implements the last block of the Enveloped Merkle-Damgard construction
 */

int Vortex::Final(BitSequence *hashval)
{
	vortex_uint8_t last_block[128];
	vortex_varset(last_block, 0, 128);

	if((!hashval) || (!(&vortexState)))
	{
		perror("Final(): null pointer passed\n");
		return NULL_POINTER_ERROR;
	}
	switch(vortexState.hashbitlen)
	{
		case 224: 
		case 256:	
			vortex_varcpy(last_block, vortexState.hash, 32);
			last_block[32] = 1;
			*(vortex_uint64_t *)&last_block[56] = vortexState.padding_length;
			vortex_varcpy(vortexState.hash, vortexState.ta_tb, 32);
			last_vortex_256_block(vortexState.hash, last_block, vortexState.number_of_rounds, vortexState.mul_type, vortexState.degree_of_diffusion);
			vortex_varcpy((vortex_uint8_t *)hashval, vortexState.hash, 32);
			break;
		case 384:
		case 512:
			vortex_varcpy(last_block, vortexState.hash, 64);
			last_block[64] = 1;
			*(vortex_uint64_t *)&last_block[120] = vortexState.padding_length;
			vortex_varcpy(vortexState.hash, vortexState.ta_tb, 64);
			last_vortex_512_block(vortexState.hash, last_block, vortexState.number_of_rounds, vortexState.mul_type, vortexState.degree_of_diffusion);
			vortex_varcpy((vortex_uint8_t *)hashval, vortexState.hash, 64);
			break;
		default:
			perror("Final(): bad hash type\n");
			return BAD_HASHBITLEN;
	}
	set_format(hashval, (DataLength)vortexState.hashbitlen);
	return SUCCESS;
}


int Vortex::Hash(int hashbitlen, 
				const BitSequence *data, 
				DataLength databitlen, 
				BitSequence *hashval)
{
	int error_code;
	vortex_uint64_t l[2];
	BitSequence *buf;
	l[1] = 0;
	l[0] = databitlen;
	if((!hashval) || (!data))
	{
		perror("Hash(): null pointer passed\n");
		return NULL_POINTER_ERROR;
	}
	vortex_set_default_parameters();
	vortex_number_of_rounds_g = vortexNumRounds;
	vortex_generate_constants();
	buf = (BitSequence *)malloc((databitlen+7)/8);
	vortex_varcpy(buf, (vortex_uint8_t *)data, (vortex_uint32_t)((databitlen+7)/8));
	set_format((BitSequence *)buf, databitlen);
	switch(hashbitlen)
	{
		case 224: 
		case 256:
			error_code = tunable_vortex(buf, l, (vortex_uint8_t *)hashval, (vortex_uint32_t)hashbitlen,
				vortex_number_of_rounds_g, vortex_mul_type_g, vortex_a0_b0_32_g, vortex_ta_tb_32_g, vortex_degree_of_diffusion_g);
			break;
		case 384:
		case 512:
			error_code = tunable_vortex(buf, l, (vortex_uint8_t *)hashval, (vortex_uint32_t)hashbitlen,
				vortex_number_of_rounds_g, vortex_mul_type_g, vortex_a0_b0_64_g, vortex_ta_tb_64_g, vortex_degree_of_diffusion_g);
			break;
		default:
			perror("Hash(): bad hash type\n");
			return BAD_HASHBITLEN;
	}
	if(error_code == SUCCESS)
	{
		set_format(hashval, (DataLength)hashbitlen);
	}
	free(buf);
	return error_code;
}

Vortex::Vortex(const int numRounds) {
	if ((numRounds == -1) || numRounds > 15) {
		vortexNumRounds = VORTEX_MAX_NUMBER_OF_ROUNDS;
	} else {
		vortexNumRounds = numRounds;
	}
}