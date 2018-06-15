#include "hash_stream.h"
#include "hash_factory.h"
#include "hash_interface.h"
#include "streams.h"
#include <algorithm>

namespace hash {

template <typename I>
void hash_data(hash_interface &hasher,
               const I &data,
               std::uint8_t *hash,
               const std::size_t hash_size) {
    using std::to_string;

    int status = hasher.Init(int(hash_size * 8));
    if (status != 0)
        throw std::runtime_error("cannot initialize hash (code: " + to_string(status) + ")");

    status = hasher.Update(&(*data.begin()), 8 * (data.size()));
    if (status != 0)
        throw std::runtime_error("cannot update the hash (code: " + to_string(status) + ")");

    status = hasher.Final(hash);
    if (status != 0)
        throw std::runtime_error("cannot finalize the hash (code: " + to_string(status) + ")");
}

hash_stream::hash_stream(
    const json &config,
    default_seed_source &seeder,
    std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
    const std::size_t osize)
    : stream(osize) // round osize to multiple of _hash_input_size
    , _round(config.at("round"))
    , _hash_size(std::size_t(config.at("hash_size")))
    , _source(make_stream(
          config.at("source"),
          seeder,
          pipes,
          config.value("input_size", _hash_size))) // if input size is not defined, use hash-size
    , _hasher(hash_factory::create(config.at("algorithm"), unsigned(_round))) {
    if (osize % _hash_size != 0) {
        // not necessary wrong, but we never needed this, we always did
        // this by mistake. Change to warning if needed
        throw std::runtime_error("Output size is not multiple of hash size");
    }
    logger::info() << "stream source is hash function: " << config.at("algorithm") << std::endl;
}

hash_stream::hash_stream(hash_stream &&) = default;
hash_stream::~hash_stream() = default;

vec_cview hash_stream::next() {
    auto hash = _data.data();
    for (std::size_t i = 0; i < _data.size(); i += _hash_size) {
        vec_cview view = _source->next();

        hash_data(*_hasher, view, &hash[i], _hash_size);
    }

    return make_view(_data.cbegin(), osize());
}

} // namespace hash
