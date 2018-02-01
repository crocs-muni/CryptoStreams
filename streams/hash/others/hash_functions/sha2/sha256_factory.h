#pragma once

#include <streams/hash/sha3/hash_functions/CHI/chi.h>
#include <streams/hash/hash_interface.h>
#include "sha256_factory.h"
#include "sha256.h"

namespace others {

class sha256_factory : public hash::hash_interface {

public:
    sha256_factory(unsigned int rounds)
            :_rounds(rounds) {}

    int Init(int others_bitsize) override;
    int Update(const BitSequence* data, DataLength data_bitsize) override;
    int Final(BitSequence* others) override;

private:
    unsigned int _rounds;
    SHA256_CTX _ctx;
};

}
