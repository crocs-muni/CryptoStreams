#ifndef __HAMSI_TABLES_H__
#define __HAMSI_TABLES_H__

//#include "words.h"

// round constants
extern const unsigned int hamsi_alpha[2][4][8];

// IV's
extern const unsigned int hamsi_iv224[8];
extern const unsigned int hamsi_iv256[8];
extern const unsigned int hamsi_iv384[16];
extern const unsigned int hamsi_iv512[16];

// expansion tables
extern const unsigned int hamsi_T256[4][256][8];
extern const unsigned int hamsi_T512[8][256][16];

#endif
