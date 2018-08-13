//
// Created by mhajas on 11/14/17.
//

#ifndef EACIRC_STREAMS_GENERATOR_H
#define EACIRC_STREAMS_GENERATOR_H

#include <eacirc-core/json.h>
#include <eacirc-core/random.h>
#include <unordered_map>
#include "prng_interface.h"
#include <stream.h>


namespace prng {

    std::unique_ptr<prng_interface>
    create_testu01_interface(const json& configuration, default_seed_source& seeder, std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes);
    std::unique_ptr<prng_interface>
    create_std_prng_interface(const json& configuration, default_seed_source& seeder, std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes);

    class prng_factory {

    public:
        prng_factory(const json& configuration, default_seed_source& seeder, std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes);
        void generate_bits(unsigned char* data, size_t number_of_bytes);

    private:
        std::unique_ptr<prng_interface> create_prng_interface(const json& configuration, default_seed_source& seeder, std::unordered_map<std::string, std::shared_ptr<std::unique_ptr<stream>>> &pipes) {
            if(configuration.at("algorithm").get<std::string>().find("testu01") == 0) {
                return create_testu01_interface(configuration, seeder, pipes);
            } else if (configuration.at("algorithm").get<std::string>().find("std") == 0) {
                return create_std_prng_interface(configuration, seeder, pipes);
            }

            throw std::runtime_error("requested prng named \"" + configuration.at("algorithm").get<std::string>() +
                                     "\" is either broken or does not exists");
        }

        std::unique_ptr<prng_interface> _generator;
    };
}


#endif //EACIRC_STREAMS_GENERATOR_H
