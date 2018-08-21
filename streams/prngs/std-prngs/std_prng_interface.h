//
// Created by mhajas on 11/14/17.
//
#pragma once

#include <streams/prngs/prng_interface.h>
#include <utility>
#include <stream.h>
#include <random>

namespace prng {

    template <typename Generator, typename SeedType, typename OutputType>
    class std_prng_interface : public prng_interface {
    protected:
        std::unique_ptr<stream> _seeder;
        vec_cview _seed;
        Generator _generator;
        bool _reseed_for_each_test_vector;

    public:
        explicit std_prng_interface(std::unique_ptr<stream> &seeder, bool reseed_for_each_test_vector)
                : _seeder(std::move(seeder))
                , _seed(_seeder->next())
                , _generator(*reinterpret_cast<const SeedType*>(_seed.data()))
                , _reseed_for_each_test_vector(reseed_for_each_test_vector) {}

        void generate_bits(std::uint8_t *data, size_t number_of_bytes) override {
            size_t current_index = 0;

            while (number_of_bytes > 0) {

                OutputType generated_data = _generator(); // Get new Bytes from generator

                for (auto i = 0; i < std::min<size_t>(number_of_bytes, sizeof(OutputType)); i++) {
                    data[current_index++] = static_cast<std::uint8_t>(generated_data & 0xFF);
                    generated_data >>= 8;
                }

                number_of_bytes -= std::min<size_t>(number_of_bytes, sizeof(OutputType));
            }

            if (_reseed_for_each_test_vector) {
                _seed = _seeder->next();
                _generator.seed(*reinterpret_cast<const SeedType*>(_seed.data()));
            }
        }
    };
}

