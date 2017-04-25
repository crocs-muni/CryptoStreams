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

estream_stream::estream_stream(const json& config, default_seed_source& seeder, std::size_t osize)
    : stream(osize)
    , _initfreq(create_init_frequency(config.at("init-frequency")))
    , _block_size(config.at("block-size"))
    , _rng(config.at("generator").get<std::string>(), seeder)
    , _source(make_stream(config.at("plaintext-type"), seeder, _block_size))
    , _plaintext(compute_vector_size(_block_size, osize))
    , _encrypted(compute_vector_size(_block_size, osize))
    , _algorithm(config.at("algorithm"),
                 config.at("round").is_null()
                     ? core::optional<unsigned>{core::nullopt_t{}}
                     : core::optional<unsigned>{unsigned(config.at("round"))},
                 config.at("iv-type"), config.at("key-type")) {
  logger::info() << "stream source is estream cipher: " << config.at("algorithm") << std::endl;

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
  for (auto beg = _plaintext.begin(); beg != _plaintext.end(); beg += _block_size) {
    vec_view view = _source->next();

    std::move(view.begin(), view.end(), beg);
  }

  _algorithm.encrypt(_plaintext.data(), _encrypted.data(), _plaintext.size());

  return make_cview(_encrypted);
}
