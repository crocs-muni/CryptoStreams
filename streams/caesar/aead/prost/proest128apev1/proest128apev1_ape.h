#ifndef APE_H
#define APE_H

#include "proest128apev1_api.h"
#include "proest128apev1_proest128.h"

#define APE_RBYTES (PROEST_STATEBYTES - CRYPTO_KEYBYTES) // = 16
#define APE_CBYTES CRYPTO_KEYBYTES

#endif
