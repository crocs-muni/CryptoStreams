 /*
 * XTEA
 * (C) 1999-2009,2016 Jack Lloyd
 *
 * Botan is released under the Simplified BSD License (see license.txt)
 */
 
 #include "xtea.h"
 
 namespace Botan {

     const int BLOCK_SIZE = 64;

/**
* Load a big-endian word
* @param in a pointer to some bytes
* @param off an offset into the array
* @return off'th T of in, as a big-endian value
*//*
template<typename T>
inline T load_be(const uint8_t in[], size_t off)
{
  in += off * sizeof(T);
  T out = 0;
  for(size_t i = 0; i != sizeof(T); ++i)
     out = static_cast<T>((out << 8) | in[i]);
  return out;
}*/

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
* Make a uint32_t from four bytes
* @param i0 the first byte
* @param i1 the second byte
* @param i2 the third byte
* @param i3 the fourth byte
* @return i0 || i1 || i2 || i3
*/
inline uint32_t make_uint32(uint8_t i0, uint8_t i1, uint8_t i2, uint8_t i3)
{
  return ((static_cast<uint32_t>(i0) << 24) |
          (static_cast<uint32_t>(i1) << 16) |
          (static_cast<uint32_t>(i2) <<  8) |
          (static_cast<uint32_t>(i3)));
}

     /**
* Load eight big-endian words
* @param in a pointer to some bytes
* @param x0 where the first word will be written
* @param x1 where the second word will be written
* @param x2 where the third word will be written
* @param x3 where the fourth word will be written
* @param x4 where the fifth word will be written
* @param x5 where the sixth word will be written
* @param x6 where the seventh word will be written
* @param x7 where the eighth word will be written
*/
     template<typename T>
     inline void load_be(const uint8_t in[],
                         T& x0, T& x1, T& x2, T& x3,
                         T& x4, T& x5, T& x6, T& x7)
     {
         x0 = load_be<T>(in, 0);
         x1 = load_be<T>(in, 1);
         x2 = load_be<T>(in, 2);
         x3 = load_be<T>(in, 3);
         x4 = load_be<T>(in, 4);
         x5 = load_be<T>(in, 5);
         x6 = load_be<T>(in, 6);
         x7 = load_be<T>(in, 7);
     }

     /**
* Load two big-endian words
* @param in a pointer to some bytes
* @param x0 where the first word will be written
* @param x1 where the second word will be written
*/
     template<typename T>
     inline void load_be(const uint8_t in[], T& x0, T& x1)
     {
         x0 = load_be<T>(in, 0);
         x1 = load_be<T>(in, 1);
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
* Store two big-endian words
* @param out the output byte array
* @param x0 the first word
* @param x1 the second word
*/
     template<typename T>
     inline void store_be(uint8_t out[], T x0, T x1)
     {
         store_be(x0, out + (0 * sizeof(T)));
         store_be(x1, out + (1 * sizeof(T)));
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

 
 /*
 * XTEA Encryption
 */
 void XTEA::encrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const
    {
    //verify_key_set(m_EK.empty() == false);
 
    const uint32_t* EK = &m_EK[0];
 
    const size_t blocks4 = blocks / 4;
    const size_t blocks_left = blocks % 4;
 
    for(size_t i = 0; i < blocks4; i++)
       {
       uint32_t L0, R0, L1, R1, L2, R2, L3, R3;
       load_be(in + 4*BLOCK_SIZE*i, L0, R0, L1, R1, L2, R2, L3, R3);
 
       for(size_t r = 0; r != rounds; ++r)
          {
          L0 += (((R0 << 4) ^ (R0 >> 5)) + R0) ^ EK[2*r];
          L1 += (((R1 << 4) ^ (R1 >> 5)) + R1) ^ EK[2*r];
          L2 += (((R2 << 4) ^ (R2 >> 5)) + R2) ^ EK[2*r];
          L3 += (((R3 << 4) ^ (R3 >> 5)) + R3) ^ EK[2*r];
 
          R0 += (((L0 << 4) ^ (L0 >> 5)) + L0) ^ EK[2*r+1];
          R1 += (((L1 << 4) ^ (L1 >> 5)) + L1) ^ EK[2*r+1];
          R2 += (((L2 << 4) ^ (L2 >> 5)) + L2) ^ EK[2*r+1];
          R3 += (((L3 << 4) ^ (L3 >> 5)) + L3) ^ EK[2*r+1];
          }
 
       store_be(out + 4*BLOCK_SIZE*i, L0, R0, L1, R1, L2, R2, L3, R3);
       }
       
    for(size_t i = 0; i < blocks_left; ++i)
       {
       uint32_t L, R;
       load_be(in + BLOCK_SIZE*(4*blocks4+i), L, R);
 
       for(size_t r = 0; r != 32; ++r)
          {
          L += (((R << 4) ^ (R >> 5)) + R) ^ EK[2*r];
          R += (((L << 4) ^ (L >> 5)) + L) ^ EK[2*r+1];
          }
 
       store_be(out + BLOCK_SIZE*(4*blocks4+i), L, R);
       }
    }
 
 /*
 * XTEA Decryption
 */
 void XTEA::decrypt_n(const uint8_t in[], uint8_t out[], size_t blocks, unsigned int rounds) const
    {
    //verify_key_set(m_EK.empty() == false);
 
    const uint32_t* EK = &m_EK[0];
 
    const size_t blocks4 = blocks / 4;
    const size_t blocks_left = blocks % 4;
 
    for(size_t i = 0; i < blocks4; i++)
       {
       uint32_t L0, R0, L1, R1, L2, R2, L3, R3;
       load_be(in + 4*BLOCK_SIZE*i, L0, R0, L1, R1, L2, R2, L3, R3);
 
       for(size_t r = 0; r != rounds; ++r)
          {
          R0 -= (((L0 << 4) ^ (L0 >> 5)) + L0) ^ EK[63 - 2*r];
          R1 -= (((L1 << 4) ^ (L1 >> 5)) + L1) ^ EK[63 - 2*r];
          R2 -= (((L2 << 4) ^ (L2 >> 5)) + L2) ^ EK[63 - 2*r];
          R3 -= (((L3 << 4) ^ (L3 >> 5)) + L3) ^ EK[63 - 2*r];
 
          L0 -= (((R0 << 4) ^ (R0 >> 5)) + R0) ^ EK[62 - 2*r];
          L1 -= (((R1 << 4) ^ (R1 >> 5)) + R1) ^ EK[62 - 2*r];
          L2 -= (((R2 << 4) ^ (R2 >> 5)) + R2) ^ EK[62 - 2*r];
          L3 -= (((R3 << 4) ^ (R3 >> 5)) + R3) ^ EK[62 - 2*r];
          }
 
       store_be(out + 4*BLOCK_SIZE*i, L0, R0, L1, R1, L2, R2, L3, R3);
       }
       
    for(size_t i = 0; i < blocks_left; ++i)
       {
       uint32_t L, R;
       load_be(in + BLOCK_SIZE*(4*blocks4+i), L, R);
 
       for(size_t r = 0; r != 32; ++r)
          {
          R -= (((L << 4) ^ (L >> 5)) + L) ^ m_EK[63 - 2*r];
          L -= (((R << 4) ^ (R >> 5)) + R) ^ m_EK[62 - 2*r];
          }
 
       store_be(out + BLOCK_SIZE*(4*blocks4+i), L, R);
       }
    }
 
 /*
 * XTEA Key Schedule
 */
 void XTEA::key_schedule(const uint8_t key[], size_t)
    {
    m_EK.resize(64);

    std::vector<uint32_t> UK(4);
    //secure_vector<uint32_t> UK(4);
    for(size_t i = 0; i != 4; ++i)
       UK[i] = load_be<uint32_t>(key, i);
 
    uint32_t D = 0;
    for(size_t i = 0; i != 64; i += 2)
       {
       m_EK[i  ] = D + UK[D % 4];
       D += 0x9E3779B9;
       m_EK[i+1] = D + UK[(D >> 11) % 4];
       }
    }
 /*
 void XTEA::clear()
    {
    zap(m_EK);
    }
 */
 }
