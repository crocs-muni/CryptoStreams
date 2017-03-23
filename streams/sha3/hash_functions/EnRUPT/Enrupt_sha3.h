#ifndef ENRUPT_SHA3_H
#define ENRUPT_SHA3_H

#include "../../sha3_interface.h"
#include "EnRUPT_opt.h"

class Enrupt : public SHA3 {

private:
enruptHashState enruptState;

public:
int Init (int hashbitlen);
int Update (const EnruptBitSequence *data, EnruptDataLength databitlen);
int Final (EnruptBitSequence *hashval);
int Hash (int hashbitlen, const EnruptBitSequence *data, EnruptDataLength databitlen, EnruptBitSequence *hashval);

};

#endif