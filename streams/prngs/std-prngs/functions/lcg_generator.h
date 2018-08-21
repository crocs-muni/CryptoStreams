//
// Created by mhajas on 1/8/18.
//

#pragma once

#include <streams.h>
#include <eacirc-core/json.h>
#include <streams/prngs/std-prngs/std_prng_interface.h>

namespace prng {
    
    class lcg_generator : public std_prng_interface<std::minstd_rand, uint32_t, uint32_t> {
    public:
        lcg_generator(const json &config, default_seed_source &seeder, std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes) : lcg_generator(
                make_stream(config.at("seeder"), seeder, pipes, 4), config.value("reseed_for_each_test_vector", false)) {}

        explicit lcg_generator(std::unique_ptr<stream> seeder, bool reseed)
                : std_prng_interface(seeder, reseed) {}
    };
}
