#ifndef LUFFA_SHA3_H
#define LUFFA_SHA3_H

#include "../../sha3_interface.h"

class Luffa : public sha3_interface {

#define LUFFA_DEFAULT_ROUNDS 8

/* The length of digests*/
#define LUFFA_DIGEST_BIT_LEN_224 224
#define LUFFA_DIGEST_BIT_LEN_256 256
#define LUFFA_DIGEST_BIT_LEN_384 384
#define LUFFA_DIGEST_BIT_LEN_512 512

/*********************************/
/* The parameters of Luffa       */
#define LUFFA_MSG_BLOCK_BIT_LEN 256  /*The bit length of a message block*/
#define LUFFA_MSG_BLOCK_BYTE_LEN (LUFFA_MSG_BLOCK_BIT_LEN >> 3) /* The byte length
                                                     * of a message block*/

/* The number of blocks in Luffa */
#define LUFFA_WIDTH_224 3
#define LUFFA_WIDTH_256 3
#define LUFFA_WIDTH_384 4
#define LUFFA_WIDTH_512 5

/* The limit of the length of message */
#define LUFFA_LIMIT_224 64
#define LUFFA_LIMIT_256 64
#define LUFFA_LIMIT_384 128
#define LUFFA_LIMIT_512 128
/*********************************/


typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2} HashReturn;

typedef struct {
    int hashbitlen;
    unsigned long long bitlen[2]; /* Message length in bits */
    unsigned int rembitlen; /* Length of buffer data to be hashed */
    unsigned int buffer[8]; /* Buffer to be hashed */
    unsigned int chainv[40];   /* Chaining values */
} hashState;

private:
int luffaNumRounds;
hashState luffaState;

public:
Luffa(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
void Update256(const BitSequence *data, DataLength databitlen);
void Update384(const BitSequence *data, DataLength databitlen);
void Update512(const BitSequence *data, DataLength databitlen);
void rnd256();
void rnd384();
void rnd512();
void finalization224(unsigned int *b);
void finalization256(unsigned int *b);
void finalization384(unsigned int *b);
void finalization512(unsigned int *b);
void process_last_msgs256();
void process_last_msgs384();
void process_last_msgs512();

};

#endif