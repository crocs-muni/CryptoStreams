
/* Written by Michael E. Kounavis, Commmunications Technology Lab,
   Intel Corporation, 2008
 */


#ifndef VORTEX_CORE_H
#define VORTEX_CORE_H

#include "int_types.h"

int
vortex(vortex_uint8_t *data, 
			   vortex_uint64_t *length, 
			   vortex_uint8_t *hash, 
			   vortex_uint32_t type);


int
tunable_vortex(vortex_uint8_t  *data, 
			   vortex_uint64_t *length, 
			   vortex_uint8_t  *hash, 
			   vortex_uint32_t type,
			   vortex_uint8_t  number_of_rounds,
			   vortex_uint8_t  mul_type,
			   vortex_uint8_t  *a0_b0,
			   vortex_uint8_t  *ta_tb,
			   vortex_uint8_t  degree_of_diffusion);


int
vortex_224(vortex_uint8_t  *data, 
		   vortex_uint64_t *length, 
		   vortex_uint8_t  *hash, 
		   vortex_uint8_t  number_of_rounds,
		   vortex_uint8_t  mul_type,
		   vortex_uint8_t  *a0_b0,
		   vortex_uint8_t  *ta_tb,
		   vortex_uint8_t  degree_of_diffusion);


int
vortex_384(vortex_uint8_t  *data, 
		   vortex_uint64_t *length, 
		   vortex_uint8_t  *hash, 
		   vortex_uint8_t  number_of_rounds,
		   vortex_uint8_t  mul_type,
		   vortex_uint8_t  *a0_b0,
		   vortex_uint8_t  *ta_tb,
		   vortex_uint8_t  degree_of_diffusion);


int
vortex_256(vortex_uint8_t  *data, 
		   vortex_uint64_t *length_pointer, 
		   vortex_uint8_t  *hash, 
		   vortex_uint8_t  number_of_rounds,
		   vortex_uint8_t  mul_type,
		   vortex_uint8_t  *a0_b0,
		   vortex_uint8_t  *ta_tb,
		   vortex_uint8_t  degree_of_diffusion);


int
vortex_256_block(vortex_uint8_t *hash,
				 vortex_uint8_t *data,
				 vortex_uint8_t number_of_rounds, 
				 vortex_uint8_t mul_type);


int
last_vortex_256_block(vortex_uint8_t *hash,
					  vortex_uint8_t *data,
					  vortex_uint8_t number_of_rounds, 
					  vortex_uint8_t mul_type,
					  vortex_uint8_t degree_of_diffusion);


int
vortex_256_sub_block(vortex_uint8_t *hash,
					 vortex_uint8_t *data,
					 vortex_uint8_t number_of_rounds, 
					 vortex_uint8_t mul_type);


int
last_vortex_256_sub_block(vortex_uint8_t *hash,
						  vortex_uint8_t *data,
						  vortex_uint8_t number_of_rounds, 
						  vortex_uint8_t mul_type,
						  vortex_uint8_t degree_of_diffusion);


int
vortex_vma_256(vortex_uint8_t *a,
		vortex_uint8_t *b,
		vortex_uint8_t mul_type);


int
vortex_512(vortex_uint8_t  *data, 
		   vortex_uint64_t *length_pointer, 
		   vortex_uint8_t  *hash, 
		   vortex_uint8_t  number_of_rounds,
		   vortex_uint8_t  mul_type,
		   vortex_uint8_t  *a0_b0,
		   vortex_uint8_t  *ta_tb,
		   vortex_uint8_t  degree_of_diffusion);


int
vortex_512_block(vortex_uint8_t *hash,
				 vortex_uint8_t *data,
				 vortex_uint8_t number_of_rounds, 
				 vortex_uint8_t mul_type);


int
last_vortex_512_block(vortex_uint8_t *hash,
					  vortex_uint8_t *data,
					  vortex_uint8_t number_of_rounds, 
					  vortex_uint8_t mul_type,
					  vortex_uint8_t degree_of_diffusion);


int
vortex_512_sub_block(vortex_uint8_t *hash,
					 vortex_uint8_t *data,
					 vortex_uint8_t number_of_rounds, 
					 vortex_uint8_t mul_type);



int
last_vortex_512_sub_block(vortex_uint8_t *hash,
						  vortex_uint8_t *data,
						  vortex_uint8_t number_of_rounds, 
						  vortex_uint8_t mul_type,
						  vortex_uint8_t degree_of_diffusion);


int
vortex_vma_512(vortex_uint8_t *a,
		vortex_uint8_t *b,
		vortex_uint8_t mul_type);

int
vortex_generate_constants(void);

int
vortex_generate_random_tweak_values(void);

int
vortex_set_default_parameters(void);


#endif