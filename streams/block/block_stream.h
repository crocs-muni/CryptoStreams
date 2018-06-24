#pragma once

#include "stream.h"
#include <eacirc-core/json.h>
#include <eacirc-core/logger.h>
#include <eacirc-core/optional.h>
#include <eacirc-core/random.h>
#include <memory>

namespace block {

struct block_cipher;

struct block_stream : public stream {
public:
    block_stream(const json &config,
                 default_seed_source &seeder,
                 std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
                 const std::size_t osize);
    block_stream(block_stream &&);
    ~block_stream() override;

    vec_cview next() override;

private:

    const std::size_t _round;
    const std::size_t _block_size;
    const int64_t _reinit_freq;
    std::size_t _i;

    std::unique_ptr<stream> _source;
    std::unique_ptr<stream> _iv;
    std::unique_ptr<stream> _key;

    const bool _run_encryption;
    std::unique_ptr<block_cipher> _encryptor;
};

} // namespace block
