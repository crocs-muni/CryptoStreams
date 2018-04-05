/*************************** HEADER FILES ***************************/
#include <cstdlib>
#include <cstring>
#include <memory>
#include "ripemd160_factory.h"

using namespace hash;

/*********************** hash adapter **********************/
namespace others {
    Ripemd160::Ripemd160(const int numRounds) {
        if (numRounds <= 0 || numRounds > ripemd160_full_rounds) {
            throw std::out_of_range("Valid numRounds range for Ripemd160 is <1-80>");
        }

        this->m_rounds = (unsigned) numRounds;
    }

    int Ripemd160::Init(int hashbitlen) {
        if (hashbitlen != 160) {
            throw std::out_of_range("Ripemd160 supports only 160 bit output");
        }

        rhash_ripemd160_init(&(this->m_state));
        return 0;
    }

    int Ripemd160::Update(const BitSequence *data, DataLength databitlen) {
        rhash_ripemd160_update(&(this->m_state), data, static_cast<uint32_t>(databitlen / 8), m_rounds);
        return 0;
    }

    int Ripemd160::Final(BitSequence *hashval) {
        rhash_ripemd160_final(&(this->m_state), hashval, m_rounds);
        return 0;
    }

    int Ripemd160::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval) {
        int result;
        result = Ripemd160::Init(hashbitlen);
        if (result != 0)
            return result;
        result = Ripemd160::Update(data, databitlen);
        if (result != 0)
            return result;
        result = Ripemd160::Final(hashval);
        return result;
    }
}
