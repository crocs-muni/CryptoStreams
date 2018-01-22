#include "others_stream.h"
#include "streams.h"
#include "others_factory.h"
#include "others_interface.h"
#include <algorithm>

namespace others {

std::size_t compute_hash_size(const std::size_t hash_size, const std::size_t osize) {
    if (hash_size > osize)
        return hash_size;
    if (hash_size % osize)
        return ((osize / hash_size) + 1) * hash_size;
    return osize;
}

template <typename I>
void hash_data(others_interface& hasher, const I& data, std::uint8_t* hash, const std::size_t hash_size) {
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

others_stream::others_stream(const json& config, default_seed_source &seeder, const std::size_t osize)
    : stream(osize)
    , _round(config.at("round"))
    , _hash_input_size(std::size_t(config.at("hash-bitsize")) / 8)
    , _source_size(config.at("source").find("size") == config.at("source").end()
                   ? _hash_input_size
                   : std::size_t(config.at("source").at("size")))
    , _source(make_stream(config.at("source"), seeder, _source_size))
    , _hasher(others_factory::create(config.at("algorithm"), unsigned(_round)))
    , _data(compute_hash_size(_hash_input_size, osize)) { // round osize to multiple of _hash_input_size
    logger::info() << "stream source is hash function: " << config.at("algorithm") << std::endl;

    if ((std::size_t(config.at("hash-bitsize")) % 8) != 0)
        throw std::runtime_error("the SHA-3 hash-bitsize parameter must be multiple of 8");
}

others_stream::others_stream(others_stream&&) = default;
others_stream::~others_stream() = default;

vec_cview others_stream::next() {
    auto data = _data.data();
    for (std::size_t i = 0; i < _data.size(); i += _hash_input_size) {
        vec_cview view = _source->next();

        hash_data(*_hasher, view, &data[i], _hash_input_size);
    }

    return make_view(_data.cbegin(), osize());
}

} // namespace others
