#pragma once

#include <cstdint>

namespace block {

    std::uint32_t u8_to_u32_copy(const uint8_t* in) {
        return std::uint32_t((in[0] << 24) + (in[1] << 16) + (in[2] << 8) + in[3]);
    }

    void u32_to_u8_copy(uint8_t* out, const uint32_t in) {
        out[0] = uint8_t((in >> 24) & 0xFF);
        out[1] = uint8_t((in >> 16) & 0xFF);
        out[2] = uint8_t((in >> 8) & 0xFF);
        out[3] = uint8_t(in & 0xFF);
    }

}
