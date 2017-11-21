#include "block_stream.h"
#include "streams.h"
#include "block_factory.h"
#include "block_cipher.h"

namespace block {

    static std::size_t compute_vector_size(const std::size_t block_size, const std::size_t osize) {
        if (block_size > osize)
            return block_size;
        if (block_size % osize)
            return ((osize / block_size) + 1) * block_size;
        return osize;
    }

    block_stream::block_stream(const json& config, default_seed_source& seeder, std::size_t osize)
        : stream(osize)
        , _round(config.at("round"))
        , _block_size(config.at("block-size"))
        , _source(make_stream(config.at("plaintext"), seeder, _block_size))
        , _iv(make_stream(config.at("iv"), seeder, _block_size))
        , _key(make_stream(config.at("key"), seeder, unsigned(config.at("key-size"))))
        , _encryptor(make_block_cipher(config.at("algorithm"), unsigned(_round), unsigned(_block_size), true))
        , _data(compute_vector_size(_block_size, osize))
    {
        logger::info() << "stream source is block cipher: " << config.at("algorithm") << std::endl;

        if (int(config.at("round")) < 0)
            throw std::runtime_error("The least number of rounds is 0.");
        if (int(config.at("block-size")) < 8)
            throw std::runtime_error("The block size is at least 8 bytes");
        if (osize == 0)
            throw std::runtime_error("The output size has to be at least 1 byte");

        /* others modes than ECB are not implemented yet
        vec_view iv_view = _iv->next();
        _encryptor->ivsetup(iv_view.data(), iv_view.size());
        */

        vec_view key_view = _key->next();
        _encryptor->keysetup(key_view.data(), std::uint32_t(key_view.size()));
    }

    block_stream::block_stream(block_stream&&) = default;
    block_stream::~block_stream() = default;

    vec_view block_stream::next() {

        // TODO: reinit key for every vector: does it make sense?
        // if (_b_reinit_every_tv) {
        //    vec_view key_view = _key->next();
        //    _encryptor->ECRYPT_keysetup(key_view.data(), 8 * _block_size, 8 * _block_size);
        // }

        for (auto beg = _data.begin(); beg != _data.end(); beg += _block_size) {
            vec_view view = _source->next();

            _encryptor->encrypt(view.data(), &(*beg));
        }

        return make_view(_data.cbegin(), osize());
    }

}
