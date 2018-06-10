//
// Created by mhajas on 11/14/17.
//
#include <memory>
#include <streams/prngs/prng_interface.h>
#include <eacirc-core/json.h>
#include <memory.h>
//#include <streams/prngs/testu01-prngs/functions/unif01/dummy_generator.h>
#include <streams/prngs/testu01-prngs/functions/ulcg/ulcg_generator.h>
#include <streams/prngs/testu01-prngs/functions/umrg/umrg_generator.h>
#include <streams/prngs/testu01-prngs/functions/uxorshift/uxorshift_generator.h>

namespace prng {


    std::unique_ptr<prng_interface>
    create_testu01_interface(const json& configuration, default_seed_source& seeder) {
        std::string name = configuration.at("algorithm");

        if(name == "testu01-ulcg") return std::make_unique<ulcg_generator>(configuration, seeder);
        if(name == "testu01-umrg") return std::make_unique<umrg_generator>(configuration, seeder);
        if(name == "testu01-uxorshift") return std::make_unique<uxorshift_generator>(configuration, seeder);

        throw std::runtime_error("requested testu01 prng named \"" + name +
                                 "\" is either broken or does not exists");
    }

}
