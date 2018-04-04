#ifndef SANDSTORM_SHA3_H
#define SANDSTORM_SHA3_H

#include "../../sha3_interface.h"
#include "SHA3_ref.h"

class SandStorm : public SHA3 {

private:
int sandStormNumRounds;
int tunableSecurityParameter;
sandHashState sandState;

public:
SandStorm(const int numRounds);
int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
void createMessageFromQueue(DataLength m[8], sandHashState *state);
void swapMessage(DataLength m[8]);
void createMessageFromBlock(DataLength M[8], DataLength input[5][4]);
void Do_Block_ModMix_Ref (unsigned long long db[8], unsigned long long block_buffer[5][4]);
void compress(sandHashState *state);
void initCompress(sandHashState *state);
void printPrevBlock(DataLength input[5][4], int level);
void print64BS(BitSequence bstmp[64], int id);
void printQueue(sandHashState *state, int id);
void printMessage(DataLength Mess[8]);
unsigned long long F(unsigned long long x);
unsigned long long G(unsigned long long x);
unsigned long long Ch(unsigned long long a, unsigned long long b, unsigned long long c);
unsigned long long RotLeft(unsigned long long n, int i);
unsigned long long Rot32(unsigned long long n);
unsigned long long SB(unsigned long long x);
unsigned long long C(int i);
int mm(int i);
unsigned long long Msg_M(int i);
void bit_mix();
void round_in(unsigned long long buffer[4], int);
void round_out(unsigned long long prev_block[4], unsigned long long current_block[4]);
void round4_out(unsigned long long current_block[4]);
void do_round(int round_num);
void block_init();
sand_u128 k128(unsigned long long hi, unsigned long long lo);
sand_u128 a128(sand_u128 x, sand_u128 y);
sand_u128 x128(sand_u128 x, sand_u128 y);
sand_u128 c128(sand_u128 x);
sand_u128 b128(sand_u128 x, sand_u128 y);
sand_u128 r128(sand_u128 x, int sh);
sand_u128 m128(unsigned long long x, unsigned long long y);
sand_u128 s128(unsigned long long x);
sand_u128 sandwf(sand_u128 x);
sand_u128 sandwg(sand_u128 x);
sand_u128 sb128(sand_u128 x);
void mscpr(sand_u128 *ms);
void sandwmsgsch(sand_u128 *MS);
void sandwcmprs(sand_u128 msg[], sand_u128 prev[], int level, int xrnds);
void hstpr(sandHashState *hst);
void sandwdoit(sandHashState *hstate);
void sandwinit(sandHashState *hstate, int hashsize);
void sandwupdate(sandHashState *hstate, unsigned long long bitsofdata, const unsigned char *data);
void sandwfinish(sandHashState *hstate);
void sandwprint(sandHashState *hstate);

};

#endif