//
// Created by mhajas on 6/10/18.
//

#include <eacirc-core/random.h>
#include <eacirc-core/json.h>
#include <streams/prngs/prng_interface.h>
#include <streams/prngs/std-prngs/functions/lcg_generator.h>
#include <streams/prngs/std-prngs/functions/subtract_with_carry_generator.h>
#include <streams/prngs/std-prngs/functions/mesenne_twister_generator.h>

namespace prng {

    std::unique_ptr<prng_interface>
    create_std_prng_interface(const json& configuration, default_seed_source& seeder, std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes) {
        std::string name = configuration.at("algorithm");

        if(name == "std_lcg") return std::make_unique<lcg_generator>(configuration, seeder, pipes);
        if(name == "std_subtract_with_carry") return std::make_unique<subtract_with_carry_generator>(configuration, seeder, pipes);
        if(name == "std_mersenne_twister") return std::make_unique<mersenne_twister_generator>(configuration, seeder, pipes);

        throw std::runtime_error("requested std prng named \"" + name +
                                 "\" is either broken or does not exists");

    }

}