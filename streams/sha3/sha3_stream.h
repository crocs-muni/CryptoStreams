#pragma once

#include "stream.h"
#include <memory>
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>

namespace sha3 {

struct sha3_interface;

std::size_t compute_hash_size(const std::size_t hash_size, const std::size_t osize);

template <typename I> void hash_data(sha3_interface& hasher, const I& data, std::uint8_t* hash, const std::size_t hash_size);


struct sha3_stream : stream {
    sha3_stream(const json& config, default_seed_source& seeder, const std::size_t osize);
    sha3_stream(sha3_stream&&);
    ~sha3_stream();

    vec_cview next() override;

private:
    const std::size_t _round;
    const std::size_t _hash_input_size;
    const std::size_t _source_size;

    std::unique_ptr<stream> _source;
    std::unique_ptr<sha3_interface> _hasher;

    std::vector<value_type> _data;
};

} // namespace sha3
