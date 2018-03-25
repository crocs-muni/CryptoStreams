#include "sha3_stream.h"
#include "streams.h"
#include "sha3_factory.h"
#include "sha3_interface.h"
#include <algorithm>

namespace sha3 {

template <typename I>
void hash_data(sha3_interface& hasher, const I& data, std::uint8_t* hash, const std::size_t hash_size) {
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

sha3_stream::sha3_stream(const json& config, default_seed_source &seeder, const std::size_t osize, core::optional<stream *> plt_stream)
    : stream(osize) // round osize to multiple of _hash_input_size
    , _round(config.at("round"))
    , _hash_size(std::size_t(config.at("hash-size")))
    , _source(make_stream(config.at("source"), seeder, config.value("input-size", _hash_size))) // if input size is not defined, use hash-size
    , _prepared_stream_source(!plt_stream ? nullptr : *plt_stream)
    , _hasher(sha3_factory::create(config.at("algorithm"), unsigned(_round))) {
    if (osize % _hash_size != 0) // not necessary wrong, but we never needed this, we always did this by mistake. Change to warning if needed
        throw std::runtime_error("Output size is not multiple of hash size");
    logger::info() << "stream source is sha3 function: " << config.at("algorithm") << std::endl;

}

sha3_stream::sha3_stream(sha3_stream&&) = default;
sha3_stream::~sha3_stream() = default;

vec_cview sha3_stream::next() {
    auto hash = _data.data();
    for (std::size_t i = 0; i < _data.size(); i += _hash_size) {
        vec_cview view = get_next_ptx();

        hash_data(*_hasher, view, &hash[i], _hash_size);
    }

    return make_view(_data.cbegin(), osize());
}

vec_cview sha3_stream::get_next_ptx()
{
    if (_prepared_stream_source) {
        return _prepared_stream_source->next();
    } else {
        return _source->next();
    }
}

} // namespace sha3
