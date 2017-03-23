#include "StreamHash_sha3.h"
#include "sbox32.h"

int StreamHash::Init(int hashbitlen) {
    int i;

    switch(hashbitlen) {
        case 224: case 256: case 384: case 512: break;
        default: return BAD_HASHLEN;
    }
    streamhashState.hashbitlen=hashbitlen;
    streamhashState.tablen=hashbitlen/32;
    for(i=0; i<streamhashState.tablen; ++i)
        streamhashState.tabval[i]=0; /* initial state value */
    streamhashState.bitval=streamhashState.bitlen=0;
    return SUCCESS;
}

#define streamhash_process_word(i) streamhashState.tabval[i]^=streamhash_sbox32[(streamhashState.tabval[i]^b^i)&0xff]

int StreamHash::Update(const BitSequence *data,
        DataLength databitlen) {
    DataLength p, divlen=databitlen/8;
    BitSequence modlen=databitlen%8;
    int b;

    /* "all calls to  update will contain data lengths that
       are divisible by 8, except, possibly, the last call" */
    if(streamhashState.bitlen) /* it was previously changed to a non-zero value */
        return FAIL;

    /* the main loop */
    for(p=0; p<divlen; ++p) { /* each input byte */
        b=data[p];
        switch(streamhashState.tablen) {
        case 16: /* 512 bits */
            streamhash_process_word(15);
            streamhash_process_word(14);
            streamhash_process_word(13);
            streamhash_process_word(12);
        case 12: /* 384 bits */
            streamhash_process_word(11);
            streamhash_process_word(10);
            streamhash_process_word(9);
            streamhash_process_word(8);
        case 8: /* 256 bits */
            streamhash_process_word(7);
        case 7: /* 224 bits */
            streamhash_process_word(6);
            streamhash_process_word(5);
            streamhash_process_word(4);
            streamhash_process_word(3);
            streamhash_process_word(2);
            streamhash_process_word(1);
            streamhash_process_word(0);
        }
    }

    /* save the remaining bits if any */
    if(modlen) { /* data length is not divisible by 8 (last call) */
        streamhashState.bitval=(BitSequence)(data[p] & 0xff<<(8-modlen));
        streamhashState.bitlen=modlen;
    }
    return SUCCESS;
}

int StreamHash::Final(BitSequence *hashval) {
    int i;

    /* update the state vector with the remaining input bits */
    hashval[0]=streamhashState.bitval;
    hashval[1]=streamhashState.bitlen;
    streamhashState.bitlen=0; /* reset the number of remaining bits */
    StreamHash::Update(hashval, 16); /* two bytes */

    /* update with least significant bytes of the state vector */
    for(i=0; i<streamhashState.tablen; ++i) {
        hashval[2*i+1]=(BitSequence)(streamhashState.tabval[i]>>8);
        hashval[2*i]=(BitSequence)(streamhashState.tabval[i]); /* LSB */
    }
    StreamHash::Update(hashval, 16*streamhashState.tablen);

    /* diffuse the state vector  */
    switch(streamhashState.tablen) {
    case 7: /* 224 bits */
        for(i=0; i<3; ++i) { /* 3 interations per 32-bit word */
            streamhashState.tabval[1]+=streamhashState.tabval[0];
            streamhashState.tabval[2]+=streamhashState.tabval[1];
            streamhashState.tabval[3]+=streamhashState.tabval[2];
            streamhashState.tabval[4]+=streamhashState.tabval[3];
            streamhashState.tabval[5]+=streamhashState.tabval[4];
            streamhashState.tabval[6]+=streamhashState.tabval[5];
            streamhashState.tabval[0]+=streamhashState.tabval[6];
        }
        break;
    case 8: /* 256 bits */
        for(i=0; i<3; ++i) { /* 3 interations per 32-bit word */
            streamhashState.tabval[1]+=streamhashState.tabval[0];
            streamhashState.tabval[2]+=streamhashState.tabval[1];
            streamhashState.tabval[3]+=streamhashState.tabval[2];
            streamhashState.tabval[4]+=streamhashState.tabval[3];
            streamhashState.tabval[5]+=streamhashState.tabval[4];
            streamhashState.tabval[6]+=streamhashState.tabval[5];
            streamhashState.tabval[7]+=streamhashState.tabval[6];
            streamhashState.tabval[0]+=streamhashState.tabval[7];
        }
        break;
    case 12: /* 384 bits */
        for(i=0; i<3; ++i) { /* 3 interations per 32-bit word */
            streamhashState.tabval[1]+=streamhashState.tabval[0];
            streamhashState.tabval[2]+=streamhashState.tabval[1];
            streamhashState.tabval[3]+=streamhashState.tabval[2];
            streamhashState.tabval[4]+=streamhashState.tabval[3];
            streamhashState.tabval[5]+=streamhashState.tabval[4];
            streamhashState.tabval[6]+=streamhashState.tabval[5];
            streamhashState.tabval[7]+=streamhashState.tabval[6];
            streamhashState.tabval[8]+=streamhashState.tabval[7];
            streamhashState.tabval[9]+=streamhashState.tabval[8];
            streamhashState.tabval[10]+=streamhashState.tabval[9];
            streamhashState.tabval[11]+=streamhashState.tabval[10];
            streamhashState.tabval[0]+=streamhashState.tabval[11];
        }
        break;
    case 16: /* 512 bits */
        for(i=0; i<3; ++i) { /* 3 interations per 32-bit word */
            streamhashState.tabval[1]+=streamhashState.tabval[0];
            streamhashState.tabval[2]+=streamhashState.tabval[1];
            streamhashState.tabval[3]+=streamhashState.tabval[2];
            streamhashState.tabval[4]+=streamhashState.tabval[3];
            streamhashState.tabval[5]+=streamhashState.tabval[4];
            streamhashState.tabval[6]+=streamhashState.tabval[5];
            streamhashState.tabval[7]+=streamhashState.tabval[6];
            streamhashState.tabval[8]+=streamhashState.tabval[7];
            streamhashState.tabval[9]+=streamhashState.tabval[8];
            streamhashState.tabval[10]+=streamhashState.tabval[9];
            streamhashState.tabval[11]+=streamhashState.tabval[10];
            streamhashState.tabval[12]+=streamhashState.tabval[11];
            streamhashState.tabval[13]+=streamhashState.tabval[12];
            streamhashState.tabval[14]+=streamhashState.tabval[13];
            streamhashState.tabval[15]+=streamhashState.tabval[14];
            streamhashState.tabval[0]+=streamhashState.tabval[15];
        }
    }

    /* copy the state vector using high-endian byte order */
    /* and diffuse the result */
    hashval[0]=(BitSequence)(streamhashState.tabval[0]>>24);
    hashval[1]=hashval[0]+(BitSequence)(streamhashState.tabval[0]>>16);
    hashval[2]=hashval[1]+(BitSequence)(streamhashState.tabval[0]>>8);
    hashval[3]=hashval[2]+(BitSequence)(streamhashState.tabval[0]);

    hashval[4]=hashval[3]+(BitSequence)(streamhashState.tabval[1]>>24);
    hashval[5]=hashval[4]+(BitSequence)(streamhashState.tabval[1]>>16);
    hashval[6]=hashval[5]+(BitSequence)(streamhashState.tabval[1]>>8);
    hashval[7]=hashval[6]+(BitSequence)(streamhashState.tabval[1]);

    hashval[8]=hashval[7]+(BitSequence)(streamhashState.tabval[2]>>24);
    hashval[9]=hashval[8]+(BitSequence)(streamhashState.tabval[2]>>16);
    hashval[10]=hashval[9]+(BitSequence)(streamhashState.tabval[2]>>8);
    hashval[11]=hashval[10]+(BitSequence)(streamhashState.tabval[2]);

    hashval[12]=hashval[11]+(BitSequence)(streamhashState.tabval[3]>>24);
    hashval[13]=hashval[12]+(BitSequence)(streamhashState.tabval[3]>>16);
    hashval[14]=hashval[13]+(BitSequence)(streamhashState.tabval[3]>>8);
    hashval[15]=hashval[14]+(BitSequence)(streamhashState.tabval[3]);

    hashval[16]=hashval[15]+(BitSequence)(streamhashState.tabval[4]>>24);
    hashval[17]=hashval[16]+(BitSequence)(streamhashState.tabval[4]>>16);
    hashval[18]=hashval[17]+(BitSequence)(streamhashState.tabval[4]>>8);
    hashval[19]=hashval[18]+(BitSequence)(streamhashState.tabval[4]);

    hashval[20]=hashval[19]+(BitSequence)(streamhashState.tabval[5]>>24);
    hashval[21]=hashval[20]+(BitSequence)(streamhashState.tabval[5]>>16);
    hashval[22]=hashval[21]+(BitSequence)(streamhashState.tabval[5]>>8);
    hashval[23]=hashval[22]+(BitSequence)(streamhashState.tabval[5]);

    hashval[24]=hashval[23]+(BitSequence)(streamhashState.tabval[6]>>24);
    hashval[25]=hashval[24]+(BitSequence)(streamhashState.tabval[6]>>16);
    hashval[26]=hashval[25]+(BitSequence)(streamhashState.tabval[6]>>8);
    hashval[27]=hashval[26]+(BitSequence)(streamhashState.tabval[6]);

    if(streamhashState.tablen==7) {
        hashval[0]+=hashval[27];
        return SUCCESS;
    }

    hashval[28]=hashval[27]+(BitSequence)(streamhashState.tabval[7]>>24);
    hashval[29]=hashval[28]+(BitSequence)(streamhashState.tabval[7]>>16);
    hashval[30]=hashval[29]+(BitSequence)(streamhashState.tabval[7]>>8);
    hashval[31]=hashval[30]+(BitSequence)(streamhashState.tabval[7]);

    if(streamhashState.tablen==8) {
        hashval[0]+=hashval[31];
        return SUCCESS;
    }

    hashval[32]=hashval[31]+(BitSequence)(streamhashState.tabval[8]>>24);
    hashval[33]=hashval[32]+(BitSequence)(streamhashState.tabval[8]>>16);
    hashval[34]=hashval[33]+(BitSequence)(streamhashState.tabval[8]>>8);
    hashval[35]=hashval[34]+(BitSequence)(streamhashState.tabval[8]);

    hashval[36]=hashval[35]+(BitSequence)(streamhashState.tabval[9]>>24);
    hashval[37]=hashval[36]+(BitSequence)(streamhashState.tabval[9]>>16);
    hashval[38]=hashval[37]+(BitSequence)(streamhashState.tabval[9]>>8);
    hashval[39]=hashval[38]+(BitSequence)(streamhashState.tabval[9]);

    hashval[40]=hashval[39]+(BitSequence)(streamhashState.tabval[10]>>24);
    hashval[41]=hashval[40]+(BitSequence)(streamhashState.tabval[10]>>16);
    hashval[42]=hashval[41]+(BitSequence)(streamhashState.tabval[10]>>8);
    hashval[43]=hashval[42]+(BitSequence)(streamhashState.tabval[10]);

    hashval[44]=hashval[43]+(BitSequence)(streamhashState.tabval[11]>>24);
    hashval[45]=hashval[44]+(BitSequence)(streamhashState.tabval[11]>>16);
    hashval[46]=hashval[45]+(BitSequence)(streamhashState.tabval[11]>>8);
    hashval[47]=hashval[46]+(BitSequence)(streamhashState.tabval[11]);

    if(streamhashState.tablen==12) {
        hashval[0]+=hashval[47];
        return SUCCESS;
    }

    hashval[48]=hashval[47]+(BitSequence)(streamhashState.tabval[12]>>24);
    hashval[49]=hashval[48]+(BitSequence)(streamhashState.tabval[12]>>16);
    hashval[50]=hashval[49]+(BitSequence)(streamhashState.tabval[12]>>8);
    hashval[51]=hashval[50]+(BitSequence)(streamhashState.tabval[12]);

    hashval[52]=hashval[51]+(BitSequence)(streamhashState.tabval[13]>>24);
    hashval[53]=hashval[52]+(BitSequence)(streamhashState.tabval[13]>>16);
    hashval[54]=hashval[53]+(BitSequence)(streamhashState.tabval[13]>>8);
    hashval[55]=hashval[54]+(BitSequence)(streamhashState.tabval[13]);

    hashval[56]=hashval[55]+(BitSequence)(streamhashState.tabval[14]>>24);
    hashval[57]=hashval[56]+(BitSequence)(streamhashState.tabval[14]>>16);
    hashval[58]=hashval[57]+(BitSequence)(streamhashState.tabval[14]>>8);
    hashval[59]=hashval[58]+(BitSequence)(streamhashState.tabval[14]);

    hashval[60]=hashval[59]+(BitSequence)(streamhashState.tabval[15]>>24);
    hashval[61]=hashval[60]+(BitSequence)(streamhashState.tabval[15]>>16);
    hashval[62]=hashval[61]+(BitSequence)(streamhashState.tabval[15]>>8);
    hashval[63]=hashval[62]+(BitSequence)(streamhashState.tabval[15]);

    hashval[0]+=hashval[63];
    return SUCCESS;
}

int StreamHash::Hash(int hashbitlen, const BitSequence *data,
        DataLength databitlen, BitSequence *hashval) {
    //hashState state;

    return ((int)StreamHash::Init(hashbitlen) ||
        (int)StreamHash::Update(data, databitlen) ||
        (int)StreamHash::Final(hashval));
}