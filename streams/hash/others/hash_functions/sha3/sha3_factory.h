#pragma once

#include <streams/hash/hash_interface.h>
#include "sha3.h"

namespace others {

class sha3_factory : public hash::hash_interface {

public:
    sha3_factory(unsigned int rounds)
            :_rounds(rounds) {}

    int Init(int bitsize) override;
    int Update(const hash::BitSequence* data, hash::DataLength data_bitsize) override;
    int Final(hash::BitSequence* hash) override;
    int Hash(int hash_bitsize, const hash::BitSequence* data, hash::DataLength data_bitsize, hash::BitSequence* hash) override;

private:
    unsigned int _rounds;
    sha3_ctx _ctx;
};

}
