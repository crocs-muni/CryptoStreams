/*************************** HEADER FILES ***************************/
#include <cstdlib>
#include <cstring>
#include <memory>
#include <stdexcept>
#include "tiger_factory.h"

using namespace hash;

/*********************** SHA3 adapter **********************/
namespace others {
    Tiger::Tiger(const int numRounds) {
        if (numRounds <= 0 || numRounds > tiger_full_rounds) {
            throw std::out_of_range("Valid numRounds range for Ripemd160 is <1-80>");
        }

        this->m_rounds = (unsigned) numRounds;
    }

    int Tiger::Init(int hashbitlen) {
        if (hashbitlen != tiger_hash_length*8) {
            throw std::out_of_range("Tiger supports only 192 bit output");
        }

        rhash_tiger_init(&(this->m_state));
        return 0;
    }

    int Tiger::Update(const BitSequence *data, DataLength databitlen) {
        rhash_tiger_update(&(this->m_state), data, static_cast<uint32_t>(databitlen / 8), m_rounds);
        return 0;
    }

    int Tiger::Final(BitSequence *hashval) {
        rhash_tiger_final(&(this->m_state), hashval, m_rounds);
        return 0;
    }

    int Tiger::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval) {
        int result;
        result = Tiger::Init(hashbitlen);
        if (result != 0)
            return result;
        result = Tiger::Update(data, databitlen);
        if (result != 0)
            return result;
        result = Tiger::Final(hashval);
        return result;
    }
}
