#pragma once

#include "stream.h"
#include "stream_cipher.h"
#include <eacirc-core/json.h>
#include <eacirc-core/random.h>

namespace stream_ciphers {

struct stream_stream : stream {
    stream_stream(const json &config,
                  default_seed_source &seeder,
                  std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
                  const std::size_t osize);

    vec_cview next() override;

private:

    const bool _reinit;
    const std::size_t _block_size;
    constexpr static unsigned default_iv_size = 16;
    constexpr static unsigned default_key_size = 16;

    std::unique_ptr<stream> _iv_stream;
    std::unique_ptr<stream> _key_stream;

    std::unique_ptr<stream> _source;

    std::vector<std::uint8_t> _plaintext;

    stream_cipher _algorithm;
};

} // namespace stream_ciphers
