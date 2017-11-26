//
// Created by mhajas on 11/14/17.
//


/**  Change to factory only , use uint8_t, b**/

#ifndef EACIRC_STREAMS_PRNG_STREAM_H
#define EACIRC_STREAMS_PRNG_STREAM_H


#include <stream.h>
#include "prng_factory.h"

namespace prng {

    class prng_stream : public stream {
    public:
        prng_stream(const json& config, default_seed_source& seeder, std::size_t osize);

        vec_cview next() override;

    private:
        std::unique_ptr<prng_factory> _generator;
        std::vector<uint8_t> _data;
    };
}


#endif //EACIRC_STREAMS_PRNG_STREAM_H
