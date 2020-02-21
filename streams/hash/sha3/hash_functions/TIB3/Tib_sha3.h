#ifndef TIB_SHA3_H
#define TIB_SHA3_H

#include "../../sha3_interface.h"
extern "C" {
#include "inupfin.h"
}
namespace sha3 {

class Tib : public sha3_interface {

#define TIB_DEFAULT_ROUNDS 16

private:
int tibNumRounds;
tibhashState tibState;

public:
Tib(const int numRounds);
/* prototypes of functions required by the API */
int Init (int hashbitlen);
int Update (const BitSequence *data, DataLength databitlen);
int Final (BitSequence *hashval);
int Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval);

};

}
#endif