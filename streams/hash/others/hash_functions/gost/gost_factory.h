#pragma once

#include <streams/hash/hash_interface.h>
#include "gost.h"

namespace others {
    class Gost : public hash::hash_interface {

    private:
        gost_ctx m_state;
        unsigned m_rounds;

    public:

        Gost(int numRounds = gost_full_rounds);
        int Init(int hash_bitsize);
        int Update(const hash::BitSequence *data, hash::DataLength databitlen);
        int Final(hash::BitSequence *hashval);

        int Hash(int hashbitlen, const hash::BitSequence *data, hash::DataLength databitlen, hash::BitSequence *hashval);

    };
}
