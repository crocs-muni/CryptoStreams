#include "block_stream.h"
#include "streams.h"
#include "block_factory.h"
#include "block_cipher.h"
#include <eacirc-core/json.h>

namespace block {

    static int64_t reinit_freq(const json& config) {
        try {
            std::string init_freq = config.at("init-frequency");
            if (init_freq == "only-once") {
                return -1;
            } else {
                int64_t init_freq_int = std::stoll(init_freq);
                if (init_freq_int < 1) {
                    throw std::runtime_error("Reinitialization frequency has to be higher or equal to 1.");
                }
                return init_freq_int;
            }
        } catch (std::out_of_range& e) {
            // this field is voluntary, we return presumed value "only-once"
            return -1;
        }
    }

    block_stream::block_stream(const json& config, default_seed_source& seeder, const std::size_t osize, core::optional<stream *> plt_stream)
        : stream(osize)
        , _round(config.at("round"))
        , _block_size(config.at("block-size"))
        , _reinit_freq(reinit_freq(config))
        , _i(0)
        , _source(make_stream(config.at("plaintext"), seeder, _block_size))
        , _iv(make_stream(config.at("iv"), seeder, _block_size))
        , _key(make_stream(config.at("key"), seeder, unsigned(config.at("key-size"))))
        , _encryptor(make_block_cipher(config.at("algorithm"), unsigned(_round),
                                       unsigned(_block_size), unsigned(config.at("key-size")), true))
        , _prepared_stream_source(!plt_stream ? nullptr : *plt_stream)
    {
        logger::info() << "stream source is block cipher: " << config.at("algorithm") << std::endl;

        if (int(config.at("round")) < 0)
            throw std::runtime_error("The least number of rounds is 0.");
        if (_block_size < 4)
            throw std::runtime_error("The block size is at least 4 bytes");
        if (osize == 0)
            throw std::runtime_error("The output size has to be at least 1 byte");
        if (osize % _block_size != 0) // not necessary wrong, but we never needed this, we always did this by mistake. Change to warning if needed
            throw std::runtime_error("Output size is not multiple of block size");

        /* others modes than ECB are not implemented yet
        vec_view iv_view = _iv->next();
        _encryptor->ivsetup(iv_view.data(), iv_view.size());
        */

        vec_cview key_view = _key->next();
        _encryptor->keysetup(key_view.data(), std::uint32_t(key_view.size()));
    }

    block_stream::block_stream(block_stream&&) = default;
    block_stream::~block_stream() = default;

    vec_cview block_stream::next() {
        ++_i;
        if (_reinit_freq != -1 && _i % std::size_t(_reinit_freq) == 0) {
            vec_cview key_view = _key->next();
            _encryptor->keysetup(key_view.data(), std::uint32_t(key_view.size()));
        }

        for (auto beg = _data.begin(); beg != _data.end(); beg += _block_size) {
            vec_cview view = get_next_ptx();

            _encryptor->encrypt(view.data(), &(*beg));
        }

        return make_view(_data.cbegin(), osize());
    }

    vec_cview block_stream::get_next_ptx()
    {
        if (_prepared_stream_source) {
            return _prepared_stream_source->next();
        } else {
            return _source->next();
        }
    }

}
