#ifndef ESSENCE_SHA3_H
#define ESSENCE_SHA3_H

#include "../../sha3_interface.h"

class Essence : public sha3_interface {

//NASTAVENIE RUND:
#define ESSENCE_COMPRESS_NUM_STEPS 32 /* MUST BE MULTIPLE OF 8 */
#define ESSENCE_COMPRESS_NUM_STEPS_MIN 8 /* MINIMUM OF STEPS */

#define ESSENCE_MD_BLOCK_SIZE_IN_BYTES 1048576LL
#define ESSENCE_HASH_TREE_LEVEL 0
#define ESSENCE_ORGANIZATIONAL_SMALL_CONSTANT 0xb7e15162
#define ESSENCE_ORGANIZATIONAL_BIG_CONSTANT 0x8aed2a6abf715880LL

#define ESSENCE_DEBUG_LEVEL 0x10

#define ESSENCE_COMPRESS_512(a,b,c,d) \
  essence_compress_512((a),(b),(c),(d))

#define ESSENCE_COMPRESS_256(a,b,c,d) \
  essence_compress_256((a),(b),(c),(d))

//typedef unsigned char BitSequence;
//typedef unsigned char byte;

//typedef unsigned long long uint64_t;
//typedef unsigned int uint32_t;
//typedef long long int64_t;
//typedef int int32_t;

//typedef uint64_t DataLength;

typedef enum {
  SUCCESS = 0,
  FAIL = 1,
  BAD_HASHBITLEN = 2,
  DATA_ALIGNMENT_ERROR = 3
} HashReturn;

typedef struct{
  unsigned long long running_hash[8];
  unsigned long long chain_vars[8];
  unsigned long long MDBIV_init[8];
#if ESSENCE_HASH_TREE_LEVEL > 0
  unsigned long long merkle_tree_hashes[8*(ESSENCE_HASH_TREE_LEVEL)];
#endif
  unsigned long long last_md_block_number;
  unsigned long long current_md_block_datalen;
  unsigned long long residual_data[8];
  int residual_data_bitlen;
  int hashbitlen;
  int within_md_block;
} hashState;

private:
int essenceNumRounds;
hashState essenceState;

public:
Essence(const int numRounds);
int Init(int);
int Update(const BitSequence *,DataLength);
int Final(BitSequence *);
int Hash(int,const BitSequence*,DataLength,BitSequence *);

private:
void PrintState();
void Join_512(unsigned long long *hash_a,unsigned long long *hash_b);
void Join_256(unsigned int *hash_a,unsigned int *hash_b);
void Merge_Tree_512(unsigned long long *chain_vars);
void Merge_Tree_256(unsigned int *chain_vars);
int Update_256(const BitSequence *data,DataLength databitlen);
int Update_512(const BitSequence *data,DataLength databitlen);
int Final_512(BitSequence *hashval);
int Final_256(BitSequence *hashval);

};

#endif