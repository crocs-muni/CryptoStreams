#pragma once

#include <cstdint>

using BitSequence = unsigned char;
using DataLength = unsigned long long;

struct sha3_interface {
    virtual ~sha3_interface() = default;

    virtual int Init(int hash_bitsize) = 0;
    virtual int Update(const BitSequence* data, DataLength data_bitsize) = 0;
    virtual int Final(BitSequence* hash) = 0;

    virtual int
    Hash(int hash_bitsize, const BitSequence* data, DataLength data_bitsize, BitSequence* hash) = 0;
};
