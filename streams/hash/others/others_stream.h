#pragma once

#include "stream.h"
#include <memory>
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>
#include "../hash_stream.h"

namespace others {

struct others_interface;

std::size_t compute_hash_size(const std::size_t hash_size, const std::size_t osize);

template <typename I> void hash_data(others_interface& hasher, const I& data, std::uint8_t* hash, const std::size_t hash_size);


struct others_stream : public hash::hash_stream {
    others_stream(const json& config, default_seed_source& seeder, const std::size_t osize);
    others_stream(others_stream&&);
    ~others_stream();

    vec_cview next() override;

private:
    const std::size_t _round;
    const std::size_t _hash_input_size;
    const std::size_t _source_size;

    std::unique_ptr<stream> _source;
    std::unique_ptr<others_interface> _hasher;

    std::vector<value_type> _data;
};

} // namespace others
