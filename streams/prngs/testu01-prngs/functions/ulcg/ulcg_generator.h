//
// Created by mhajas on 1/8/18.
//

#pragma once

#include <streams.h>
#include <streams/prngs/testu01-prngs/testu01_interface.h>
#include <eacirc-core/json.h>

extern "C" {
#include "ulcg.h"
};

namespace prng {

    /**
     * Linear congruential generator LCG (http://www.iro.umontreal.ca/~simardr/testu01/guideshorttestu01.pdf on page 23)
     *
     * x_i = [a * x_(i - 1) + c] mod m
     *
     * s : initial state
     */
    class ulcg_generator : public uniform_generator_interface<7> {
    public:
        ulcg_generator(const json &config, default_seed_source &seeder, std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes, int64_t m = 9223372036854775783, int64_t a = 4645906587823291368, int64_t c = 0) : ulcg_generator( // 7B seed and each output has 8B
                make_stream(config.at("seeder"), seeder, pipes, get_viable_number_of_bytes(m)), config.value("reseed_for_each_test_vector", false), m, a, c) {}

        explicit ulcg_generator(std::unique_ptr<stream> seeder, bool reseed, int64_t m, int64_t a, int64_t c)
                : uniform_generator_interface(
                [m, a, c](const value_type *seed) {

                    int64_t s = 0;

                    for (int64_t i = 0; i < get_viable_number_of_bytes(m); i++) {
                        s |= static_cast<int64_t>(seed[i]) << (8 * i);
                    }

                    return std::unique_ptr<unif01_Gen, void (*)(unif01_Gen *)>(ulcg_CreateLCG(m, a, c, s),
                                                                               ulcg_DeleteGen);
                }

                , seeder, reseed
        ) {}
    };
}
