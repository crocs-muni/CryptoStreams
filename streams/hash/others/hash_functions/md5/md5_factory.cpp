#pragma once

#include "md5_factory.h"

namespace others {

int md5_factory::Init(int others_bitsize) {
    md5_init(&_ctx);
    return 0;
}

int md5_factory::Update(const BitSequence *data, DataLength data_bitsize) {
    md5_update(&_ctx, data, data_bitsize, _rounds);
    return 0;
}

int md5_factory::Final(BitSequence *others) {
    md5_final(&_ctx, others, _rounds);
    return hash::hash_interface::Final(others);
}
} // namespace others
