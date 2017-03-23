#include "estream_stream.h"
#include "eacirc/streams.h"

static estream_init_frequency create_init_frequency(const std::string& frequency) {
  // clang-format off
    if (frequency == "only-once")    return estream_init_frequency::ONLY_ONCE;
    if (frequency == "every-vector") return estream_init_frequency::EVERY_VECTOR;
  // clang-format on

  throw std::runtime_error("requested eSTREAM initialization frequency named \"" + frequency +
                           "\" does not exist");
}

estream_stream::estream_stream(const json& config, default_seed_source& seeder, std::size_t osize)
    : stream(osize)
    , _initfreq(create_init_frequency(config.at("init-frequency")))
    , _rng(config.at("generator").get<std::string>(), seeder)
    , _source(make_stream(config.at("plaintext-type"), seeder, estream_cipher::block_size))
    , _plaintext(estream_cipher::block_size)
    , _encrypted(estream_cipher::block_size)
    , _algorithm(config.at("algorithm"),
                 config.at("round").is_null()
                     ? core::optional<unsigned>{core::nullopt_t{}}
                     : core::optional<unsigned>{unsigned(config.at("round"))},
                 config.at("iv-type"), config.at("key-type")) {
  if (_initfreq == estream_init_frequency::ONLY_ONCE) {
    _algorithm.setup_key(_rng);
    _algorithm.setup_iv(_rng);
  }
}

vec_view estream_stream::next() {
  if (_initfreq == estream_init_frequency::EVERY_VECTOR) {
    _algorithm.setup_key(_rng);
    _algorithm.setup_iv(_rng);
  }

  vec_view view = _source->next();

  _algorithm.encrypt(&(*view.begin()), _encrypted.data(), osize());

  return make_cview(_encrypted);
}
