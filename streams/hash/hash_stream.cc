#include "hash_stream.h"
#include "streams.h"
#include "hash_interface.h"
#include "hash_factory.h"
#include <algorithm>

namespace hash {


template <typename I>
void hash_data(hash_interface& hasher, const I& data, std::uint8_t* hash, const std::size_t hash_size) {
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

hash_stream::hash_stream(const json& config, default_seed_source &seeder, const std::size_t osize, core::optional<stream *> plt_stream)
    : stream(compute_hash_size(std::size_t(config.at("hash-bitsize")) / 8, osize)) // round osize to multiple of _hash_input_size
    , _round(config.at("round"))
    , _hash_input_size(std::size_t(config.at("hash-bitsize")) / 8)
    , _source_size(config.at("source").find("size") == config.at("source").end()
                   ? _hash_input_size
                   : std::size_t(config.at("source").at("size")))

    , _source(make_stream(config.at("source"), seeder, _source_size))
    , _prepared_stream_source(!plt_stream ? nullptr : *plt_stream)
    , _hasher(hash_factory::create(config.at("algorithm"), unsigned(_round)))
    , _data(compute_hash_size(_hash_input_size, osize)) { // round osize to multiple of _hash_input_size
    logger::info() << "stream source is hash function: " << config.at("algorithm") << std::endl;

    if ((std::size_t(config.at("hash-bitsize")) % 8) != 0)
        throw std::runtime_error("the hash-bitsize parameter must be multiple of 8");
}

hash_stream::hash_stream(hash_stream&&) = default;
hash_stream::~hash_stream() = default;

vec_cview hash_stream::next() {
    auto data = _data.data();
    for (std::size_t i = 0; i < _data.size(); i += _hash_input_size) {
        vec_cview view = get_next_ptx();

        hash_data(*_hasher, view, &hash[i], _hash_size);
    }
    
        return make_view(_data.cbegin(), osize());
}

vec_cview hash_stream::get_next_ptx()
{
    if (_prepared_stream_source) {
        return _prepared_stream_source->next();
    } else {
        return _source->next();
    }
}

} // namespace hash
