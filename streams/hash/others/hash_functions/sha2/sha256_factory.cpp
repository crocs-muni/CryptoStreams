#include "sha256_factory.h"

namespace others{

    int sha256_factory::Init(int others_bitsize) {
        sha256_init(& _ctx);
        return 0;
    }

    int sha256_factory::Update(const hash::BitSequence *data, hash::DataLength data_bitsize) {
        sha256_update(& _ctx, data, data_bitsize/8,_rounds);
        return 0;
    }

    int sha256_factory::Final(hash::BitSequence *others) {
        sha256_final(& _ctx, others, _rounds);
        return 0;
    }

    int sha256_factory::Hash(int hash_bitsize, const hash::BitSequence *data, hash::DataLength data_bitsize,
                             hash::BitSequence *hash) {
        Init(hash_bitsize);
        Update(data,data_bitsize/8);
        Final(hash);
        return 0;
    }

} // namespace others
