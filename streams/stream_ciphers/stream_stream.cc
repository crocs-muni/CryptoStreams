#include "stream_stream.h"
#include "streams.h"

stream_stream::stream_stream(const json& config, default_seed_source& seeder, const std::size_t osize, core::optional<stream *> plt_stream)
    : stream(osize)
    , _reinit(config.at("key").at("type") == "repeating-stream" || config.at("iv").at("type") == "repeating-stream")
    , _block_size(config.at("block-size"))
    , _iv_stream(make_stream(config.at("iv"), seeder, config.value("iv-size", default_iv_size)))
    , _key_stream(make_stream(config.at("key"), seeder, config.value("key-size", default_key_size)))
    , _source(make_stream(config.at("plaintext"), seeder, _block_size))
    , _prepared_stream_source(!plt_stream ? nullptr : *plt_stream)
    , _plaintext(osize)
    , _algorithm(config.at("algorithm"), unsigned(config.at("round")), _iv_stream->osize(), _key_stream->osize()) {

    if (osize % _block_size != 0) // not necessary wrong, but we never needed this, we always did this by mistake. Change to warning if needed
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
    vec_cview view = get_next_ptx();

    std::move(view.begin(), view.end(), beg);
  }

  _algorithm.encrypt(_plaintext.data(), _data.data(), _plaintext.size());

  return make_cview(_data);
}

vec_cview stream_stream::get_next_ptx()
{
    if (_prepared_stream_source) {
        return _prepared_stream_source->next();
    } else {
        return _source->next();
    }
}
