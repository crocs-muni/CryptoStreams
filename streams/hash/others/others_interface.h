#pragma once

#include <cstdint>
#include "../hash_interface.h"

namespace others {

using BitSequence = unsigned char;
using DataLength = unsigned long long;

struct others_interface : public hash::hash_interface {
    ~others_interface() override = default;

    int Init(int others_bitsize) override = 0;
    int Update(const BitSequence* data, DataLength data_bitsize) override = 0;
    int Final(BitSequence* others) override = 0;

    virtual int
    Others(int others_bitsize, const BitSequence* data, DataLength data_bitsize, BitSequence* hash) = 0;
};

} // namespace others
