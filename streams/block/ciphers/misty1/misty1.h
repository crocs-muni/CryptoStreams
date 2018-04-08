/**
 * Source: https://botan.randombit.net/doxygen/dir_c85d3e3c5052e9ad9ce18c6863244a25.html
 */
/*
* MISTY1
* (C) 1999-2008 Jack Lloyd
*
* Botan is released under the Simplified BSD License (see license.txt)
*/

#ifndef BOTAN_MISTY1_H_
#define BOTAN_MISTY1_H_


#include <cstdint>
#include <cstdio>
#include <vector>

namespace Botan {

    /**
    * MISTY1 with 8 rounds
    */
    class MISTY1 final
{
    public:
    void encrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const ;
    void decrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const ;
/*
    void clear() override;
    std::string name() const override { return "MISTY1"; }
BlockCipher* clone() const override { return new MISTY1; }*/

void key_schedule(const uint8_t[], size_t);
    private:
std::vector<uint16_t> m_EK, m_DK;
};

}

#endif