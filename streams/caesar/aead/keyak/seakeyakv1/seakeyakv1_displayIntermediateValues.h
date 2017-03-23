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

#ifndef _displayIntermediateValues_h_
#define _displayIntermediateValues_h_

#include <stdio.h>
#include "seakeyakv1_KeccakF-interface.h"

// CHANGE namespace moved due to includes
namespace Seakeyakv1_raw {

void displaySetIntermediateValueFile(FILE *f);
void displaySetLevel(int level);
void displayBytes(int level, const char *text, const unsigned char *bytes, unsigned int size);
void displayBits(int level, const char *text, const unsigned char *data, unsigned int size, int MSBfirst);
void displayStateAsBytes(int level, const char *text, const unsigned char *state);
#if (KeccakF_width == 1600)
void displayStateAs32bitWords(int level, const char *text, const unsigned int *state);
#endif
void displayStateAsLanes(int level, const char *text, void *statePointer);
void displayRoundNumber(int level, unsigned int i);
void displayText(int level, const char *text);

} // namespace Seakeyakv1_raw

#endif
