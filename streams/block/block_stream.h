#pragma once

#include "stream.h"
#include <memory>
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>
#include <eacirc-core/logger.h>
#include <eacirc-core/optional.h>

namespace block {
    struct block_cipher;

    struct block_stream : public stream
    {
    public:
        block_stream(const json& config, default_seed_source &seeder, const std::size_t osize, core::optional<stream *> plt_stream);
        block_stream(block_stream&&);
        ~block_stream() override;

        vec_cview next() override;

    private:
        vec_cview get_next_ptx();

        const std::size_t _round;
        const std::size_t _block_size;
        const int64_t _reinit_freq;
        std::size_t _i;

        std::unique_ptr<stream> _source;
        std::unique_ptr<stream> _iv;
        std::unique_ptr<stream> _key;

        std::unique_ptr<block_cipher> _encryptor;
        stream *_prepared_stream_source;
    };

}
