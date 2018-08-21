//
// Created by mhajas on 11/14/17.
//

#pragma once

#include <memory>
namespace prng {

    class prng_interface {
    public:
        virtual void generate_bits(unsigned char* data, size_t number_of_bytes) = 0;
    };
}
