#pragma once


#include <cstdint>
#include <streams/hash/hash_interface.h>
#include "sha1.h"
namespace others {

    class sha1_factory : public hash::hash_interface {

    public:
        sha1_factory(unsigned int rounds)
                :_rounds(rounds) {}

        int Init(int others_bitsize) override;
        int Update(const hash::BitSequence* data, hash::DataLength data_bitsize) override;
        int Final(hash::BitSequence* others) override;

    private:
        unsigned int _rounds;
        SHA1_CTX _ctx;
    };
} // namespace others
