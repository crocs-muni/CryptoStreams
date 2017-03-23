
#include "fugue.h"
#include "fugue_256.h"
#include "fugue_384.h"
#include "fugue_512.h"


static struct {
    int       hashbitlen;
    fugueHashCfg   Cfg;
    unsigned long    IV[16];
    }         hashSizes[] = {{224,{7 ,30,2,5,13},{0}}
                            ,{256,{8 ,30,2,5,13},{0}}
                            ,{384,{12,36,3,6,13},{0}}
                            ,{512,{16,36,4,8,13},{0}}
                            ,{0}
                            };

int make_fugue = 0;

void
Make_Fugue ()
{
    uint_32t input[16];
    fugueHashState hs;
    int i;

    if (make_fugue) return;
    make_fugue = 1;

    /* generate IV for each hashbitlen in hashSizes table */
    for (i=0; hashSizes[i].hashbitlen; i++)
    {
        int n = hashSizes[i].Cfg.n;

        /* clear initial hash state, this also effectively swets the IV to zero's */
        memset (&hs, 0, sizeof (fugueHashState));

        /* set NIST hashbitlen field in fugueHashState based on hashwordlen field of hashSizes table */
        hs.hashbitlen = n*32;

        /* save pointer to Fugue configuration for hashbitlen in Cfg field in fugueHashState */
        hs.Cfg = &hashSizes[i].Cfg;

        /* set 1 word message to 32-bit big-endian of hashbitlen number */
        input[0] = FUGUE_HO2BE_4 (hashSizes[i].hashbitlen);

        /* process 1 word message into fugueHashState */
        switch (hs.hashbitlen)
        {
        case 224:
        case 256: fugue_update_256 (&hs, (char*) input, 1); break;
        case 384: fugue_update_384 (&hs, (char*) input, 1); break;
        case 512: fugue_update_512 (&hs, (char*) input, 1); break;
        }

        /* process final rounds and place message digest in hashSizes tables IV field */
        switch (hs.hashbitlen)
        {
        case 224:
        case 256: fugue_final_256 (&hs, (char*) hashSizes[i].IV); break;
        case 384: fugue_final_384 (&hs, (char*) hashSizes[i].IV); break;
        case 512: fugue_final_512 (&hs, (char*) hashSizes[i].IV); break;
        }

    }
}

unsigned long
Init_Fugue (fugueHashState *hs, int hashbitlen, const int rounds1, const int rounds2, const int rounds3, const int rounds4)
{
    int i;
	//NASTAVENIE RUND:
	hashSizes[0].Cfg.r = rounds1;
	hashSizes[1].Cfg.r = rounds1;
	hashSizes[2].Cfg.r = rounds2;
	hashSizes[3].Cfg.r = rounds3;
	hashSizes[0].Cfg.r = rounds4;
	hashSizes[1].Cfg.r = rounds4;
	hashSizes[2].Cfg.r = rounds4;
	hashSizes[3].Cfg.r = rounds4;

    Make_Fugue();
    if (!hs) return 0;
    memset (hs, 0, sizeof (fugueHashState));
    for (i=0; hashSizes[i].hashbitlen; i++)
        if (hashSizes[i].hashbitlen==hashbitlen)
        {
            int n = hashSizes[i].Cfg.n;
            int s = hashSizes[i].Cfg.s;
            hs->hashbitlen = n*32;
            hs->Cfg = &hashSizes[i].Cfg;
            memcpy (&hs->State[s-n].d, hashSizes[i].IV, n*4);
            return 1;
        }
    return 0;
}

unsigned long
Load_Fugue (fugueHashState* hs, int hashbitlen, const unsigned long* iv, int hashwordlen)
{
    int i;

    Make_Fugue ();
    if (!hs) return 0;
    memset (hs, 0, sizeof (fugueHashState));
    for (i=0; hashSizes[i].hashbitlen; i++)
        if (hashSizes[i].hashbitlen==hashbitlen && hashSizes[i].Cfg.n==hashwordlen)
        {
            int n = hashSizes[i].Cfg.n;
            int s = hashSizes[i].Cfg.s;
            hs->hashbitlen = n*32;
            hs->Cfg = &hashSizes[i].Cfg;
            memcpy (&hs->State[s-n].d, iv, n*4);
            return 1;
        }
    return 0;
}

unsigned long
Next_Fugue (fugueHashState* hs, const unsigned long* msg, unsigned long long len)
{
    if (hs && hs->Cfg)
    {
        switch (hs->hashbitlen)
        {
        case 224:
        case 256: fugue_update_256 (hs, (char*) msg, len); break;
        case 384: fugue_update_384 (hs, (char*) msg, len); break;
        case 512: fugue_update_512 (hs, (char*) msg, len); break;
        }
        return 1;
    }
    return 0;
}

unsigned long
Done_Fugue (fugueHashState* hs, unsigned long* md, int* hashwordlen)
{
    if (hs && hs->Cfg)
    {
        int n = hs->Cfg->n;
        switch (hs->hashbitlen)
        {
        case 224:
        case 256: fugue_final_256 (hs, (char*) md); break;
        case 384: fugue_final_384 (hs, (char*) md); break;
        case 512: fugue_final_512 (hs, (char*) md); break;
        }
        if (hashwordlen) *hashwordlen = n;
        return 1;
    }
    return 0;
}
