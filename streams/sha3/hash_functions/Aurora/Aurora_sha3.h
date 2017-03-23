#ifndef AURORA_SHA3_H
#define AURORA_SHA3_H

#include "../../sha3_interface.h"

#ifndef __x86_64__
typedef unsigned long AURORA_UINT32;
#else
typedef unsigned int AURORA_UINT32;
#endif /* ?__x86_64__ */


class Aurora : public sha3_interface {

private:
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;

/* AURORA parameters */
#define AURORA_BLK_SIZE 64
#define AURORA_ROUNDS 17

/* AURORA-256 parameters */
#define AURORA256_DIGEST_SIZE 32
#define AURORA224_DIGEST_SIZE 28
#define AURORA256_DIGEST_SIZE_BIT (AURORA256_DIGEST_SIZE * 8)
#define AURORA224_DIGEST_SIZE_BIT (AURORA224_DIGEST_SIZE * 8)

/* AURORA-512/384 parameters */
#define AURORA512_DIGEST_SIZE 64
#define AURORA384_DIGEST_SIZE 48
#define AURORA512_DIGEST_SIZE_BIT (AURORA512_DIGEST_SIZE * 8)
#define AURORA384_DIGEST_SIZE_BIT (AURORA384_DIGEST_SIZE * 8)

/* AURORA context */
typedef struct {
  AURORA_UINT32 h[16];
  DataLength blk_num;
  int cnt;
  AURORA_UINT32 blk_idx;
  unsigned char buff[AURORA_BLK_SIZE];
} AURORA_CTX;

typedef struct {
  int hashbitlen;

  /* The following are algorithm-specific parameters.           *
   * Users do not have to set any parameter to these variables. */
  AURORA_CTX ctx;
  void (*HashUpdate) (AURORA_CTX *, const BitSequence *, DataLength , const int);
  void (*HashFinal) (BitSequence *, AURORA_CTX *, const int);
} hashState;

private:
int auroraNumRounds;
hashState auroraState;

public:
Aurora(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
static void Aurora256CF(AURORA_CTX *hctx, const BitSequence *m, const AURORA_UINT32 mask, const int rounds);
static void Aurora512MF(AURORA_CTX *hctx, const AURORA_UINT32 mask, const int rounds);
static void Aurora512CF(AURORA_CTX *hctx, const BitSequence *m, const AURORA_UINT32 mask, const int rounds);
static void AuroraInit0(AURORA_CTX *hctx);
static void AuroraInit1(AURORA_CTX *hctx);
static void Aurora256Update(AURORA_CTX *hctx, const BitSequence *msg, DataLength msg_bitlen, const int rounds);
static void Aurora256Final(BitSequence *digest, AURORA_CTX *hctx, const int rounds);
static void Aurora224Final(BitSequence *digest, AURORA_CTX *hctx, const int rounds);
static void Aurora512Update(AURORA_CTX *hctx, const BitSequence *msg, DataLength msg_bitlen, const int rounds);
static void Aurora512Final(BitSequence *digest, AURORA_CTX *hctx, const int rounds);
static void Aurora384Final(BitSequence *digest, AURORA_CTX *hctx, const int rounds);

};

#endif