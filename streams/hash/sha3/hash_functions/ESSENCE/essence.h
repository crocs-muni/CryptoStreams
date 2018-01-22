/* FILE: essence.h
 *
 * AUTHOR: Jason Worth Martin <jason.worth.martin@gmail.com>
 *
 * DESCRIPTION: This file contains macros and declarations
 * used internally in the ESSENCE implementation.
 *
 */
#ifndef _ESSENCE_H_
#define _ESSENCE_H_
//#include "essence_api.h"

#if ESSENCE_USE_PARALLEL_CODE == 1
#include <stdlib.h>
#endif

#if ESSENCE_DEBUG_LEVEL > 0
#include <stdio.h>
#endif

/*
 * This macro corresponds to the F function
 * used in ESSENCE.
 *
 * least significant outputs = 0x0c9ac1ac7974f57b
 * most significant outputs  = 0x997083d5c9316b14
 *
 * This is the Reduced Normal Form of the F function.  This is the
 * same code as in Appendix A of ESSENCE: A Family of Cryptographic
 * Hashing Algorithms.
 *
 */
#define F_func_Reduced_Normal_Form(a,b,c,d,e,f,g)	\
  (							\
   ((a)&(~(b))&(c)&(d)&(e)&(f)&(~(g)))^			\
   ((a)&(b)&(c)&(e)&(~(f))&(g))^			\
   ((a)&(b)&(d)&(~(e))&(g))^				\
   ((a)&(b)&(e)&(f)&(g))^				\
   ((a)&(~(b))&(d)&(e)&(f))^				\
   ((a)&(c)&(e)&(f)&(g))^				\
   ((a)&(~(c))&(d)&(f)&(~(g)))^				\
   ((a)&(d)&(e)&(~(f))&(g))^				\
   ((b)&(c)&(d)&(f)&(g))^				\
   ((b)&(d)&(e)&(~(f))&(g))^				\
   ((c)&(d)&(e)&(f)&(~(g)))^				\
   ((a)&(b)&(~(c))&(g))^				\
   ((~(a))&(b)&(c)&(~(f)))^				\
   ((b)&(c)&(~(e))&(g))^				\
   ((b)&(~(c))&(d)&(e))^				\
   ((a)&(b)&(f))^					\
   ((a)&(~(b))&(e))^					\
   ((a)&(c)&(g))^					\
   ((a)&(d)&(~(g)))^					\
   ((a)&(e)&(g))^					\
   ((~(a))&(e)&(f))^					\
   ((b)&(d)&(~(f)))^					\
   ((b)&(e)&(g))^					\
   ((~(b))&(f)&(g))^					\
   ((c)&(~(d))&(~(e)))^					\
   ((~(c))&(d)&(f))^					\
   ((d)&(e)&(f))^					\
   ((d)&(~(e))&(g))^					\
   ((d)&(f)&(g))^					\
   ((~(a)))^						\
   ((b))^						\
   ((f))						\
							)
    
/* F_func
 *
 * This is a faster version of the F function for most processors.  It
 * is algebraically equivalent to the Reduced Normal Form version
 * above, but uses fewer total operations and fewer intermediate
 * values.  However, the Reduced Normal Form version above has a much
 * shorter critical path (the Reduced Normal Form version is 2-level
 * logic) so the Reduced Normal Form version will execute faster on
 * processors that have massive register files and many ALU execution
 * units.  On an Itanium, use the Reduced Normal Form version.  For
 * most other processors, use this one.
 */
#define F_func(a,b,c,d,e,f,g)				\
  (							\
   (((f)&(((d)&(((c)&((((a)&(~(b))&(e)&(~(g))) ^	\
		       ((b)&(g)) ^			\
		       ((e)&(~(g)))))) ^		\
		((~(c))&((~(((a)&(~(g))))))) ^		\
		((((a)&(~(b))&(e)) ^			\
		  ((e)) ^				\
		  ((g)))))) ^				\
	  ((~(((a)&(b)&(e)&(g)) ^			\
	      ((a)&(c)&(e)&(g)) ^			\
	      ((a)&(b)) ^				\
	      ((~(a))&(e)) ^				\
	      ((~(b))&(g))))))) ^			\
    ((~(f))&(((b)&((((a)&(c)&(e)&(g)) ^			\
		    ((d)&(e)&(g)) ^			\
		    ((~(a))&(c)) ^			\
		    ((d))))) ^				\
	     ((((a)&(d)&(e)&(g)))))) ^			\
    (((e)&((((b)&(~(c))&(d)) ^				\
	    ((a)&(~(b))) ^				\
	    ((a)&(g)) ^					\
	    ((b)&(g))))) ^				\
     ((~(e))&((((a)&(b)&(d)&(g)) ^			\
	       ((b)&(c)&(g)) ^				\
	       ((c)&(~(d))) ^				\
	       ((d)&(g))))) ^				\
     ((((a)&(b)&(~(c))&(g)) ^				\
       ((a)&(c)&(g)) ^					\
       ((a)&(d)&(~(g))) ^				\
       ((~(a))) ^					\
       ((b))))))					\
							)




/*
 * This corresponds to the polynomial:
 *
 * p_64 = x^64 + x^63 + x^61 + x^60 + x^55 + x^53 + x^50 + x^49 + x^46
 * + x^44 + x^41 + x^40 + x^36 + x^33 + x^32 + x^31 + x^30 + x^29 +
 * x^26 + x^25 + x^23 + x^20 + x^18 + x^17 + x^14 + x^13 + x^11 + x^8
 * + x^7 + x^4 + x^2 + x + 1
 * 
 */
#define P_64 0xb0a65313e6966997LL


/*
 * This corresponds to the polynomial:
 *
 * p_32 = x^32 + x^31 + x^24 + x^22 + x^19 + x^17 + x^13 + x^12 + x^11
 * + x^9 + x^8 + x^5 + x^4 + x^2 + 1
 *
 */
#define P_32 0x814a3b35



/*
 * Definitions for the L_32 and L_64 tables
 * defined in the essence_L_tables.c file
 */

extern unsigned int L_32_table[256];
extern unsigned long long L_64_table[256];




/* ******************************************************************************************
 * ******************************************************************************************
 *
 * The following function declarations are for use with the various
 * alternative implementations of the compression functions.  These
 * include assembly level implementations which need to be separately
 * compiled and linked.
 *
 * ******************************************************************************************
 * *****************************************************************************************/

/*
 * void essence_compress_512_64(unsigned long long *Chaining_Variables,
 *                              unsigned char *input,
 *                              unsigned long long input_size_in_512_bit_blocks,
 *			        unsigned long long num_steps)
 *
 * This an x86_64 assembly code implementation of the 512 bit
 * compression function using the SSE instructions.  It is in the
 * file: "essence_compress_512_64_const_time.yasm".
 *
 *  USES LOOK-UP TABLES:   YES
 *  ASSUMES x86 CPU:       YES
 *  USES SSE CODE:         YES
 *  ASSUMES 64-BIT:        YES
 *  ASSUMES LITTLE ENDIAN: YES
 *
 */
void essence_compress_512_64(unsigned long long *,
			     unsigned char *,
			     unsigned long long,
			     unsigned long long);


/*
 * void essence_compress_512_64_const_time(unsigned long long *Chaining_Variables,
 *                                         unsigned char *input,
 *                                         unsigned long long input_size_in_512_bit_blocks,
 *			                   unsigned long long num_steps)
 *
 * This an x86_64 assembly code implementation of the 512 bit
 * compression function using the SSE instructions.  This code runs in
 * constant time.  It is in the file:
 * "essence_compress_512_64_const_time.yasm".
 *
 *  USES LOOK-UP TABLES:   NO
 *  ASSUMES x86 CPU:       YES
 *  USES SSE CODE:         YES
 *  ASSUMES 64-BIT:        YES
 *  ASSUMES LITTLE ENDIAN: YES
 *
 */
void essence_compress_512_64_const_time(unsigned long long *,
			                unsigned char *,
			                unsigned long long,
			                unsigned long long);

/*
 * void essence_compress_512(unsigned long long *Chaining_Variables,
 *                           unsigned char *input,
 *                           unsigned long long input_size_in_512_bit_blocks,
 *			     unsigned long long num_steps)
 *
 * This is a C code demonstration of how to implement the 512 bit
 * compression function.  It is in the file
 * "essence_compress_512.c"
 *
 *  USES LOOK-UP TABLES:   YES
 *  ASSUMES x86 CPU:       NO
 *  USES SSE CODE:         NO
 *  ASSUMES 64-BIT:        NO
 *  ASSUMES LITTLE ENDIAN: NO
 *
 */
void essence_compress_512(unsigned long long *,
			  unsigned char *,
			  unsigned long long,
			  unsigned long long);


/*
 * void essence_compress_512_const_time(unsigned long long *Chaining_Variables,
 *                                      unsigned char *input,
 *                                      unsigned long long input_size_in_512_bit_blocks,
 *			                unsigned long long num_steps)
 *
 * This is a C code demonstration of how to implement the 512 bit
 * compression function in constant time.  It is in the file
 * "essence_compress_512_const_time.c"
 *
 *  USES LOOK-UP TABLES:   NO
 *  ASSUMES x86 CPU:       NO
 *  USES SSE CODE:         NO
 *  ASSUMES 64-BIT CPU:    NO
 *  ASSUMES LITTLE ENDIAN: NO
 *
 */
void essence_compress_512_const_time(unsigned long long *,
			             unsigned char *,
			             unsigned long long,
			             unsigned long long);



/*
 * void essence_compress_256_64_dual(unsigned int *Chaining_Variables_a,
 *                                   unsigned int *Chaining_Variables_b,
 *                                   unsigned char *input_a,
 *                                   unsigned char *input_b
 *                                   unsigned long long input_size_in_256_bit_blocks,
 *			             unsigned long long num_steps)
 *
 * This function implements TWO copies of the compression function in
 * parallel on a single CPU core by making use of the SSE intructions.
 * It is implemented in x86_64 assembly code in the file
 * "essence_compress_256_64_dual.yasm"
 *
 *  USES LOOK-UP TABLES:   YES
 *  ASSUMES x86 CPU:       YES
 *  USES SSE CODE:         YES
 *  ASSUMES 64-BIT CPU:    YES
 *  ASSUMES LITTLE ENDIAN: YES
 *
 */
void essence_compress_256_64_dual(unsigned int *,
			          unsigned int *,
			          unsigned char *,
			          unsigned char *,
			          unsigned long long,
			          unsigned long long);


/*
 * void essence_compress_256_64_dual_const_time(unsigned int *Chaining_Variables_a,
 *                                              unsigned int *Chaining_Variables_b,
 *                                              unsigned char *input_a,
 *                                              unsigned char *input_b
 *                                              unsigned long long input_size_in_256_bit_blocks,
 *			                        unsigned long long num_steps)
 *
 * This function implements TWO copies of the compression function in
 * parallel on a single CPU core by making use of the SSE intructions.
 * It runs in constant time.  It is implemented in x86_64 assembly
 * code in the file "essence_compress_256_64_dual_const_time.yasm"
 *
 *  USES LOOK-UP TABLES:   NO
 *  ASSUMES x86 CPU:       YES
 *  USES SSE CODE:         YES
 *  ASSUMES 64-BIT CPU:    YES
 *  ASSUMES LITTLE ENDIAN: YES
 *
 */
void essence_compress_256_64_dual_const_time(unsigned int *,
			                     unsigned int *,
			                     unsigned char *,
			                     unsigned char *,
			                     unsigned long long,
			                     unsigned long long);


/*
 * void essence_compress_256_64(unsigned int *Chaining_Variables,
 *                              unsigned char *input,
 *                              unsigned long long input_size_in_256_bit_blocks,
 *			        unsigned long long num_steps)
 *
 * This is a C code implementation of the 256-bit compression
 * function.  It is optimized for 64-bit machines.  It is in the file
 * "essence_compress_256_64.c"
 *
 *  USES LOOK-UP TABLES:   YES
 *  ASSUMES x86 CPU:       NO
 *  USES SSE CODE:         NO
 *  ASSUMES 64-BIT CPU:    NO
 *  ASSUMES LITTLE ENDIAN: NO
 *
 */
void essence_compress_256_64(unsigned int *,
			     unsigned char *,
			     unsigned long long,
			     unsigned long long);


/*
 * void essence_compress_256_64_const_time(unsigned int *Chaining_Variables,
 *                                         unsigned char *input,
 *                                         unsigned long long input_size_in_256_bit_blocks,
 *			                   unsigned long long num_steps)
 *
 * This is a C code implementation of the 256-bit compression function.  It
 * is optimized for 64-bit machines.  It runs in constant time.  It is in the
 * file "essence_compress_256_64_const_time.c"
 *
 *  USES LOOK-UP TABLES:   NO
 *  ASSUMES x86 CPU:       NO
 *  USES SSE CODE:         NO
 *  ASSUMES 64-BIT CPU:    NO
 *  ASSUMES LITTLE ENDIAN: NO
 *
 */
void essence_compress_256_64_const_time(unsigned int *,
			                unsigned char *,
			                unsigned long long,
			                unsigned long long);


/*
 * void essence_compress_256(unsigned int *Chaining_Variables,
 *                           unsigned char *input,
 *                           unsigned long long input_size_in_256_bit_blocks,
 *			     unsigned long long num_steps)
 *
 * This is a C code implementation of the 256-bit compression
 * function.  It is in the file "essence_compress_256.c"
 *
 *  USES LOOK-UP TABLES:   YES
 *  ASSUMES x86 CPU:       NO
 *  USES SSE CODE:         NO
 *  ASSUMES 64-BIT CPU:    NO
 *  ASSUMES LITTLE ENDIAN: NO
 *
 */
void essence_compress_256(unsigned int *,
			  unsigned char *,
			  unsigned long long,
			  unsigned long long);

/*
 * void essence_compress_256_const_time(unsigned int *Chaining_Variables,
 *                                      unsigned char *input,
 *                                      unsigned long long input_size_in_256_bit_blocks,
 *			                unsigned long long num_steps)
 *
 * This is a C code implementation of the 256-bit compression
 * function.  It is in the file "essence_compress_256.c"
 *
 *  USES LOOK-UP TABLES:   NO
 *  ASSUMES x86 CPU:       NO
 *  USES SSE CODE:         NO
 *  ASSUMES 64-BIT CPU:    NO
 *  ASSUMES LITTLE ENDIAN: NO
 *
 */
void essence_compress_256_const_time(unsigned int *,
				     unsigned char *,
				     unsigned long long,
				     unsigned long long);

#endif /* _ESSENCE_H_ */
