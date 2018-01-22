/*
The Keccak sponge function, designed by Guido Bertoni, Joan Daemen,
Michaël Peeters and Gilles Van Assche. For more information, feedback or
questions, please refer to our website: http://keccak.noekeon.org/

Implementation by the designers,
hereby denoted as "the implementer".

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

#ifndef _KeccakPermutationInterface_h_
#define _KeccakPermutationInterface_h_

#include "KeccakF-1600-int-set.h"

void KeccakInitialize( void );
void KeccakInitializeState(unsigned char *state);
void KeccakPermutation(unsigned char *state, unsigned int rounds);
#ifdef ProvideFast576
void KeccakAbsorb576bits(unsigned char *state, const unsigned char *data, unsigned int rounds);
#endif
#ifdef ProvideFast832
void KeccakAbsorb832bits(unsigned char *state, const unsigned char *data, unsigned int rounds);
#endif
#ifdef ProvideFast1024
void KeccakAbsorb1024bits(unsigned char *state, const unsigned char *data, unsigned int rounds);
#endif
#ifdef ProvideFast1088
void KeccakAbsorb1088bits(unsigned char *state, const unsigned char *data, unsigned int rounds);
#endif
#ifdef ProvideFast1152
void KeccakAbsorb1152bits(unsigned char *state, const unsigned char *data, unsigned int rounds);
#endif
#ifdef ProvideFast1344
void KeccakAbsorb1344bits(unsigned char *state, const unsigned char *data, unsigned int rounds);
#endif
void KeccakAbsorb(unsigned char *state, const unsigned char *data, unsigned int laneCount, unsigned int rounds);
#ifdef ProvideFast1024
void KeccakExtract1024bits(const unsigned char *state, unsigned char *data);
#endif
void KeccakExtract(const unsigned char *state, unsigned char *data, unsigned int laneCount);

#endif
