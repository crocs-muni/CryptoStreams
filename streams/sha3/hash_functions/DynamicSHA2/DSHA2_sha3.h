#ifndef DSHA2_SHA3_H
#define DSHA2_SHA3_H

#include <stdlib.h>
#include "../../sha3_interface.h"

class DSHA2 : public sha3_interface {

#define DSHA2_DEFAULT_ROUNDS 17

#define DSHA2_ROTR64(x,n)   (((x) >> (n)) | ((x) << (64 - (n))))
#define DSHA2_ROTR32(x,n)   (((x) >> (n)) | ((x) << (32 - (n))))

typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;
typedef struct
{
    DataLength databitlen[2];
    unsigned int hashbitlen;
    unsigned int blocksize;
    BitSequence block[129];
    unsigned long hashval[16];
} hashState;

private:
int dsha2NumRounds;
hashState dsha2State;
std::uint64_t _comp11;
std::uint64_t _comp12;
std::uint64_t _comp13;
std::uint64_t _comp14;
std::uint64_t _G;
std::uint64_t _comp21;
std::uint64_t _comp22;
std::uint64_t _comp23;
std::uint64_t _comp24;
std::uint64_t _comp25;
std::uint64_t _comp26;
std::uint64_t _comp27;
std::uint64_t _R1;
std::uint64_t _R2;
std::uint64_t _R3;
std::uint64_t _rot;

public:
DSHA2(const int numRounds, std::uint64_t heatmap)
    : dsha2NumRounds(numRounds)
    , _comp11(heatmap & 0x0001)
    , _comp12(heatmap & 0x0002)
    , _comp13(heatmap & 0x0004)
    , _comp14(heatmap & 0x0008)
    , _G(heatmap & 0x0010)
    , _comp21(heatmap & 0x0020)
    , _comp22(heatmap & 0x0040)
    , _comp23(heatmap & 0x0080)
    , _comp24(heatmap & 0x0100)
    , _comp25(heatmap & 0x0200)
    , _comp26(heatmap & 0x0400)
    , _comp27(heatmap & 0x0800)
    , _R1(heatmap & 0x1000)
    , _R2(heatmap & 0x2000)
    , _R3(heatmap & 0x4000)
    , _rot(heatmap & 0x8000) {}


int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
int sha32_compile();
int sha64_compile();

};

#endif
