#pragma once

#include <streams/hash/sha3/hash_functions/CHI/chi.h>
#include "sha1_factory.h"

namespace others{

int sha1_factory::Init(int others_bitsize) {
    sha1_init(& _ctx);
    return 0;
}

int sha1_factory::Update(const BitSequence *data, DataLength data_bitsize) {
    sha1_update(& _ctx, data, data_bitsize, _rounds);
    return 0;
}

    int sha1_factory::Final(BitSequence *others) {
        sha1_final(& _ctx, others, _rounds);
        return hash::hash_interface::Final(others);
    }

}
