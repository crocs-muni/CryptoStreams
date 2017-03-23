#include "CubeHash_sha3.h"

#define CUBEHASH_ROTATE(a,b) (((a) << (b)) | ((a) >> (32 - b)))

Cubehash::Cubehash(const int numRounds) {
	if (numRounds == -1) {
		cubehashNumRounds = CUBEHASH_ROUNDS;
	} else {
		cubehashNumRounds = numRounds;
	}
}

void Cubehash::transform()
{
  int i;
  int r;
  cubehash_myuint32 y[16];

  for (r = 0;r < cubehashNumRounds;++r) {
    for (i = 0;i < 16;++i) cubehashState.x[i + 16] += cubehashState.x[i];
    for (i = 0;i < 16;++i) y[i ^ 8] = cubehashState.x[i];
    for (i = 0;i < 16;++i) cubehashState.x[i] = CUBEHASH_ROTATE(y[i],7);
    for (i = 0;i < 16;++i) cubehashState.x[i] ^= cubehashState.x[i + 16];
    for (i = 0;i < 16;++i) y[i ^ 2] = cubehashState.x[i + 16];
    for (i = 0;i < 16;++i) cubehashState.x[i + 16] = y[i];
    for (i = 0;i < 16;++i) cubehashState.x[i + 16] += cubehashState.x[i];
    for (i = 0;i < 16;++i) y[i ^ 4] = cubehashState.x[i];
    for (i = 0;i < 16;++i) cubehashState.x[i] = CUBEHASH_ROTATE(y[i],11);
    for (i = 0;i < 16;++i) cubehashState.x[i] ^= cubehashState.x[i + 16];
    for (i = 0;i < 16;++i) y[i ^ 1] = cubehashState.x[i + 16];
    for (i = 0;i < 16;++i) cubehashState.x[i + 16] = y[i];
  }
}

int Cubehash::Init(int hashbitlen)
{
  int i;
  int j;

  if (hashbitlen < 8) return BAD_HASHBITLEN;
  if (hashbitlen > 512) return BAD_HASHBITLEN;
  if (hashbitlen != 8 * (hashbitlen / 8)) return BAD_HASHBITLEN;

  cubehashState.hashbitlen = hashbitlen;
  for (i = 0;i < 32;++i) cubehashState.x[i] = 0;
  cubehashState.x[0] = hashbitlen / 8;
  cubehashState.x[1] = CUBEHASH_BLOCKBYTES;
  cubehashState.x[2] = cubehashNumRounds;
  for (i = 0;i < 10;++i) Cubehash::transform();
  cubehashState.pos = 0;
  return SUCCESS;
}

int Cubehash::Update(const BitSequence *data,
                  DataLength databitlen)
{
  /* caller promises us that previous data had integral number of bytes */
  /* so cubehashState.pos is a multiple of 8 */

  while (databitlen >= 8) {
    cubehash_myuint32 u = *data;
    u <<= 8 * ((cubehashState.pos / 8) % 4);
    cubehashState.x[cubehashState.pos / 32] ^= u;
    data += 1;
    databitlen -= 8;
    cubehashState.pos += 8;
    if (cubehashState.pos == 8 * CUBEHASH_BLOCKBYTES) {
      Cubehash::transform();
      cubehashState.pos = 0;
    }
  }
  if (databitlen > 0) {
    cubehash_myuint32 u = *data;
    u <<= 8 * ((cubehashState.pos / 8) % 4);
    cubehashState.x[cubehashState.pos / 32] ^= u;
    cubehashState.pos += databitlen;
  }
  return SUCCESS;
}

int Cubehash::Final(BitSequence *hashval)
{
  int i;
  cubehash_myuint32 u;

  u = (128 >> (cubehashState.pos % 8));
  u <<= 8 * ((cubehashState.pos / 8) % 4);
  cubehashState.x[cubehashState.pos / 32] ^= u;
  Cubehash::transform();
  cubehashState.x[31] ^= 1;
  for (i = 0;i < 10;++i) Cubehash::transform();
  for (i = 0;i < cubehashState.hashbitlen / 8;++i) hashval[i] = cubehashState.x[i / 4] >> (8 * (i % 4));

  return SUCCESS;
}

int Cubehash::Hash(int hashbitlen, const BitSequence *data,
                DataLength databitlen, BitSequence *hashval)
{
  //hashState state;
  if (Cubehash::Init(hashbitlen) != SUCCESS) return BAD_HASHBITLEN;
  Cubehash::Update(data,databitlen);
  return Cubehash::Final(hashval);
}