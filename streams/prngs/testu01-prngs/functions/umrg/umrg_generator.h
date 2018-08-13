//
// Created by mhajas on 1/19/18.
//

#ifndef EACIRC_STREAMS_UMRG_GENERATOR_H // TODO: use #pragma once
#define EACIRC_STREAMS_UMRG_GENERATOR_H

#include <streams.h>
#include <streams/prngs/testu01-prngs/testu01_interface.h>
#include <eacirc-core/json.h>

extern "C" {
#include "umrg.h"
};

namespace prng {

    /**
     * Multiple recursive generator MRG (http://www.iro.umontreal.ca/~simardr/testu01/guideshorttestu01.pdf on page 28)
     *
     * x_n = (a_1 * x_(n−1) + · · · + a_k * x_(n−k)) mod m.
     *
     * s : initial state
     *
     * source of parameters: https://dl.acm.org/citation.cfm?id=169698
     */
    class umrg_generator : public uniform_generator_interface<7> {
    public:
        explicit umrg_generator(const json& config, default_seed_source &seeder, std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes,
                bool reseed = false, uint64_t m = 9223372036854775783, std::vector<long> a = {2975962250, 2909704450})
                : umrg_generator(make_stream(config.at("seeder"), seeder, pipes, a.size() * get_viable_number_of_bytes(m)), config.value("reseed_for_each_test_vector", false), m, a) {}

        explicit umrg_generator(std::unique_ptr<stream> seeder, bool reseed, uint64_t m, std::vector<long>& a)
                : uniform_generator_interface(
                [m, a](const value_type *seed) mutable -> auto {
                    std::vector<long> s(a.size());

                    for (auto i = 0; i < s.size(); i++) {
                        uint64_t value_of_seed = 0;
                        for (auto j = 0; j < testu01_interface::get_viable_number_of_bytes(m); j++) {
                            value_of_seed |= static_cast<uint64_t>(seed[i * testu01_interface::get_viable_number_of_bytes(m) + j]) <<  (j*8);
                        }

                        s[i] = value_of_seed;
                    }

                    return std::unique_ptr<unif01_Gen, void (*)(unif01_Gen *)>(umrg_CreateMRG(m, static_cast<int>(a.size()), a.data(), s.data()), umrg_DeleteMRG);
                }
                , seeder, reseed
        ) {}

        stream* get_seeder_stream() {
            return _seeder.get();
        }
    };
}

#endif //EACIRC_STREAMS_UMRG_GENERATOR_H
