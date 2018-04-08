 /*
 * Noekeon
 * (C) 1999-2008 Jack Lloyd
 *
 * Botan is released under the Simplified BSD License (see license.txt)
 */
 
 #include "noekeon.h"

 const int BLOCK_SIZE = 128;

 /**
 * Bit rotation left by a compile-time constant amount
 * @param input the input word
 * @return input rotated left by ROT bits
 */
 template<size_t ROT, typename T>
 inline T rotl(T input)
 {
    static_assert(ROT > 0 && ROT < 8*sizeof(T), "Invalid rotation constant");
    return static_cast<T>((input << ROT) | (input >> (8*sizeof(T) - ROT)));
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
* Store four big-endian words
* @param out the output byte array
* @param x0 the first word
* @param x1 the second word
* @param x2 the third word
* @param x3 the fourth word
*/
 template<typename T>
 inline void store_be(uint8_t out[], T x0, T x1, T x2, T x3)
 {
    store_be(x0, out + (0 * sizeof(T)));
    store_be(x1, out + (1 * sizeof(T)));
    store_be(x2, out + (2 * sizeof(T)));
    store_be(x3, out + (3 * sizeof(T)));
 }

 namespace Botan {
 
 namespace {
 
 /*
 * Noekeon's Theta Operation
 */
 inline void theta(uint32_t& A0, uint32_t& A1,
                   uint32_t& A2, uint32_t& A3,
                   const uint32_t EK[4])
    {
    uint32_t T = A0 ^ A2;
    T ^= rotl<8>(T) ^ rotr<8>(T);
    A1 ^= T;
    A3 ^= T;
 
    A0 ^= EK[0];
    A1 ^= EK[1];
    A2 ^= EK[2];
    A3 ^= EK[3];
 
    T = A1 ^ A3;
    T ^= rotl<8>(T) ^ rotr<8>(T);
    A0 ^= T;
    A2 ^= T;
    }
 
 /*
 * Theta With Null Key
 */
 inline void theta(uint32_t& A0, uint32_t& A1,
                   uint32_t& A2, uint32_t& A3)
    {
    uint32_t T = A0 ^ A2;
    T ^= rotl<8>(T) ^ rotr<8>(T);
    A1 ^= T;
    A3 ^= T;
 
    T = A1 ^ A3;
    T ^= rotl<8>(T) ^ rotr<8>(T);
    A0 ^= T;
    A2 ^= T;
    }
 
 /*
 * Noekeon's Gamma S-Box Layer
 */
 inline void gamma(uint32_t& A0, uint32_t& A1, uint32_t& A2, uint32_t& A3)
    {
    A1 ^= ~A3 & ~A2;
    A0 ^= A2 & A1;
 
    uint32_t T = A3;
    A3 = A0;
    A0 = T;
 
    A2 ^= A0 ^ A1 ^ A3;
 
    A1 ^= ~A3 & ~A2;
    A0 ^= A2 & A1;
    }
 
 }
 /*
 size_t Noekeon::parallelism() const
    {
 #if defined(BOTAN_HAS_NOEKEON_SIMD)
    if(CPUID::has_simd_32())
       {
       return 4;
       }
 #endif
 
    return 1;
    }
 
 std::string Noekeon::provider() const
    {
 #if defined(BOTAN_HAS_NOEKEON_SIMD)
    if(CPUID::has_simd_32())
       {
       return "simd";
       }
 #endif
 
    return "base";
    }*/
 
 /*
 * Noekeon Round Constants
 */
 const uint8_t Noekeon::RC[] = {
    0x80, 0x1B, 0x36, 0x6C, 0xD8, 0xAB, 0x4D, 0x9A,
    0x2F, 0x5E, 0xBC, 0x63, 0xC6, 0x97, 0x35, 0x6A,
    0xD4 };
 
 /*
 * Noekeon Encryption
 */
 void Noekeon::encrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const
    {
    //verify_key_set(m_EK.empty() == false);
 
 #if defined(BOTAN_HAS_NOEKEON_SIMD)
    if(CPUID::has_simd_32())
       {
       while(blocks >= 4)
          {
          simd_encrypt_4(in, out);
          in += 4 * BLOCK_SIZE;
          out += 4 * BLOCK_SIZE;
          blocks -= 4;
          }
       }
 #endif
 
    for(size_t i = 0; i != blocks; ++i)
       {
       uint32_t A0 = load_be<uint32_t>(in, 0);
       uint32_t A1 = load_be<uint32_t>(in, 1);
       uint32_t A2 = load_be<uint32_t>(in, 2);
       uint32_t A3 = load_be<uint32_t>(in, 3);
 
       for(size_t j = 0; j != rounds; ++j)
          {
          A0 ^= RC[j];
          theta(A0, A1, A2, A3, m_EK.data());
 
          A1 = rotl<1>(A1);
          A2 = rotl<5>(A2);
          A3 = rotl<2>(A3);
 
          gamma(A0, A1, A2, A3);
 
          A1 = rotr<1>(A1);
          A2 = rotr<5>(A2);
          A3 = rotr<2>(A3);
          }
 
       A0 ^= RC[16];
       theta(A0, A1, A2, A3, m_EK.data());
 
       store_be(out, A0, A1, A2, A3);
 
       in += BLOCK_SIZE;
       out += BLOCK_SIZE;
       }
    }
 
 /*
 * Noekeon Encryption
 */
 void Noekeon::decrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const
    {
    //verify_key_set(m_DK.empty() == false);
 
 #if defined(BOTAN_HAS_NOEKEON_SIMD)
    if(CPUID::has_simd_32())
       {
       while(blocks >= 4)
          {
          simd_decrypt_4(in, out);
          in += 4 * BLOCK_SIZE;
          out += 4 * BLOCK_SIZE;
          blocks -= 4;
          }
       }
 #endif
 
    for(size_t i = 0; i != blocks; ++i)
       {
       uint32_t A0 = load_be<uint32_t>(in, 0);
       uint32_t A1 = load_be<uint32_t>(in, 1);
       uint32_t A2 = load_be<uint32_t>(in, 2);
       uint32_t A3 = load_be<uint32_t>(in, 3);
 
       for(size_t j = rounds; j != 0; --j)
          {
          theta(A0, A1, A2, A3, m_DK.data());
          A0 ^= RC[j];
 
          A1 = rotl<1>(A1);
          A2 = rotl<5>(A2);
          A3 = rotl<2>(A3);
 
          gamma(A0, A1, A2, A3);
 
          A1 = rotr<1>(A1);
          A2 = rotr<5>(A2);
          A3 = rotr<2>(A3);
          }
 
       theta(A0, A1, A2, A3, m_DK.data());
       A0 ^= RC[0];
 
       store_be(out, A0, A1, A2, A3);
 
       in += BLOCK_SIZE;
       out += BLOCK_SIZE;
       }
    }
 
 /*
 * Noekeon Key Schedule
 */
 void Noekeon::key_schedule(const uint8_t key[], size_t)
    {
    uint32_t A0 = load_be<uint32_t>(key, 0);
    uint32_t A1 = load_be<uint32_t>(key, 1);
    uint32_t A2 = load_be<uint32_t>(key, 2);
    uint32_t A3 = load_be<uint32_t>(key, 3);
 
    for(size_t i = 0; i != 16; ++i)
       {
       A0 ^= RC[i];
       theta(A0, A1, A2, A3);
 
       A1 = rotl<1>(A1);
       A2 = rotl<5>(A2);
       A3 = rotl<2>(A3);
 
       gamma(A0, A1, A2, A3);
 
       A1 = rotr<1>(A1);
       A2 = rotr<5>(A2);
       A3 = rotr<2>(A3);
       }
 
    A0 ^= RC[16];
 
    m_DK.resize(4);
    m_DK[0] = A0;
    m_DK[1] = A1;
    m_DK[2] = A2;
    m_DK[3] = A3;
 
    theta(A0, A1, A2, A3);
 
    m_EK.resize(4);
    m_EK[0] = A0;
    m_EK[1] = A1;
    m_EK[2] = A2;
    m_EK[3] = A3;
    }
 
 /*
 * Clear memory of sensitive data
 *//*
 void Noekeon::clear()
    {
    zap(m_EK);
    zap(m_DK);
    }
 */
 }
