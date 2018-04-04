/* FILE: essence_compress_512
 *
 * AUTHOR: Jason Worth Martin <jason.worth.martin@gmail.com>
 *
 * DESCRIPTION:  This file implements the ESSENCE-512 compression
 * function in fully portable ANSI C99.  IT IS NOT OPTIMIZED!
 *
 */
#include "essence.h"

void essence_compress_512(unsigned long long *Chaining_Variables,
			  unsigned char *input,
			  unsigned long long input_size_in_512_bit_blocks,
			  unsigned long long num_steps)
{
  unsigned long long r[8];
  unsigned long long r_orig[8];
  unsigned long long k[8];
  unsigned long long i,j;
  unsigned long long tmp_r, tmp_k, F_r, F_k;

#if (ESSENCE_DEBUG_LEVEL & 0x08)
  printf("\n*** Entering 512-bit Compression Function ***\n");
  for(i=0;i<8;i++)
    {
      printf("Chaining_Variables[%i] = 0x%.16llx\n",i,
	     Chaining_Variables[i]);
    }
  printf("input_size_in_512_bit_blocks = %lli\n",
	 input_size_in_512_bit_blocks);
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
  while(input_size_in_512_bit_blocks>0)
    {
      /*
       * Read in the input
       */
#if (ESSENCE_DEBUG_LEVEL & 0x08)
      printf("Next 64 bytes of input:");
      for(i=0;i<8;i++)
	{
	  printf("\n input bytes %2i - %2i: ",
		 (i*8),
		 (i*8+3));
	  for(j=0;j<8;j++)
	    {
	      printf("0x%.2x ",input[i*8+j]);
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
	  tmp_k = ( (0x00000000000000ffLL &  (unsigned long long)(*input)           ) |
		    (0x000000000000ff00LL & ((unsigned long long)(*(input+1)) << 8 )) |
		    (0x0000000000ff0000LL & ((unsigned long long)(*(input+2)) << 16)) |
		    (0x00000000ff000000LL & ((unsigned long long)(*(input+3)) << 24)) |
		    (0x000000ff00000000LL & ((unsigned long long)(*(input+4)) << 32)) |
		    (0x0000ff0000000000LL & ((unsigned long long)(*(input+5)) << 40)) |
		    (0x00ff000000000000LL & ((unsigned long long)(*(input+6)) << 48)) |
		    (0xff00000000000000LL & ((unsigned long long)(*(input+7)) << 56)) );
	  input += 8;
	  k[i] = tmp_k;
	}

      /*
       * Save r0-r7 for final xor
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
	   * This implements L_64 on r0 and k0
	   */
	  tmp_r = L_64_table[tmp_r >> 56] ^ (tmp_r << 8);
	  tmp_k = L_64_table[tmp_k >> 56] ^ (tmp_k << 8);

	  tmp_r = L_64_table[tmp_r >> 56] ^ (tmp_r << 8);
	  tmp_k = L_64_table[tmp_k >> 56] ^ (tmp_k << 8);

	  tmp_r = L_64_table[tmp_r >> 56] ^ (tmp_r << 8);
	  tmp_k = L_64_table[tmp_k >> 56] ^ (tmp_k << 8);

	  tmp_r = L_64_table[tmp_r >> 56] ^ (tmp_r << 8);
	  tmp_k = L_64_table[tmp_k >> 56] ^ (tmp_k << 8);

	  tmp_r = L_64_table[tmp_r >> 56] ^ (tmp_r << 8);
	  tmp_k = L_64_table[tmp_k >> 56] ^ (tmp_k << 8);

	  tmp_r = L_64_table[tmp_r >> 56] ^ (tmp_r << 8);
	  tmp_k = L_64_table[tmp_k >> 56] ^ (tmp_k << 8);

	  tmp_r = L_64_table[tmp_r >> 56] ^ (tmp_r << 8);
	  tmp_k = L_64_table[tmp_k >> 56] ^ (tmp_k << 8);

	  tmp_r = L_64_table[tmp_r >> 56] ^ (tmp_r << 8);
	  tmp_k = L_64_table[tmp_k >> 56] ^ (tmp_k << 8);
	  /*
	   * Done with L_64.
	   *
	   * At this point:
	   *
	   *     tmp_r = L_64(r[0])
	   *     tmp_k = L_64(k[0])
	   */
	  F_r = F_func(r[6],r[5],r[4],r[3],r[2],r[1],r[0]);
	  F_k = F_func(k[6],k[5],k[4],k[3],k[2],k[1],k[0]);

#if (ESSENCE_DEBUG_LEVEL & 0x08)
	  printf("\n--- Step number %i\n",i);
	  printf("    Values before stepping logic:\n");
	  for(j=0;j<8;j++)
	    {
	      printf("    k[%i] = 0x%.16llx\n",j,k[j]);
	    }
	  for(j=0;j<8;j++)
	    {
	      printf("    r[%i] = 0x%.16llx\n",j,r[j]);
	    }
	  printf("    F(k[6],k[5],k[4],k[3],k[2],k[1],k[0]) = 0x%.16llx\n",F_k);
	  printf("    F(r[6],r[5],r[4],r[3],r[2],r[1],r[0]) = 0x%.16llx\n",F_r);
	  printf("    L(k[0]) = 0x%.16llx\n",tmp_k);
	  printf("    L(r[0]) = 0x%.16llx\n",tmp_r);
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
	      printf("    k[%i] = 0x%.16llx\n",j,k[j]);
	    }
	  for(j=0;j<8;j++)
	    {
	      printf("    r[%i] = 0x%.16llx\n",j,r[j]);
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
	  printf("   r[%i] = 0x%.16llx\n",j,r[j]);
	}
#endif

      --input_size_in_512_bit_blocks;
    }

  /*
   * Write out the chaining variables.
   */
  for(i=0;i<8;i++)
    {
      Chaining_Variables[i] = r[i];
    }
#if (ESSENCE_DEBUG_LEVEL & 0x08)
  printf("\n*** Leaving 512-bit Compression Function ***\n");
  for(i=0;i<8;i++)
    {
      printf("Chaining_Variables[%i] = 0x%.16llx\n",i,
	     Chaining_Variables[i]);
    }
#endif
}
