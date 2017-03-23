#include <string.h>
#include <stdio.h>
#include "Blake_sha3.h"

/*
  the 10 permutations of {0,...15}
*/
static const unsigned char blake_sigma[][16] = {
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 } ,
    { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 } ,
    { 11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4 } ,
    {  7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8 } ,
    {  9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13 } ,
    {  2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9 } ,
    { 12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11 } ,
    { 13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10 } ,
    {  6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5 } ,
    { 10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13 , 0 }, 
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15 } ,
    { 14, 10,  4,  8,  9, 15, 13,  6,  1, 12,  0,  2, 11,  7,  5,  3 } ,
    { 11,  8, 12,  0,  5,  2, 15, 13, 10, 14,  3,  6,  7,  1,  9,  4 } ,
    {  7,  9,  3,  1, 13, 12, 11, 14,  2,  6,  5, 10,  4,  0, 15,  8 } ,
    {  9,  0,  5,  7,  2,  4, 10, 15, 14,  1, 11, 12,  6,  8,  3, 13 } ,
    {  2, 12,  6, 10,  0, 11,  8,  3,  4, 13,  7,  5, 15, 14,  1,  9 } ,
    { 12,  5,  1, 15, 14, 13,  4, 10,  0,  7,  6,  3,  9,  2,  8, 11 } ,
    { 13, 11,  7, 14, 12,  1,  3,  9,  5,  0, 15,  4,  8,  6,  2, 10 } ,
    {  6, 15, 14,  9, 11,  3,  0,  8, 12,  2, 13,  7,  1,  4, 10,  5 } ,
    { 10,  2,  8,  4,  7,  6,  1,  5, 15, 11,  9, 14,  3, 12, 13 , 0 }  
  };

/*
  constants for BLAKE-32 and BLAKE-28
*/
static const unsigned int blake_c32[16] = {
    0x243F6A88, 0x85A308D3,
    0x13198A2E, 0x03707344,
    0xA4093822, 0x299F31D0,
    0x082EFA98, 0xEC4E6C89,
    0x452821E6, 0x38D01377,
    0xBE5466CF, 0x34E90C6C,
    0xC0AC29B7, 0xC97C50DD,
    0x3F84D5B5, 0xB5470917 
};

/*
  constants for BLAKE-64 and BLAKE-48
*/
static const unsigned long long blake_c64[16] = {
  0x243F6A8885A308D3ULL,0x13198A2E03707344ULL,
  0xA4093822299F31D0ULL,0x082EFA98EC4E6C89ULL,
  0x452821E638D01377ULL,0xBE5466CF34E90C6CULL,
  0xC0AC29B7C97C50DDULL,0x3F84D5B5B5470917ULL,
  0x9216D5D98979FB1BULL,0xD1310BA698DFB5ACULL,
  0x2FFD72DBD01ADFB7ULL,0xB8E1AFED6A267E96ULL,
  0xBA7C9045F12C7F99ULL,0x24A19947B3916CF7ULL,
  0x0801F2E2858EFC16ULL,0x636920D871574E69ULL
};

/*
  blake_padding data
*/
static const BitSequence blake_padding[129] =
  {
    0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

/*
  initial values ( IVx for BLAKE-x)
*/
static const unsigned int blake_IV256[8]={
  0x6A09E667, 0xBB67AE85,
  0x3C6EF372, 0xA54FF53A,
  0x510E527F, 0x9B05688C,
  0x1F83D9AB, 0x5BE0CD19
};
static const unsigned int blake_IV224[8]={
  0xC1059ED8, 0x367CD507,
  0x3070DD17, 0xF70E5939,
  0xFFC00B31, 0x68581511,
  0x64F98FA7, 0xBEFA4FA4
};
static const unsigned long long blake_IV384[8]={
  0xCBBB9D5DC1059ED8ULL, 0x629A292A367CD507ULL,
  0x9159015A3070DD17ULL, 0x152FECD8F70E5939ULL,
  0x67332667FFC00B31ULL, 0x8EB44A8768581511ULL,
  0xDB0C2E0D64F98FA7ULL, 0x47B5481DBEFA4FA4ULL
};
static const unsigned long long blake_IV512[8]={
  0x6A09E667F3BCC908ULL, 0xBB67AE8584CAA73BULL,
  0x3C6EF372FE94F82BULL, 0xA54FF53A5F1D36F1ULL,
  0x510E527FADE682D1ULL, 0x9B05688C2B3E6C1FULL,
  0x1F83D9ABFB41BD6BULL, 0x5BE0CD19137E2179ULL
};

Blake::Blake(const int numRounds) {
	if (numRounds == -1) {
		blakeNumRounds32 = BLAKE_NB_ROUNDS32;
		blakeNumRounds64 = BLAKE_NB_ROUNDS64;
	} else {
		blakeNumRounds32 = numRounds;
		blakeNumRounds64 = numRounds;
	}
}

int Blake::compress32( const BitSequence * datablock ) {

#define BLAKE_ROT32(x,n) (((x)<<(32-n))|( (x)>>(n)))
#define BLAKE_ADD32(x,y)   ((unsigned int)((x) + (y)))
#define BLAKE_XOR32(x,y)    ((unsigned int)((x) ^ (y)))

#define G32(a,b,c,d,i) \
  do {\
    v[a] = BLAKE_XOR32(m[blake_sigma[round][i]], blake_c32[blake_sigma[round][i+1]])+BLAKE_ADD32(v[a],v[b]);\
    v[d] = BLAKE_ROT32(BLAKE_XOR32(v[d],v[a]),16);\
    v[c] = BLAKE_ADD32(v[c],v[d]);\
    v[b] = BLAKE_ROT32(BLAKE_XOR32(v[b],v[c]),12);\
    v[a] = BLAKE_XOR32(m[blake_sigma[round][i+1]], blake_c32[blake_sigma[round][i]])+BLAKE_ADD32(v[a],v[b]); \
    v[d] = BLAKE_ROT32(BLAKE_XOR32(v[d],v[a]), 8);\
    v[c] = BLAKE_ADD32(v[c],v[d]);\
    v[b] = BLAKE_ROT32(BLAKE_XOR32(v[b],v[c]), 7);\
  } while (0)

  unsigned int v[16];
  unsigned int m[16];
  int round;

  /* get message */
  m[ 0] = BLAKE_U8TO32_BE(datablock + 0);
  m[ 1] = BLAKE_U8TO32_BE(datablock + 4);
  m[ 2] = BLAKE_U8TO32_BE(datablock + 8);
  m[ 3] = BLAKE_U8TO32_BE(datablock +12);
  m[ 4] = BLAKE_U8TO32_BE(datablock +16);
  m[ 5] = BLAKE_U8TO32_BE(datablock +20);
  m[ 6] = BLAKE_U8TO32_BE(datablock +24);
  m[ 7] = BLAKE_U8TO32_BE(datablock +28);
  m[ 8] = BLAKE_U8TO32_BE(datablock +32);
  m[ 9] = BLAKE_U8TO32_BE(datablock +36);
  m[10] = BLAKE_U8TO32_BE(datablock +40);
  m[11] = BLAKE_U8TO32_BE(datablock +44);
  m[12] = BLAKE_U8TO32_BE(datablock +48);
  m[13] = BLAKE_U8TO32_BE(datablock +52);
  m[14] = BLAKE_U8TO32_BE(datablock +56);
  m[15] = BLAKE_U8TO32_BE(datablock +60);


  /* initialization */
  v[ 0] = blakeState.h32[0];
  v[ 1] = blakeState.h32[1];
  v[ 2] = blakeState.h32[2];
  v[ 3] = blakeState.h32[3];
  v[ 4] = blakeState.h32[4];
  v[ 5] = blakeState.h32[5];
  v[ 6] = blakeState.h32[6];
  v[ 7] = blakeState.h32[7];
  v[ 8] = blakeState.salt32[0];
  v[ 8] ^= 0x243F6A88;
  v[ 9] = blakeState.salt32[1];
  v[ 9] ^= 0x85A308D3;
  v[10] = blakeState.salt32[2];
  v[10] ^= 0x13198A2E;
  v[11] = blakeState.salt32[3];
  v[11] ^= 0x03707344;
  v[12] =  0xA4093822;
  v[13] =  0x299F31D0;
  v[14] =  0x082EFA98;
  v[15] =  0xEC4E6C89;
  if (blakeState.nullt == 0) { 
    v[12] ^= blakeState.t32[0];
    v[13] ^= blakeState.t32[0];
    v[14] ^= blakeState.t32[1];
    v[15] ^= blakeState.t32[1];
  }

  for(round=0; round<blakeNumRounds32; ++round) {

    G32( 0, 4, 8,12, 0);
    G32( 1, 5, 9,13, 2);
    G32( 2, 6,10,14, 4);
    G32( 3, 7,11,15, 6);

    G32( 3, 4, 9,14,14);   
    G32( 2, 7, 8,13,12);
    G32( 0, 5,10,15, 8);
    G32( 1, 6,11,12,10);

  }

  blakeState.h32[0] ^= v[ 0]; 
  blakeState.h32[1] ^= v[ 1];    
  blakeState.h32[2] ^= v[ 2];    
  blakeState.h32[3] ^= v[ 3];    
  blakeState.h32[4] ^= v[ 4];    
  blakeState.h32[5] ^= v[ 5];    
  blakeState.h32[6] ^= v[ 6];    
  blakeState.h32[7] ^= v[ 7];
  blakeState.h32[0] ^= v[ 8]; 
  blakeState.h32[1] ^= v[ 9];    
  blakeState.h32[2] ^= v[10];    
  blakeState.h32[3] ^= v[11];    
  blakeState.h32[4] ^= v[12];    
  blakeState.h32[5] ^= v[13];    
  blakeState.h32[6] ^= v[14];    
  blakeState.h32[7] ^= v[15];
  blakeState.h32[0] ^= blakeState.salt32[0];
  blakeState.h32[1] ^= blakeState.salt32[1];    
  blakeState.h32[2] ^= blakeState.salt32[2];    
  blakeState.h32[3] ^= blakeState.salt32[3];    
  blakeState.h32[4] ^= blakeState.salt32[0];    
  blakeState.h32[5] ^= blakeState.salt32[1];    
  blakeState.h32[6] ^= blakeState.salt32[2];    
  blakeState.h32[7] ^= blakeState.salt32[3];      

  return SUCCESS;
}


int Blake::compress64( const BitSequence * datablock ) {

#define BLAKE_ROT64(x,n) (((x)<<(64-n))|( (x)>>(n)))
#define BLAKE_ADD64(x,y)   ((unsigned long long)((x) + (y)))
#define BLAKE_XOR64(x,y)    ((unsigned long long)((x) ^ (y)))
  
#define G64(a,b,c,d,i)\
  do { \
    v[a] = BLAKE_ADD64(v[a],v[b])+BLAKE_XOR64(m[blake_sigma[round][i]], blake_c64[blake_sigma[round][i+1]]);\
    v[d] = BLAKE_ROT64(BLAKE_XOR64(v[d],v[a]),32);\
    v[c] = BLAKE_ADD64(v[c],v[d]);\
    v[b] = BLAKE_ROT64(BLAKE_XOR64(v[b],v[c]),25);\
    v[a] = BLAKE_ADD64(v[a],v[b])+BLAKE_XOR64(m[blake_sigma[round][i+1]], blake_c64[blake_sigma[round][i]]);\
    v[d] = BLAKE_ROT64(BLAKE_XOR64(v[d],v[a]),16);\
    v[c] = BLAKE_ADD64(v[c],v[d]);\
    v[b] = BLAKE_ROT64(BLAKE_XOR64(v[b],v[c]),11);\
  } while (0)

  unsigned long long v[16];
  unsigned long long m[16];
  int round;


  /* get message */
  m[ 0] = BLAKE_U8TO64_BE(datablock +  0);
  m[ 1] = BLAKE_U8TO64_BE(datablock +  8);
  m[ 2] = BLAKE_U8TO64_BE(datablock + 16);
  m[ 3] = BLAKE_U8TO64_BE(datablock + 24);
  m[ 4] = BLAKE_U8TO64_BE(datablock + 32);
  m[ 5] = BLAKE_U8TO64_BE(datablock + 40);
  m[ 6] = BLAKE_U8TO64_BE(datablock + 48);
  m[ 7] = BLAKE_U8TO64_BE(datablock + 56);
  m[ 8] = BLAKE_U8TO64_BE(datablock + 64);
  m[ 9] = BLAKE_U8TO64_BE(datablock + 72);
  m[10] = BLAKE_U8TO64_BE(datablock + 80);
  m[11] = BLAKE_U8TO64_BE(datablock + 88);
  m[12] = BLAKE_U8TO64_BE(datablock + 96);
  m[13] = BLAKE_U8TO64_BE(datablock +104);
  m[14] = BLAKE_U8TO64_BE(datablock +112);
  m[15] = BLAKE_U8TO64_BE(datablock +120);


  /* initialization */
  v[ 0] = blakeState.h64[0];
  v[ 1] = blakeState.h64[1];
  v[ 2] = blakeState.h64[2];
  v[ 3] = blakeState.h64[3];
  v[ 4] = blakeState.h64[4];
  v[ 5] = blakeState.h64[5];
  v[ 6] = blakeState.h64[6];
  v[ 7] = blakeState.h64[7];
  v[ 8] = blakeState.salt64[0];
  v[ 8] ^= 0x243F6A8885A308D3ULL;
  v[ 9] = blakeState.salt64[1];
  v[ 9] ^= 0x13198A2E03707344ULL;
  v[10] = blakeState.salt64[2];
  v[10] ^= 0xA4093822299F31D0ULL;
  v[11] = blakeState.salt64[3];
  v[11] ^= 0x082EFA98EC4E6C89ULL;


  v[12] =  0x452821E638D01377ULL;
  v[13] =  0xBE5466CF34E90C6CULL;
  v[14] =  0xC0AC29B7C97C50DDULL;
  v[15] =  0x3F84D5B5B5470917ULL;

  if (blakeState.nullt == 0) { 
    v[12] ^= blakeState.t64[0];
    v[13] ^= blakeState.t64[0];
    v[14] ^= blakeState.t64[1];
    v[15] ^= blakeState.t64[1];
  }

  for(round=0; round<blakeNumRounds64; ++round) {
    
    G64( 0, 4, 8,12, 0);
    G64( 1, 5, 9,13, 2);
    G64( 2, 6,10,14, 4);
    G64( 3, 7,11,15, 6);    

    G64( 3, 4, 9,14,14);   
    G64( 2, 7, 8,13,12);
    G64( 0, 5,10,15, 8);
    G64( 1, 6,11,12,10);

  }

  blakeState.h64[0] ^= v[ 0]; 
  blakeState.h64[1] ^= v[ 1];    
  blakeState.h64[2] ^= v[ 2];    
  blakeState.h64[3] ^= v[ 3];    
  blakeState.h64[4] ^= v[ 4];    
  blakeState.h64[5] ^= v[ 5];    
  blakeState.h64[6] ^= v[ 6];    
  blakeState.h64[7] ^= v[ 7];
  blakeState.h64[0] ^= v[ 8]; 
  blakeState.h64[1] ^= v[ 9];    
  blakeState.h64[2] ^= v[10];    
  blakeState.h64[3] ^= v[11];    
  blakeState.h64[4] ^= v[12];    
  blakeState.h64[5] ^= v[13];    
  blakeState.h64[6] ^= v[14];    
  blakeState.h64[7] ^= v[15];
  blakeState.h64[0] ^= blakeState.salt64[0];
  blakeState.h64[1] ^= blakeState.salt64[1];    
  blakeState.h64[2] ^= blakeState.salt64[2];    
  blakeState.h64[3] ^= blakeState.salt64[3];    
  blakeState.h64[4] ^= blakeState.salt64[0];    
  blakeState.h64[5] ^= blakeState.salt64[1];    
  blakeState.h64[6] ^= blakeState.salt64[2];    
  blakeState.h64[7] ^= blakeState.salt64[3];   

  return SUCCESS;
}


int Blake::Init( int hashbitlen ) {

  int i;

  if ( (hashbitlen == 224) || (hashbitlen == 256) )  {
    /* 224- and 256-bit versions (32-bit words) */

    if (hashbitlen == 224) 
      memcpy( blakeState.h32, blake_IV224, sizeof(blake_IV224) );      
    else 
      memcpy( blakeState.h32, blake_IV256, sizeof(blake_IV256) );

    blakeState.t32[0] = 0;
    blakeState.t32[1] = 0;

    for(i=0; i<64; ++i)
      blakeState.data32[i] = 0;

    blakeState.salt32[0] = 0;
    blakeState.salt32[1] = 0;
    blakeState.salt32[2] = 0;
    blakeState.salt32[3] = 0;
     
  }
  else if ( (hashbitlen == 384) || (hashbitlen == 512) ){
    /* 384- and 512-bit versions (64-bit words) */

    if (hashbitlen == 384) 
      memcpy( blakeState.h64, blake_IV384, sizeof(blake_IV384) );      
    else 
      memcpy( blakeState.h64, blake_IV512, sizeof(blake_IV512) );

    blakeState.t64[0] = 0;
    blakeState.t64[1] = 0;

    for(i=0; i<64; ++i)
      blakeState.data64[i] = 0;
    
    blakeState.salt64[0] = 0;
    blakeState.salt64[1] = 0;
    blakeState.salt64[2] = 0;
    blakeState.salt64[3] = 0;    

    
  }
  else
    return BAD_HASHBITLEN;

  blakeState.hashbitlen = hashbitlen;
  blakeState.datalen = 0;
  blakeState.init = 1;
  blakeState.nullt = 0;

  return SUCCESS;
}



int Blake::AddSalt( const BitSequence * salt ) {


  /* if hashbitlen=224 or 256, then the salt should be 128-bit (16 bytes) */
  /* if hashbitlen=384 or 512, then the salt should be 256-bit (32 bytes) */

  /* fail if Init() was not called before */
  if (blakeState.init != 1) 
    return FAIL;

  if ( blakeState.hashbitlen < 384 ) {
    blakeState.salt32[0] = BLAKE_U8TO32_BE(salt + 0);
    blakeState.salt32[1] = BLAKE_U8TO32_BE(salt + 4);
    blakeState.salt32[2] = BLAKE_U8TO32_BE(salt + 8);
    blakeState.salt32[3] = BLAKE_U8TO32_BE(salt +12);
  }
  else {
    blakeState.salt64[0] = BLAKE_U8TO64_BE(salt + 0);
    blakeState.salt64[1] = BLAKE_U8TO64_BE(salt + 8);
    blakeState.salt64[2] = BLAKE_U8TO64_BE(salt +16);
    blakeState.salt64[3] = BLAKE_U8TO64_BE(salt +24);
  }

  return SUCCESS;
}



int Blake::Update32( const BitSequence * data, DataLength databitlen ) {


  int fill;
  int left; /* to handle data inputs of up to 2^64-1 bits */
  
  if ( ( databitlen == 0 ) && (blakeState.datalen != 512 ) )
    return SUCCESS;

  left = (blakeState.datalen >> 3); 
  fill = 64 - left;

  /* compress remaining data filled with new bits */
  if( left && ( ((databitlen >> 3) & 0x3F) >= fill ) ) {
    memcpy( (void *) (blakeState.data32 + left),
	    (void *) data, fill );
    /* update counter */
    blakeState.t32[0] += 512;
    if (blakeState.t32[0] == 0)
      blakeState.t32[1]++;
      
    compress32( blakeState.data32 );
    data += fill;
    databitlen  -= (fill << 3); 
      
    left = 0;
  }

  /* compress data until enough for a block */
  while( databitlen >= 512 ) {

    /* update counter */
    blakeState.t32[0] += 512;

    if (blakeState.t32[0] == 0)
      blakeState.t32[1]++;
    compress32( data );
    data += 64;
    databitlen  -= 512;
  }
  
  if( databitlen > 0 ) {
    memcpy( (void *) (blakeState.data32 + left),
	    (void *) data, databitlen>>3 );
    blakeState.datalen = (left<<3) + databitlen;
    /* when non-8-multiple, add remaining bits (1 to 7)*/
    if ( databitlen & 0x7 )
      blakeState.data32[left + (databitlen>>3)] = data[databitlen>>3];
  }
  else
    blakeState.datalen=0;


  return SUCCESS;
}

int Blake::Update64( const BitSequence * data, DataLength databitlen ) {


  int fill;
  int left;

  if ( ( databitlen == 0 ) && (blakeState.datalen != 1024 ) )
    return SUCCESS;

  left = (blakeState.datalen >> 3);
  fill = 128 - left;

  /* compress remaining data filled with new bits */
  if( left && ( ((databitlen >> 3) & 0x7F) >= fill ) ) {
    memcpy( (void *) (blakeState.data64 + left),
	    (void *) data, fill );
    /* update counter  */
   blakeState.t64[0] += 1024;

   compress64( blakeState.data64 );
   data += fill;
   databitlen  -= (fill << 3); 
      
    left = 0;
  }

  /* compress data until enough for a block */
  while( databitlen >= 1024 ) {
  
    /* update counter */
   blakeState.t64[0] += 1024;
   compress64( data );
    data += 128;
    databitlen  -= 1024;
  }

  if( databitlen > 0 ) {
    memcpy( (void *) (blakeState.data64 + left),
	    (void *) data, ( databitlen>>3 ) & 0x7F );
    blakeState.datalen = (left<<3) + databitlen;

    /* when non-8-multiple, add remaining bits (1 to 7)*/
    if ( databitlen & 0x7 )
      blakeState.data64[left + (databitlen>>3)] = data[databitlen>>3];
  }
  else
    blakeState.datalen=0;

  return SUCCESS;
}


int Blake::Update(const BitSequence * data, DataLength databitlen ) {

  if ( blakeState.hashbitlen < 384 )
    return Update32( data, databitlen );
  else
    return Update64( data, databitlen );
}


int Blake::Final32( BitSequence * hashval ) {


  unsigned char msglen[8];
  BitSequence zz=0x00,zo=0x01,oz=0x80,oo=0x81;

  /* 
     copy nb. bits hash in total as a 64-bit BE word
  */
  unsigned int low, high;
  low  = blakeState.t32[0] + blakeState.datalen;
  high = blakeState.t32[1];
  if ( low < blakeState.datalen )
    high++;
  BLAKE_U32TO8_BE(  msglen + 0, high );
  BLAKE_U32TO8_BE(  msglen + 4, low  );

  if ( blakeState.datalen % 8 == 0) {
    /* message bitlength multiple of 8 */

    if ( blakeState.datalen == 440 ) {
      /* special case of one blake_padding byte */
      blakeState.t32[0] -= 8;
      if ( blakeState.hashbitlen == 224 ) 
	Update32( &oz, 8 );
      else
	Update32( &oo, 8 );
    }
    else {
      if ( blakeState.datalen < 440 ) {
	/* use t=0 if no remaining data */
	if ( blakeState.datalen == 0 ) 
	  blakeState.nullt=1;
	/* enough space to fill the block  */
	blakeState.t32[0] -= 440 - blakeState.datalen;
	Update32( blake_padding, 440 - blakeState.datalen );
      }
      else {
	/* NOT enough space, need 2 compressions */
	blakeState.t32[0] -= 512 - blakeState.datalen;
	Update32( blake_padding, 512 - blakeState.datalen );
	blakeState.t32[0] -= 440;
	Update32( blake_padding+1, 440 );  /* padd with zeroes */
	blakeState.nullt = 1; /* raise flag to set t=0 at the next compress */
      }
      if ( blakeState.hashbitlen == 224 ) 
	Update32( &zz, 8 );
      else
	Update32( &zo, 8 );
      blakeState.t32[0] -= 8;
    }
    blakeState.t32[0] -= 64;
    Update32( msglen, 64 );    
  }
  else {  
    /* message bitlength NOT multiple of 8 */

    /*  add '1' */
    blakeState.data32[blakeState.datalen/8] &= (0xFF << (8-blakeState.datalen%8)); 
    blakeState.data32[blakeState.datalen/8] ^= (0x80 >> (blakeState.datalen%8)); 

    if (( blakeState.datalen > 440 ) && ( blakeState.datalen < 447 )) {
      /*  special case of one blake_padding byte */
      if ( blakeState.hashbitlen == 224 ) 
	blakeState.data32[blakeState.datalen/8] ^= 0x00;
      else
	blakeState.data32[blakeState.datalen/8] ^= 0x01;
      blakeState.t32[0] -= (8 - (blakeState.datalen%8));
      /* set datalen to a 8 multiple */
      blakeState.datalen = (blakeState.datalen&(DataLength)0xfffffffffffffff8ULL)+8;
    }
    else { 
      if (blakeState.datalen < 440) {
	/* enough space to fill the block */
	blakeState.t32[0] -= 440 - blakeState.datalen;
	blakeState.datalen = (blakeState.datalen&(DataLength)0xfffffffffffffff8ULL)+8;
	Update( blake_padding+1, 440 - blakeState.datalen );
      }
      else { 
	if (blakeState.datalen > 504 ) {
	  /* special case */
	  blakeState.t32[0] -= 512 - blakeState.datalen;
	  blakeState.datalen=512;
	  Update32( blake_padding+1, 0 );
	  blakeState.t32[0] -= 440;
	  Update32( blake_padding+1, 440 );
	  blakeState.nullt = 1; /* raise flag for t=0 at the next compress */
	}
	else {
	  /* NOT enough space, need 2 compressions */
	  blakeState.t32[0] -= 512 - blakeState.datalen;
	  /* set datalen to a 8 multiple */
	  blakeState.datalen = (blakeState.datalen&(DataLength)0xfffffffffffffff8ULL)+8;
	  Update32( blake_padding+1, 512 - blakeState.datalen );
	  blakeState.t32[0] -= 440;
	  Update32( blake_padding+1, 440 );
	  blakeState.nullt = 1; /* raise flag for t=0 at the next compress */
	}
      }
      blakeState.t32[0] -= 8;
      if ( blakeState.hashbitlen == 224 ) 
	Update32( &zz, 8 );
      else
	Update32( &zo, 8 );
    }
    blakeState.t32[0] -= 64;
    Update32( msglen, 64 ); 
  }

  BLAKE_U32TO8_BE( hashval + 0, blakeState.h32[0]);
  BLAKE_U32TO8_BE( hashval + 4, blakeState.h32[1]);
  BLAKE_U32TO8_BE( hashval + 8, blakeState.h32[2]);
  BLAKE_U32TO8_BE( hashval +12, blakeState.h32[3]);
  BLAKE_U32TO8_BE( hashval +16, blakeState.h32[4]);
  BLAKE_U32TO8_BE( hashval +20, blakeState.h32[5]);
  BLAKE_U32TO8_BE( hashval +24, blakeState.h32[6]);

  if ( blakeState.hashbitlen == 256 ) {
    BLAKE_U32TO8_BE( hashval +28, blakeState.h32[7]);
  }
  
  return SUCCESS;
}


int Blake::Final64( BitSequence * hashval ) {


  unsigned char msglen[16];
  BitSequence zz=0x00,zo=0x01,oz=0x80,oo=0x81;

  /* copy nb. bits hash in total as a 128-bit BE word */
  unsigned long long low, high;
  low  = blakeState.t64[0] + blakeState.datalen;
  high = blakeState.t64[1];
  if ( low < blakeState.datalen )
    high++;
  BLAKE_U64TO8_BE(  msglen + 0, high );
  BLAKE_U64TO8_BE(  msglen + 8, low  );

  if ( blakeState.datalen % 8 == 0) {
    /* message bitlength multiple of 8 */

    if ( blakeState.datalen == 888 ) {
      /* special case of one blake_padding byte */
      blakeState.t64[0] -= 8; 
      if ( blakeState.hashbitlen == 384 ) 
	Update64( &oz, 8 );
      else
	Update64( &oo, 8 );
    }
    else {
      if ( blakeState.datalen < 888 ) {
	/* use t=0 if no remaining data */
	if ( blakeState.datalen == 0 ) 
	  blakeState.nullt=1;
	/* enough space to fill the block */
	blakeState.t64[0] -= 888 - blakeState.datalen;
	Update64( blake_padding, 888 - blakeState.datalen );
      }
      else { 
	/* NOT enough space, need 2 compressions */
	blakeState.t64[0] -= 1024 - blakeState.datalen; 
	Update64( blake_padding, 1024 - blakeState.datalen );
	blakeState.t64[0] -= 888;
	Update64( blake_padding+1, 888 );  /* padd with zeros */
	blakeState.nullt = 1; /* raise flag to set t=0 at the next compress */
      }
      if ( blakeState.hashbitlen == 384 ) 
	Update64( &zz, 8 );
      else
	Update( &zo, 8 );
      blakeState.t64[0] -= 8;
    }
    blakeState.t64[0] -= 128;
    Update( msglen, 128 );    
  }
  else {  
    /* message bitlength NOT multiple of 8 */

    /* add '1' */
    blakeState.data64[blakeState.datalen/8] &= (0xFF << (8-blakeState.datalen%8)); 
    blakeState.data64[blakeState.datalen/8] ^= (0x80 >> (blakeState.datalen%8)); 

    if (( blakeState.datalen > 888 ) && ( blakeState.datalen < 895 )) {
      /*  special case of one blake_padding byte */
      if ( blakeState.hashbitlen == 384 ) 
	blakeState.data64[blakeState.datalen/8] ^= zz;
      else
	blakeState.data64[blakeState.datalen/8] ^= zo;
      blakeState.t64[0] -= (8 - (blakeState.datalen%8));
      /* set datalen to a 8 multiple */
      blakeState.datalen = (blakeState.datalen&(DataLength)0xfffffffffffffff8ULL)+8;
    }
    else { 
      if (blakeState.datalen < 888) {
	/* enough space to fill the block */
	blakeState.t64[0] -= 888 - blakeState.datalen;
	blakeState.datalen = (blakeState.datalen&(DataLength)0xfffffffffffffff8ULL)+8;
	Update64( blake_padding+1, 888 - blakeState.datalen );
      }
      else {
	if (blakeState.datalen > 1016 ) {
	  /* special case */
	  blakeState.t64[0] -= 1024 - blakeState.datalen;
	  blakeState.datalen=1024;
	  Update64( blake_padding+1, 0 );
	  blakeState.t64[0] -= 888;
	  Update64( blake_padding+1, 888 );
	  blakeState.nullt = 1; /* raise flag for t=0 at the next compress */
	}
	else {
	  /* NOT enough space, need 2 compressions */
	  blakeState.t64[0] -= 1024 - blakeState.datalen;
	  /* set datalen to a 8 multiple */
	  blakeState.datalen = (blakeState.datalen&(DataLength)0xfffffffffffffff8ULL)+8;
	  Update64( blake_padding+1, 1024 - blakeState.datalen );
	  blakeState.t64[0] -= 888;
	  Update64( blake_padding+1, 888 );
	  blakeState.nullt = 1; /* raise flag for t=0 at the next compress */
	}
      }
      blakeState.t64[0] -= 8;
      if ( blakeState.hashbitlen == 384 ) 
	Update64( &zz, 8 );
      else
	Update64( &zo, 8 );
    }
    blakeState.t64[0] -= 128;
    Update( msglen, 128 ); 
  }

  BLAKE_U64TO8_BE( hashval + 0, blakeState.h64[0]);
  BLAKE_U64TO8_BE( hashval + 8, blakeState.h64[1]);
  BLAKE_U64TO8_BE( hashval +16, blakeState.h64[2]);
  BLAKE_U64TO8_BE( hashval +24, blakeState.h64[3]);
  BLAKE_U64TO8_BE( hashval +32, blakeState.h64[4]);
  BLAKE_U64TO8_BE( hashval +40, blakeState.h64[5]);

  if ( blakeState.hashbitlen == 512 ) {
    BLAKE_U64TO8_BE( hashval +48, blakeState.h64[6]);
    BLAKE_U64TO8_BE( hashval +56, blakeState.h64[7]);
  }
  
  return SUCCESS;
}

int Blake::Final( BitSequence * hashval ) {
  
  if ( blakeState.hashbitlen < 384 )
    return Final32( hashval );
  else
    return Final64( hashval );
}

int Blake::Hash( int hashbitlen, const BitSequence * data, DataLength databitlen, 
		 BitSequence * hashval ) {

  int ret;
  //hashState state;

  ret = Blake::Init( hashbitlen );
  if ( ret != SUCCESS ) return ret;

  ret = Blake::Update( data, databitlen );
  if ( ret != SUCCESS ) return ret;

  ret = Blake::Final( hashval );

 return ret;

}