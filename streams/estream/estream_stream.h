#pragma once

#include "estream_cipher.h"
#include <core/json.h>
#include <core/random.h>
#include <core/stream.h>

enum class estream_init_frequency { ONLY_ONCE, EVERY_VECTOR };

struct estream_stream : stream {
    estream_stream(const json& config, default_seed_source& seeder, std::size_t osize);

    vec_view next() override;

private:
    const estream_init_frequency _initfreq;

    polymorphic_generator _rng;
    std::unique_ptr<stream> _source;

    std::vector<std::uint8_t> _plaintext;
    std::vector<std::uint8_t> _encrypted;

    estream_cipher _algorithm;
};
