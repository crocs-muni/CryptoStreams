#ifndef DSHA_SHA3_H
#define DSHA_SHA3_H

#include <stdlib.h>
#include "../../sha3_interface.h"

class DSHA : public sha3_interface {

#define DSHA_DEFAULT_ROUNDS 16 /* from 0 to 15 */

#define DSHA_ROTR64(x,n)   (((x) >> (n)) | ((x) << (64 - (n))))
#define DSHA_ROTR32(x,n)   (((x) >> (n)) | ((x) << (32 - (n))))

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
int dshaNumRounds;
hashState dshaState;
std::uint64_t _R101;
std::uint64_t _R102;
std::uint64_t _R103;
std::uint64_t _R104;
std::uint64_t _R105;
std::uint64_t _R106;
std::uint64_t _R107;
std::uint64_t _R108;
std::uint64_t _R109;
std::uint64_t _rotate;
std::uint64_t _G1;
std::uint64_t _G2;
std::uint64_t _G3;

public:
DSHA(const int numRounds, std::uint64_t heatmap)
    : dshaNumRounds(numRounds)
    , _R101(heatmap & 0x0001)
    , _R102(heatmap & 0x0002)
    , _R103(heatmap & 0x0004)
    , _R104(heatmap & 0x0008)
    , _R105(heatmap & 0x0010)
    , _R106(heatmap & 0x0020)
    , _R107(heatmap & 0x0040)
    , _R108(heatmap & 0x0080)
    , _R109(heatmap & 0x0100)
    , _rotate(heatmap & 0x0200)
    , _G1(heatmap & 0x0400)
    , _G2(heatmap & 0x0800)
    , _G3(heatmap & 0x1000) {}

/*
unsigned long heatmapR101 = enabledMask & 0x0001;
unsigned long heatmapR102 = enabledMask & 0x0002;
unsigned long heatmapR103 = enabledMask & 0x0004;
unsigned long heatmapR104 = enabledMask & 0x0008;
unsigned long heatmapR105 = enabledMask & 0x0010;
unsigned long heatmapR106 = enabledMask & 0x0020;
unsigned long heatmapR107 = enabledMask & 0x0040;
unsigned long heatmapR108 = enabledMask & 0x0080;
unsigned long heatmapR109 = enabledMask & 0x0100;
unsigned long heatmapRotate = enabledMask & 0x0200;
unsigned long heatmapG1 = enabledMask & 0x0400;
unsigned long heatmapG2 = enabledMask & 0x0800;
unsigned long heatmapG3 = enabledMask & 0x1000;
*/

int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

private:
int sha32_compile();
int sha64_compile();

};

#endif
