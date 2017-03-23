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

#include "ketjejrv1_brg_endian.h"
#include "ketjejrv1_displayIntermediateValues.h"
#include "ketjejrv1_KeccakP-200-reference.h"

// CHANGE namespace moved due to includes
namespace Ketjejrv1_raw {

typedef unsigned char UINT8;
typedef UINT8 tKeccakLane;

#define nrRounds 18

void KeccakF200Round(tKeccakLane *state, unsigned int indexRound); // From KeccakF-200/Reference

void KeccakP200_StatePermute(void *state, unsigned int nr)
{
    displayStateAsBytes(1, "Input of permutation", (const unsigned char *)state);
    for(nr=nrRounds-nr; nr<nrRounds; nr++)
        // CHANGE static cast added
		KeccakF200Round(static_cast<tKeccakLane*>(state), nr);
    displayStateAsBytes(1, "State after permutation", (const unsigned char *)state);
}

} // namespace Ketjejrv1_raw
