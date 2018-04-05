#pragma once

#include <streams/hash/hash_interface.h>
#include "whirlpool.h"

namespace others {
    class Whirlpool : public hash::hash_interface {

    private:
        whirlpool_ctx m_state;
        unsigned m_rounds;

    public:

        Whirlpool(int numRounds = WHIRPOOL_ROUNDS);
        int Init(int hash_bitsize);
        int Update(const hash::BitSequence *data, hash::DataLength databitlen);
        int Final(hash::BitSequence *hashval);

        int Hash(int hashbitlen, const hash::BitSequence *data, hash::DataLength databitlen, hash::BitSequence *hashval);

    };
}
