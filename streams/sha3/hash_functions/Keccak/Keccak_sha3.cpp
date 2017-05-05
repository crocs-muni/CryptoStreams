#include <string.h>
#include <stdexcept>
#include "Keccak_sha3.h"
extern "C" {
#include "KeccakF-1600-interface.h"
#include "KeccakF-1600-opt32-settings.h"
}

typedef unsigned char UINT8;
typedef unsigned long long int UINT64;
typedef UINT64 tKeccakLane;

#ifndef LITTLE_ENDIAN
/** Function to load a 64-bit value using the little-endian (LE) convention.
  * On a LE platform, this could be greatly simplified using a cast.
  */

static UINT64 load64(const UINT8 *x)
{
    int i;
    UINT64 u=0;

    for(i=7; i>=0; --i) {
        u <<= 8;
        u |= x[i];
    }
    return u;
}

/** Function to store a 64-bit value using the little-endian (LE) convention.
  * On a LE platform, this could be greatly simplified using a cast.
  */
static void store64(UINT8 *x, UINT64 u)
{
    unsigned int i;

    for(i=0; i<8; ++i) {
        x[i] = u;
        u >>= 8;
    }
}

/** Function to XOR into a 64-bit value using the little-endian (LE) convention.
  * On a LE platform, this could be greatly simplified using a cast.
  */
static void xor64(UINT8 *x, UINT64 u)
{
    unsigned int i;

    for(i=0; i<8; ++i) {
        x[i] ^= u;
        u >>= 8;
    }
}
#endif

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define ROL64(a, offset) ((((UINT64)a) << offset) ^ (((UINT64)a) >> (64-offset)))
#define i(x, y) ((x)+5*(y))

#ifdef LITTLE_ENDIAN
    #define readLane(x, y)          (((tKeccakLane*)state)[i(x, y)])
    #define writeLane(x, y, lane)   (((tKeccakLane*)state)[i(x, y)]) = (lane)
    #define XORLane(x, y, lane)     (((tKeccakLane*)state)[i(x, y)]) ^= (lane)
#else
    #define readLane(x, y)          load64((UINT8*)state+sizeof(tKeccakLane)*i(x, y))
    #define writeLane(x, y, lane)   store64((UINT8*)state+sizeof(tKeccakLane)*i(x, y), lane)
    #define XORLane(x, y, lane)     xor64((UINT8*)state+sizeof(tKeccakLane)*i(x,y), lane)
#endif

/**
  * Function that computes the linear feedback shift register (LFSR) used to
  * define the round constants (see [Keccak Reference, Section 1.2]).
  */
int LFSR86540(UINT8 *LFSR)
{
    int result = ((*LFSR) & 0x01) != 0;
    if (((*LFSR) & 0x80) != 0)
        // Primitive polynomial over GF(2): x^8+x^6+x^5+x^4+1
        (*LFSR) = ((*LFSR) << 1) ^ 0x71;
    else
        (*LFSR) <<= 1;
    return result;
}

/**
 * Function that computes the Keccak-f[1600] permutation on the given state.
 */
void Keccak::KeccakF1600_StatePermute(void *state)
{
    unsigned int round, x, y, j, t;
    UINT8 LFSRstate = 0x01;

    for(round=0; round < m_rounds; round++) {

        if(_theta) {   // === θ step (see [Keccak Reference, Section 2.3.2]) ===
            tKeccakLane C[5], D;
            D = 0;
            // Compute the parity of the columns
            for(x=0; x<5; x++)
                C[x] = readLane(x, 0) ^ readLane(x, 1) ^ readLane(x, 2) ^ readLane(x, 3) ^ readLane(x, 4);
            for(x=0; x<5; x++) {
                // Compute the θ effect for a given column
                if(_d1) D += C[(x+4)%5];
                if(_d2) D ^= ROL64(C[(x+1)%5], 1);
        //D = C[(x+4)%5] ^ ROL64(C[(x+1)%5], 1);
                // Add the θ effect to the whole column
                for (y=0; y<5; y++)
                    XORLane(x, y, D);
            }
        }

        if(_rho){   // === ρ and π steps (see [Keccak Reference, Sections 2.3.3 and 2.3.4]) ===
            tKeccakLane current, temp;
            // Start at coordinates (1 0)
            x = 1; y = 0;
            current = readLane(x, y);
            // Iterate over ((0 1)(2 3))^t * (1 0) for 0 ≤ t ≤ 23
            for(t=0; t<24; t++) {
                // Compute the rotation constant r = (t+1)(t+2)/2
                unsigned int r = ((t+1)*(t+2)/2)%64;
                // Compute ((0 1)(2 3)) * (x y)
                unsigned int Y = (2*x+3*y)%5; x = y; y = Y;
                // Swap current and state(x,y), and rotate
                temp = readLane(x, y);
                writeLane(x, y, ROL64(current, r));
                current = temp;
            }
        }

           // === χ step (see [Keccak Reference, Section 2.3.1]) ===
            tKeccakLane temp[5], temp1, temp2, temp3;
            for(y=0; y<5; y++) {
                // Take a copy of the plane
                for(x=0; x<5; x++)
                    temp[x] = readLane(x, y);
                // Compute χ on the plane
                for(x=0; x<5; x++)
            temp1 = 0;
            temp2 = 0;
            temp3 = 0;
            if(_chi1) temp1 = temp[x];
            if(_chi2) temp2 = ~temp[(x+1)%5];
            if(_chi3) temp3 = temp[(x+2)%5];
            if(temp2 == 0) {
                writeLane(x, y, temp1 ^ temp3);
            }
            else{
                if(temp3 = 0) writeLane(x, y, temp1 ^ temp2);
                        else writeLane(x, y, temp1 ^((~temp[(x+1)%5]) & temp[(x+2)%5]));
            }}


        if(_iota){   // === ι step (see [Keccak Reference, Section 2.3.5]) ===
            for(j=0; j<7; j++) {
                unsigned int bitPosition = (1<<j)-1; //2^j-1
                if (LFSR86540(&LFSRstate))
                    XORLane(0, 0, (tKeccakLane)1<<bitPosition);
            }
        }
    }
}




int Keccak::Init(int hashbitlen)
{
    switch(hashbitlen) {
        case 0: // Default parameters, arbitrary length output
            InitSponge((spongeState*)&keccakState, 1024, 576);
            break;
        case 224:
            InitSponge((spongeState*)&keccakState, 1152, 448);
            break;
        case 256:
            InitSponge((spongeState*)&keccakState, 1088, 512);
            break;
        case 384:
            InitSponge((spongeState*)&keccakState, 832, 768);
            break;
        case 512:
            InitSponge((spongeState*)&keccakState, 576, 1024);
            break;
        default:
            return BAD_HASHLEN;
    }
    keccakState.fixedOutputLength = hashbitlen;
    return SUCCESS;
}

int Keccak::Update(const BitSequence *data, DataLength databitlen)
{

        unsigned int rateInBytes = keccakState.rate/8;
        unsigned int blockSize = 0;
        unsigned int i;
        // === Initialize the state ===
            memset(keccakState.state, 0, sizeof(keccakState.state));
    // === Absorb all the input blocks ===
        while(databitlen > 0) {
            blockSize = MIN(databitlen, rateInBytes);
            for(i=0; i<blockSize; i++)
                keccakState.state[i] ^= data[i];
            data += blockSize;
            databitlen -= blockSize;

            if (blockSize == rateInBytes) {
                Keccak::KeccakF1600_StatePermute(keccakState.state);
                blockSize = 0;
            }
        }

        // === Do the padding and switch to the squeezing phase ===
        // Absorb the last few bits and add the first bit of padding (which coincides with the delimiter in delimitedSuffix)
        keccakState.state[blockSize] ^= keccakState.bitsInQueue;
        // If the first bit of padding is at position rate-1, we need a whole new block for the second bit of padding
        if (((keccakState.bitsInQueue & 0x80) != 0) && (blockSize == (rateInBytes-1)))
            Keccak::KeccakF1600_StatePermute(keccakState.state);
        // Add the second bit of padding
        keccakState.state[rateInBytes-1] ^= 0x80;
        // Switch to the squeezing phase
        Keccak::KeccakF1600_StatePermute(keccakState.state);
    return SUCCESS;
    /*if ((databitlen % 8) == 0)
        return Absorb((spongeState*)&keccakState, data, databitlen);
    else {
        int ret = Absorb((spongeState*)&keccakState, data, databitlen - (databitlen % 8));
        if (ret == SUCCESS) {
            unsigned char lastByte;
            // Align the last partial byte to the least significant bits
            lastByte = data[databitlen/8] >> (8 - (databitlen % 8));
            return Absorb((spongeState*)&keccakState, &lastByte, databitlen % 8);
        }
        else
            return ret;
    }*/
}

int Keccak::Final(BitSequence *hashval)
{
    unsigned int outputByteLen = keccakState.fixedOutputLength;
    unsigned int rateInBytes = keccakState.rate/8;
        unsigned int blockSize = 0;
    // === Squeeze out all the output blocks ===
        while(outputByteLen > 0) {
            blockSize = MIN(outputByteLen, rateInBytes);
            memcpy(hashval, keccakState.state, blockSize);
            hashval += blockSize;
            outputByteLen -= blockSize;

            if (outputByteLen > 0)
                KeccakF1600_StatePermute(keccakState.state);
        }
        return SUCCESS;
    //return Squeeze(&keccakState, hashval, keccakState.fixedOutputLength);
}

int Keccak::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
    //hashState state;
    //HashReturn result;
    int result;

    if ((hashbitlen != 224) && (hashbitlen != 256) && (hashbitlen != 384) && (hashbitlen != 512))
        return BAD_HASHLEN; // Only the four fixed output lengths available through this API
    result = Keccak::Init(hashbitlen);
    if (result != SUCCESS)
        return result;
    result = Keccak::Update(data, databitlen);
    if (result != SUCCESS)
        return result;
    result = Keccak::Final(hashval);
    return result;
}
