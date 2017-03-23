#include "Fugue_sha3.h"
extern "C" {
#include "fugue.h"
}

Fugue::Fugue(const int numRounds) {
	if (numRounds == -1) {
		fugueNumRoundsParam1 = FUGUE_ROUNDS_PARAM_R_224_256;
		fugueNumRoundsParam2 = FUGUE_ROUNDS_PARAM_R_384;
		fugueNumRoundsParam3 = FUGUE_ROUNDS_PARAM_R_512;
		fugueNumRoundsParam4 = FUGUE_ROUNDS_PARAM_T;
	} else {
		fugueNumRoundsParam1 = numRounds;
		fugueNumRoundsParam2 = numRounds;
		fugueNumRoundsParam3 = numRounds;
		fugueNumRoundsParam4 = numRounds;
	}
}

int
Fugue::Init (int hashbitlen)
{
    if (Init_Fugue(&fugueState, hashbitlen, fugueNumRoundsParam1, fugueNumRoundsParam2, 
		fugueNumRoundsParam3, fugueNumRoundsParam4)) return SUCCESS;
    return FAIL;
}

int
Fugue::Update (const BitSequence *data, DataLength databitlen)
{
    if (!(&fugueState) || !fugueState.Cfg)
        return FAIL;
    if (!databitlen)
        return SUCCESS;
    if (fugueState.TotalBits&7)
        return FAIL;
    if (fugueState.TotalBits&31)
    {
        int need = 32-(fugueState.TotalBits&31);
        if (need>databitlen)
        {
            memcpy ((unsigned char*)fugueState.Partial+((fugueState.TotalBits&31)/8), data, (databitlen+7)/8);
            fugueState.TotalBits += databitlen;
            return SUCCESS;
        }
        else
        {
            memcpy ((unsigned char*)fugueState.Partial+((fugueState.TotalBits&31)/8), data, need/8);
            Next_Fugue (&fugueState, fugueState.Partial, 1);
            fugueState.TotalBits += need;
            databitlen -= need;
            data += need/8;
        }
    }
    if (databitlen>31)
    {
        Next_Fugue (&fugueState, (unsigned long*)data, databitlen/32);
        fugueState.TotalBits += (databitlen/32)*32;
        data += (databitlen/32)*4;
        databitlen &= 31;
    }
    if (databitlen)
    {
        memcpy ((unsigned char*)fugueState.Partial, data, (databitlen+7)/8);
        fugueState.TotalBits += databitlen;
    }
    return SUCCESS;
}

int
Fugue::Final (BitSequence *hashval)
{
    if (!(&fugueState) || !fugueState.Cfg)
        return FAIL;
    if (fugueState.TotalBits&31)
    {
        int need = 32-(fugueState.TotalBits&31);
        memset ((unsigned char*)fugueState.Partial+(((fugueState.TotalBits&31)+7)/8), 0, need/8);
        Next_Fugue (&fugueState, fugueState.Partial, 1);
    }
    fugueState.TotalBits = FUGUE_HO2BE_8 (fugueState.TotalBits);
    Next_Fugue (&fugueState, (unsigned long*)&fugueState.TotalBits, 2);
    Done_Fugue (&fugueState, (unsigned long*)hashval, NULL);
    return SUCCESS;
}

int
Fugue::Hash (int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
    //fugueHashState HS;

    if (Fugue::Init (hashbitlen) == SUCCESS)
        if (Fugue::Update (data, databitlen) == SUCCESS)
            return Fugue::Final (hashval);
    return FAIL;
}