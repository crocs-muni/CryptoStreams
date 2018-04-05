#pragma once

#include <streams/hash/hash_interface.h>
#include "tiger.h"

namespace others {
    class Tiger : public hash::hash_interface {

    private:
        tiger_ctx m_state;
        unsigned m_rounds;

    public:

        Tiger(int numRounds = tiger_full_rounds);
        int Init(int hash_bitsize);
        int Update(const hash::BitSequence *data, hash::DataLength databitlen);
        int Final(hash::BitSequence *hashval);

        int Hash(int hashbitlen, const hash::BitSequence *data, hash::DataLength databitlen, hash::BitSequence *hashval);

    };
}

