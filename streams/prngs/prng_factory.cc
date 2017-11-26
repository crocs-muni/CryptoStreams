//
// Created by mhajas on 11/14/17.
//

#include "prng_factory.h"
#include <memory.h>
#include <streams/prngs/testu01-prngs/testu01_factory.h>
namespace prng {

    prng_factory::prng_factory(const json& config, default_seed_source& seeder)
            : _generator(create_prng_interface(config, seeder))
    {}

    void prng_factory::generate_bits(unsigned char *data, size_t number_of_bytes) {
        _generator->generate_bits(data, number_of_bytes);
    }
}
