#pragma once

#include "md5_factory.h"

namespace others {

int md5_factory::Init(int others_bitsize) {
    md5_init(&_ctx);
    return 0;
}

int md5_factory::Update(const hash::BitSequence *data, hash::DataLength data_bitsize) {
    md5_update(&_ctx, data, data_bitsize/8, _rounds);
    return 0;
}

int md5_factory::Final(hash::BitSequence *others) {
    md5_final(&_ctx, others, _rounds);
    return 0;
}

int md5_factory::Hash(int hash_bitsize, const hash::BitSequence *data, hash::DataLength data_bitsize,
                      hash::BitSequence *hash) {
    Init(hash_bitsize);
    Update(data,data_bitsize/8);
    Final(hash);
    return 0;
}
} // namespace others
