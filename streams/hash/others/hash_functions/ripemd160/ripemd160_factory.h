#pragma once

#include <streams/hash/hash_interface.h>
#include "ripemd160.h"

namespace others {
    class Ripemd160 : public hash::hash_interface {

    private:
        ripemd160_ctx m_state;
        unsigned m_rounds;

    public:

        Ripemd160(int numRounds = ripemd160_full_rounds);
        int Init(int hash_bitsize);
        int Update(const hash::BitSequence *data, hash::DataLength databitlen);
        int Final(hash::BitSequence *hashval);

        int Hash(int hashbitlen, const hash::BitSequence *data, hash::DataLength databitlen, hash::BitSequence *hashval);

    };
}
