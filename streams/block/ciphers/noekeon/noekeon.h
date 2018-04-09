/**
 * Source: https://github.com/randombit/botan/tree/release-2/src/lib/block/noekeon
 */
 /*
 * Noekeon
 * (C) 1999-2008 Jack Lloyd
 *
 * Botan is released under the Simplified BSD License (see license.txt)
 */
 
 #ifndef BOTAN_NOEKEON_H_
 #define BOTAN_NOEKEON_H_

#include <cstdio>
#include <cstdint>
#include <vector>

 namespace Botan {
 
 /**
 * Noekeon
 */
 class Noekeon final
    {
    public:
       void encrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const ;
       void decrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const ;
 
       /*std::string provider() const override;
       void clear() override;
       std::string name() const override { return "Noekeon"; }
       BlockCipher* clone() const override { return new Noekeon; }
       size_t parallelism() const override;*/

     void key_schedule(const uint8_t[], size_t) ;
    private:
 #if defined(BOTAN_HAS_NOEKEON_SIMD)
       void simd_encrypt_4(const uint8_t in[], uint8_t out[]) const;
       void simd_decrypt_4(const uint8_t in[], uint8_t out[]) const;
 #endif
 
       /**
       * The Noekeon round constants
       */
       static const uint8_t RC[17];

       std::vector<uint32_t> m_EK, m_DK;
    };
 
 }
 
 #endif
