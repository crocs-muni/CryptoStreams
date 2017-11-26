//
// Created by mhajas on 11/14/17.
//

#ifndef EACIRC_STREAMS_GENERATOR_H
#define EACIRC_STREAMS_GENERATOR_H

#include <eacirc-core/json.h>
#include <eacirc-core/random.h>
#include "prng_interface.h"


namespace prng {

    std::unique_ptr<prng_interface>
    create_testu01_interface(const json& configuration, default_seed_source& seeder);

    class prng_factory {

    public:
        prng_factory(const json& configuration, default_seed_source& seeder);
        void generate_bits(unsigned char* data, size_t number_of_bytes);

    private:
        std::unique_ptr<prng_interface> create_prng_interface(const json& configuration, default_seed_source& seeder) {
            if(configuration.at("algorithm").get<std::string>().find("testu01") == 0) {
                return create_testu01_interface(configuration, seeder);
            }
        }

        std::unique_ptr<prng_interface> _generator;
    };
}


#endif //EACIRC_STREAMS_GENERATOR_H
