#include <stdio.h>
extern "C" {
#include "essence.h"
}
#include "Essence_sha3.h"


unsigned long long essence_expansion_of_pi_64[8] = {
  0x243f6a8885a308d3LL,
  0x13198a2e03707344LL,
  0xa4093822299f31d0LL,
  0x082efa98ec4e6c89LL,
  0x452821e638d01377LL,
  0xbe5466cf34e90c6cLL,
  0xc0ac29b7c97c50ddLL,
  0x3f84d5b5b5470917LL
};

/*
 * Expansion of the fractional part of pi give as 32-bit unsigned
 * integers.  Note that the ESSENCE standard uses Little Endian
 * ordering, so we have ordered these entries to match the 64-bit
 * version.
 */
unsigned int essence_expansion_of_pi_32[8] = {
  0x85a308d3,
  0x243f6a88,
  0x03707344,
  0x13198a2e,
  0x299f31d0,
  0xa4093822,
  0xec4e6c89,
  0x082efa98
};
/* *******************************************************************
 *
 * HashReturn Init(hashState *state,
 *                 int hashbitlen)
 *
 * state -- a structure that holds the hashState information
 *
 * hashbitlen -- an integer value that indicates the length of the
 *               hash output in bits.
 *
 * Initializes the hash state structure.
 *
 * ******************************************************************/
int Essence::Init(int hashbitlen)
{
  unsigned long long tmp_64;
  unsigned int tmp_32;
  unsigned long long *qw_array;
  unsigned int *dw_array;
  unsigned char *byte_array;
  int i;

  if ((hashbitlen < 128) ||
      (hashbitlen > 512))
    {
      /*
       * If we have an invalid hashbitlen, then
       * return the error condition.
       */
      return(BAD_HASHBITLEN);
    }

  /*
   * Initialize the running_hash to digits of pi.
   */
  if (hashbitlen > 256)
    {
      for(i=0;i<8;i++)
	{
	  essenceState.running_hash[i] = essence_expansion_of_pi_64[i];
	}
    }
  else
    {
      for(i=0;i<8;i++)
	{
	  ((unsigned int*)(essenceState.running_hash))[i] = essence_expansion_of_pi_32[i];
	}
      /*
       * This isn't actually necessary, but we want to make
       * debugging easier, so we initialize the rest to zero.
       */
      for(i=4;i<8;i++)
	{
	  essenceState.running_hash[i] = 0LL;
	}      
    }



  /*
   * BEGIN: initialize MDBIV_init.
   *
   * See essence_api.h for a description of this variable.
   *
   * We go through some contortions here to deal with endian issues.
   * The standard defines all our values in LITTLE ENDIAN terms,
   * but we want this code to run on big endian machines as well,
   * so we force the issue by setting up the MDBIV_init array
   * appropriately for the target architecture.  This way, within
   * the update function we can simply copy the array using the
   * native byte ordering.
   *
   * Since the byte ordering will depend up if we are using 32-bit
   * or 64-bit registers for the compression function, we will
   * have separate cases for using the 256-bit compression function
   * and the 512-bit compression function.
   *
   * We use the qw_array (qw stands for quadword which is the Intel
   * terminology for a 64-bit integer) to alias the MDBIV_init
   * array in the 512-bit case.
   *
   * We use the dw_array (dw stands for doubleword which is the Intel
   * terminology for a 32-bit integer) to alias the MDBIV_init
   * array in the 256-bit case.
   */

  if (hashbitlen > 256)
    {
      /*
       * Using the 512-bit compression function, so construct
       * array for 64-bit registers.
       */
      qw_array = (unsigned long long *)(essenceState.MDBIV_init);

      /*
       * Bytes 0-7: The current block number, which we initialize to
       * zero.  The Update function must set it as necessary.
       */
      qw_array[0] = 0LL;
      

      /*
       * Bytes 8-15: ESSENCE_MD_BLOCK_SIZE_IN_BYTES
       */
      qw_array[1] = (unsigned long long)(ESSENCE_MD_BLOCK_SIZE_IN_BYTES);
      
      /*
       * Bytes 16-17: hasbitlen
       */
      tmp_32 = (unsigned int)hashbitlen;
      tmp_64 = ((unsigned long long)tmp_32) & 0x000000000000ffffLL;

      
      /*
       * Byte 18: essenceNumRounds
       */
      tmp_64 |= ( (((unsigned long long)(essenceNumRounds)) << 16) & 
		  0x0000000000ff0000LL);
      
      /*
       * Byte 19: ESSENCE_HASH_TREE_LEVEL
       */
      tmp_64 |= ( (((unsigned long long)(ESSENCE_HASH_TREE_LEVEL)) << 24) & 
		  0x00000000ff000000LL);

      
      /*
       * Bytes 20-23: ESSENCE_ORGANIZATIONAL_SMALL_CONSTANT
       */
      tmp_32 = (unsigned int)(ESSENCE_ORGANIZATIONAL_SMALL_CONSTANT);
      tmp_64 |= ( (((unsigned long long)tmp_32) << 32) &
		  0xffffffff00000000LL);

      qw_array[2] = tmp_64;
      
      /*
       * Bytes 24-31: ESSENCE_ORGANIZATIONAL_BIG_CONSTANT
       */
      qw_array[3] = (unsigned long long)(ESSENCE_ORGANIZATIONAL_BIG_CONSTANT);
      
      /*
       * The remaining bytes are set to the default values.
       */
      qw_array[4] = essence_expansion_of_pi_64[4];
      qw_array[5] = essence_expansion_of_pi_64[5];
      qw_array[6] = essence_expansion_of_pi_64[6];
      qw_array[7] = essence_expansion_of_pi_64[7];
    }
  else
    {
      /*
       * Using the 256-bit compression function, so construct
       * array for 32-bit registers.
       */
      dw_array = (unsigned int *)(essenceState.MDBIV_init);

      /*
       * Bytes 0-7: The current block number, which we initialize to
       * zero.  The Update function must set it as necessary.
       */
      dw_array[0] = 0;
      dw_array[1] = 0;
      

      /*
       * Bytes 8-15: ESSENCE_MD_BLOCK_SIZE_IN_BYTES
       */
      tmp_64 = (unsigned long long)(ESSENCE_MD_BLOCK_SIZE_IN_BYTES);
      dw_array[2] = (unsigned int)tmp_64;
      dw_array[3] = (unsigned int)(tmp_64 >> 32);

      /*
       * Bytes 16-17: hasbitlen
       */
      tmp_32 = (unsigned int)hashbitlen;
      
      /*
       * Byte 18: essenceNumRounds
       */
      tmp_32 |= ( (((unsigned int)(essenceNumRounds)) << 16) & 
		  0x00ff0000);
      
      /*
       * Byte 19: ESSENCE_HASH_TREE_LEVEL
       */
      tmp_32 |= ( (((unsigned int)(ESSENCE_HASH_TREE_LEVEL)) << 24) & 
		  0xff000000);

      dw_array[4] = tmp_32;

      /*
       * Bytes 20-23: ESSENCE_ORGANIZATIONAL_SMALL_CONSTANT
       */
      dw_array[5] = (unsigned int)(ESSENCE_ORGANIZATIONAL_SMALL_CONSTANT);

      
      /*
       * Bytes 24-31: ESSENCE_ORGANIZATIONAL_BIG_CONSTANT
       */
      tmp_64 = (unsigned long long)(ESSENCE_ORGANIZATIONAL_BIG_CONSTANT);
      dw_array[6] = (unsigned int)tmp_64;
      dw_array[7] = (unsigned int)(tmp_64 >> 32);

      /*
       * Here we zero out the remaining space.
       */
      for(i=8;i<16;i++)
	{
	  dw_array[i] = 0;
	}
    }
  /*
   * END: initialize MDBIV_init
   */
  

  /*
   * Note: Since essenceState.last_md_block_number is a 64-bit value, it is
   * critical to specify the constant as a "long long" type.
   */
  essenceState.last_md_block_number = 0xffffffffffffffffLL;
  essenceState.residual_data_bitlen = 0;
  essenceState.hashbitlen = hashbitlen;
  essenceState.within_md_block = 0;
  essenceState.current_md_block_datalen = 0;

#if ESSENCE_DEBUG_LEVEL & 1
  printf("\n\n\n\n******** ******** ******** ");
  printf("******** ******** ******** LEAVING Init\n");
  Essence::PrintState();
#endif
  return(SUCCESS);
}
/* *******************************************************************
 *
 * void Join_512(unsigned long long *hash_a,
 *               unsigned long long *hash_b)
 *
 * Sets hash_a to be the JOIN of hash_a with hash_b.
 *
 * ******************************************************************/
void Essence::Join_512(unsigned long long *hash_a,
	      unsigned long long *hash_b)
{
  unsigned long long chain_vars[8];
  unsigned long long num_steps;
  unsigned long long i;

  /*
   * For Endian reasons, we need to copy the hashes into a temporary
   * buffer.
   */
  unsigned char data_buffer[128];
  int j;

  for(i=0;i<8;i++)
    {
      for(j=0;j<8;j++)
	{
	  data_buffer[i*8+j] = (unsigned char)(hash_a[i] >> (j*8));
	}
    }

  for(i=0;i<8;i++)
    {
      for(j=0;j<8;j++)
	{
	  data_buffer[64+i*8+j] = (unsigned char)(hash_b[i] >> (j*8));
	}
    }

  for(i=0;i<8;i++)
    {
      chain_vars[i] = essence_expansion_of_pi_64[i];
    }

  num_steps = essenceNumRounds;
  ESSENCE_COMPRESS_512(chain_vars,
		       data_buffer,
		       2LL,
		       num_steps);  

  /*
   * Now we copy the results out of the chain_vars
   * and into the hash_a localtion.
   */
  for(i=0;i<8;i++)
    {
      hash_a[i] = chain_vars[i];
    }

}
/* *******************************************************************
 *
 * void Join_256(unsigned int *hash_a,
 *               unsigned int *hash_b)
 *
 * Sets hash_a to be the JOIN of hash_a with hash_b.
 *
 * ******************************************************************/
void Essence::Join_256(unsigned int *hash_a,
	      unsigned int *hash_b)
{
  unsigned int chain_vars[8];
  unsigned long long num_steps;
  unsigned long long i;

  /*
   * For Endian reasons, we need to copy the hashes into a temporary
   * buffer.
   */
  unsigned char data_buffer[32];
  int j;

  for(i=0;i<8;i++)
    {
      for(j=0;j<4;j++)
	{
	  data_buffer[i*4+j] = (unsigned char)(hash_a[i] >> (j*8));
	}
    }

  for(i=0;i<8;i++)
    {
      for(j=0;j<4;j++)
	{
	  data_buffer[32+i*4+j] = (unsigned char)(hash_b[i] >> (j*8));
	}
    }

  for(i=0;i<8;i++)
    {
      chain_vars[i] = essence_expansion_of_pi_32[i];
    }

  num_steps = (unsigned long long)(essenceNumRounds);
  ESSENCE_COMPRESS_256(chain_vars,
		       data_buffer,
		       2LL,
		       num_steps);  

  /*
   * Now we copy the results out of the chain_vars
   * and into the hash_a localtion.
   */
  for(i=0;i<8;i++)
    {
      hash_a[i] = chain_vars[i];
    }

}
/* *******************************************************************
 *
 * void Merge_Tree_512(hashState *state, unsigned long long *chain_vars)
 *
 * state -- a structure that holds the hashState information
 *
 * This function should be called exactly when a Merkle-Damgaard block
 * has been completed.  It will update the current Merkle hash tree
 * and the running hash as needed.  It will also reset the state
 * variables: within_md_block, current_md_block_datalen, and
 * last_md_block_number
 *
 * If the ESSENCE_HASH_TREE_LEVEL parameter is set to zero, then
 * the input chain_vars are JOINed with running hash.
 *
 * Otherwise, the input chain_vars are merged into the current
 * Merkle hash tree based on the following rules:
 *
 * Rule 1: A leaf node exists if the block that it correspons to has
 *         been hashed, in which case the value of the leaf is just
 *         the resulting hash.
 *
 * Rule 2: A node is defined to exist if at least one of its children
 *         exist.
 *
 * Rule 3: If both children of a node exists, then it's value is the JOIN
 *         of the values of its children.  If only one child of a node exists
 *         then its value is simple equal to the value of its child.
 *
 * 
 * Serial Implementation
 * ---------------------
 *
 * The following pseudo-code describes the serial algorithm used in
 * this function.  It uses ESSENCE_HASH_TREE_LEVEL storage locations
 * for intermediate tree hashes, and it has the property that every
 * fully populated subtree has a correct top-node value according to
 * the rules above.  The remaining partially populated subtrees may
 * easily have their correct hash values computed by the Final
 * function.  This algorithm assumes that the block hashes are
 * provided sequentially and in order of block number.
 *
 * The variables:
 *
 * merkle_tree_hashes -- An array of ESSENCE_HASH_TREE_LEVEL
 *                       intermediate storage locations.
 *
 * block_num -- the block number whose hash is being merged into the
 *              tree.
 *
 * blocks_per_tree -- The number of blocks used for each fully
 *                    populated Merkle hash tree, which is just
 *                    2^ESSENCE_HASH_TREE_LEVEL.
 *
 * hash -- The hash of the Merkle-Damgaard block.
 *
 * The algorithm:
 *
 * level_mask = 1
 * if block_num & level_mask then
 *   merkle_tree_hashes[0] = JOIN(merkle_tree_hashes[0],hash)
 *   for level in 1 to ESSENCE_HASH_TREE_LEVEL - 1 do
 *      level_mask <<= 1
 *      if block_num & level_mask then
 *         merkle_tree_hashes[level] = JOIN(merkle_tree_hashes[level],
 *                                          merkle_tree_hashes[level-1])
 *      else
 *         merkle_tree_hashes[level] = merkle_tree_hashes[level-1]
 *         return
 *      end if
 *   end for
 * else
 *    merkle_tree_hashes[0] = hash
 *    return
 * end if
 *
 * 
 * The way the algorithm works is that each fully populated subtree
 * gets computed and has its hash stored in its parent.  When only
 * one child has been fully populated, it's value is stored in the
 * parent which clears up the lower level storage locations for the
 * next child's subtrees.  When both children are fully populated, then
 * the results are Joined and the result is propigated up one level.
 *
 * ******************************************************************/
void Essence::Merge_Tree_512(unsigned long long *chain_vars)
{
  unsigned long long num_steps;
  unsigned long long *root_hash;
  int i,j, blocks_per_tree, leaf_num, level, level_mask;
  unsigned char byte_array[64];
  
  essenceState.last_md_block_number += 1;
  essenceState.within_md_block = 0;
  essenceState.current_md_block_datalen = 0;

  num_steps = (unsigned long long)(essenceNumRounds);

#if ESSENCE_HASH_TREE_LEVEL == 0
  /*
   * If the hash tree level is zero, then we just
   * compress the most recent hash with the running
   * hash.
   */
  for(i=0;i<8;i++)
    {
      for(j=0;j<8;j++)
	{
	  byte_array[i*8+j] = (unsigned char)(chain_vars[i] >> (8*j));
	}
    }
  ESSENCE_COMPRESS_512(essenceState.running_hash,
		       byte_array,
		       1LL,
		       num_steps);
#else /* ESSENCE_HASH_TREE_LEVEL > 0 */
  /*
   * Now we figure out which leaf our last_md_block was
   * in, and where in the tree to join it.
   */
  blocks_per_tree = (1 << ESSENCE_HASH_TREE_LEVEL);
  leaf_num = ((int)(essenceState.last_md_block_number)) % blocks_per_tree;
  
  level_mask = 1;
  level = 0;
  if (leaf_num & level_mask)
    {
      Essence::Join_512(((unsigned long long *)(essenceState.merkle_tree_hashes))+8*level,
	       chain_vars);
#if ESSENCE_DEBUG_LEVEL & 4
      printf("\n\n\n\n******** ******** ******** ");
      printf("******** ******** ******** LEAVING Essence::Join_512\n");
      Essence::PrintState();
#endif

      for(level=1; level < ESSENCE_HASH_TREE_LEVEL; level++)
	{
	  level_mask <<= 1;
	  if (leaf_num & level_mask)
	    {
	      Essence::Join_512(((unsigned long long *)(essenceState.merkle_tree_hashes))+8*level,
		       ((unsigned long long *)(essenceState.merkle_tree_hashes))+8*(level-1));
#if ESSENCE_DEBUG_LEVEL & 4
	      printf("\n\n\n\n******** ******** ******** ");
	      printf("******** ******** ******** LEAVING Essence::Join_512\n");
	      Essence::PrintState();
#endif

	    }
	  else
	    {
	      for(i=0;i<8;i++)
		{
		  (((unsigned long long *)(essenceState.merkle_tree_hashes))+8*level)[i] =
		    (((unsigned long long *)(essenceState.merkle_tree_hashes))+8*(level-1))[i];
		}
	      break;
	    }
	}
      /*
       * If we get to the root of the tree, then we need to
       * Join it with the running hash.
       */
      if (leaf_num == (blocks_per_tree - 1))
	{
	  root_hash = ( ((unsigned long long *)(essenceState.merkle_tree_hashes)) + 
			8*(ESSENCE_HASH_TREE_LEVEL-1) );
	  /*
	   * Now, we have to setup for a call to the compression function.
	   */
	  for(i=0;i<8;i++)
	    {
	      for(j=0;j<8;j++)
		{
		  byte_array[i*8+j] = (unsigned char)(root_hash[i] >> (8*j));
		}
	    }
	  ESSENCE_COMPRESS_512(essenceState.running_hash,
			       byte_array,
			       1LL,
			       num_steps);
	}
    }
  else
    {
      for(i=0;i<8;i++)
	{
	  (((unsigned long long *)(essenceState.merkle_tree_hashes))+8*level)[i] =
	    chain_vars[i];
	}
    }
#endif /* ESSENCE_HASH_TREE_LEVEL > 0 */

#if ESSENCE_DEBUG_LEVEL & 2
  printf("\n\n\n\n******** ******** ******** ");
  printf("******** ******** ******** LEAVING Merge_Tree_512\n");
  Essence::PrintState();
#endif

}
/* *******************************************************************
 *
 * void Merge_Tree_256(hashState *state, unsigned int *chain_vars)
 *
 * state -- a structure that holds the hashState information
 *
 * This function should be called exactly when a Merkle-Damgaard block
 * has been completed.  It will update the current Merkle hash tree
 * and the running hash as needed.  It will also reset the state
 * variables: within_md_block, current_md_block_datalen, and
 * last_md_block_number
 *
 * If the ESSENCE_HASH_TREE_LEVEL parameter is set to zero, then
 * the input chain_vars are JOINed with running hash.
 *
 * Otherwise, the input chain_vars are merged into the current
 * Merkle hash tree based on the following rules:
 *
 * Rule 1: A leaf node exists if the block that it correspons to has
 *         been hashed, in which case the value of the leaf is just
 *         the resulting hash.
 *
 * Rule 2: A node is defined to exist if at least one of its children
 *         exist.
 *
 * Rule 3: If both children of a node exists, then it's value is the JOIN
 *         of the values of its children.  If only one child of a node exists
 *         then its value is simple equal to the value of its child.
 *
 * 
 * Serial Implementation
 * ---------------------
 *
 * The following pseudo-code describes the serial algorithm used in
 * this function.  It uses ESSENCE_HASH_TREE_LEVEL storage locations
 * for intermediate tree hashes, and it has the property that every
 * fully populated subtree has a correct top-node value according to
 * the rules above.  The remaining partially populated subtrees may
 * easily have their correct hash values computed by the Final
 * function.  This algorithm assumes that the block hashes are
 * provided sequentially and in order of block number.
 *
 * The variables:
 *
 * merkle_tree_hashes -- An array of ESSENCE_HASH_TREE_LEVEL
 *                       intermediate storage locations.
 *
 * block_num -- the block number whose hash is being merged into the
 *              tree.
 *
 * blocks_per_tree -- The number of blocks used for each fully
 *                    populated Merkle hash tree, which is just
 *                    2^ESSENCE_HASH_TREE_LEVEL.
 *
 * hash -- The hash of the Merkle-Damgaard block.
 *
 * The algorithm:
 *
 * level_mask = 1
 * if block_num & level_mask then
 *   merkle_tree_hashes[0] = JOIN(merkle_tree_hashes[0],hash)
 *   for level in 1 to ESSENCE_HASH_TREE_LEVEL - 1 do
 *      level_mask <<= 1
 *      if block_num & level_mask then
 *         merkle_tree_hashes[level] = JOIN(merkle_tree_hashes[level],
 *                                          merkle_tree_hashes[level-1])
 *      else
 *         merkle_tree_hashes[level] = merkle_tree_hashes[level-1]
 *         return
 *      end if
 *   end for
 * else
 *    merkle_tree_hashes[0] = hash
 *    return
 * end if
 *
 * 
 * The way the algorithm works is that each fully populated subtree
 * gets computed and has its hash stored in its parent.  When only
 * one child has been fully populated, it's value is stored in the
 * parent which clears up the lower level storage locations for the
 * next child's subtrees.  When both children are fully populated, then
 * the results are Joined and the result is propigated up one level.
 *
 * ******************************************************************/
void Essence::Merge_Tree_256(unsigned int *chain_vars)
{
  unsigned long long num_steps;
  unsigned int *root_hash;
  int i,j, blocks_per_tree, leaf_num, level, level_mask;
  unsigned char byte_array[32];
  
  essenceState.last_md_block_number += 1;
  essenceState.within_md_block = 0;
  essenceState.current_md_block_datalen = 0;

  num_steps = (unsigned long long)(essenceNumRounds);

#if ESSENCE_HASH_TREE_LEVEL == 0
  /*
   * If the hash tree level is zero, then we just
   * compress the most recent hash with the running
   * hash.
   */
  for(i=0;i<8;i++)
    {
      for(j=0;j<4;j++)
	{
	  byte_array[i*4+j] = (unsigned char)(chain_vars[i] >> (8*j));
	}
    }
  ESSENCE_COMPRESS_256((unsigned int *)(essenceState.running_hash),
		       byte_array,
		       1LL,
		       num_steps);
#else /* ESSENCE_HASH_TREE_LEVEL > 0 */
  /*
   * Now we figure out which leaf our last_md_block was
   * in, and where in the tree to join it.
   */
  blocks_per_tree = (1 << ESSENCE_HASH_TREE_LEVEL);
  leaf_num = ((int)(essenceState.last_md_block_number)) % blocks_per_tree;
  
  level_mask = 1;
  level = 0;
  if (leaf_num & level_mask)
    {
      Essence::Join_256(((unsigned int *)(essenceState.merkle_tree_hashes))+8*level,
	       chain_vars);
#if ESSENCE_DEBUG_LEVEL & 4
      printf("\n\n\n\n******** ******** ******** ");
      printf("******** ******** ******** LEAVING Essence::Join_256\n");
      Essence::PrintState();
#endif

      for(level=1; level < ESSENCE_HASH_TREE_LEVEL; level++)
	{
	  level_mask <<= 1;
	  if (leaf_num & level_mask)
	    {
	      Essence::Join_256(((unsigned int *)(essenceState.merkle_tree_hashes))+8*level,
		       ((unsigned int *)(essenceState.merkle_tree_hashes))+8*(level-1));
#if ESSENCE_DEBUG_LEVEL & 4
	      printf("\n\n\n\n******** ******** ******** ");
	      printf("******** ******** ******** LEAVING Essence::Join_256\n");
	      Essence::PrintState();
#endif

	    }
	  else
	    {
	      for(i=0;i<8;i++)
		{
		  (((unsigned int *)(essenceState.merkle_tree_hashes))+8*level)[i] =
		    (((unsigned int *)(essenceState.merkle_tree_hashes))+8*(level-1))[i];
		}
	      break;
	    }
	}
      /*
       * If we get to the root of the tree, then we need to
       * Join it with the running hash.
       */
      if (leaf_num == (blocks_per_tree - 1))
	{
	  root_hash = ( ((unsigned int *)(essenceState.merkle_tree_hashes)) + 
			8*(ESSENCE_HASH_TREE_LEVEL-1) );
	  /*
	   * Now, we have to setup for a call to the compression function.
	   */
	  for(i=0;i<8;i++)
	    {
	      for(j=0;j<4;j++)
		{
		  byte_array[i*4+j] = (unsigned char)(root_hash[i] >> (8*j));
		}
	    }
	  ESSENCE_COMPRESS_256((unsigned int *)(essenceState.running_hash),
			       byte_array,
			       1LL,
			       num_steps);
	}
    }
  else
    {
      for(i=0;i<8;i++)
	{
	  (((unsigned int *)(essenceState.merkle_tree_hashes))+8*level)[i] =
	    chain_vars[i];
	}
    }
#endif /* ESSENCE_HASH_TREE_LEVEL > 0 */
  
#if ESSENCE_DEBUG_LEVEL & 2
  printf("\n\n\n\n******** ******** ******** ");
  printf("******** ******** ******** LEAVING Essence::Merge_Tree_256\n");
  Essence::PrintState();
#endif

}
/* *******************************************************************
 *
 * HashReturn Update_256(hashState *state,
 *                       const BitSequence *data,
 *                       DataLength databitlen)
 *
 * state -- a structure that holds the hashState information
 *
 * data -- the data to be hashed
 *
 * databitlen -- the length, in bits, of the data to be hashed
 *
 * Updates the hash state structure with the given data.
 * Assumes that we will be using the 256-bit compression function.
 * This function should only be called by Update.
 * ******************************************************************/
int Essence::Update_256(const BitSequence *data,
		      DataLength databitlen)
{
  unsigned int *chain_vars;
  unsigned int *intermediate_tree_hashes;
  unsigned int *MDBIV_init;
  unsigned long long current_md_block_num;
  unsigned long long tmp_64;
  unsigned long long data_bytelen;
  unsigned long long num_steps;
  unsigned long long orig_md_block_num;
  unsigned long long residual_data_byte_index;
  unsigned long long residual_data_byte_offset;
  unsigned long long num_blocks;
  unsigned long long essence_md_block_size_in_256bit_blocks;
  unsigned long long tmp_buffer[8];
  unsigned char *residual_data;
  int i, j, num_bytes_to_copy, tmp, num_complete_md_blocks, start_point;


  /*
   * If we aren't getting enough new data to call the
   * compression function, then just copy it in and
   * exit
   */
  if ((essenceState.residual_data_bitlen + databitlen) < 256)
    {
      residual_data = (unsigned char *)(essenceState.residual_data);
      residual_data_byte_index = essenceState.residual_data_bitlen >> 3;
      data_bytelen = databitlen >>3;
      if (databitlen%8 != 0)
	{
	  data_bytelen++;
	}
      for (i=0;i<data_bytelen;i++)
	{
	  residual_data[residual_data_byte_index + i] = data[i];
	}
      essenceState.residual_data_bitlen += databitlen;
      if ((essenceState.residual_data_bitlen % 8) != 0)
	{
	  /*
	   * If the residual data doesn't end on a byte boundary, then
	   * we zero out the remaining bit positions.
	   */
	  residual_data_byte_index = ((essenceState.residual_data_bitlen) >> 3);
	  residual_data[residual_data_byte_index] =
	    (residual_data[residual_data_byte_index] & 
	     ((unsigned char)(0xff) << (8 - ((essenceState.residual_data_bitlen) % 8))) );
	}
      return(SUCCESS);
    }


  /*
   * If we make it to this point, then we know that we will be
   * calling the compression function, so we need to setup for
   * it.
   *
   * We set a variable to the number of steps of the compression
   * function.  While this might seem superfluous, it is actually
   * necessary to ensure that the constant essenceNumRounds
   * is correctly expanded by some compilers (e.g Apple's gcc version
   * 4.0.1 in 32-bit mode) for function calls.
   */

  num_steps = (unsigned long long)(essenceNumRounds);


  if (essenceState.residual_data_bitlen > 0)
    {
      /*
       * If we have any residual data from the last call, we
       * deal with it here:
       *
       */
      
      /*
       * If we are beginning a new Merkle-Damgaard block, then we must
       * set the chaining variables to the correct Merkle-Damgaard
       * Block Initialization Vector (MDBIV).
       */
      if (! essenceState.within_md_block)
	{
	  current_md_block_num = essenceState.last_md_block_number + 1;
	  essenceState.within_md_block = 1;
	  chain_vars = (unsigned int *)(essenceState.chain_vars);
	  MDBIV_init = (unsigned int *)(essenceState.MDBIV_init);
	  chain_vars[0] = (unsigned int)current_md_block_num;
	  chain_vars[1] = (unsigned int)(current_md_block_num >> 32);
	  for(i=2;i<8;i++)
	    {
	      chain_vars[i] = MDBIV_init[i];
	    }
	  essenceState.current_md_block_datalen = 0;
	}
      
      
      /*
       * We copy enough of the input data here to fill up
       * the residual data block and then we call the
       * compression function once to deal with it.
       * Recall that from the NIST standard, our current
       * residual data is byte aligned.
       *
       * We adjust our data pointer and our databitlen
       * to account for the data we have just hashed.
       */
      residual_data = (unsigned char *)(essenceState.residual_data);
      residual_data_byte_index = essenceState.residual_data_bitlen >> 3;
      num_bytes_to_copy = 32 - residual_data_byte_index;
      for(i=0;i<num_bytes_to_copy;i++)
	{
	  residual_data[residual_data_byte_index + i] = data[i];
	}
      data += num_bytes_to_copy;
      databitlen -= 8*num_bytes_to_copy;
      ESSENCE_COMPRESS_256((unsigned int *)(essenceState.chain_vars),
			   (unsigned char*)(essenceState.residual_data),
			   1LL,
			   num_steps);
      essenceState.current_md_block_datalen += 256;
      essenceState.residual_data_bitlen = 0;
      if ( (essenceState.current_md_block_datalen >> 3) == 
	   ESSENCE_MD_BLOCK_SIZE_IN_BYTES )
	{
	  /*
	   * If we just finished off a MD block, then we
	   * need to deal with it.
	   */
	  Essence::Merge_Tree_256((unsigned int *)(essenceState.chain_vars));
	}
    }
  

  /*
   * Here we copy over any data that won't be
   * hashed into the residual_data area.
   */
  tmp = databitlen % 256;
  if (tmp != 0)
    {
      databitlen -= tmp;
      residual_data_byte_offset = databitlen >> 3;
      residual_data = (unsigned char *)(essenceState.residual_data);
      for(i=0;i<(tmp >> 3);i++)
	{
	  residual_data[i] = data[residual_data_byte_offset + i];
	}
      /*
       * If the databitlen % 8 != 0, then we copy one more byte and
       * clear the non-data bits of the last byte of data.  Recall
       * that the NIST standard says that the data in the last byte
       * will be BIG ENDIAN justified (i.e. located at the most
       * significant end of the byte rather than the least significant
       * end).
       */
      if ((tmp % 8) != 0)
	{
	  residual_data[i] = ( data[residual_data_byte_offset + i] & 
			       ((unsigned char)(0xff) << (8 - (tmp % 8))) );
	}
      essenceState.residual_data_bitlen = tmp;
    }


  /*
   * If we have already dealt with all the data, then exit.
   */
  if (databitlen == 0)
    {
      return(SUCCESS);
    }



  /*
   * At this point, we no longer have to worry about the
   * residual data from the last call, and the remaining
   * databitlen is positive and divisible by 256.
   */
  if (essenceState.current_md_block_datalen + databitlen >= (ESSENCE_MD_BLOCK_SIZE_IN_BYTES*8) )
    {
      /*
       * In this case, we have at least one complete MD block to deal
       * with including the data that has already been hashed in the
       * current block.
       */
      
      
      /*
       * If we are in the middle of a MD block, then we finish the
       * current block first.
       */
      num_blocks = (ESSENCE_MD_BLOCK_SIZE_IN_BYTES*8 - 
		    essenceState.current_md_block_datalen)/256;
      if (essenceState.within_md_block)
	{
	  ESSENCE_COMPRESS_256((unsigned int *)(essenceState.chain_vars),
			       (unsigned char *)data,
			       num_blocks,
			       num_steps);
	  data += num_blocks*32;
	  databitlen -= num_blocks*256;
	  Essence::Merge_Tree_256((unsigned int *)(essenceState.chain_vars));
	}
      
      
      /*
       * At this point we are at th beginning of a new MD block.
       */
      
      /*
       * Figure out how many complete MD blocks of data we have
       * available.
       */
      data_bytelen = databitlen >> 3;
      num_complete_md_blocks = (int)(data_bytelen -
				     (data_bytelen % (ESSENCE_MD_BLOCK_SIZE_IN_BYTES)) );
      num_complete_md_blocks = num_complete_md_blocks/(ESSENCE_MD_BLOCK_SIZE_IN_BYTES);
      
      if (num_complete_md_blocks > 0)
 	{
	  /*
	   * Setup some variables.
	   */
	  orig_md_block_num = essenceState.last_md_block_number + 1;
	  essence_md_block_size_in_256bit_blocks = ESSENCE_MD_BLOCK_SIZE_IN_BYTES;
	  essence_md_block_size_in_256bit_blocks >>= 5;
	  MDBIV_init = (unsigned int *)(essenceState.MDBIV_init);
	  chain_vars = (unsigned int *)(essenceState.chain_vars);
	  for(i=0;i<num_complete_md_blocks;i++)
	    {
	      /*
	       * Initialize the chaining variables with the MDBIV
	       */
	      chain_vars[0] = (unsigned int)(orig_md_block_num + i);
	      chain_vars[1] = (unsigned int)((orig_md_block_num + i) >> 32);
	      for(j=2;j<8;j++)
		{
		  chain_vars[j] = MDBIV_init[j];
		}
	      /*
	       * Now hash.
	       */
	      ESSENCE_COMPRESS_256(chain_vars,
				   (unsigned char *)(data+i*(ESSENCE_MD_BLOCK_SIZE_IN_BYTES)),
				   essence_md_block_size_in_256bit_blocks,
				   num_steps);
	      /*
	       * Now Merge
	       */
	      Essence::Merge_Tree_256(chain_vars);
	    }
	  databitlen -= (unsigned long long)(num_complete_md_blocks * 
				   ESSENCE_MD_BLOCK_SIZE_IN_BYTES * 8);
	  data += num_complete_md_blocks * ESSENCE_MD_BLOCK_SIZE_IN_BYTES;
	}
    }
  
  /*
   * At this point, we have less than one MD block to deal with.
   */
  if (databitlen > 0)
    {
      /*
       * If we are beginning a new Merkle-Damgaard block, then we must
       * set the chaining variables to the correct Merkle-Damgaard
       * Block Initialization Vector (MDBIV).
       */
      if (! essenceState.within_md_block)
	{
	  current_md_block_num = essenceState.last_md_block_number + 1;
	  essenceState.within_md_block = 1;
	  chain_vars = (unsigned int *)(essenceState.chain_vars);
	  MDBIV_init = (unsigned int *)(essenceState.MDBIV_init);
	  chain_vars[0] = (unsigned int)current_md_block_num;
	  chain_vars[1] = (unsigned int)(current_md_block_num >> 32);
	  for(i=2;i<8;i++)
	    {
	      chain_vars[i] = MDBIV_init[i];
	    }
	  essenceState.current_md_block_datalen = 0;
	}
      ESSENCE_COMPRESS_256((unsigned int *)(essenceState.chain_vars),
			   (unsigned char *)data,
			   (databitlen >> 8),
			   num_steps);
      essenceState.current_md_block_datalen += databitlen;
    }
  return(SUCCESS);
}
/* *******************************************************************
 *
 * HashReturn Update_512(hashState *state,
 *                       const BitSequence *data,
 *                       DataLength databitlen)
 *
 * state -- a structure that holds the hashState information
 *
 * data -- the data to be hashed
 *
 * databitlen -- the length, in bits, of the data to be hashed
 *
 * Updates the hash state structure with the given data.
 * Assumes that we will be using the 512-bit compression function.
 * This function should only be called by Update.
 * ******************************************************************/
int Essence::Update_512(const BitSequence *data,
		      DataLength databitlen)
{
  unsigned long long *chain_vars;
  unsigned long long *intermediate_tree_hashes;
  unsigned long long *MDBIV_init;
  unsigned long long current_md_block_num;
  unsigned long long tmp_64;
  unsigned long long data_bytelen;
  unsigned long long num_steps;
  unsigned long long orig_md_block_num;
  unsigned long long residual_data_byte_index;
  unsigned long long residual_data_byte_offset;
  unsigned long long num_blocks;
  unsigned long long essence_md_block_size_in_512bit_blocks;
  unsigned char *residual_data;
  int i, j, num_bytes_to_copy, tmp, num_complete_md_blocks;


  /*
   * If we aren't getting enough new data to call the
   * compression function, then just copy it in and
   * exit
   */
  if ((essenceState.residual_data_bitlen + databitlen) < 512)
    {
      residual_data = (unsigned char *)(essenceState.residual_data);
      residual_data_byte_index = essenceState.residual_data_bitlen >> 3;
      data_bytelen = databitlen >>3;
      if (databitlen%8 != 0)
	{
	  data_bytelen++;
	}
      for (i=0;i<data_bytelen;i++)
	{
	  residual_data[residual_data_byte_index + i] = data[i];
	}
      essenceState.residual_data_bitlen += databitlen;
      if ((essenceState.residual_data_bitlen % 8) != 0)
	{
	  /*
	   * If the residual data doesn't end on a byte boundary, then
	   * we zero out the remaining bit positions.
	   */
	  residual_data_byte_index = ((essenceState.residual_data_bitlen) >> 3);
	  residual_data[residual_data_byte_index] =
	    (residual_data[residual_data_byte_index] & 
	     ((unsigned char)(0xff) << (8 - ((essenceState.residual_data_bitlen) % 8))) );
	}
      return(SUCCESS);
    }


  /*
   * If we make it to this point, then we know that we will be
   * calling the compression function, so we need to setup for
   * it.
   *
   * We set a variable to the number of steps of the compression
   * function.  While this might seem superfluous, it is actually
   * necessary to ensure that the constant essenceNumRounds
   * is correctly expanded by some compilers (e.g Apple's gcc version
   * 4.0.1 in 32-bit mode) for function calls.
   */

  num_steps = (unsigned long long)(essenceNumRounds);


  if (essenceState.residual_data_bitlen > 0)
    {
      /*
       * If we have any residual data from the last call, we
       * deal with it here:
       *
       */
      
      /*
       * If we are beginning a new Merkle-Damgaard block, then we must
       * set the chaining variables to the correct Merkle-Damgaard
       * Block Initialization Vector (MDBIV).
       */
      if (! essenceState.within_md_block)
	{
	  current_md_block_num = essenceState.last_md_block_number + 1;
	  essenceState.within_md_block = 1;
	  chain_vars = (unsigned long long *)(essenceState.chain_vars);
	  MDBIV_init = (unsigned long long *)(essenceState.MDBIV_init);
	  chain_vars[0] = current_md_block_num;
	  for(i=1;i<8;i++)
	    {
	      chain_vars[i] = MDBIV_init[i];
	    }
	  essenceState.current_md_block_datalen = 0;
	}
      
      
      /*
       * We copy enough of the input data here to fill up
       * the residual data block and then we call the
       * compression function once to deal with it.
       * Recall that from the NIST standard, our current
       * residual data is byte aligned.
       *
       * We adjust our data pointer and our databitlen
       * to account for the data we have just hashed.
       */
      residual_data = (unsigned char *)(essenceState.residual_data);
      residual_data_byte_index = essenceState.residual_data_bitlen >> 3;
      num_bytes_to_copy = 64 - residual_data_byte_index;
      for(i=0;i<num_bytes_to_copy;i++)
	{
	  residual_data[residual_data_byte_index + i] = data[i];
	}
      data += num_bytes_to_copy;
      databitlen -= 8*num_bytes_to_copy;
      ESSENCE_COMPRESS_512(essenceState.chain_vars,
			   (unsigned char*)(essenceState.residual_data),
			   1LL,
			   num_steps);
      essenceState.current_md_block_datalen += 512;
      essenceState.residual_data_bitlen = 0;
      if ( (essenceState.current_md_block_datalen >> 3) == 
	   ESSENCE_MD_BLOCK_SIZE_IN_BYTES )
	{
	  /*
	   * If we just finished off a MD block, then we
	   * need to deal with it.
	   */
	  Essence::Merge_Tree_512(essenceState.chain_vars);
	}
    }
  

  /*
   * Here we copy over any data that won't be
   * hashed into the residual_data area.
   */
  tmp = databitlen % 512;
  if (tmp != 0)
    {
      databitlen -= tmp;
      residual_data_byte_offset = databitlen >> 3;
      residual_data = (unsigned char *)(essenceState.residual_data);
      for(i=0;i<(tmp >> 3);i++)
	{
	  residual_data[i] = data[residual_data_byte_offset + i];
	}
      /*
       * If the databitlen % 8 != 0, then we copy one more byte and
       * clear the non-data bits of the last byte of data.  Recall
       * that the NIST standard says that the data in the last byte
       * will be BIG ENDIAN justified (i.e. located at the most
       * significant end of the byte rather than the least significant
       * end).
       */
      if ((tmp % 8) != 0)
	{
	  residual_data[i] = ( data[residual_data_byte_offset + i] & 
			       ((unsigned char)(0xff) << (8 - (tmp % 8))) );
	}
      essenceState.residual_data_bitlen = tmp;
    }


  /*
   * If we have already dealt with all the data, then exit.
   */
  if (databitlen == 0)
    {
      return(SUCCESS);
    }



  /*
   * At this point, we no longer have to worry about the
   * residual data from the last call, and the remaining
   * databitlen is positive and divisible by 512.
   */
  if (essenceState.current_md_block_datalen + databitlen >= (ESSENCE_MD_BLOCK_SIZE_IN_BYTES*8) )
    {
      /*
       * In this case, we have at least one complete block to deal
       * with including the data that has already been hashed in the
       * current block.
       */
      
      
      /*
       * If we are in the middle of a MD block, then we finish the
       * current block first.
       */
      num_blocks = (ESSENCE_MD_BLOCK_SIZE_IN_BYTES*8 - 
		    essenceState.current_md_block_datalen)/512;
      if (essenceState.within_md_block)
	{
	  ESSENCE_COMPRESS_512(essenceState.chain_vars,
			       (unsigned char *)data,
			       num_blocks,
			       num_steps);
	  data += num_blocks*64;
	  databitlen -= num_blocks*512;
	  Essence::Merge_Tree_512(essenceState.chain_vars);
	}
      
      
      /*
       * At this point we are at th beginning of a new MD block.
       */
      
      /*
       * Figure out how many complete MD blocks of data we have
       * available.
       */
      data_bytelen = databitlen >> 3;
      num_complete_md_blocks = (int)(data_bytelen -
				     (data_bytelen % (ESSENCE_MD_BLOCK_SIZE_IN_BYTES)) );
      num_complete_md_blocks = num_complete_md_blocks/(ESSENCE_MD_BLOCK_SIZE_IN_BYTES);
      
      if (num_complete_md_blocks > 0)
 	{
	  /*
	   * Setup some variables.
	   */
	  orig_md_block_num = essenceState.last_md_block_number + 1;
	  essence_md_block_size_in_512bit_blocks = ESSENCE_MD_BLOCK_SIZE_IN_BYTES;
	  essence_md_block_size_in_512bit_blocks >>= 6;
	  MDBIV_init = (unsigned long long *)(essenceState.MDBIV_init);
	  chain_vars = (unsigned long long *)(essenceState.chain_vars);
	  for(i=0;i<num_complete_md_blocks;i++)
	    {
	      /*
	       * Initialize the chaining variables with the MDBIV
	       */
	      chain_vars[0] = orig_md_block_num + i;
	      for(j=1;j<8;j++)
		{
		  chain_vars[j] = MDBIV_init[j];
		}
	      /*
	       * Now hash.
	       */
	      ESSENCE_COMPRESS_512(chain_vars,
				   (unsigned char *)(data+i*(ESSENCE_MD_BLOCK_SIZE_IN_BYTES)),
				   essence_md_block_size_in_512bit_blocks,
				   num_steps);
	      /*
	       * Now Merge
	       */
	      Essence::Merge_Tree_512(chain_vars);
	    }

	  databitlen -= (unsigned long long)(num_complete_md_blocks * 
				   ESSENCE_MD_BLOCK_SIZE_IN_BYTES * 8);
	  data += num_complete_md_blocks * ESSENCE_MD_BLOCK_SIZE_IN_BYTES;
	}
    }
  
  /*
   * At this point, we have less than one MD block to deal with.
   */
  if (databitlen > 0)
    {
      /*
       * If we are beginning a new Merkle-Damgaard block, then we must
       * set the chaining variables to the correct Merkle-Damgaard
       * Block Initialization Vector (MDBIV).
       */
      if (! essenceState.within_md_block)
	{
	  current_md_block_num = essenceState.last_md_block_number + 1;
	  essenceState.within_md_block = 1;
	  chain_vars = (unsigned long long *)(essenceState.chain_vars);
	  MDBIV_init = (unsigned long long *)(essenceState.MDBIV_init);
	  chain_vars[0] = current_md_block_num;
	  for(i=1;i<8;i++)
	    {
	      chain_vars[i] = MDBIV_init[i];
	    }
	  essenceState.current_md_block_datalen = 0;
	}
      ESSENCE_COMPRESS_512(essenceState.chain_vars,
			   (unsigned char *)data,
			   (databitlen >> 9),
			   num_steps);
      essenceState.current_md_block_datalen += databitlen;
    }
  return(SUCCESS);
}
/* *******************************************************************
 * HashReturn Update(hashState *state,
 *                   const BitSequence *data,
 *                   DataLength databitlen)
 *
 * state -- a structure that holds the hashState information
 *
 * data -- the data to be hashed
 *
 * databitlen -- the length, in bits, of the data to be hashed
 *
 * Updates the hash state structure with the given data.
 *
 * NOTE:  We may assume that the databitlen will be divisible
 * by 8 except, possibly, for the last call to Update.  Therefore
 * the data will be byte aligned.
 *
 * ******************************************************************/
int Essence::Update(const BitSequence *data,
		  DataLength databitlen)
{
  int exit_code;
  if (databitlen == 0)
    {
      /*
       * If we don't have any data, then do nothing.
       */
      exit_code = SUCCESS;
    }
  else if ((essenceState.residual_data_bitlen)%8 != 0)
    {
      /*
       * If we have already received a call with datalength
       * not a multiple of 8, then we should not have been
       * called again, so return a data alignment error.
       */
      exit_code = DATA_ALIGNMENT_ERROR;
    }
  else if (essenceState.hashbitlen > 256)
    {
      exit_code = Essence::Update_512(data,databitlen);
    }
  else
    {
      exit_code = Essence::Update_256(data,databitlen);
    }

#if ESSENCE_DEBUG_LEVEL & 1
  printf("\n\n\n\n******** ******** ******** ");
  printf("******** ******** ******** LEAVING Update\n");
  Essence::PrintState();
#endif
  return(exit_code);
}
/* *******************************************************************
 * HashReturn Final_512(hashState *state,
 *                      BitSequence *hashval)
 *
 * state -- a structure that holds the hashState information
 *
 * hashval -- the storage for the final hash value to be
 *            returned.
 *
 * This function is for hash lengths over greater than 256 bits.
 * It does the following:
 *
 * 1. Complete the hashing of any residual data.
 *
 * 2. Complete the last Merkle Hash Tree.
 *
 * 3. Incorporate the top hash from the last hash tree into
 *    the running hash.
 *
 * 4. Form the "final block" with the strenghtening padding and
 *    hash it with the running hash.
 *
 * 5. Return the desired hash in the space pointed to by hashval. 
 *
 * ******************************************************************/
int Essence::Final_512(BitSequence *hashval)
{
  unsigned long long num_steps;
  unsigned long long current_md_block_num;
  unsigned long long last_block_data_bitlen;
  unsigned long long *chain_vars;
  unsigned long long *MDBIV_init;
  unsigned long long *root_hash;
  unsigned long long final_block[8];
  unsigned char *residual_data;
  unsigned char byte_array[64];
  int residual_data_byte_index;
  int blocks_per_tree;
  int leaf_num, level, level_mask;
  int num_bytes_to_copy;
  int i,j;

  num_steps = (unsigned long long)(essenceNumRounds);

  /* **** **** **** **** **** **** **** **** **** **** **** ****
   * 1. Complete the hashing of any residual data.
   * **** **** **** **** **** **** **** **** **** **** **** ***/
  if (essenceState.residual_data_bitlen > 0)
    {
      /*
       * As per the ESSENCE standard.  The residual data block is padded
       * out with all zeros.
       */
      residual_data_byte_index = (essenceState.residual_data_bitlen) >> 3;
      residual_data = (unsigned char*)(essenceState.residual_data);
      if ((essenceState.residual_data_bitlen % 8) != 0)
	{
	  /*
	   * If the data doesn't end on a byte boundary, then we
	   * zero out the remaining bits in the byte.  The NIST
	   * standard specifies that the data bits will be in the
	   * most significant bits of the byte.
	   */
	  residual_data[residual_data_byte_index] =
	    (residual_data[residual_data_byte_index] & 
	     ((unsigned char)(0xff) << (8 - ((essenceState.residual_data_bitlen) % 8))) );
	  residual_data_byte_index += 1;
	}
      while(residual_data_byte_index < 64)
	{
	  residual_data[residual_data_byte_index] = 0;
	  residual_data_byte_index++;
	}

      /*
       * If we are beginning a new Merkle-Damgaard block, then we must
       * set the chaining variables to the correct Merkle-Damgaard
       * Block Initialization Vector (MDBIV).
       */
      if (! essenceState.within_md_block)
	{
	  current_md_block_num = essenceState.last_md_block_number + 1;
	  essenceState.within_md_block = 1;
	  chain_vars = (unsigned long long *)(essenceState.chain_vars);
	  MDBIV_init = (unsigned long long *)(essenceState.MDBIV_init);
	  chain_vars[0] = current_md_block_num;
	  for(i=1;i<8;i++)
	    {
	      chain_vars[i] = MDBIV_init[i];
	    }
	  essenceState.current_md_block_datalen = 0;
	}
      ESSENCE_COMPRESS_512(essenceState.chain_vars,
			   (unsigned char *)(essenceState.residual_data),
			   1LL,
			   num_steps);
      essenceState.current_md_block_datalen += essenceState.residual_data_bitlen;
    }

  last_block_data_bitlen = essenceState.current_md_block_datalen;
  

  /* **** **** **** **** **** **** **** **** **** **** **** ****
   * 2. Complete the last Merkle Hash Tree.
   * **** **** **** **** **** **** **** **** **** **** **** ***/
  
  /*
   * At this point, all of the DM blocks have been hashed.
   * However, we may need to merge the last block into
   * the current hash tree.
   */
  if (essenceState.within_md_block)
    {
      Essence::Merge_Tree_512(essenceState.chain_vars);
    }
  
#if ESSENCE_HASH_TREE_LEVEL > 0
  /*
   * Since the Merge_Tree algorithm only ensures that
   * the fully populated subtrees are correct, we need
   * to fix any subtrees that aren't fully populated.
   */
  blocks_per_tree = (1 << ESSENCE_HASH_TREE_LEVEL);
  leaf_num = (essenceState.last_md_block_number) % blocks_per_tree;
  level_mask = 1;
  for(level=1; level < ESSENCE_HASH_TREE_LEVEL; level++)
    {
      level_mask <<= 1;
      if (leaf_num & level_mask)
	{
	  Essence::Join_512(((unsigned long long *)(essenceState.merkle_tree_hashes))+8*level,
		   ((unsigned long long *)(essenceState.merkle_tree_hashes))+8*(level-1));
#if ESSENCE_DEBUG_LEVEL & 4
	  printf("\n\n\n\n******** ******** ******** ");
	  printf("******** ******** ******** LEAVING Essence::Join_512\n");
	  Essence::PrintState();
#endif

	}
      else
	{
	  for(i=0;i<8;i++)
	    {
	      (((unsigned long long *)(essenceState.merkle_tree_hashes))+8*level)[i] =
		(((unsigned long long *)(essenceState.merkle_tree_hashes))+8*(level-1))[i];
	    }
	}
    }


  /* **** **** **** **** **** **** **** **** **** **** **** ****
   * 3. Compress root of last tree into the running hash.
   * **** **** **** **** **** **** **** **** **** **** **** ***/
  root_hash = ( ((unsigned long long *)(essenceState.merkle_tree_hashes)) + 
		8*(ESSENCE_HASH_TREE_LEVEL-1) );
  for(i=0;i<8;i++)
    {
      for(j=0;j<8;j++)
	{
	  byte_array[i*8+j] = (unsigned char)(root_hash[i] >> (8*j));
	}
    }
  ESSENCE_COMPRESS_512(essenceState.running_hash,
		       byte_array,
		       1LL,
		       num_steps);
#endif /* ESSENCE_HASH_TREE_LEVEL > 0 */


  /* **** **** **** **** **** **** **** **** **** **** **** ****
   * 4. Construct the final block and compress it.
   * **** **** **** **** **** **** **** **** **** **** **** ***/
  MDBIV_init = (unsigned long long *)(essenceState.MDBIV_init);
  for(i=0;i<8;i++)
    {
      final_block[i] = MDBIV_init[i];
    }
  final_block[0] = essenceState.last_md_block_number;
  final_block[3] = last_block_data_bitlen;

  for(i=0;i<8;i++)
    {
      for(j=0;j<8;j++)
	{
	  byte_array[i*8+j] = (unsigned char)(final_block[i] >> (8*j));
	}
    }
  ESSENCE_COMPRESS_512(essenceState.running_hash,
		       byte_array,
		       1LL,
		       num_steps);
  

  /* **** **** **** **** **** **** **** **** **** **** **** ****
   * 5. Copy the final hash out to hashval.
   * **** **** **** **** **** **** **** **** **** **** **** ***/
  num_bytes_to_copy = (essenceState.hashbitlen) >> 3;
  if ((essenceState.hashbitlen) % 8 > 0 )
    {
      num_bytes_to_copy += 1;
    }
  for(i=0;i<num_bytes_to_copy;i++)
    {
      hashval[i] = (unsigned char)((essenceState.running_hash)[(i >> 3)] >> ((i % 8)*8));
    }
  if ((essenceState.hashbitlen) % 8 > 0 )
    {
      /*
       * If the hashbitlen wasn't a multiple of 8, then we clear the
       * unused bits of the last byte.
       */
      hashval[i] = hashval[i] & (0xff << (8 - (essenceState.hashbitlen % 8)) );
    }

#if ESSENCE_DEBUG_LEVEL & 1
  printf("\n\n\n\n******** ******** ******** ");
  printf("******** ******** ******** LEAVING Final\n");
  Essence::PrintState();
#endif

  return(SUCCESS);
}
/* *******************************************************************
 * HashReturn Final_256(hashState *state,
 *                      BitSequence *hashval)
 *
 * state -- a structure that holds the hashState information
 *
 * hashval -- the storage for the final hash value to be
 *            returned.
 *
 * This function is for hash lengths of 256 bits or less.
 * It does the following:
 *
 * 1. Complete the hashing of any residual data.
 *
 * 2. Complete the last Merkle Hash Tree.
 *
 * 3. Incorporate the top hash from the last hash tree into
 *    the running hash.
 *
 * 4. Form the "final block" with the strenghtening padding and
 *    hash it with the running hash.
 *
 * 5. Return the desired hash in the space pointed to by hashval. 
 *
 * ******************************************************************/
int Essence::Final_256(BitSequence *hashval)
{
  unsigned long long num_steps;
  unsigned long long current_md_block_num;
  unsigned long long last_block_data_bitlen;
  unsigned int *chain_vars;
  unsigned int *MDBIV_init;
  unsigned int *root_hash;
  unsigned int *running_hash;
  unsigned int final_block[8];
  unsigned char *residual_data;
  unsigned char byte_array[32];
  int residual_data_byte_index;
  int blocks_per_tree;
  int leaf_num, level, level_mask;
  int num_bytes_to_copy;
  int i,j;

  num_steps = (unsigned long long)(essenceNumRounds);

  /* **** **** **** **** **** **** **** **** **** **** **** ****
   * 1. Complete the hashing of any residual data.
   * **** **** **** **** **** **** **** **** **** **** **** ***/
  if (essenceState.residual_data_bitlen > 0)
    {
      /*
       * As per the ESSENCE standard.  The residual data block is padded
       * out with all zeros.
       */
      residual_data_byte_index = (essenceState.residual_data_bitlen) >> 3;
      residual_data = (unsigned char *)(essenceState.residual_data);
      if ((essenceState.residual_data_bitlen % 8) != 0)
	{
	  /*
	   * If the data doesn't end on a byte boundary, then we
	   * zero out the remaining bits in the byte.  The NIST
	   * standard specifies that the data bits will be in the
	   * most significant bits of the byte.
	   */
	  residual_data[residual_data_byte_index] =
	    (residual_data[residual_data_byte_index] & 
	     (((unsigned char)(0xff)) << (8 - ((essenceState.residual_data_bitlen) % 8))) );
	  residual_data_byte_index += 1;
	}
      while(residual_data_byte_index < 32)
	{
	  residual_data[residual_data_byte_index] = 0;
	  residual_data_byte_index++;
	}

      /*
       * If we are beginning a new Merkle-Damgaard block, then we must
       * set the chaining variables to the correct Merkle-Damgaard
       * Block Initialization Vector (MDBIV).
       */
      if (! essenceState.within_md_block)
	{
	  current_md_block_num = essenceState.last_md_block_number + 1;
	  essenceState.within_md_block = 1;
	  chain_vars = (unsigned int *)(essenceState.chain_vars);
	  MDBIV_init = (unsigned int *)(essenceState.MDBIV_init);
	  chain_vars[0] = (unsigned int)current_md_block_num;
	  chain_vars[1] = (unsigned int)(current_md_block_num >> 32);
	  for(i=2;i<8;i++)
	    {
	      chain_vars[i] = MDBIV_init[i];
	    }
	  essenceState.current_md_block_datalen = 0;
	}
      ESSENCE_COMPRESS_256((unsigned int *)(essenceState.chain_vars),
			   (unsigned char *)(essenceState.residual_data),
			   1LL,
			   num_steps);
      essenceState.current_md_block_datalen += essenceState.residual_data_bitlen;
    }

  last_block_data_bitlen = essenceState.current_md_block_datalen;


  /* **** **** **** **** **** **** **** **** **** **** **** ****
   * 2. Complete the last Merkle Hash Tree.
   * **** **** **** **** **** **** **** **** **** **** **** ***/
  
  /*
   * At this point, all of the DM blocks have been hashed.
   * However, we may need to merge the last block into
   * the current hash tree.
   */
  if (essenceState.within_md_block)
    {
      Essence::Merge_Tree_256((unsigned int *)(essenceState.chain_vars));
    }
  
#if ESSENCE_HASH_TREE_LEVEL > 0
  /*
   * Since the Merge_Tree algorithm only ensures that
   * the fully populated subtrees are correct, we need
   * to fix any subtrees that aren't fully populated.
   */
  blocks_per_tree = (1 << ESSENCE_HASH_TREE_LEVEL);
  leaf_num = (essenceState.last_md_block_number) % blocks_per_tree;
  level_mask = 1;
  for(level=1; level < ESSENCE_HASH_TREE_LEVEL; level++)
    {
      level_mask <<= 1;
      if (leaf_num & level_mask)
	{
	  Essence::Join_256(((unsigned int *)(essenceState.merkle_tree_hashes))+8*level,
		   ((unsigned int *)(essenceState.merkle_tree_hashes))+8*(level-1));
#if ESSENCE_DEBUG_LEVEL & 4
	  printf("\n\n\n\n******** ******** ******** ");
	  printf("******** ******** ******** LEAVING Essence::Join_256\n");
	  Essence::PrintState();
#endif

	}
      else
	{
	  for(i=0;i<8;i++)
	    {
	      (((unsigned int *)(essenceState.merkle_tree_hashes))+8*level)[i] =
		(((unsigned int *)(essenceState.merkle_tree_hashes))+8*(level-1))[i];
	    }
	}
    }


  /* **** **** **** **** **** **** **** **** **** **** **** ****
   * 3. Compress root of last tree into the running hash.
   * **** **** **** **** **** **** **** **** **** **** **** ***/
  root_hash = ( ((unsigned int *)(essenceState.merkle_tree_hashes)) + 
		8*(ESSENCE_HASH_TREE_LEVEL-1) );
  for(i=0;i<8;i++)
    {
      for(j=0;j<4;j++)
	{
	  byte_array[i*4+j] = (unsigned char)(root_hash[i] >> (8*j));
	}
    }
  ESSENCE_COMPRESS_256((unsigned int *)(essenceState.running_hash),
		       byte_array,
		       1LL,
		       num_steps);
#endif /* ESSENCE_HASH_TREE_LEVEL > 0 */


  /* **** **** **** **** **** **** **** **** **** **** **** ****
   * 4. Construct the final block and compress it.
   * **** **** **** **** **** **** **** **** **** **** **** ***/
  MDBIV_init = (unsigned int *)(essenceState.MDBIV_init);
  for(i=0;i<8;i++)
    {
      final_block[i] = MDBIV_init[i];
    }
  final_block[0] = (unsigned int)(essenceState.last_md_block_number);
  final_block[1] = (unsigned int)((essenceState.last_md_block_number) >> 32);
  final_block[6] = (unsigned int)(last_block_data_bitlen);
  final_block[7] = (unsigned int)(last_block_data_bitlen >> 32);

  for(i=0;i<8;i++)
    {
      for(j=0;j<4;j++)
	{
	  byte_array[i*4+j] = (unsigned char)(final_block[i] >> (8*j));
	}
    }
  ESSENCE_COMPRESS_256((unsigned int *)(essenceState.running_hash),
		       byte_array,
		       1LL,
		       num_steps);
  

  /* **** **** **** **** **** **** **** **** **** **** **** ****
   * 5. Copy the final hash out to hashval.
   * **** **** **** **** **** **** **** **** **** **** **** ***/
  num_bytes_to_copy = (essenceState.hashbitlen) >> 3;
  if ((essenceState.hashbitlen) % 8 > 0 )
    {
      num_bytes_to_copy += 1;
    }
  running_hash = (unsigned int*)(essenceState.running_hash);
  for(i=0;i<num_bytes_to_copy;i++)
    {
      hashval[i] = (unsigned char)(running_hash[i>>2] >> ((i%4)*8));
    }
  if ((essenceState.hashbitlen) % 8 > 0 )
    {
      /*
       * If the hashbitlen wasn't a multiple of 8, then we clear the
       * unused bits of the last byte.
       */
      hashval[i] = hashval[i] & (0xff << (8 - (essenceState.hashbitlen % 8)) );
    }

#if ESSENCE_DEBUG_LEVEL & 1
  printf("\n\n\n\n******** ******** ******** ");
  printf("******** ******** ******** LEAVING Final\n");
  Essence::PrintState();
#endif

  return(SUCCESS);
}
/* *******************************************************************
 * HashReturn Final(hashState *state,
 *                  BitSequence *hashval)
 *
 * state -- a structure that holds the hashState information
 *
 * hashval -- the storage for the final hash value to be
 *            returned.
 *
 * ******************************************************************/
int Essence::Final(BitSequence *hashval)
{
  int exit_code;

  if (essenceState.hashbitlen > 256)
    {
      exit_code = Essence::Final_512(hashval);
    }
  else
    {
      exit_code = Essence::Final_256(hashval);
    }
  return(exit_code);
}
/* *******************************************************************
 * HashReturn Hash(int hashbitlen,
 *                 const BitSequence *data,
 *                 DataLength databitlen
 *                 BitSequence *hashval)
 *
 * hashbitlen -- The size of the hash to be returned in bits.
 *
 * data -- The data to be hashed.
 *
 * databitlen -- The length, in bits, of data to be hashed.
 *
 * hashval -- the storage for the final hash value to be
 *            returned.
 *
 * ******************************************************************/
int Essence::Hash(int hashbitlen,
		const BitSequence *data,
		DataLength databitlen,
		BitSequence *hashval)
{
  int exit_code;
  //hashState the_hash_state;

  exit_code = Essence::Init(hashbitlen);
  if (exit_code == SUCCESS)
    {
      exit_code = Essence::Update(data,databitlen);
      if (exit_code == SUCCESS)
	{
	  exit_code = Essence::Final(hashval);
	}
    }
  return(exit_code);
}

#if ESSENCE_DEBUG_LEVEL > 0
/* *******************************************************************
 * PrintState(hashState *state)
 *
 * state -- the hash state
 *
 * DESCRIPTION: Print the hash state information to stdout.  All of
 * the arrays are treated as arrays of bytes, and they are printed
 * sequentially.  This function is provided for debugging purposes.
 *
 * ******************************************************************/
void Essence::PrintState()
{
  unsigned char *byte_array;
  unsigned int *dword_array;
  unsigned long long *qword_array;
  int i,j;

  printf("hashbitlen: %i\n",essenceState.hashbitlen);
  if (essenceState.hashbitlen > 256)
    {
      printf("Using 512-bit compression function values\n\n");
      for(i=0;i<8;i++)
	{
	  printf("running_hash[%i] = 0x%.16llx\n",i,
		 essenceState.running_hash[i]);
	}

      printf("\n");
      for(i=0;i<8;i++)
	{
	  printf("chain_vars[%i] = 0x%.16llx\n",i,
		 essenceState.chain_vars[i]);
	}

      printf("\nMDBIV_init:");
      byte_array = (unsigned char*)(essenceState.MDBIV_init);
      for(i=0;i<8;i++)
	{
	  printf("\n Bytes %.2i - %.2i:          ",8*i,8*i+7);
	  for(j=0;j<8;j++)
	    {
	      printf(" 0x%.2x",byte_array[i*8+j]);
	    }
	}

      printf("\nMDBIV_init (reversed, merged):");
      byte_array = (unsigned char*)(essenceState.MDBIV_init);
      for(i=7;i>=0;i--)
	{
	  printf("\n Quad Word %i:                 0x",i);
	  for(j=7;j>=0;j--)
	    {
	      printf("%.2x",byte_array[i*8+j]);
	    }
	}

#if ESSENCE_HASH_TREE_LEVEL > 0
      printf("\n\nmerkle_tree_hashes:\n");
      for(i=0;i<ESSENCE_HASH_TREE_LEVEL;i++)
	{
	  for(j=0;j<8;j++)
	    {
	      printf("merkle_tree_hashes[%i*8+%i] = 0x%.16llx\n",
		     i,j,
		     essenceState.merkle_tree_hashes[i*8+j]);
	    }
	}
#endif /* ESSENCE_HASH_TREE_LEVEL > 0 */

      printf("\nlast_md_block_number = 0x%.16llx\n",
	     essenceState.last_md_block_number);

      printf("\ncurrent_md_block_datalen = 0x%.16llx\n",
	     essenceState.current_md_block_datalen);

      printf("\nresidual data:\n");
      byte_array = (unsigned char*)(essenceState.residual_data);
      for(i=0;i<8;i++)
	{
	  printf("\n Bytes %.2i - %.2i:          ",8*i,8*i+7);
	  for(j=0;j<8;j++)
	    {
	      printf(" 0x%.2x",byte_array[i*8+j]);
	    }
	}
      printf("\nresidual_data_bitlen = %i\n",
	     essenceState.residual_data_bitlen);

      printf("\nhashbitlen = %i\n",
	     essenceState.hashbitlen);

      printf("\nwithin_md_block = %i\n",
	     essenceState.within_md_block);
    }
  else
    {
      printf("Using 256-bit compression function values\n\n");
      for(i=0;i<8;i++)
	{
	  printf("running_hash[%i] = 0x%.8x\n",i,
		 ((unsigned int *)(essenceState.running_hash))[i]);
	}

      printf("\n");
      for(i=0;i<8;i++)
	{
	  printf("chain_vars[%i] = 0x%.8x\n",i,
		 ((unsigned int *)(essenceState.chain_vars))[i]);
	}

      printf("\nMDBIV_init:");
      byte_array = (unsigned char*)(essenceState.MDBIV_init);
      for(i=0;i<8;i++)
	{
	  printf("\n Bytes %.2i - %.2i:          ",4*i,4*i+3);
	  for(j=0;j<4;j++)
	    {
	      printf(" 0x%.2x",byte_array[i*4+j]);
	    }
	}

      printf("\nMDBIV_init (reversed, merged):");
      byte_array = (unsigned char*)(essenceState.MDBIV_init);
      for(i=7;i>=0;i--)
	{
	  printf("\n Double Word %i:                 0x",i);
	  for(j=3;j>=0;j--)
	    {
	      printf("%.2x",byte_array[i*4+j]);
	    }
	}

#if ESSENCE_HASH_TREE_LEVEL > 0
      printf("\n\nmerkle_tree_hashes:\n");
      for(i=0;i<ESSENCE_HASH_TREE_LEVEL;i++)
	{
	  for(j=0;j<8;j++)
	    {
	      printf("merkle_tree_hashes[%i*8+%i] = 0x%.8x\n",
		     i,j,
		     ((unsigned int*)(essenceState.merkle_tree_hashes))[i*8+j]);
	    }
	}
#endif /* ESSENCE_HASH_TREE_LEVEL > 0 */

      printf("\nlast_md_block_number = 0x%.16llx\n",
	     essenceState.last_md_block_number);

      printf("\ncurrent_md_block_datalen = 0x%.16llx\n",
	     essenceState.current_md_block_datalen);

      printf("\nresidual data:\n");
      byte_array = (unsigned char*)(essenceState.residual_data);
      for(i=0;i<8;i++)
	{
	  printf("\n Bytes %.2i - %.2i:          ",8*i,8*i+7);
	  for(j=0;j<8;j++)
	    {
	      printf(" 0x%.2x",byte_array[i*8+j]);
	    }
	}
      printf("\nresidual_data_bitlen = %i\n",
	     essenceState.residual_data_bitlen);

      printf("\nhashbitlen = %i\n",
	     essenceState.hashbitlen);

      printf("\nwithin_md_block = %i\n",
	     essenceState.within_md_block);
    }
  fflush(stdout);
}

Essence::Essence(const int numRounds) {
	if (numRounds == -1) {
		essenceNumRounds = ESSENCE_COMPRESS_NUM_STEPS;
	} else if (numRounds >= 0 && numRounds < 8){
		essenceNumRounds = ESSENCE_COMPRESS_NUM_STEPS_MIN;
	} else {
		essenceNumRounds = numRounds;
	}
}

#endif /* ESSENCE_DEBUG_LEVEL > 0 */