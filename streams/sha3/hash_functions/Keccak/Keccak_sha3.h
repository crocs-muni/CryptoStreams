#ifndef KECCAK_SHA3_H
#define KECCAK_SHA3_H

#define KECCAK_FULL_ROUNDS 24

#include "../../sha3_interface.h"
extern "C" {
#include "KeccakSponge.h"
}

class Keccak : public sha3_interface {

/*typedef unsigned char BitSequence;
typedef unsigned long long DataLength;*/
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;

typedef spongeState hashState;

private:
hashState keccakState;
unsigned m_rounds;
void KeccakF1600_StatePermute(void *state);
std::uint64_t _theta;
std::uint64_t _rho;
std::uint64_t _iota;
std::uint64_t _d1;
std::uint64_t _d2;
std::uint64_t _chi1;
std::uint64_t _chi2;
std::uint64_t _chi3;

public:
Keccak(const int numRounds, std::uint64_t heatmap)
    : m_rounds(numRounds)
    , _theta(heatmap & 0x0004)
    , _rho(heatmap & 0x0008)
    , _iota(heatmap & 0x0080)
    , _d1(heatmap & 0x0001)
    , _d2(heatmap & 0x0002)
    , _chi1(heatmap & 0x0010)
    , _chi2(heatmap & 0x0020)
    , _chi3(heatmap & 0x0040) {}

int Init(int hashbitlen);
int Update(const BitSequence *data, DataLength databitlen);
int Final(BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

};

#endif
