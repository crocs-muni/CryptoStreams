//
// Created by mhajas on 1/20/18.
//

#ifndef EACIRC_STREAMS_UCARRY_GENERATOR_H
#define EACIRC_STREAMS_UCARRY_GENERATOR_H

#include <streams/prngs/testu01-prngs/testu01_interface.h>
#include <eacirc-core/json.h>

extern "C" {
#include "ucarry.h"
};

namespace prng {

    /**
     * Generators based on linear recurrences with carry (http://www.iro.umontreal.ca/~simardr/testu01/guideshorttestu01.pdf on page 31)
     *
     * MWC generator (Multiply with carry)
     *
     * TODO: formula
     *
     */
//    class ucarry_MWC_generator : public uniform_generator_interface {
//    public:
//        explicit ucarry_MWC_generator(const json &config, default_seed_source &seeder) : ucarry_MWC_generator(
//                config.at("seeder"), seeder, config.value("reseed_for_each_test_vector", false),
//                config.value("include_seed_in_output", false)) {}
//
//        ucarry_MWC_generator(const json &seeder_config, default_seed_source &seeder, bool reseed = false,
//                             bool include_seed = false, uint64_t w = 32,
//                             std::vector<uint64_t> a = {11138366, 11808124}, uint64_t c = 1)
//                : ucarry_MWC_generator(
//                make_stream(seeder_config, seeder, a.size() * get_viable_number_of_bytes(std::pow(2, w)))
//                , reseed, include_seed, w, a, c) {}
//
//        ucarry_MWC_generator(std::unique_ptr<stream> seeder, bool reseed, bool include_seed, uint64_t w,
//                             std::vector<uint64_t > &a, uint64_t c)
//                : uniform_generator_interface(
//                [m, a](const value_type *seed) mutable -> auto {
//                    std::vector<long> s(a.size());
//
//                    for (auto i = 0; i < s.size(); i++) {
//                        uint64_t value_of_seed = 0;
//                        for (auto j = 0; j < testu01_interface::get_viable_number_of_bytes(m); j++) {
//                            value_of_seed |=
//                                    static_cast<uint64_t>(seed[i * testu01_interface::get_viable_number_of_bytes(m) +
//                                                               j]) << (j * 8);
//                        }
//
//                        s[i] = value_of_seed;
//                    }
//
//                    return std::unique_ptr<unif01_Gen, void (*)(unif01_Gen *)>(
//                            ucarry_CreateMWC(m, static_cast<int>(a.size()), a.data(), s.data()), umrg_DeleteMRG);
//                }, seeder, reseed, include_seed, m
//        ) {}
//    };
}

#endif //EACIRC_STREAMS_UCARRY_GENERATOR_H
