/****************************************************************
 *                                                              *
 *  This program is part of an implementation of the block      *
 *  cipher RC6(TM) that is being submitted to NIST as a         *
 *  candidate for the AES.                                      *
 *                                                              *
 *  Ronald L. Rivest and RSA Laboratories, the submitters of    *
 *  RC6, retain all rights to RC6 and to this code, except for  *
 *  those which NIST requires to be waived for submissions.     *
 *                                                              *
 *  Copyright (C) 1998 RSA Data Security, Inc.                  *
 *                                                              *
 ****************************************************************/

/* This file contains a reference implementation of RC6 in C. */




#include <string.h>
#include <ctype.h>

#include "rc6_ref.h"

namespace block {
namespace rc6 {

static unsigned RC6_N_ROUNDS;

/* The "magic constants" for RC6 with 32-bit wordsize */
#define P32 0xb7e15163
#define Q32 0x9e3779b9


/* Macro for left-rotating a 32-bit dword by some amount */
#define ROTL(W,c) (((W) << ((c) & 0x1f)) | ((W) >> (32-((c) & 0x1f))))

/* Macro for right-rotating a 32-bit dword by some amount */
#define ROTR(W,c) (((W) >> ((c) & 0x1f)) | ((W) << (32-((c) & 0x1f))))




/*
 * Here we implement a few general utility functions
 */



/*
 * Load an uint32_t as a little-endian sequence of 4 bytes
 */
static uint32_t LoadDword(BYTE* ptr)
{
  return (((uint32_t) *ptr)+
          (((uint32_t) *(ptr+1)) << 8)+
          (((uint32_t) *(ptr+2)) << 16)+
          (((uint32_t) *(ptr+3)) << 24));
}



/*
 * Store an uint32_t as a little-endian sequence of 4 bytes
 */
static void StoreDword(uint32_t dword, BYTE* ptr)
{
  *ptr = (BYTE) dword;
  *(ptr+1) = (BYTE) (dword >> 8);
  *(ptr+2) = (BYTE) (dword >> 16);
  *(ptr+3) = (BYTE) (dword >> 24);
}



/*
 * ConvertDigitToNumber() converts a single ASCII hexadecimal digit
 * to its numeric equivalent in the range 0x0-0xf.
 */
static BYTE ConvertDigitToNumber(char digit)
{
  if (isdigit(digit))
    return ((BYTE) (digit-'0'));

  if (islower(digit))
    return ((BYTE) (digit-'a'+0xa));

  /* Apparently, it's an uppercase letter */
  return ((BYTE) (digit-'A'+0xa));
}



/*
 * ConvertDigitsToBytes() converts a sequence of pairs of ASCII hex
 * digits into a sequence of bytes.  It takes an argument, n, which
 * specifies how many bytes will be produced: 2n hex digits get
 * converted to n bytes.
 */
static void ConvertDigitsToBytes(char* digits, BYTE* bytes, int n)
{
  int i;

  for (i = 0; i < n*2; i += 2) {
    BYTE msn = ConvertDigitToNumber(digits[i]);
    BYTE lsn = ConvertDigitToNumber(digits[i+1]);
    bytes[i/2] = (BYTE) ((msn << 4) | lsn);
  }
}




/*
 * The next three functions-- Rc6ComputeKeySchedule(), Rc6EncryptBlock(),
 * and Rc6DecryptBlock()-- form the bottom-level implementation of RC6.
 */



/*
 * Rc6ComputeKeySchedule() computes a key schedule from a variable-length
 * key.  It does *not* properly test the length of the key in advance, so
 * a key passed to it should not have an out-of-range length.  Valid key
 * lengths are 0-255 bytes.
 *
 * The key passed to RC6ComputeKeySchedule() is a sequence of bytes, NOT
 * an ASCII string.
 *
 * Rc6ComputeKeySchedule() places the schedule it computes in an array
 * of 2+2*ROUNDS+2 = 44 dwords specified by a pointer, S, that is input.
 */
static void Rc6ComputeKeySchedule(BYTE* key, int keyLengthInBytes,
                                  uint32_t* S)
{
  uint32_t L[(255+4-1)/4];
  const int t = 2+2*RC6_MAX_ROUNDS+2;
  int count;
  int u;
  int i,j;
  uint32_t A,B;
  int startOfExtraDword, numberOfExtraBytes;


  /* Compute number of dwords taken up by key */
  int c = (keyLengthInBytes+4-1)/4;

  /* Special case: if the key is 0 bytes, then we'll start out with a
   * single element with the value 0 in the array of key dwords */
  if (c == 0)
    c = 1;


  /* Zero out array of key dwords */
  for (count = 0; count < c; count++)
    L[count] = 0;


  /* Load all the key bytes into the array of key dwords, with the
   * possible exception of the final 1-3 bytes */
  for (count = 0; count < keyLengthInBytes/4; count++)
    L[count] = LoadDword(key+count*4);


  /* Load any extra key bytes at the end into the last dword */
  startOfExtraDword = keyLengthInBytes & 0xfffffffc;
  numberOfExtraBytes = keyLengthInBytes & 0x3;
  if (numberOfExtraBytes > 0) {
    L[c-1] = (uint32_t) key[startOfExtraDword];
    if (numberOfExtraBytes > 1) {
      L[c-1] |= (((uint32_t) key[startOfExtraDword+1]) << 8);
      if (numberOfExtraBytes > 2)
        L[c-1] |= (((uint32_t) key[startOfExtraDword+2]) << 16);
    }
  }


  /* Initialize key schedule array */
  S[0] = P32;
  for (count = 1; count < t; count++)
    S[count] = S[count-1]+Q32;


  /* The number of passes in the key schedule set-up algorithm is equal to
   * 3 times the size of whichever is larger: the array of key dwords, or
   * the array of key schedule dwords that we're computing. */
  u = 3*((c>t) ? c : t);


  i = j = 0;
  A = B = 0;

  /* Mix the key into the key schedule array over the course of u rounds */
  for (count = 1; count <= u; count++) {
    uint32_t sum;

    sum = A+B;
    S[i] += sum;
    A = S[i] = ROTL(S[i], 3);

    sum = A+B;
    L[j] += sum;
    B = L[j] = ROTL(L[j], sum);

    i = (i+1) % t;
    j = (j+1) % c;
  }
}



/*
 * Rc6EncryptBlock() encrypts a single [16-byte] block of plaintext.
 * The key schedule to be used is passed in as a pointer, S, to the
 * array of 44 dwords of which the key schedule is comprised.
 */
static void Rc6EncryptBlock(uint32_t* S,
                            BYTE* plaintext, BYTE* ciphertext)
{
  int i;

  /* Load A, B, C, and D registers from plaintext */
  uint32_t A = LoadDword(plaintext);
  uint32_t B = LoadDword(plaintext+4);
  uint32_t C = LoadDword(plaintext+8);
  uint32_t D = LoadDword(plaintext+12);


  /* Do pseudo-round #0: pre-whitening of B and D */
  B += S[0];
  D += S[1];


  /* Perform round #1, #2, ..., #ROUNDS of encryption */
    for (i = 1; i <= RC6_N_ROUNDS; i++) {
    uint32_t t, u;

    t = B*(2*B+1);
    t = ROTL(t, 5);

    u = D*(2*D+1);
    u = ROTL(u, 5);

    A ^= t;
    A = ROTL(A, u);
    A += S[2*i];

    C ^= u;
    C = ROTL(C, t);
    C += S[2*i+1];

    {
      uint32_t temp = A;
      A = B;
      B = C;
      C = D;
      D = temp;
    }
  }


  /* Do pseudo-round #(ROUNDS+1): post-whitening of A and C */
  A += S[2*RC6_N_ROUNDS+2];
  C += S[2*RC6_N_ROUNDS+3];


  /* Store A, B, C, and D registers to ciphertext */
  StoreDword(A, ciphertext);
  StoreDword(B, ciphertext+4);
  StoreDword(C, ciphertext+8);
  StoreDword(D, ciphertext+12);
}



/*
 * Rc6DecryptBlock() decrypts a single [16-byte] block of ciphertext.
 * The key schedule to be used is passed in as a pointer ("S") to the
 * array of 44 dwords of which the key schedule is comprised.
 */
static void Rc6DecryptBlock(uint32_t* S,
                            BYTE* ciphertext, BYTE* plaintext)
{
  int i;

  /* Load A, B, C, and D registers from ciphertext */
  uint32_t A = LoadDword(ciphertext);
  uint32_t B = LoadDword(ciphertext+4);
  uint32_t C = LoadDword(ciphertext+8);
  uint32_t D = LoadDword(ciphertext+12);


  /* Undo pseudo-round #(ROUNDS+1): post-whitening of A and C */
  C -= S[2*RC6_N_ROUNDS+3];
  A -= S[2*RC6_N_ROUNDS+2];


  /* Undo round #ROUNDS, ..., #2, #1 of encryption */
  for (i = RC6_N_ROUNDS; i >= 1; i--) {
    uint32_t t, u;

    {
      uint32_t temp = D;
      D = C;
      C = B;
      B = A;
      A = temp;
    }

    t = B*(2*B+1);
    t = ROTL(t, 5);

    u = D*(2*D+1);
    u = ROTL(u, 5);

    C -= S[2*i+1];
    C = ROTR(C, t);
    C ^= u;

    A -= S[2*i];
    A = ROTR(A, u);
    A ^= t;
  }


  /* Undo pseudo-round #0: pre-whitening of B and D */
  D -= S[1];
  B -= S[0];


  /* Store A, B, C, and D registers to plaintext */
  StoreDword(A, plaintext);
  StoreDword(B, plaintext+4);
  StoreDword(C, plaintext+8);
  StoreDword(D, plaintext+12);
}




/*
 * The six functions below add modes (ECB, CBC, and 1-bit CFB) to the
 * simple block cipher functionality implemented above.
 */



/*
 * Rc6EncryptEcb() encrypts a specified number of blocks in ECB mode.
 */
static void Rc6EncryptEcb(uint32_t* S, int numberOfBlocks,
                          BYTE* plaintext, BYTE* ciphertext)
{
  for ( ; numberOfBlocks-- > 0; plaintext += 16, ciphertext += 16)
    /* Encrypt block */
    Rc6EncryptBlock(S, plaintext, ciphertext);
}



/*
 * Rc6DecryptEcb() decrypts a specified number of blocks in ECB mode.
 */
static void Rc6DecryptEcb(uint32_t* S, int numberOfBlocks,
                          BYTE* ciphertext, BYTE* plaintext)
{
  for ( ; numberOfBlocks-- > 0; ciphertext += 16, plaintext += 16)
    /* Decrypt block */
    Rc6DecryptBlock(S, ciphertext, plaintext);
}



/*
 * Rc6EncryptCbc() encrypts a specified number of blocks in CBC mode.
 * In the process, it alters the 16-byte value pointed to by ivBytes.
 */
static void Rc6EncryptCbc(uint32_t* S, BYTE* IV, int numberOfBlocks,
                          BYTE* plaintext, BYTE* ciphertext)
{
  for ( ; numberOfBlocks-- > 0; plaintext += 16, ciphertext += 16) {
    int i;

    /* XOR IV and plaintext */
    for (i = 0; i < 16; i++)
      IV[i] ^= plaintext[i];


    /* Encrypt XORed plaintext */
    Rc6EncryptBlock(S, IV, ciphertext);


    /* Store ciphertext as IV for next block */
    memcpy((void*) IV, (void*) ciphertext, 16);
  }
}



/*
 * Rc6DecryptCbc() decrypts a specified number of blocks in CBC mode.
 * In the process, it alters the 16-byte value pointed to by ivBytes.
 */
static void Rc6DecryptCbc(uint32_t* S, BYTE* IV, int numberOfBlocks,
                          BYTE* ciphertext, BYTE* plaintext)
{
  for ( ; numberOfBlocks-- > 0; ciphertext += 16, plaintext += 16) {
    BYTE savedCiphertext[16];
    int i;

    /* Save ciphertext block in case the ciphertext and plaintext buffers
     * overlap */
    memcpy((void*) savedCiphertext, (void*) ciphertext, 16);


    /* Recover XORed plaintext */
    Rc6DecryptBlock(S, ciphertext, plaintext);


    /* XOR plaintext and IV to get plaintext */
    for (i = 0; i < 16; i++)
      plaintext[i] ^= IV[i];


    /* Keep ciphertext as IV for next block */
    memcpy((void*) IV, (void*) savedCiphertext, 16);
  }
}



/*
 * Rc6EncryptCfb1() encrypts a specified number of bits (*not* blocks) in
 * 1-bit CFB mode.  In the process, it alters the 16-byte value pointed
 * to by ivBytes.
 */
static void Rc6EncryptCfb1(uint32_t* S, BYTE* IV, int numberOfBits,
                           BYTE* plaintext, BYTE* ciphertext)
{
  int bitsProcessed;


  for (bitsProcessed = 0; bitsProcessed < numberOfBits; bitsProcessed++) {
    int bitIndex = bitsProcessed % 8;
 	  int bitMask = 0x80 >> bitIndex;
    int plaintextBit, maskingBit, ciphertextBit;
    BYTE encryptedIv[16];
    int i;


    if (bitIndex == 0)
      *ciphertext = 0;


    /* Get bit of plaintext (as a 0-1 value) */
    plaintextBit = ((*plaintext & bitMask) != 0);


    /* Encrypt IV and get masking bit (as a 0-1 value) for this text bit */
    Rc6EncryptBlock(S, IV, encryptedIv);
    maskingBit = ((encryptedIv[0] & 0x80) != 0);


    /* Compute bit of ciphertext and put it in the current ciphertext byte */
    ciphertextBit = plaintextBit ^ maskingBit;
    *ciphertext |= (BYTE) (ciphertextBit*bitMask);


    /* Now shift the entire IV 1 bit to the left */
    for (i = 0; i < 15; i++)
      IV[i] = (BYTE) ((IV[i] << 1) | ((IV[i+1] & 0x80) != 0));

    IV[15] <<= 1;


    /* Put the bit of ciphertext just created into right end of IV */
    IV[15] = (BYTE) (IV[15] | ciphertextBit);


    /* If that was the end of a byte, bump the plaintext/ciphertext pointers */
    if (bitIndex == 7) {
      plaintext++;
      ciphertext++;
    }
  }
}



/*
 * Rc6DecryptCfb1() decrypts a specified number of bytes (*not* blocks) in
 * 1-bit CFB mode.  In the process, it alters the 16-byte value pointed
 * to by ivBytes.
 */
static void Rc6DecryptCfb1(uint32_t* S, BYTE* IV, int numberOfBits,
                           BYTE* ciphertext, BYTE* plaintext)
{
  int bitsProcessed;


  for (bitsProcessed = 0; bitsProcessed < numberOfBits; bitsProcessed++) {
    int bitIndex = bitsProcessed % 8;
 	  int bitMask = 0x80 >> bitIndex;
    int ciphertextBit, maskingBit, plaintextBit;
    BYTE encryptedIv[16];
    int i;


    if (bitIndex == 0)
      *plaintext = 0;


    /* Get bit of ciphertext (as a 0-1 value) */
    ciphertextBit = ((*ciphertext & bitMask) != 0);


    /* Encrypt IV and get masking bit (as a 0-1 value) for this text bit */
    Rc6EncryptBlock(S, IV, encryptedIv);
    maskingBit = ((encryptedIv[0] & 0x80) != 0);


    /* Compute bit of plaintext and put it in the current plaintext byte */
    plaintextBit = ciphertextBit ^ maskingBit;
    *plaintext |= (BYTE) (plaintextBit*bitMask);


    /* Now shift the entire IV 1 bit to the left */
    for (i = 0; i < 15; i++)
      IV[i] = (BYTE) ((IV[i] << 1) | ((IV[i+1] & 0x80) != 0));

    IV[15] <<= 1;


    /* Put the bit of ciphertext just used into right end of IV */
    IV[15] = (BYTE) (IV[15] | ciphertextBit);


    /* If that was the end of a byte, bump the ciphertext/plaintext pointers */
    if (bitIndex == 7) {
      ciphertext++;
      plaintext++;
    }
  }
}




/*
 * Below, we implement all the functions that have actually been specified
 * by NIST for their C API.
 */



/*
 * makeKey() initializes a keyInstance.
 */
int makeKey(keyInstance* key, BYTE direction, int keyLen, char* keyMaterial)
{
  BYTE keyBytes[255];
  int count;


  if ((direction != DIR_ENCRYPT) && (direction != DIR_DECRYPT))
    return BAD_KEY_DIR;
  key -> direction = direction;


  /* Check key length.  RC6 can use keys of length 0-255 bytes
   * (0-2040 bits, in multiples of 8). */
  if ((keyLen < 0) || (keyLen > 2040) || ((keyLen % 8) != 0))
    return BAD_KEY_MAT;


  /* Check hexadecimalness of key material */
  for (count = 0; count < keyLen/4; count++)
    if (!isxdigit(keyMaterial[count]))
      return BAD_KEY_MAT;


  /* Copy ASCII key */
  memcpy(key -> keyMaterial, keyMaterial, keyLen/4);
  key -> keyMaterial[keyLen/4] = 0;
  key -> keyLen = keyLen;


  /* Get key as a sequence of bytes */
  ConvertDigitsToBytes(keyMaterial, keyBytes, keyLen/8);


  Rc6ComputeKeySchedule(keyBytes, keyLen/8, key -> S);


  return TRUE;
}



/*
 * cipherInit() initializes a cipherInstance.
 */
int cipherInit(cipherInstance* cipher, BYTE mode, char* IV)
{
  int count;

  if ((mode != MODE_ECB) && (mode != MODE_CBC) && (mode != MODE_CFB1))
    return BAD_CIPHER_MODE;
  cipher -> mode = mode;


  if (IV != NULL) {

    /* Check hexadecimalness of IV material */
    for (count = 0; count < 32; count++)
      if (!isxdigit(IV[count]))
        return BAD_IV_MAT;


    /* Get IV as a sequence of bytes */
    ConvertDigitsToBytes(IV, cipher -> IV, 16);
  }


  return TRUE;
}



/*
 * blockEncrypt() encrypts some number of blocks of plaintext.
 */
int blockEncrypt(cipherInstance *cipher, keyInstance *key,
                 BYTE *input, int inputLen, BYTE *outBuffer,
                 unsigned rounds)
{
    RC6_N_ROUNDS = rounds;
  if (key -> direction != DIR_ENCRYPT)
    return BAD_KEY_MAT;
    /* The API document says that BAD_KEY_MATERIAL should be returned
     * in this situation; however, there is no such error code.  It
     * appears that BAD_KEY_MAT (or possibly BAD_KEY_DIR) is appropriate
     * here. */


  switch(cipher -> mode) {

    case MODE_ECB: {
      int numberOfBlocks = inputLen/128;

      Rc6EncryptEcb(key -> S, numberOfBlocks, input, outBuffer);

      /* Note that we completely ignore partial blocks of plaintext */
      return (numberOfBlocks*128);
    }


    case MODE_CBC: {
      int numberOfBlocks = inputLen/128;

      Rc6EncryptCbc(key -> S, cipher -> IV, numberOfBlocks, input, outBuffer);

      /* Note that we completely ignore partial blocks of plaintext */
      return (numberOfBlocks*128);
    }


    case MODE_CFB1: {
      Rc6EncryptCfb1(key -> S, cipher -> IV, inputLen, input, outBuffer);

      /* Note that we completely process every bit of plaintext */
      return inputLen;
    }


    default: {
      return BAD_CIPHER_STATE;
    }
  }
}



/*
 * blockDecrypt() decrypts some number of blocks of ciphertext.
 */
int blockDecrypt(cipherInstance *cipher, keyInstance *key,
                 BYTE *input, int inputLen, BYTE *outBuffer,
                 unsigned rounds)
{
    RC6_N_ROUNDS = rounds;
  if (key -> direction != DIR_DECRYPT)
    return BAD_KEY_MAT;
    /* The API document says that BAD_KEY_MATERIAL should be returned
     * in this situation; however, there is no such error code.  It
     * appears that BAD_KEY_MAT (or possibly BAD_KEY_DIR) is appropriate
     * here. */


  switch(cipher -> mode) {

    case MODE_ECB: {
      int numberOfBlocks = inputLen/128;

      Rc6DecryptEcb(key -> S, numberOfBlocks, input, outBuffer);

      /* Note that we completely ignore partial blocks of ciphertext */
      return (numberOfBlocks*128);
    }


    case MODE_CBC: {
      int numberOfBlocks = inputLen/128;

      Rc6DecryptCbc(key -> S, cipher -> IV, numberOfBlocks, input, outBuffer);

      /* Note that we completely ignore partial blocks of ciphertext */
      return (numberOfBlocks*128);
    }


    case MODE_CFB1: {
      Rc6DecryptCfb1(key -> S, cipher -> IV, inputLen, input, outBuffer);

      /* Note that we completely process every bit of plaintext */
      return inputLen;
    }


    default: {
      return BAD_CIPHER_STATE;
    }
  }
}

} // namespace rc6
} // namespace block
