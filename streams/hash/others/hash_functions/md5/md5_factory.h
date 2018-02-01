#pragma once


#include <streams/hash/hash_interface.h>
#include <streams/hash/sha3/hash_functions/CHI/chi.h>
#include "md5.h"

namespace others {

    class md5_factory : public hash::hash_interface {

    public:
        md5_factory(unsigned int rounds)
                : _rounds(rounds) {}

        int Init(int others_bitsize) override;

        int Update(const BitSequence *data, DataLength data_bitsize) override;

        int Final(BitSequence *others) override;

    private:
        unsigned int _rounds;
        MD5_CTX _ctx;
    };

} // namespace others

