 /*
 * SHACAL-2
 * (C) 2017 Jack Lloyd
 *
 * Botan is released under the Simplified BSD License (see license.txt)
 */
 
 #ifndef BOTAN_SHACAL2_H_
 #define BOTAN_SHACAL2_H_

 #include <vector>

 namespace Botan {
 
 /**
 * SHACAL2
 */
 class SHACAL2 final
    {
    public:
       void encrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const;
       void decrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const;
 /*
       std::string provider() const override;
       void clear() override;
       std::string name() const override { return "SHACAL2"; }
       BlockCipher* clone() const override { return new SHACAL2; }
       size_t parallelism() const override;*/
 

       void key_schedule(const uint8_t[], size_t);
 private:
 #if defined(BOTAN_HAS_SHACAL2_SIMD)
       void simd_encrypt_4(const uint8_t in[], uint8_t out[]) const;
       void simd_decrypt_4(const uint8_t in[], uint8_t out[]) const;
 #endif
 
 #if defined(BOTAN_HAS_SHACAL2_X86)
       void x86_encrypt_blocks(const uint8_t in[], uint8_t out[], size_t blocks) const;
 #endif
 
       std::vector<uint32_t> m_RK;
    };
 
 }
 
 #endif
