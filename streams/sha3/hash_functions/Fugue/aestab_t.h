#ifndef _AESTAB_T_H
#define _AESTAB_T_H

#include <stdio.h>
#include <limits.h>

#if defined(__WORDSIZE) && __WORDSIZE == 64
typedef unsigned int uint_32t;
typedef unsigned long uint_64t;
#else
typedef unsigned long uint_32t;
typedef unsigned long long uint_64t;
#endif
typedef unsigned char uint_8t;

#endif
