/*************************** HEADER FILES ***************************/
#include <cstdlib>
#include <cstring>
#include <memory>
#include "whirlpool_factory.h"

using namespace hash;

/*********************** SHA3 adapter **********************/
namespace others {
    Whirlpool::Whirlpool(const int numRounds) {
        if (numRounds <= 0 || numRounds > WHIRPOOL_ROUNDS) {
            throw std::out_of_range("Valid numRounds range for Whirlpool is <1-10>");
        }

        this->m_rounds = (unsigned) numRounds;
    }

    int Whirlpool::Init(int hashbitlen) {
        if (hashbitlen != 8*whirlpool_block_size) {
            throw std::out_of_range("Whirlpool supports only 512 bit output");
        }

        rhash_whirlpool_init(&(this->m_state));
        return 0;
    }

    int Whirlpool::Update(const BitSequence *data, DataLength databitlen) {
        rhash_whirlpool_update(&(this->m_state), data, static_cast<uint32_t>(databitlen / 8), m_rounds);
        return 0;
    }

    int Whirlpool::Final(BitSequence *hashval) {
        rhash_whirlpool_final(&(this->m_state), hashval, m_rounds);
        return 0;
    }

    int Whirlpool::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval) {
        int result;
        result = Whirlpool::Init(hashbitlen);
        if (result != 0)
            return result;
        result = Whirlpool::Update(data, databitlen);
        if (result != 0)
            return result;
        result = Whirlpool::Final(hashval);
        return result;
    }
}
