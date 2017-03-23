#include <string.h>     /* get the memcpy/memset functions */
extern "C" {
#include "skein.h"      /* get the Skein API definitions   */
}
#include "Skein_sha3.h"/* get the  AHS  API definitions   */

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* select the context size and init the context */
int Skein::Init(int hashbitlen)
    {
#if SKEIN_256_NIST_MAX_HASH_BITS
    if (hashbitlen <= SKEIN_256_NIST_MAX_HASHBITS)
        {
        Skein_Assert(hashbitlen > 0,BAD_HASHLEN);
        skeinState.statebits = 64*SKEIN_256_STATE_WORDS;
        return Skein_256_Init(&skeinState.u.ctx_256,(size_t) hashbitlen, _num_rounds);
        }
#endif
    if (hashbitlen <= SKEIN_512_NIST_MAX_HASHBITS)
        {
        skeinState.statebits = 64*SKEIN_512_STATE_WORDS;
        return Skein_512_Init(&skeinState.u.ctx_512,(size_t) hashbitlen, _num_rounds);
        }
    else
        {
        skeinState.statebits = 64*SKEIN1024_STATE_WORDS;
        return Skein1024_Init(&skeinState.u.ctx1024,(size_t) hashbitlen, _num_rounds);
        }
    }

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* process data to be hashed */
int Skein::Update(const BitSequence *data, DataLength databitlen)
    {
    /* only the final Update() call is allowed do partial bytes, else assert an error */
    Skein_Assert((skeinState.u.h.T[1] & SKEIN_T1_FLAG_BIT_PAD) == 0 || databitlen == 0, FAIL);

    Skein_Assert(skeinState.statebits % 256 == 0 && (skeinState.statebits-256) < 1024,FAIL);
    if ((databitlen & 7) == 0)  /* partial bytes? */
        {
        switch ((skeinState.statebits >> 8) & 3)
            {
            case 2:  return Skein_512_Update(&skeinState.u.ctx_512,data,databitlen >> 3, _num_rounds);
            case 1:  return Skein_256_Update(&skeinState.u.ctx_256,data,databitlen >> 3, _num_rounds);
            case 0:  return Skein1024_Update(&skeinState.u.ctx1024,data,databitlen >> 3, _num_rounds);
            default: return FAIL;
            }
        }
    else
        {   /* handle partial final byte */
        size_t bCnt = (databitlen >> 3) + 1;                  /* number of bytes to handle (nonzero here!) */
        u08b_t b,mask;

        mask = (u08b_t) (1u << (7 - (databitlen & 7)));       /* partial byte bit mask */
        b    = (u08b_t) ((data[bCnt-1] & (0-mask)) | mask);   /* apply bit padding on final byte */

        switch ((skeinState.statebits >> 8) & 3)
            {
            case 2:  Skein_512_Update(&skeinState.u.ctx_512,data,bCnt-1, _num_rounds); /* process all but the final byte    */
                     Skein_512_Update(&skeinState.u.ctx_512,&b  ,  1   , _num_rounds); /* process the (masked) partial byte */
                     break;
            case 1:  Skein_256_Update(&skeinState.u.ctx_256,data,bCnt-1, _num_rounds); /* process all but the final byte    */
                     Skein_256_Update(&skeinState.u.ctx_256,&b  ,  1   , _num_rounds); /* process the (masked) partial byte */
                     break;
            case 0:  Skein1024_Update(&skeinState.u.ctx1024,data,bCnt-1, _num_rounds); /* process all but the final byte    */
                     Skein1024_Update(&skeinState.u.ctx1024,&b  ,  1   , _num_rounds); /* process the (masked) partial byte */
                     break;
            default: return FAIL;
            }
        Skein_Set_Bit_Pad_Flag(skeinState.u.h);                    /* set tweak flag for the final call */
        
        return SUCCESS;
        }
    }

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* finalize hash computation and output the result (hashbitlen bits) */
int Skein::Final(BitSequence *hashval)
    {
    Skein_Assert(skeinState.statebits % 256 == 0 && (skeinState.statebits-256) < 1024,FAIL);
    switch ((skeinState.statebits >> 8) & 3)
        {
        case 2:  return Skein_512_Final(&skeinState.u.ctx_512,hashval, _num_rounds);
        case 1:  return Skein_256_Final(&skeinState.u.ctx_256,hashval, _num_rounds);
        case 0:  return Skein1024_Final(&skeinState.u.ctx1024,hashval, _num_rounds);
        default: return FAIL;
        }
    }

/*++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
/* all-in-one hash function */
int Skein::Hash(int hashbitlen, const BitSequence *data, /* all-in-one call */
                DataLength databitlen,BitSequence *hashval)
    {
    //hashState  state;
    int r = Skein::Init(hashbitlen);
    if (r == SUCCESS)
        { /* these calls do not fail when called properly */
        r = Skein::Update(data,databitlen);
        Skein::Final(hashval);
        }
    return r;
    }
