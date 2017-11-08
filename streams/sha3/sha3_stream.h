#pragma once

#include "stream.h"
#include <memory>
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>

struct sha3_interface;

struct sha3_stream : stream {
    sha3_stream(const json& config, default_seed_source& seeder, std::size_t osize);
    sha3_stream(sha3_stream&&);
    ~sha3_stream();

    vec_view next() override;

private:
    const std::size_t _round;
    const std::size_t _hash_input_size;
    const std::size_t _source_size;

    std::unique_ptr<stream> _source;
    std::unique_ptr<sha3_interface> _hasher;

    std::vector<value_type> _data;
};
