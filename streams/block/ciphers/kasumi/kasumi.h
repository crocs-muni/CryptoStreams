/**
 * Source: https://botan.randombit.net/doxygen/dir_c85d3e3c5052e9ad9ce18c6863244a25.html
 */
 /**
 * KASUMI
 * (C) 1999-2007 Jack Lloyd
 *
 * Botan is released under the Simplified BSD License (see license.txt)
 */
 
 #ifndef BOTAN_KASUMI_H_
 #define BOTAN_KASUMI_H_

#include <vector>

 namespace Botan {
 
 /**
 * KASUMI, the block cipher used in 3G telephony
 */
 class KASUMI final
    {
    public:
       void encrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const ;
       void decrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const ;
 
       void clear();
       //std::string name() const override { return "KASUMI"; }
       //BlockCipher* clone() const override { return new KASUMI; }

       void key_schedule(const uint8_t[], size_t) ;

        std::vector<uint16_t> m_EK;
       //secure_vector<uint16_t> m_EK;
    };
 
 }
 
 #endif
