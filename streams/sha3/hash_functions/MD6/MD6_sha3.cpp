#include <stdio.h>
#include "MD6_sha3.h"

int MD6::Init( int hashbitlen )
{ int err;
  if ((err = md6_init( (md6_state *) &mdsixState, 
		       hashbitlen,
			   mdsixNumRounds
		       )))
    return err;
  mdsixState.hashbitlen = hashbitlen;
  return SUCCESS;
}

int MD6::Update( const BitSequence *data, DataLength databitlen )
{ return md6_update( (md6_state *) &mdsixState, 
		     (unsigned char *)data, 
		     (uint64_t) databitlen );
}

int MD6::Final( BitSequence *hashval )
{ return md6_final( (md6_state *) &mdsixState,
		    (unsigned char *) hashval
		    );
}

int MD6::Hash( int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval )
{ int err;
  //md6_state state;
  if ((err = MD6::Init( hashbitlen ))) 
    return err;
  if ((err = MD6::Update( data, databitlen ))) 
    return err;
  return MD6::Final( hashval );
}

MD6::MD6(const int numRounds) {
	if (numRounds == -1) {
		mdsixNumRounds = MD6_DEFAULT_ROUNDS;
	} else {
		mdsixNumRounds = numRounds;
	}
}