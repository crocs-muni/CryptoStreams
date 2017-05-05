#ifndef TANGLE_SHA3_H
#define TANGLE_SHA3_H

#include "../../sha3_interface.h"

class Tangle : public sha3_interface {

#define TANGLE_DEFAULT_ROUNDS_224	72
#define TANGLE_DEFAULT_ROUNDS_256	80
#define TANGLE_DEFAULT_ROUNDS_384	96
#define TANGLE_DEFAULT_ROUNDS_512	112
#define TANGLE_DEFAULT_ROUNDS_768	128
#define TANGLE_DEFAULT_ROUNDS_1024	144

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHBITLEN = 2 } HashReturn;


typedef struct {
    int hashbitlen;				/* digest size in bits */
    unsigned char rounds;					/* number of rounds */
    unsigned int H[32];					/* current digest */
    unsigned char  rdata[1024];			/* remaining data */
    unsigned int rlen;					/* remaining length in bits */
    unsigned long long total;					/* total length in bits */
} hashState;

private:
int tangleNumRounds224;
int	tangleNumRounds256;
int	tangleNumRounds384;
int	tangleNumRounds512;
int	tangleNumRounds768;
int	tangleNumRounds1024;
hashState tangleState;
std::int64_t _extract1;
std::int64_t _extract2;
std::int64_t _b1;
std::int64_t _b2;
std::int64_t _xorTangleF;
std::int64_t _xorW;
std::int64_t _xorTangleK;
std::int64_t _xorB;
std::int64_t _add;
std::int64_t _xor1;
std::int64_t _c1;
std::int64_t _c2;
std::int64_t _c3;
std::int64_t _p;
std::int64_t _q;
std::int64_t _s;



public:
Tangle(const int numRounds, std::uint64_t heatmap)
    : tangleNumRounds224(numRounds)
    , tangleNumRounds256(numRounds)
    , tangleNumRounds384(numRounds)
    , tangleNumRounds512(numRounds)
    , tangleNumRounds768(numRounds)
    , tangleNumRounds1024(numRounds)
    , _extract1(heatmap & 0x0001)
    , _extract2(heatmap & 0x0002)
    , _b1(heatmap & 0x0004)
    , _b2(heatmap & 0x0008)
    , _xorTangleF(heatmap & 0x0010)
    , _xorW(heatmap & 0x0020)
    , _xorTangleK(heatmap & 0x0040)
    , _xorB(heatmap & 0x0080)
    , _add(heatmap & 0x0100)
    , _xor1(heatmap & 0x0200)
    , _c1(heatmap & 0x0400)
    , _c2(heatmap & 0x0800)
    , _c3(heatmap & 0x01000)
    , _p(heatmap & 0x02000)
    , _q(heatmap & 0x04000)
    , _s(heatmap & 0x08000) {}


int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
void FuncTangle(hashState *state,const BitSequence *message); /* Tangle transform function */

};

#endif
