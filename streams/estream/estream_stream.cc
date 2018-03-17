#include "estream_stream.h"
#include "streams.h"

static estream_init_frequency create_init_frequency(const std::string& frequency) {
  // clang-format off
    if (frequency == "only-once")    return estream_init_frequency::ONLY_ONCE;
    if (frequency == "every-vector") return estream_init_frequency::EVERY_VECTOR;
  // clang-format on

  throw std::runtime_error("requested eSTREAM initialization frequency named \"" + frequency +
                           "\" does not exist");
}

static std::size_t compute_vector_size(const std::size_t block_size, const std::size_t osize) {
  if (block_size > osize)
    return block_size;
  if (block_size % osize)
    return ((osize / block_size) + 1) * block_size;
  return osize;
}



static std::unique_ptr<stream> create_iv_stream(const json& iv_config, default_seed_source& seeder, const std::size_t osize, const std::string& generator) {
    // clang-format off
    if (iv_config.is_object())     return make_stream(iv_config, seeder, osize);
    if (iv_config == "zeros")      return std::make_unique<false_stream>(osize);
    if (iv_config == "ones")       return std::make_unique<true_stream >(osize); // Should be stream of 0x01u not 0xFF
    if (iv_config == "random")     return make_stream(json::parse("{ \"type\": " + generator + "-stream}"), seeder, osize);
    if (iv_config == "biasrandom") throw std::logic_error("feature not yet implemented");
    // clang-format on

    throw std::runtime_error("requested eSTREAM IV type named \"" + iv_config.dump() + "\" does not exist");
}

static std::unique_ptr<stream> create_key_stream(const json& key_config, default_seed_source& seeder, const std::size_t osize, const std::string& generator) {
    // clang-format off
    if (key_config.is_object())     return make_stream(key_config, seeder, osize);
    if (key_config == "zeros")      return std::make_unique<false_stream>(osize);
    if (key_config == "ones")       return std::make_unique<true_stream>(osize); // Should be stream of 0x01u not 0xFF
    if (key_config == "random")     return make_stream(json::parse("{ \"type\": " + generator + "-stream}"), seeder, osize);
    if (key_config == "biasrandom") throw std::logic_error("feature not yet implemented");
    // clang-format on

    throw std::runtime_error("requested eSTREAM IV type named \"" + key_config.dump() + "\" does not exist");
}

estream_stream::estream_stream(const json& config, default_seed_source& seeder, const std::size_t osize, core::optional<stream *> plt_stream)
    : stream(osize)
    , _initfreq(create_init_frequency(config.at("init-frequency")))
    , _block_size(config.at("block-size"))
    , _iv_stream(create_iv_stream(config.at("iv-type"),
                                  seeder,
                                  config.value("iv-size", default_iv_size),
                                  config.at("generator")))
    , _key_stream(create_key_stream(config.at("key-type"),
                                    seeder,
                                    config.value("key-size", default_key_size),
                                    config.at("generator")))
    , _source(make_stream(config.at("plaintext-type"), seeder, _block_size))
    , _prepared_stream_source(!plt_stream ? nullptr : *plt_stream)
    , _plaintext(compute_vector_size(_block_size, osize))
    , _algorithm(config.at("algorithm"),
                 config.at("round").is_null()
                     ? core::nullopt_t{}
                     : core::optional<unsigned>{unsigned(config.at("round"))},
                 _iv_stream->osize(), _key_stream->osize()) {

    logger::info() << "stream source is estream cipher: " << config.at("algorithm") << std::endl;
    if (_initfreq == estream_init_frequency::ONLY_ONCE) {
    _algorithm.setup_key(_key_stream, _iv_stream->osize());
    _algorithm.setup_iv(_iv_stream);
  }
}

vec_cview estream_stream::next() {
  if (_initfreq == estream_init_frequency::EVERY_VECTOR) {
      _algorithm.setup_key(_key_stream, _iv_stream->osize());
      _algorithm.setup_iv(_iv_stream);
  }
  for (auto beg = _plaintext.begin(); beg != _plaintext.end(); beg += _block_size) {
    vec_cview view = get_next_ptx();

    std::move(view.begin(), view.end(), beg);
  }

  _algorithm.encrypt(_plaintext.data(), _data.data(), _plaintext.size());

  return make_cview(_data);
}

vec_cview estream_stream::get_next_ptx()
{
    if (_prepared_stream_source) {
        return _prepared_stream_source->next();
    } else {
        return _source->next();
    }
}
