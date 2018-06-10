//
// Created by mhajas on 1/8/18.
//

#ifndef EACIRC_STREAMS_LCG_GENERATOR_H
#define EACIRC_STREAMS_LCG_GENERATOR_H

#include <streams.h>
#include <eacirc-core/json.h>
#include <streams/prngs/std-prngs/std_prng_interface.h>

namespace prng {
    
    class lcg_generator : public std_prng_interface<std::minstd_rand, uint32_t, uint32_t> {
    public:
        lcg_generator(const json &config, default_seed_source &seeder) : lcg_generator( // 7B seed and each output has 8B
                make_stream(config.at("seeder"), seeder, 4), config.value("reseed_for_each_test_vector", false), config.value("include_seed_in_output", false)) {}

        explicit lcg_generator(std::unique_ptr<stream> seeder, bool reseed, bool include_seed)
                : std_prng_interface(seeder, reseed, include_seed) {}
    };
}

#endif //EACIRC_STREAMS_LCG_GENERATOR_H
