#pragma once

#include <cstdint>

#if defined(__APPLE__) || defined(_WIN32)
#define __BIG_ENDIAN 0x1000
#define __LITTLE_ENDIAN 0x0001
#define __BYTE_ORDER __LITTLE_ENDIAN
#else
#include <endian.h>
#endif

namespace block {

    std::uint32_t u8_to_u32_copy(const uint8_t* in) {
#if __BYTE_ORDER == __BIG_ENDIAN
        return std::uint32_t((in[0] << 24) + (in[1] << 16) + (in[2] << 8) + in[3]);
#elif __BYTE_ORDER == __LITTLE_ENDIAN
        return std::uint32_t((in[3] << 24) + (in[2] << 16) + (in[1] << 8) + in[0]);
#endif
    }

    void u32_to_u8_copy(uint8_t* out, const uint32_t in) {
#if __BYTE_ORDER == __BIG_ENDIAN
        out[0] = uint8_t((in >> 24) & 0xFF);
        out[1] = uint8_t((in >> 16) & 0xFF);
        out[2] = uint8_t((in >> 8) & 0xFF);
        out[3] = uint8_t(in & 0xFF);
#elif __BYTE_ORDER == __LITTLE_ENDIAN
        out[3] = uint8_t((in >> 24) & 0xFF);
        out[2] = uint8_t((in >> 16) & 0xFF);
        out[1] = uint8_t((in >> 8) & 0xFF);
        out[0] = uint8_t(in & 0xFF);
#endif
    }
}
