#pragma once

#include <streams/hash/hash_interface.h>
#include "md5.h"

namespace others {

    class md5_factory : public hash::hash_interface {

    public:
        md5_factory(unsigned int rounds)
                : _rounds(rounds) {}

        int Init(int others_bitsize) override;

        int Update(const hash::BitSequence *data, hash::DataLength data_bitsize) override;

        int Final(hash::BitSequence *others) override;

        int Hash(int hash_bitsize, const hash::BitSequence* data, hash::DataLength data_bitsize, hash::BitSequence* hash) override;

    private:
        unsigned int _rounds;
        MD5_CTX _ctx;
    };

} // namespace others

