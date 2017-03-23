#ifndef VORTEX_SHA3_H
#define VORTEX_SHA3_H

#include "../../sha3_interface.h"
#include "int_types.h"

class Vortex : public sha3_interface {

#if 0
#define VORTEX_HAMMING_W_ANALYSIS
#endif

typedef enum { SUCCESS = 0, 
			   FAIL = 1, 
			   BAD_HASHBITLEN = 2,
			   CALL_FAILURE	= 3,		//additional vortex implementation specific errors
			   INVALID_ROUNDS = 4,
			   INVALID_LENGTH = 5,
			   NULL_POINTER_ERROR = 6,
			   } HashReturn; 

typedef struct {
	 vortex_uint32_t	hashbitlen;
	 vortex_uint64_t	padding_length;
	 vortex_uint8_t    *hash; 					//vortex specific parameters
	 vortex_uint8_t    number_of_rounds;
     vortex_uint8_t    mul_type;
	 vortex_uint8_t    degree_of_diffusion;
	 vortex_uint8_t    *a0_b0;
	 vortex_uint8_t    *ta_tb;
      } hashState;
private:
int vortexNumRounds;
hashState vortexState;

public:
Vortex(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
vortex_uint8_t reflect(vortex_uint8_t val);
vortex_uint8_t get_hamming_w(vortex_uint8_t val);
int set_format(BitSequence *data, DataLength databitlen);

#ifdef VORTEX_HAMMING_W_ANALYSIS
void
print_hamming_weight(BitSequence *data, int hashbitlen);

vortex_uint32_t
compute_hamming_weight(BitSequence *data, int length);
#endif

};

#endif