#ifndef __HAMSI_H__
#define __HAMSI_H__

//#include "words.h"

int hamsi_hash256(const int ROUNDS, unsigned int*chainVal, const unsigned char*d, int lastiter);
int hamsi_hash512(const int ROUNDS, unsigned int*chainVal, const unsigned char*d, int lastiter);

#endif
