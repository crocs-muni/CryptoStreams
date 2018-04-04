#pragma once

#include "stream.h"
#include <memory>
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>
#include <eacirc-core/optional.h>

namespace hash {

struct hash_interface;

template <typename I> void hash_data(hash_interface& hasher, const I& data, std::uint8_t* hash, const std::size_t hash_size);


struct hash_stream : stream {
    hash_stream(const json& config, default_seed_source& seeder, const std::size_t osize, core::optional<stream *> plt_stream);
    hash_stream(hash_stream&&);
    ~hash_stream() override;

    vec_cview next() override;

private:
    vec_cview get_next_ptx();

    const std::size_t _round;
    const std::size_t _hash_size;

    std::unique_ptr<stream> _source;
    stream *_prepared_stream_source;
    std::unique_ptr<hash_interface> _hasher;
};

} // namespace hash
