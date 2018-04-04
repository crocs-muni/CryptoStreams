#ifndef CRUNCH_TYPE_H
#define CRUNCH_TYPE_H

typedef unsigned char CrunchBitSequence;
typedef unsigned long long CrunchDataLength;
typedef enum { CRUNCH_SUCCESS = 0, CRUNCH_FAIL = 1, CRUNCH_BAD_HASHBITLEN = 2 } CrunchHashReturn;

typedef struct 
{
	int hashbitlen;
} crunchHashState;

#endif