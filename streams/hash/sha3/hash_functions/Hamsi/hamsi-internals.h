#ifndef __HAMSI_INTERNALS_H__
#define __HAMSI_INTERNALS_H__

#include "hamsi.h"
//#include "words.h"
#include "hamsi-tables.h"

void Mix(unsigned int* a,unsigned int* b,unsigned int* c,unsigned int* d);

void subst256(unsigned int s[5][4]);
void subst512(unsigned int s[5][8]);

void diffuse256(unsigned int s[5][4]);
void diffuse512(unsigned int s[5][8]);

#endif
