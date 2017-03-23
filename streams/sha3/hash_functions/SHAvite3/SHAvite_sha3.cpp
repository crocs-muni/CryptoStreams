#include "portable.h"
#include "SHAvite_sha3.h"
extern "C" {
#include "SHAvite3-256.h"
#include "SHAvite3-512.h"
}

u32 SHAVITE_IV_224[8] =  {0xC4C67795, 0xC0B1817F, 0xEAD88924, 0x1ABB1BB0,
		  0xE0C29152, 0xBDE046BA, 0xAEEECF99, 0x58D509D8};

u32 SHAVITE_IV_256[8] =  {0x3EECF551, 0xBF10819B, 0xE6DC8559, 0xF3E23FD5,
                  0x431AEC73, 0x79E3F731, 0x98325F05, 0xA92A31F1};

u32 SHAVITE_IV_384[16] = {0xCEB54AC9, 0xC36EA41F, 0x11D8B088, 0xBB38FF4F,
 		  0xCF73711D, 0xAB282A6E, 0x19D120D7, 0x232F65CD,
 		  0x0209951C, 0x984CE361, 0x91D48BE1, 0xCFF6BBAB,
 		  0x4E31CF3A, 0x4691F157, 0xCC6E4644, 0x0AD83828};

u32 SHAVITE_IV_512[16] = {0x6A14BA06, 0xEB784EBD, 0xAB3C0130, 0x63473C2D,
 		  0xFA564CEB, 0x336D2629, 0xE24E213E, 0xDBD15E12,
 		  0x5DA35195, 0xFEC384E7, 0xBE0B4A11, 0x6666ADE6,
 		  0xB4FFED9D, 0xF3E9C1F4, 0x5E683CFA, 0xF34CD4E9};

/* Initialization of the internal state of the hash function         */

int SHAvite::Init(int hashbitlen)
{

int i;


/* Initialization of the counter of number of bits that were hashed  */
/* so far                                                            */ 

   shaviteState.bitcount = 0;


/* Store the requested digest size                                   */

   shaviteState.DigestSize = hashbitlen;


/* Initialize the message block to empty                             */

   memset(shaviteState.buffer,0,128);


/* Set the input to the compression function to all zero             */

   memset(shaviteState.chaining_value,0,64); 


/* Set the message block to zero				     */

   memset(shaviteState.buffer,0,128);

   
/* Load the correct IV                                               */


   if (hashbitlen==224)
      {
	 for (i=0;i<8;i++) 
            SHAVITE_U32TO8_LITTLE(shaviteState.chaining_value + 4*i, SHAVITE_IV_224[i]);
         shaviteState.BlockSize = 512;
         return SUCCESS;
      }   
   if (hashbitlen==256)
      {
	 for (i=0;i<8;i++) 
            SHAVITE_U32TO8_LITTLE(shaviteState.chaining_value + 4*i, SHAVITE_IV_256[i]);
         shaviteState.BlockSize = 512;
         return SUCCESS;
      }   
   if (hashbitlen==384)
      {
	 for (i=0;i<16;i++) 
            SHAVITE_U32TO8_LITTLE(shaviteState.chaining_value + 4*i, SHAVITE_IV_384[i]);
         shaviteState.BlockSize = 1024;
         return SUCCESS;
      }   
   if (hashbitlen==512)
      {
	 for (i=0;i<16;i++) 
            SHAVITE_U32TO8_LITTLE(shaviteState.chaining_value + 4*i, SHAVITE_IV_512[i]);
         shaviteState.BlockSize = 1024;
         return SUCCESS;
      }   


/* If the digest size is not one of the official 4, return error     */

   return BAD_HASHBITLEN;
}



/* Compressing the input data, and updating the internal state       */

int SHAvite::Update (const BitSequence *data, DataLength
                   databitlen)
{

/* p is a pointer to the current location inside data that we need   */
/* to process (i.e., the first byte of the data which was not used   */
/* as an input to the compression function                           */
   u8 *p = (u8*)data;

/* len is the size of the data that was not process yet in bytes     */
   int len = databitlen>>3;

/* BlockSizeB is the size of the message block of the compression    */
/* function                                                          */
   int BlockSizeB = (shaviteState.BlockSize/8);

/* bufcnt stores the number of bytes that are were "sent" to the     */
/* compression function, but were not yet processed, as a full block */
/* has not been obtained                                             */
   int bufcnt= (shaviteState.bitcount>>3)%BlockSizeB;

/* local_bitcount contains the number of bits actually hashed so far */
   u64 local_bitcount;

/* If we had to process a message with partial bytes before, then    */
/* Update() should not have been called again.                       */
/* We just discard the extra bits, and inform the user               */

   if (shaviteState.bitcount&7ULL)
      {
         fprintf(stderr, "We are sorry, you are calling Update one time after\n"
 		         "what should have been the last call. We ignore\n"
		         "few bits of the input.\n");
         shaviteState.bitcount &= ~7ULL;
      }


/* load the number of bits hashed so far into local_bitcount         */

   local_bitcount=shaviteState.bitcount;


/* mark that we processed more bits                                  */

   shaviteState.bitcount += databitlen;


/* if the input contains a partial byte - store it independently     */

   if (databitlen&7)
      shaviteState.partial_byte = data[databitlen>>3];


/* Check if we have enough data to call the compression function     */
/* If not, just copy the input to the buffer of the message block    */

   if (bufcnt + len < BlockSizeB)
      {
         memcpy(&shaviteState.buffer[bufcnt], p, len);
         return SUCCESS;
      }


/* There is enough data to start calling the compression function.   */
/* We first check whether there is data remaining from previous      */
/* calls                                                             */

   if (bufcnt>0)
      {

/* Copy from the input the required number of bytes to fill a block  */

         memcpy(&shaviteState.buffer[bufcnt], p, BlockSizeB-bufcnt);

/* Update the location of the first byte that was not processed      */

         p += BlockSizeB-bufcnt;

/* Update the remaining number of bytes to process                   */

         len -= BlockSizeB-bufcnt;

/* Update the number of bits hashed so far (locally)                 */

         local_bitcount+=8*(BlockSizeB-bufcnt);

/* Call the respective compression function to process the current   */
/* block                                                             */

         if (shaviteState.DigestSize<257)
			 shavite_Compress256(shaviteState.buffer, shaviteState.chaining_value, local_bitcount, shaViteNumRounds256);
         else
			 shavite_Compress512(shaviteState.buffer, shaviteState.chaining_value, local_bitcount, shaViteNumRounds512);
    }


/* At this point, the only remaining data is from the message block  */
/* call the compression function as many times as possible, and      */
/* store the remaining bytes in the buffer                           */

/* Each step of the loop compresses BlockSizeB bytes                 */

   for( ; len>=BlockSizeB; len-=BlockSizeB, p+=BlockSizeB)
      {

/* Update the number of bits hashed so far (locally)                 */

         local_bitcount+=8*BlockSizeB;

/* Call the respective compression function to process the current   */
/* block                                                             */

         if (shaviteState.DigestSize<257)
            shavite_Compress256(p, shaviteState.chaining_value, local_bitcount, shaViteNumRounds256);
         else 
            shavite_Compress512(p, shaviteState.chaining_value, local_bitcount, shaViteNumRounds512);

      }


/* If there are still unprocessed bytes, store them locally and wait */
/* for more                                                          */

   if (len>0)
      memcpy(shaviteState.buffer, p, len);

   return SUCCESS;
}


/* Performing the padding scheme, and dealing with any remaining     */
/* bits                                                              */

int SHAvite::Final (BitSequence *hashval)
{

/* Stores inputs (message blocks) to the compression function        */

   u8 block[128];

/* Stores results (chaining value) of the compression function       */

   u8 result[64];

/* BlockSizeB is the size of the message block of the compression    */
/* function                                                          */

   int BlockSizeB = (shaviteState.BlockSize/8);

/* bufcnt stores the number of bytes that are were "sent" to the     */
/* compression function, but were not yet processed, as a full block */
/* has not been obtained                                             */

   int bufcnt= ((unative)shaviteState.bitcount>>3)%BlockSizeB;

   int i;

/* Copy the current chaining value into result (as a temporary step) */

   if (shaviteState.DigestSize < 257)
      memcpy(result, shaviteState.chaining_value, 32);
   else
      memcpy(result, shaviteState.chaining_value, 64);


/* Initialize block as the message block to compress with the bytes  */
/* that were not processed yet                                       */

   memset(block, 0, BlockSizeB);
   memcpy(block, shaviteState.buffer, bufcnt);


/* Pad the buffer with the byte which contains the fraction of bytes */
/* from and a bit equal to 1					     */

   block[bufcnt] = (shaviteState.partial_byte&\
      ~((0x80 >> (shaviteState.bitcount&7))-1)) | (0x80 >> (shaviteState.bitcount&7));


/* Compress the last block (according to the digest size)            */

  if (shaviteState.DigestSize<257) {


/* An additional message block is required if there are less than 10 */
/* more bytes for message length and digest length encoding          */

     if (bufcnt>=BlockSizeB-10)
        {

/* Compress the current block                                        */
           shavite_Compress256(block,result,shaviteState.bitcount, shaViteNumRounds256);

/* Generate the full padding block                                   */
           memset(block, 0, BlockSizeB);
           SHAVITE_U64TO8_LITTLE(block+BlockSizeB-10, shaviteState.bitcount);
           SHAVITE_U16TO8_LITTLE(block+BlockSizeB-2, shaviteState.DigestSize);

/* Compress the full padding block                                   */
           shavite_Compress256(block,result,0x0ULL, shaViteNumRounds256);

        }

     else

        {

/* Pad the number of bits hashed so far and the digest size to the  */
/* last message block and compress it				    */
           SHAVITE_U64TO8_LITTLE(block+BlockSizeB-10, shaviteState.bitcount);
           SHAVITE_U16TO8_LITTLE(block+BlockSizeB-2, shaviteState.DigestSize);
	   if ((shaviteState.bitcount % shaviteState.BlockSize)==0)
              shavite_Compress256(block,result, 0x0ULL, shaViteNumRounds256);
           else
              shavite_Compress256(block,result, shaviteState.bitcount, shaViteNumRounds256);
        }
   }

/* In case the requested digest size is more than 256 bits           */ 

   else

      {

/* An additional message block is required if there are less than 18 */
/* more bytes for message length and digest length encoding          */
         if (bufcnt>=BlockSizeB-18)
            {

/* Compress the current block                                        */
               shavite_Compress512(block,result,shaviteState.bitcount, shaViteNumRounds512);

/* Generate the full padding block                                   */
               memset(block, 0, BlockSizeB);
               SHAVITE_U64TO8_LITTLE(block+BlockSizeB-18, shaviteState.bitcount);

/* The following line is due to the required API of 64-bit message   */
/* length, while the hash function deals with 128-bit lengths        */
               memset(block+BlockSizeB-10,0,8);
               SHAVITE_U16TO8_LITTLE(block+BlockSizeB-2, shaviteState.DigestSize);

/* Compress the full padding block                                   */
               shavite_Compress512(block,result,0x0ULL, shaViteNumRounds512);
            }

         else

            {
/* Pad the number of bits hashed so far and the digest size to the   */
/* last message block and compress it				     */
               SHAVITE_U64TO8_LITTLE(block+BlockSizeB-18, shaviteState.bitcount);

/* The following line is due to the required API of 64-bit message   */
/* length, while the hash function deals with 128-bit lengths        */
               memset(block+BlockSizeB-10,0,8);
               SHAVITE_U16TO8_LITTLE(block+BlockSizeB-2, shaviteState.DigestSize);
	       if ((shaviteState.bitcount % shaviteState.BlockSize)==0)
                  shavite_Compress512(block,result, 0x0ULL, shaViteNumRounds512);
               else
                  shavite_Compress512(block,result, shaviteState.bitcount, shaViteNumRounds512);

            }
      }


/* Copy the result into the supplied array of bytes.                 */

   for (i=0;i<(shaviteState.DigestSize+7)/8;i++)
       hashval[i]=result[i];


/* Treat cases where the digest size is not a multiple of a byte     */

   if ((shaviteState.DigestSize)&7)
       hashval[(shaviteState.DigestSize+7)/8] &= \
              (0xFF<<(8-((shaviteState.DigestSize)%8)))&0xFF;

   return SUCCESS;

}

/* Hashing a message, from initialization till the end               */

int SHAvite::Hash (int hashbitlen, const BitSequence *data,
                 DataLength databitlen, BitSequence *hashval)
{
   int Status;
   //hashState state;

   Status = SHAvite::Init(hashbitlen);
   if (Status != SUCCESS) return (Status);
   Status = SHAvite::Update (data, databitlen);
   if (Status != SUCCESS) return (Status);
   Status = SHAvite::Final (hashval);
   if (Status != SUCCESS) return (Status);

   return (SUCCESS);

}

SHAvite::SHAvite(const int numRounds) {
	if (numRounds == -1 || numRounds == 0) {
		shaViteNumRounds256 = SHAVITE_NUM_ROUNDS_256;
		shaViteNumRounds512 = SHAVITE_NUM_ROUNDS_512;
	} else {
		shaViteNumRounds256 = numRounds;
		shaViteNumRounds512 = numRounds;
	}
}