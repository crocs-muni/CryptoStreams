//
// Created by mhajas on 11/14/17.
//

#include <eacirc-core/random.h>
#include "prng_stream.h"
#include <streams/prngs/prng_factory.h>

namespace prng {

    vec_cview prng_stream::next() {
        _generator->generate_bits(_data.data(), _data.size());
        return make_cview(_data);
    }

    prng_stream::prng_stream(const json& config, default_seed_source& seeder, std::size_t osize, std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes)
            : stream(osize)
            , _generator(std::make_unique<prng_factory>(config, seeder, pipes))
            , _data(osize)
    {}
}
