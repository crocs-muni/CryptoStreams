/**
 * Source: https://github.com/randombit/botan/tree/release-2/src/lib/block/xtea
 */
 /*
 * XTEA
 * (C) 1999-2007 Jack Lloyd
 *
 * Botan is released under the Simplified BSD License (see license.txt)
 */
 
 #ifndef BOTAN_XTEA_H_
 #define BOTAN_XTEA_H_


#include <cstdint>
#include <vector>
#include <cstdio>

 namespace Botan {
 
 /**
 * XTEA
 */
 class XTEA final
    {
    public:
       void encrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const;
       void decrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const;
 /*
       void clear() override;
       std::string name() const override { return "XTEA"; }
       BlockCipher* clone() const override { return new XTEA; }*/


       void key_schedule(const uint8_t[], size_t);

    private:
       std::vector<uint32_t > m_EK;
       //secure_vector<uint32_t> m_EK;
    };
 
 }
 
 #endif

