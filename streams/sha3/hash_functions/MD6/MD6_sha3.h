#ifndef MD6_SHA3_H
#define MD6_SHA3_H

#include "../../sha3_interface.h"
extern "C" {
#include "md6.h"
}

class MD6 : public sha3_interface {

#define MD6_DEFAULT_ROUNDS	104

typedef unsigned char BitSequence;
typedef unsigned long long DataLength;
typedef enum { SUCCESS = 0, FAIL = 1, BAD_HASHLEN = 2 } HashReturn;

//typedef md6_state mdsixhashState;

private:
int mdsixNumRounds;
md6_state mdsixState;

public:
MD6(const int numRounds);
int Init( int hashbitlen );
int Update( const BitSequence *data, DataLength databitlen );
int Final( BitSequence *hashval );
int Hash( int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval );

};

#endif