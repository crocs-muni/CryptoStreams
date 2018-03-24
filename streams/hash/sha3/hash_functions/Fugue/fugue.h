#ifndef _FUGUE_H
#define _FUGUE_H

#include "fugue_t.h"

unsigned long Init_Fugue (fugueHashState *hs, int hashbitlen, const int rounds1, 
						  const int rounds2, const int rounds3, const int rounds4);
unsigned long Load_Fugue (fugueHashState *state, int hashbitlen, const unsigned long *iv_key, int ivwordlen);
unsigned long Next_Fugue (fugueHashState *state, const unsigned long *data, unsigned long long datawordlen);
unsigned long Done_Fugue (fugueHashState *state, unsigned long *hashval, int *hashwordlen);

#endif
