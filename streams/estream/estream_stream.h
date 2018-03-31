#pragma once

#include "estream_cipher.h"
#include "stream.h"
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>

enum class estream_init_frequency { ONLY_ONCE, EVERY_VECTOR };

struct estream_stream : stream {
    estream_stream(const json& config, default_seed_source& seeder, const std::size_t osize, core::optional<stream *> plt_stream);

    vec_cview next() override;

private:
    vec_cview get_next_ptx();

    const estream_init_frequency _initfreq;
    const std::size_t _block_size;
    constexpr static unsigned default_iv_size = 16;
    constexpr static unsigned default_key_size = 16;

    std::unique_ptr<stream> _iv_stream;
    std::unique_ptr<stream> _key_stream;

    //polymorphic_generator _rng;
    std::unique_ptr<stream> _source;
    stream *_prepared_stream_source;

    std::vector<std::uint8_t> _plaintext;

    estream_cipher _algorithm;
};
