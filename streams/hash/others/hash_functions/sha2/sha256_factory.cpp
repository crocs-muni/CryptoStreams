#pragma once

#include "sha256_factory.h"

namespace others{

    int sha256_factory::Init(int others_bitsize) {
        sha256_init(& _ctx);
        return 0;
    }

    int sha256_factory::Update(const BitSequence *data, DataLength data_bitsize) {
        sha256_update(& _ctx, data, data_bitsize,_rounds);
        return 0;
    }

    int sha256_factory::Final(BitSequence *others) {
        sha256_final(& _ctx, others, _rounds);
        return hash::hash_interface::Final(others);
    }

} // namespace others