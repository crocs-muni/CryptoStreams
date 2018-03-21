#pragma once

#include "stream.h"
#include <memory>
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>
#include <eacirc-core/optional.h>

namespace sha3 {

struct sha3_interface;

template <typename I> void hash_data(sha3_interface& hasher, const I& data, std::uint8_t* hash, const std::size_t hash_size);


struct sha3_stream : stream {
    sha3_stream(const json& config, default_seed_source& seeder, const std::size_t osize, core::optional<stream *> plt_stream);
    sha3_stream(sha3_stream&&);
    ~sha3_stream() override;

    vec_cview next() override;

private:
    vec_cview get_next_ptx();

    const std::size_t _round;
    const std::size_t _hash_size;

    std::unique_ptr<stream> _source;
    stream *_prepared_stream_source;
    std::unique_ptr<sha3_interface> _hasher;
};

} // namespace sha3
