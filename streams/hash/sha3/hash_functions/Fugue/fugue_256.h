#ifndef _FUGUE_256_H
#define _FUGUE_256_H

#include "fugue_t.h"
#include "aestab_t.h"

int fugue_update_256 (fugueHashState *hs, const char *in, uint_64t len);
int fugue_final_256  (fugueHashState *hs, char *out);

#endif
