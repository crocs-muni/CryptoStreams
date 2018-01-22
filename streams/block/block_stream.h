#pragma once

#include "stream.h"
#include <memory>
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>
#include <eacirc-core/logger.h>

namespace block {
    struct block_cipher;

    struct block_stream : public stream
    {
    public:
        block_stream(const json& config, default_seed_source &seeder, const std::size_t osize);
        block_stream(block_stream&&);
        ~block_stream();

        vec_cview next() override;

    private:
        const std::size_t _round;
        const std::size_t _block_size;

        std::unique_ptr<stream> _source;
        std::unique_ptr<stream> _iv;
        std::unique_ptr<stream> _key;

        std::unique_ptr<block_cipher> _encryptor;

        std::vector<value_type> _data;
    };

}
