/* FILE: essence_compress_256.c
 *
 * AUTHOR: Jason Worth Martin <jason.worth.martin@gmail.com>
 *
 * DESCRIPTION:  This file implements the ESSENCE-256 compression
 * function in fully portable ANSI C99.  IT IS NOT OPTIMIZED!
 *
 */
#include "essence.h"

void essence_compress_256(unsigned int *Chaining_Variables,
			  unsigned char *input,
			  unsigned long long input_size_in_256_bit_blocks,
			  unsigned long long num_steps)
{
  unsigned int r[8];
  unsigned int r_orig[8];
  unsigned int k[8];
  unsigned int i,j;
  unsigned int tmp_r, tmp_k, F_r, F_k;

#if (ESSENCE_DEBUG_LEVEL & 0x08)
  printf("\n*** Entering 256-bit Compression Function ***\n");
  for(i=0;i<8;i++)
    {
      printf("Chaining_Variables[%i] = 0x%.8x\n",i,
	     Chaining_Variables[i]);
    }
  printf("input_size_in_256_bit_blocks = %lli\n",
	 input_size_in_256_bit_blocks);
  printf("num_steps = %lli\n",
	 num_steps);
  
#endif

  /*
   * Load the chaining variables into r0-r7.
   */
  for(i=0;i<8;i++)
    {
      r[i] = Chaining_Variables[i];
    }

  /*
   * Main compression loop
   */
  while(input_size_in_256_bit_blocks>0)
    {
      /*
       * Read in the input
       */
#if (ESSENCE_DEBUG_LEVEL & 0x08)
      printf("Next 32 bytes of input:");
      for(i=0;i<8;i++)
	{
	  printf("\n input bytes %2i - %2i: ",
		 (i*4),
		 (i*4+3));
	  for(j=0;j<4;j++)
	    {
	      printf("0x%.2x ",input[i*4+j]);
	    }
	}
      printf("\n\nBeginning compression loop\n\n");
      fflush(stdout);
#endif
      for(i=0;i<8;i++)
	{
	  /*
	   * Here we go through some contortions to deal with Endian
	   * issues.  Our standard defines the data as Little Endian,
	   * but we force it just in case we are on a Big Endian
	   * machine.
	   */
	  tmp_k = ( (0x000000ff &  (unsigned int)(*input)         ) |
		    (0x0000ff00 & ((unsigned int)(*(input+1)) << 8 )) |
		    (0x00ff0000 & ((unsigned int)(*(input+2)) << 16)) |
		    (0xff000000 & ((unsigned int)(*(input+3)) << 24)) );
	  input += 4;
	  k[i] = tmp_k;
	}
      /*
       * Store the values of r for the xor at
       * the end of the stepping.
       */
      for(i=0;i<8;i++)
	{
	  r_orig[i] = r[i];
	}
      for(i=0;i<num_steps;i++)
	{
	  tmp_r = r[0];
	  tmp_k = k[0];

	  /*
	   * This implements L_32 on r0 and k0
	   */
	  tmp_r = L_32_table[tmp_r >> 24] ^ (tmp_r << 8);
	  tmp_k = L_32_table[tmp_k >> 24] ^ (tmp_k << 8);

	  tmp_r = L_32_table[tmp_r >> 24] ^ (tmp_r << 8);
	  tmp_k = L_32_table[tmp_k >> 24] ^ (tmp_k << 8);

	  tmp_r = L_32_table[tmp_r >> 24] ^ (tmp_r << 8);
	  tmp_k = L_32_table[tmp_k >> 24] ^ (tmp_k << 8);

	  tmp_r = L_32_table[tmp_r >> 24] ^ (tmp_r << 8);
	  tmp_k = L_32_table[tmp_k >> 24] ^ (tmp_k << 8);
	  /*
	   * Done with L_32.
	   *
	   * At this point:
	   *
	   *     tmp_r = L_32(r[0])
	   *     tmp_k = L_32(k[0])
	   */
	  F_r = F_func(r[6],r[5],r[4],r[3],r[2],r[1],r[0]);
	  F_k = F_func(k[6],k[5],k[4],k[3],k[2],k[1],k[0]);
#if (ESSENCE_DEBUG_LEVEL & 0x08)
	  printf("\n--- Step Number %i\n",i);
	  printf("    Values before stepping logic:\n");
	  for(j=0;j<8;j++)
	    {
	      printf("    k[%i] = 0x%.8x\n",j,k[j]);
	    }
	  for(j=0;j<8;j++)
	    {
	      printf("    r[%i] = 0x%.8x\n",j,r[j]);
	    }
	  printf("    F(k[6],k[5],k[4],k[3],k[2],k[1],k[0]) = 0x%.8x\n",F_k);
	  printf("    F(r[6],r[5],r[4],r[3],r[2],r[1],r[0]) = 0x%.8x\n",F_r);
	  printf("    L(k[0]) = 0x%.8x\n",tmp_k);
	  printf("    L(r[0]) = 0x%.8x\n",tmp_r);
#endif
	  tmp_r ^=  F_r ^ r[7];
	  tmp_r ^= k[7];

	  tmp_k ^=  F_k ^ k[7];

	  r[7] = r[6];
	  k[7] = k[6];
	  r[6] = r[5];
	  k[6] = k[5];
	  r[5] = r[4];
	  k[5] = k[4];
	  r[4] = r[3];
	  k[4] = k[3];
	  r[3] = r[2];
	  k[3] = k[2];
	  r[2] = r[1];
	  k[2] = k[1];
	  r[1] = r[0];
	  k[1] = k[0];
	  r[0] = tmp_r;
	  k[0] = tmp_k;
#if (ESSENCE_DEBUG_LEVEL & 0x08)
	  printf("    Values after stepping logic:\n");
	  for(j=0;j<8;j++)
	    {
	      printf("    k[%i] = 0x%.8x\n",j,k[j]);
	    }
	  for(j=0;j<8;j++)
	    {
	      printf("    r[%i] = 0x%.8x\n",j,r[j]);
	    }
#endif
	}

      /*
       * Final xor
       */
      for(i=0;i<8;i++)
	{
	  r[i] ^= r_orig[i];
	}

#if (ESSENCE_DEBUG_LEVEL & 0x08)
      printf("\nEnd compression loop\n");
      printf(" Values after final xor:\n");
      for(j=0;j<8;j++)
	{
	  printf("   r[%i] = 0x%.8x\n",j,r[j]);
	}
#endif
      --input_size_in_256_bit_blocks;
    }

  /*
   * Write out the chaining variables.
   */
  for(i=0;i<8;i++)
    {
      Chaining_Variables[i] = r[i];
    }
#if (ESSENCE_DEBUG_LEVEL & 0x08)
  printf("\n*** Leaving 256-bit Compression Function ***\n");
  for(i=0;i<8;i++)
    {
      printf("Chaining_Variables[%i] = 0x%.8x\n",i,
	     Chaining_Variables[i]);
    }
#endif
}
