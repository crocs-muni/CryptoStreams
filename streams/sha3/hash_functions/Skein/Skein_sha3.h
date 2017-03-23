#ifndef SKEIN_SHA3_H
#define SKEIN_SHA3_H

#include "../../sha3_interface.h"
extern "C" {
#include "skein.h"
}

class Skein : public sha3_interface {

typedef enum
    {
    SUCCESS     = SKEIN_SUCCESS,
    FAIL        = SKEIN_FAIL,
    BAD_HASHLEN = SKEIN_BAD_HASHLEN
    }
    HashReturn;

//typedef size_t   DataLength;                /* bit count  type */
//typedef u08b_t   BitSequence;               /* bit stream type */

typedef struct
    {
    uint_t  statebits;                      /* 256, 512, or 1024 */
    union
        {
        Skein_Ctxt_Hdr_t h;                 /* common header "overlay" */
        Skein_256_Ctxt_t ctx_256;
        Skein_512_Ctxt_t ctx_512;
        Skein1024_Ctxt_t ctx1024;
        } u;
    }
    hashState;

private:
    hashState skeinState;
    const size_t _num_rounds;

public:
    // 256b and 512b has same rounds, only 1024 has 80 rounds
    Skein(const int num_rounds=SKEIN_256_ROUNDS_TOTAL)
        : _num_rounds(num_rounds) {}
    /* "incremental" hashing API */
    int Init  (int hashbitlen);
    int Update(const BitSequence *data, DataLength databitlen);
    int Final (BitSequence *hashval);

    /* "all-in-one" call */
    int Hash  (int hashbitlen,   const BitSequence *data,
                      DataLength databitlen,  BitSequence *hashval);

/*
** Re-define the compile-time constants below to change the selection
** of the Skein state size in the Init() function in SHA3api_ref.c.
**
** That is, the NIST API does not allow for explicit selection of the
** Skein block size, so it must be done implicitly in the Init() function.
** The selection is controlled by these constants.
*/
#ifndef SKEIN_256_NIST_MAX_HASHBITS
#define SKEIN_256_NIST_MAX_HASHBITS (0)
#endif

#ifndef SKEIN_512_NIST_MAX_HASHBITS
#define SKEIN_512_NIST_MAX_HASHBITS (512)
#endif

};

#endif
