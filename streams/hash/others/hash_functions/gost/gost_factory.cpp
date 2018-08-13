/*************************** HEADER FILES ***************************/
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdexcept>
#include "gost_factory.h"

using namespace hash;

/*********************** hash adapter **********************/
namespace others {
    Gost::Gost(const int numRounds) {
        if (numRounds <= 0 || numRounds > gost_full_rounds) {
            throw std::out_of_range("Valid numRounds range for Gost is <1-32>");
        }

        this->m_rounds = (unsigned) numRounds;
    }

    int Gost::Init(int hashbitlen) {
        if (hashbitlen != gost_hash_length*8) {
            throw std::out_of_range("Gost supports only 256 bit output");
        }

        rhash_gost_init(&(this->m_state));
        return 0;
    }

    int Gost::Update(const BitSequence *data, DataLength databitlen) {
        rhash_gost_update(&(this->m_state), data, static_cast<uint32_t>(databitlen / 8), m_rounds);
        return 0;
    }

    int Gost::Final(BitSequence *hashval) {
        rhash_gost_final(&(this->m_state), hashval, m_rounds);
        return 0;
    }

    int Gost::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval) {
        int result;
        result = Gost::Init(hashbitlen);
        if (result != 0)
            return result;
        result = Gost::Update(data, databitlen);
        if (result != 0)
            return result;
        result = Gost::Final(hashval);
        return result;
    }
}
