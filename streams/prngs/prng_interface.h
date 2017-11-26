//
// Created by mhajas on 11/14/17.
//

#ifndef EACIRC_STREAMS_PRNG_INTERFACE_H
#define EACIRC_STREAMS_PRNG_INTERFACE_H

#include <memory>
namespace prng {

    class prng_interface {
    public:
        virtual void generate_bits(unsigned char* data, size_t number_of_bytes) = 0;
    };
}
#endif //EACIRC_STREAMS_PRNG_INTERFACE_H
