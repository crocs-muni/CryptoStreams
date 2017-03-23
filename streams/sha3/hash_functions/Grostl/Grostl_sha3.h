#ifndef GROSTL_SHA3_H
#define GROSTL_SHA3_H

#include <stdio.h>
#include <stdlib.h>
#include "../../sha3_interface.h"
#include "brg_endian.h"
#include "brg_types.h"

class Grostl : public sha3_interface {

/* some sizes (number of bytes) */
#define GROSTL_ROWS 8
#define GROSTL_LENGTHFIELDLEN GROSTL_ROWS
#define GROSTL_COLS512 8
#define GROSTL_COLS1024 16

#define GROSTL_SIZE512 (GROSTL_ROWS*GROSTL_COLS512)
#define GROSTL_SIZE1024 (GROSTL_ROWS*GROSTL_COLS1024)

#define GROSTL_ROUNDS512 10
#define GROSTL_ROUNDS1024 14

#define GROSTL_ROTL32(v, n) ((((v)<<(n))|((v)>>(32-(n))))&li_32(ffffffff))

#if (PLATFORM_BYTE_ORDER == IS_BIG_ENDIAN)
#define GROSTL_EXT_BYTE(var,n) ((grostl_u8)((grostl_u32)(var) >> (8*(3-(n)))))
#define GROSTL_U32BIG(a) (a)
#endif /* IS_BIG_ENDIAN */

#if (PLATFORM_BYTE_ORDER == IS_LITTLE_ENDIAN)
#define GROSTL_EXT_BYTE(var,n) ((grostl_u8)((grostl_u32)(var) >> (8*n)))
#define GROSTL_U32BIG(a)				\
  ((GROSTL_ROTL32(a,8) & li_32(00FF00FF)) |		\
   (GROSTL_ROTL32(a,24) & li_32(FF00FF00)))
#endif /* IS_LITTLE_ENDIAN */

typedef enum { LONG, SHORT } Var;


/* NIST API begin */
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;
typedef struct {
  grostl_u32 *chaining;            /* actual state */
  grostl_u32 block_counter1,
    block_counter2;         /* message block counter(s) */
  int hashbitlen;           /* output length in bits */
  BitSequence *buffer;      /* data buffer */
  int buf_ptr;              /* data buffer pointer */
  int bits_in_last_byte;    /* no. of message bits in last byte of
			       data buffer */
  int columns;              /* no. of columns in state */
  int statesize;            /* total no. of bytes in state */
  Var v;                    /* LONG or SHORT */
} hashState;

private:
int grostlNumRounds512;
int grostlNumRounds1024;
hashState grostlState;

public:
Grostl(const int numRounds);
int Init(int);
int Update(const BitSequence*, DataLength);
int Final(BitSequence*);
int Hash(int, const BitSequence*, DataLength, BitSequence*);
/* NIST API end   */

/* helper functions */
private:
void PrintHash(const BitSequence*, int);
void Transform(hashState *ctx, const grostl_u8 *input, int msglen, const int rounds512, const int rounds1024);
void OutputTransformation(hashState *ctx, const int rounds512, const int rounds1024);

};

#endif