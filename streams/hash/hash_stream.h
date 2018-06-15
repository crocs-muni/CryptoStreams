#pragma once

#include "stream.h"
#include <eacirc-core/json.h>
#include <eacirc-core/optional.h>
#include <eacirc-core/random.h>
#include <memory>

namespace hash {

struct hash_interface;

template <typename I>
void hash_data(hash_interface &hasher,
               const I &data,
               std::uint8_t *hash,
               const std::size_t hash_size);

struct hash_stream : stream {
    hash_stream(const json &config,
                default_seed_source &seeder,
                std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
                const std::size_t osize);
    hash_stream(hash_stream &&);
    ~hash_stream() override;

    vec_cview next() override;

private:
    const std::size_t _round;
    const std::size_t _hash_size;

    std::unique_ptr<stream> _source;
    stream *_prepared_stream_source;
    std::unique_ptr<hash_interface> _hasher;
};

} // namespace hash
