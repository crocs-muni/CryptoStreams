#pragma once

#include "stream.h"
#include "stream_cipher.h"
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>

namespace stream_ciphers {

struct stream_stream : stream {
    stream_stream(const json &config,
                  default_seed_source &seeder,
                  const std::size_t osize,
                  core::optional<stream *> plt_stream);

    vec_cview next() override;

private:
    vec_cview get_next_ptx();

    const bool _reinit;
    const std::size_t _block_size;
    constexpr static unsigned default_iv_size = 16;
    constexpr static unsigned default_key_size = 16;

    std::unique_ptr<stream> _iv_stream;
    std::unique_ptr<stream> _key_stream;

    std::unique_ptr<stream> _source;
    stream *_prepared_stream_source;

    std::vector<std::uint8_t> _plaintext;

    stream_cipher _algorithm;
};

} // namespace stream_ciphers
