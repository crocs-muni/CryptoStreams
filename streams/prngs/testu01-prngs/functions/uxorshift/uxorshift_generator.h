//
// Created by mhajas on 5/29/18.
//

#pragma once

#include <vector>
#include <streams/prngs/testu01-prngs/testu01_interface.h>
#include <eacirc-core/json.h>
#include <stream.h>
#include <eacirc-core/random.h>
#include <streams.h>

extern "C" {
#include "uxorshift.h"
};

namespace prng {

    /**
     * TODO docs
     */
    class uxorshift_generator : public uniform_generator_interface<4> {

    public:
        explicit uxorshift_generator(const json& config, default_seed_source &seeder, bool reseed = false, bool include_seed = false)
                : uxorshift_generator(make_stream(config.at("seeder"), seeder, 8 * sizeof(uint32_t)), config.value("reseed_for_each_test_vector", false), config.value("include_seed_in_output", false)) {}

        explicit uxorshift_generator(std::unique_ptr<stream> seeder, bool reseed, bool include_seed)
                : uniform_generator_interface(
                [](const value_type *seed) {
                    std::vector<uint32_t> s(8);

                    for (auto i = 0; i < s.size(); i++) {
                        uint32_t value_of_seed = 0;
                        for (auto j = 0; j < sizeof(uint32_t); j++) {
                            value_of_seed |= static_cast<uint64_t>(seed[i * sizeof(uint32_t) + j]) <<  (j*8);
                        }

                        s[i] = value_of_seed;
                    }

                    return std::unique_ptr<unif01_Gen, void (*)(unif01_Gen *)>(uxorshift_CreateXorshift13(s.data()), uxorshift_DeleteGen);
                }
                , seeder, reseed, include_seed) {}
    };
}
