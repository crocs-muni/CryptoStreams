#include "stream_stream.h"
#include "streams.h"

namespace stream_ciphers {

stream_stream::stream_stream(
    const json &config,
    default_seed_source &seeder,
    std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
    const std::size_t osize)
    : stream(osize)
    , _reinit(config.at("key").at("type") == "repeating_stream" or
              config.at("iv").at("type") == "repeating_stream")
    , _block_size(config.at("block_size"))
    , _iv_stream(make_stream(config.at("iv"), seeder, pipes, config.value("iv_size", std::size_t(default_iv_size))))
    , _key_stream(make_stream(config.at("key"), seeder, pipes, config.value("key_size", std::size_t(default_key_size))))
    , _source(make_stream(config.at("plaintext"), seeder, pipes, _block_size))
    , _plaintext(osize)
    , _algorithm(config.at("algorithm"),
                 unsigned(config.at("round")),
                 _iv_stream->osize(),
                 _key_stream->osize()) {

    if (osize % _block_size != 0) // not necessary wrong, but we never needed this, we always did
                                  // this by mistake. Change to warning if needed
        throw std::runtime_error("Output size is not multiple of block size");

    logger::info() << "stream source is estream cipher: " << config.at("algorithm") << std::endl;

    if (!_reinit) {
        _algorithm.setup_key_iv(_key_stream, _iv_stream);
    }
}

vec_cview stream_stream::next() {
    if (_reinit) {
        _algorithm.setup_key_iv(_key_stream, _iv_stream);
    }
    for (auto beg = _plaintext.begin(); beg != _plaintext.end(); beg += _block_size) {
        vec_cview view = _source->next();

        std::move(view.begin(), view.end(), beg);
    }

    _algorithm.encrypt(_plaintext.data(), _data.data(), _plaintext.size());

    return make_cview(_data);
}

} // namespace stream_ciphers
