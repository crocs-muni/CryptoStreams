/*
 * SHACAL-2
 * (C) 2017 Jack Lloyd
 *
 * Botan is released under the Simplified BSD License (see license.txt)
 */

#include <cstdint>
#include <cstdio>
#include <cstring>
#include "shacal2.h"

const int BLOCK_SIZE = 256;

/**
* Zero out some bytes
* @param ptr a pointer to memory to zero
* @param bytes the number of bytes to zero in ptr
*/
inline void clear_bytes(void* ptr, size_t bytes)
{
   if(bytes > 0)
   {
      std::memset(ptr, 0, bytes);
   }
}

/**
* Zero memory before use. This simply calls memset and should not be
* used in cases where the compiler cannot see the call as a
* side-effecting operation (for example, if calling clear_mem before
* deallocating memory, the compiler would be allowed to omit the call
* to memset entirely under the as-if rule.)
*
* @param ptr a pointer to an array of Ts to zero
* @param n the number of Ts pointed to by ptr
*/
template<typename T> inline void clear_mem(T* ptr, size_t n)
{
   clear_bytes(ptr, sizeof(T)*n);
}

/**
* Bit rotation right by a compile-time constant amount
* @param input the input word
* @return input rotated right by ROT bits
*/
template<size_t ROT, typename T>
inline T rotr(T input)
{
   static_assert(ROT > 0 && ROT < 8*sizeof(T), "Invalid rotation constant");
   return static_cast<T>((input >> ROT) | (input << (8*sizeof(T) - ROT)));
}

/**
* Load a big-endian word
* @param in a pointer to some bytes
* @param off an offset into the array
* @return off'th T of in, as a big-endian value
*/
template<typename T>
inline T load_be(const uint8_t in[], size_t off)
{
   in += off * sizeof(T);
   T out = 0;
   for(size_t i = 0; i != sizeof(T); ++i)
      out = static_cast<T>((out << 8) | in[i]);
   return out;
}

/**
* Load a variable number of big-endian words
* @param out the output array of words
* @param in the input array of bytes
* @param count how many words are in in
*/
template<typename T>
inline void load_be(T out[],
                    const uint8_t in[],
                    size_t count)
{
   if(count > 0)
   {
#if defined(BOTAN_TARGET_CPU_IS_BIG_ENDIAN)
      std::memcpy(out, in, sizeof(T)*count);

#elif defined(BOTAN_TARGET_CPU_IS_LITTLE_ENDIAN)
      std::memcpy(out, in, sizeof(T)*count);
       const size_t blocks = count - (count % 4);
       const size_t left = count - blocks;

       for(size_t i = 0; i != blocks; i += 4)
          bswap_4(out + i);

       for(size_t i = 0; i != left; ++i)
          out[blocks+i] = reverse_bytes(out[blocks+i]);
#else
      for(size_t i = 0; i != count; ++i)
         out[i] = load_be<T>(in, i);
#endif
   }
}

/**
* Byte extraction
* @param byte_num which byte to extract, 0 == highest byte
* @param input the value to extract from
* @return byte byte_num of input
*/
template<typename T> inline uint8_t get_byte(size_t byte_num, T input)
{
   return static_cast<uint8_t>(
           input >> (((~byte_num)&(sizeof(T)-1)) << 3)
   );
}

/**
* Store a big-endian uint32_t
* @param in the input uint32_t
* @param out the byte array to write to
*/
inline void store_be(uint32_t in, uint8_t out[4])
{
#if defined(BOTAN_ENDIAN_B2N)
   uint32_t o = BOTAN_ENDIAN_B2N(in);
    std::memcpy(out, &o, sizeof(o));
#else
   out[0] = get_byte(0, in);
   out[1] = get_byte(1, in);
   out[2] = get_byte(2, in);
   out[3] = get_byte(3, in);
#endif
}

/**
* Store eight big-endian words
* @param out the output byte array
* @param x0 the first word
* @param x1 the second word
* @param x2 the third word
* @param x3 the fourth word
* @param x4 the fifth word
* @param x5 the sixth word
* @param x6 the seventh word
* @param x7 the eighth word
*/
template<typename T>
inline void store_be(uint8_t out[], T x0, T x1, T x2, T x3,
                     T x4, T x5, T x6, T x7)
{
   store_be(x0, out + (0 * sizeof(T)));
   store_be(x1, out + (1 * sizeof(T)));
   store_be(x2, out + (2 * sizeof(T)));
   store_be(x3, out + (3 * sizeof(T)));
   store_be(x4, out + (4 * sizeof(T)));
   store_be(x5, out + (5 * sizeof(T)));
   store_be(x6, out + (6 * sizeof(T)));
   store_be(x7, out + (7 * sizeof(T)));
}

 namespace Botan {
 
 namespace {
 
 inline void SHACAL2_Fwd(uint32_t A, uint32_t B, uint32_t C, uint32_t& D,
                         uint32_t E, uint32_t F, uint32_t G, uint32_t& H,
                         uint32_t RK)
    {
    const uint32_t A_rho = rotr<2>(A) ^ rotr<13>(A) ^ rotr<22>(A);
    const uint32_t E_rho = rotr<6>(E) ^ rotr<11>(E) ^ rotr<25>(E);
 
    H += E_rho + ((E & F) ^ (~E & G)) + RK;
    D += H;
    H += A_rho + ((A & B) | ((A | B) & C));
    }
 
 inline void SHACAL2_Rev(uint32_t A, uint32_t B, uint32_t C, uint32_t& D,
                         uint32_t E, uint32_t F, uint32_t G, uint32_t& H,
                         uint32_t RK)
    {
    const uint32_t A_rho = rotr<2>(A) ^ rotr<13>(A) ^ rotr<22>(A);
    const uint32_t E_rho = rotr<6>(E) ^ rotr<11>(E) ^ rotr<25>(E);
 
    H -= A_rho + ((A & B) | ((A | B) & C));
    D -= H;
    H -= E_rho + ((E & F) ^ (~E & G)) + RK;
    }
 
 }
 
 /*
 * SHACAL2 Encryption
 */
 void SHACAL2::encrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const
    {
    //verify_key_set(m_RK.empty() == false);
 
 #if defined(BOTAN_HAS_SHACAL2_X86)
    if(CPUID::has_intel_sha())
       {
       return x86_encrypt_blocks(in, out, blocks);
       }
 #endif
 
 #if defined(BOTAN_HAS_SHACAL2_SIMD)
    if(CPUID::has_simd_32())
       {
       while(blocks >= 4)
          {
          simd_encrypt_4(in, out);
          in += 4*BLOCK_SIZE;
          out += 4*BLOCK_SIZE;
          blocks -= 4;
          }
       }
 #endif
 
    for(size_t i = 0; i != blocks; ++i)
       {
       uint32_t A = load_be<uint32_t>(in, 0);
       uint32_t B = load_be<uint32_t>(in, 1);
       uint32_t C = load_be<uint32_t>(in, 2);
       uint32_t D = load_be<uint32_t>(in, 3);
       uint32_t E = load_be<uint32_t>(in, 4);
       uint32_t F = load_be<uint32_t>(in, 5);
       uint32_t G = load_be<uint32_t>(in, 6);
       uint32_t H = load_be<uint32_t>(in, 7);
 
       for(size_t r = 0; r != rounds; r += 1)
          {
              if(r%8 == 0)
                SHACAL2_Fwd(A, B, C, D, E, F, G, H, m_RK[r]);

              if(r%8 == 1)
                SHACAL2_Fwd(H, A, B, C, D, E, F, G, m_RK[r]);

              if(r%8 == 2)
                SHACAL2_Fwd(G, H, A, B, C, D, E, F, m_RK[r]);

              if(r%8 == 3)
                SHACAL2_Fwd(F, G, H, A, B, C, D, E, m_RK[r]);

              if(r%8 == 4)
                SHACAL2_Fwd(E, F, G, H, A, B, C, D, m_RK[r]);

              if(r%8 == 5)
                SHACAL2_Fwd(D, E, F, G, H, A, B, C, m_RK[r]);

              if(r%8 == 6)
                SHACAL2_Fwd(C, D, E, F, G, H, A, B, m_RK[r]);

              if(r%8 == 7)
                SHACAL2_Fwd(B, C, D, E, F, G, H, A, m_RK[r]);
          }
 
       store_be(out, A, B, C, D, E, F, G, H);
 
       in += BLOCK_SIZE;
       out += BLOCK_SIZE;
       }
    }
 
 /*
 * SHACAL2 Encryption
 */
 void SHACAL2::decrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const
    {
    //verify_key_set(m_RK.empty() == false);
 
 #if defined(BOTAN_HAS_SHACAL2_SIMD)
    if(CPUID::has_simd_32())
       {
       while(blocks >= 4)
          {
          simd_decrypt_4(in, out);
          in += 4*BLOCK_SIZE;
          out += 4*BLOCK_SIZE;
          blocks -= 4;
          }
       }
 #endif
 
    for(size_t i = 0; i != blocks; ++i)
       {
       uint32_t A = load_be<uint32_t>(in, 0);
       uint32_t B = load_be<uint32_t>(in, 1);
       uint32_t C = load_be<uint32_t>(in, 2);
       uint32_t D = load_be<uint32_t>(in, 3);
       uint32_t E = load_be<uint32_t>(in, 4);
       uint32_t F = load_be<uint32_t>(in, 5);
       uint32_t G = load_be<uint32_t>(in, 6);
       uint32_t H = load_be<uint32_t>(in, 7);
 
       for(size_t r = 0; r != rounds; r += 1)
          {
              if(r%8 == 0)
                SHACAL2_Rev(B, C, D, E, F, G, H, A, m_RK[63-r]);

              if(r%8 == 1)
                SHACAL2_Rev(C, D, E, F, G, H, A, B, m_RK[63-r]);

              if(r%8 == 2)
                SHACAL2_Rev(D, E, F, G, H, A, B, C, m_RK[63-r]);

              if(r%8 == 3)
                SHACAL2_Rev(E, F, G, H, A, B, C, D, m_RK[63-r]);

              if(r%8 == 4)
                SHACAL2_Rev(F, G, H, A, B, C, D, E, m_RK[63-r]);

              if(r%8 == 5)
                SHACAL2_Rev(G, H, A, B, C, D, E, F, m_RK[63-r]);

              if(r%8 == 6)
                SHACAL2_Rev(H, A, B, C, D, E, F, G, m_RK[63-r]);

              if(r%8 == 7)
                SHACAL2_Rev(A, B, C, D, E, F, G, H, m_RK[63-r]);
          }
 
       store_be(out, A, B, C, D, E, F, G, H);
 
       in += BLOCK_SIZE;
       out += BLOCK_SIZE;
       }
    }
 
 /*
 * SHACAL2 Key Schedule
 */
 void SHACAL2::key_schedule(const uint8_t key[], size_t len)
    {
    const uint32_t RC[64] = {
       0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5DBA5,
       0x3956C25B, 0x59F111F1, 0x923F82A4, 0xAB1C5ED5,
       0xD807AA98, 0x12835B01, 0x243185BE, 0x550C7DC3,
       0x72BE5D74, 0x80DEB1FE, 0x9BDC06A7, 0xC19BF174,
       0xE49B69C1, 0xEFBE4786, 0x0FC19DC6, 0x240CA1CC,
       0x2DE92C6F, 0x4A7484AA, 0x5CB0A9DC, 0x76F988DA,
       0x983E5152, 0xA831C66D, 0xB00327C8, 0xBF597FC7,
       0xC6E00BF3, 0xD5A79147, 0x06CA6351, 0x14292967,
       0x27B70A85, 0x2E1B2138, 0x4D2C6DFC, 0x53380D13,
       0x650A7354, 0x766A0ABB, 0x81C2C92E, 0x92722C85,
       0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
       0xD192E819, 0xD6990624, 0xF40E3585, 0x106AA070,
       0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5,
       0x391C0CB3, 0x4ED8AA4A, 0x5B9CCA4F, 0x682E6FF3,
       0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
       0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2
    };
 
    if(m_RK.empty())
       m_RK.resize(64);
    else
       clear_mem(m_RK.data(), m_RK.size());
 
    load_be(m_RK.data(), key, len/4);
 
    for(size_t i = 16; i != 64; ++i)
       {
       const uint32_t sigma0_15 = rotr< 7>(m_RK[i-15]) ^ rotr<18>(m_RK[i-15]) ^ (m_RK[i-15] >> 3);
       const uint32_t sigma1_2  = rotr<17>(m_RK[i- 2]) ^ rotr<19>(m_RK[i- 2]) ^ (m_RK[i- 2] >> 10);
       m_RK[i] = m_RK[i-16] + sigma0_15 + m_RK[i-7] + sigma1_2;
       }
 
    for(size_t i = 0; i != 64; ++i)
       {
       m_RK[i] += RC[i];
       }
    }
 /*
 size_t SHACAL2::parallelism() const
    {
 #if defined(BOTAN_HAS_SHACAL2_X86)
    if(CPUID::has_intel_sha())
       {
       return 4;
       }
 #endif
 
 #if defined(BOTAN_HAS_SHACAL2_SIMD)
    if(CPUID::has_simd_32())
       {
       return 4;
       }
 #endif
 
    return 1;
    }
 
 std::string SHACAL2::provider() const
    {
 #if defined(BOTAN_HAS_SHACAL2_X86)
    if(CPUID::has_intel_sha())
       {
       return "intel_sha";
       }
 #endif
 
 #if defined(BOTAN_HAS_SHACAL2_SIMD)
    if(CPUID::has_simd_32())
       {
       return "simd";
       }
 #endif
 
    return "base";
    }
 */
 /*
 * Clear memory of sensitive data
 *//*
 void SHACAL2::clear()
    {
    zap(m_RK);
    }*/
 
 }
