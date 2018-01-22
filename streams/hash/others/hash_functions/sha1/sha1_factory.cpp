#pragma once

#include "sha1_factory.h"

namespace others{

int sha1_factory::Init(int others_bitsize) {
    sha1_init(& _ctx);
    return 0;
}

int sha1_factory::Update(const hash::BitSequence *data, hash::DataLength data_bitsize) {
    sha1_update(& _ctx, data, data_bitsize/8, _rounds);
    return 0;
}

int sha1_factory::Final(hash::BitSequence *others) {
    sha1_final(& _ctx, others, _rounds);
    return 0;
}

int sha1_factory::Hash(int hash_bitsize, const hash::BitSequence *data, hash::DataLength data_bitsize, hash::BitSequence *hash) {
    Init(hash_bitsize);
    Update(data, data_bitsize);
    Final(hash);
}

}
