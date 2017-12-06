#pragma once

#include "estream_cipher.h"
#include "stream.h"
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>

enum class estream_init_frequency { ONLY_ONCE, EVERY_VECTOR };

struct estream_stream : stream {
    estream_stream(const json& config, default_seed_source& seeder, std::size_t osize);

    vec_cview next() override;

private:
    const estream_init_frequency _initfreq;
    const std::size_t _block_size;
    constexpr static unsigned default_iv_size = 16;
    constexpr static unsigned default_key_size = 16;

    std::unique_ptr<stream> _iv_stream;
    std::unique_ptr<stream> _key_stream;

    //polymorphic_generator _rng;
    std::unique_ptr<stream> _source;

    std::vector<std::uint8_t> _plaintext;
    std::vector<std::uint8_t> _encrypted;

    estream_cipher _algorithm;
};
