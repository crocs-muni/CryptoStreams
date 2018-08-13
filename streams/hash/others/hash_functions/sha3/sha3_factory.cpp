#include "sha3_factory.h"
#include "algorithm"
#include "sha3.h"
#include <stdexcept>

namespace others {

int sha3_factory::Init(int bitsize) {
    switch (bitsize) {
    case 224:
        rhash_sha3_224_init(&_ctx);
        break;
    case 256:
        rhash_sha3_256_init(&_ctx);
        break;
    case 384:
        rhash_sha3_384_init(&_ctx);
        break;
    case 512:
        rhash_sha3_512_init(&_ctx);
        break;
    }

    return 0;
}

int sha3_factory::Update(const hash::BitSequence *data, hash::DataLength data_bitsize) {
    if (data_bitsize % 8) {
        throw std::runtime_error(
            "Our SHA3 implementation does not support messages of size % 8 != 0");
    }
    rhash_sha3_update(&_ctx, data, data_bitsize / 8, _rounds);
    return 0;
}

int sha3_factory::Final(hash::BitSequence *hash) {
    rhash_sha3_final(&_ctx, hash, _rounds);
    return 0;
}

int sha3_factory::Hash(int hash_bitsize,
                       const hash::BitSequence *data,
                       hash::DataLength data_bitsize,
                       hash::BitSequence *hash) {
    Init(hash_bitsize);
    Update(data, data_bitsize);
    Final(hash);
    return 0;
}

} // namespace others
