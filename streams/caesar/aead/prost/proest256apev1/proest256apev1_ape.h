#ifndef APE_H
#define APE_H

#include "proest256apev1_api.h"
#include "proest256apev1_proest256.h"

#define APE_RBYTES (PROEST_STATEBYTES - CRYPTO_KEYBYTES) // = 32
#define APE_CBYTES CRYPTO_KEYBYTES

#endif
