#pragma once

/**
 * Source: https://github.com/B-Con/crypto-algorithms
 */

#include <cstdint>
#include <streams/hash/others/others_interface.h>
#include "sha1.h"
namespace others {

    class sha1_factory : public others_interface {

    public:
        sha1_factory(unsigned int rounds)
                :rounds(rounds) {}

        int Init(int others_bitsize) override;
        int Update(const BitSequence* data, DataLength data_bitsize) override;
        int Final(BitSequence* others) override;

    private:
        unsigned int rounds;
    };
}
