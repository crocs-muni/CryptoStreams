//
// Created by mhajas on 11/14/17.
//
#pragma once

#include <streams/prngs/prng_interface.h>
#include <utility>

extern "C" {
#include <streams/prngs/testu01-prngs/includes/unif01.h>
};

namespace prng {

    template<typename GENERATOR, typename DELETER, typename OUTPUT_TYPE>
    class testu01_interface : public prng_interface {
    protected:
        std::function<std::unique_ptr<GENERATOR, DELETER>(const value_type *)> _generator_creator;
        std::unique_ptr<stream> _seeder;
        vec_cview _seed;
        std::unique_ptr<GENERATOR, DELETER> _generator;
        bool _include_seed_for_each_test_vector; // in each test vector
        bool _reseed_for_each_test_vector;

    public:
        explicit testu01_interface(const std::function<std::unique_ptr<GENERATOR, DELETER>(const value_type *)> &creator,
                                   std::unique_ptr<stream> &seeder, bool reseed_for_each_test_vector, bool include_seed)
                : _generator_creator(std::move(creator))
                , _seeder(std::move(seeder))
                , _seed(_seeder->next())
                , _generator(_generator_creator(_seed.data()))
                , _include_seed_for_each_test_vector(include_seed)
                , _reseed_for_each_test_vector(reseed_for_each_test_vector) {}

        void generate_bits(std::uint8_t *data, size_t number_of_bytes) override {
            int current_index = 0;

            if (_include_seed_for_each_test_vector) {
                for (auto i : _seed) {
                    data[current_index++] = i;
                    number_of_bytes--;
                }
            }

            while (number_of_bytes > 0) {

                OUTPUT_TYPE generated_data = _generator->GetBits(_generator->param,
                                                              _generator->state); // Get new Bytes from generator

                for (auto i = 0; i < std::min<size_t>(number_of_bytes, sizeof(OUTPUT_TYPE)); i++) {
                    data[current_index++] = static_cast<std::uint8_t>(generated_data & 0xFF);
                    generated_data >>= 8;
                }

                number_of_bytes -= std::min<size_t>(number_of_bytes, sizeof(OUTPUT_TYPE));
            }

            if (_reseed_for_each_test_vector) {
                _seed = _seeder->next();
                _generator = _generator_creator(_seed.data());
            }
        }


        /**
         *
         * In most cases we need to avoid seeds which are greater or equal to modulo, then it is quite hard to detect
         * when this is violated and when not, therefore I decided not to generate seed for full 8 Bytes (long), but to
         * use only seeds which are guaranteed not fulfill requirements, even though we loose some possible seeds.
         *
         *
         * @tparam T
         * @param number to investigate
         * @return returns how many bytes from beginning are not zero
         */
        template<typename T>
        static size_t get_viable_number_of_bytes(T number) {
            auto ar = reinterpret_cast<uint8_t*>(&number);

            auto result = 0;

            for (int i = sizeof(T)-1; i >= 0; i--) {
                if (ar[i] == 0) {
                    result++;
                } else {
                    break;
                }
            }

            result++; // cut one more byte because it can contain zeroes too, and it also removes negative values

            result = sizeof(T) - result;

            if (result < 1) {
                throw std::runtime_error("Number doesn't contain any viable bytes");
            }

            return result;
        }
    };

    template <typename OUTPUT_TYPE>
    class uniform_generator_interface : public testu01_interface<unif01_Gen, void (*)(unif01_Gen *), OUTPUT_TYPE> {
    public:
        uniform_generator_interface(const std::function<std::unique_ptr<unif01_Gen, void (*)(unif01_Gen *)>(
                                            const value_type *)> &creator, std::unique_ptr<stream> &seeder, bool reseed_for_each_test_vector, bool include_seed)
                : testu01_interface<unif01_Gen, void (*)(unif01_Gen *), OUTPUT_TYPE>(creator, seeder, reseed_for_each_test_vector, include_seed) {}
    };
}

