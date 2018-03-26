//
// Created by Dusan Klinec on 26/03/2018.
//

#include "idea.h"
namespace block {
namespace idea {

// BEGIN https://github.com/openssl/openssl/blob/master/crypto/idea/idea_lcl.h
#define idea_mul(r, a, b, ul) \
ul=(unsigned long)a*b; \
if (ul != 0) \
    { \
    r=(ul&0xffff)-(ul>>16); \
    r-=((r)>>16); \
    } \
else \
    r=(-(int)a-b+1);        /* assuming a or b is 0 and in range */

#undef IDEA_n2l
#define IDEA_n2l(c, l)        (l =((unsigned long)(*((c)++)))<<24L, \
                     l|=((unsigned long)(*((c)++)))<<16L, \
                     l|=((unsigned long)(*((c)++)))<< 8L, \
                     l|=((unsigned long)(*((c)++))))

#undef IDEA_l2n
#define IDEA_l2n(l, c)        (*((c)++)=(unsigned char)(((l)>>24L)&0xff), \
                     *((c)++)=(unsigned char)(((l)>>16L)&0xff), \
                     *((c)++)=(unsigned char)(((l)>> 8L)&0xff), \
                     *((c)++)=(unsigned char)(((l)     )&0xff))

#undef IDEA_s2n
#define IDEA_s2n(l, c)        (*((c)++)=(unsigned char)(((l)     )&0xff), \
                     *((c)++)=(unsigned char)(((l)>> 8L)&0xff))

#undef IDEA_n2s
#define IDEA_n2s(c, l)        (l =((IDEA_INT)(*((c)++)))<< 8L, \
                     l|=((IDEA_INT)(*((c)++)))      )


#define E_IDEA(num) \
    x1&=0xffff; \
    idea_mul(x1,x1,*p,ul); p++; \
    x2+= *(p++); \
    x3+= *(p++); \
    x4&=0xffff; \
    idea_mul(x4,x4,*p,ul); p++; \
    t0=(x1^x3)&0xffff; \
    idea_mul(t0,t0,*p,ul); p++; \
    t1=(t0+(x2^x4))&0xffff; \
    idea_mul(t1,t1,*p,ul); p++; \
    t0+=t1; \
    x1^=t1; \
    x4^=t0; \
    ul=x2^t0; /* do the swap to x3 */ \
    x2=x3^t1; \
    x3=ul;
// END OF https://github.com/openssl/openssl/blob/master/crypto/idea/idea_lcl.h

// https://github.com/openssl/openssl/blob/master/crypto/idea/i_skey.c
static IDEA_INT inverse(unsigned int xin);
void IDEA_set_encrypt_key(const unsigned char *key, IDEA_KEY_SCHEDULE *ks)
{
    int i;
    IDEA_INT *kt, *kf, r0, r1, r2;

    kt = &(ks->data[0][0]);
    IDEA_n2s(key, kt[0]);
    IDEA_n2s(key, kt[1]);
    IDEA_n2s(key, kt[2]);
    IDEA_n2s(key, kt[3]);
    IDEA_n2s(key, kt[4]);
    IDEA_n2s(key, kt[5]);
    IDEA_n2s(key, kt[6]);
    IDEA_n2s(key, kt[7]);

    kf = kt;
    kt += 8;
    for (i = 0; i < 6; i++) {
        r2 = kf[1];
        r1 = kf[2];
        *(kt++) = ((r2 << 9) | (r1 >> 7)) & 0xffff;
        r0 = kf[3];
        *(kt++) = ((r1 << 9) | (r0 >> 7)) & 0xffff;
        r1 = kf[4];
        *(kt++) = ((r0 << 9) | (r1 >> 7)) & 0xffff;
        r0 = kf[5];
        *(kt++) = ((r1 << 9) | (r0 >> 7)) & 0xffff;
        r1 = kf[6];
        *(kt++) = ((r0 << 9) | (r1 >> 7)) & 0xffff;
        r0 = kf[7];
        *(kt++) = ((r1 << 9) | (r0 >> 7)) & 0xffff;
        r1 = kf[0];
        if (i >= 5)
            break;
        *(kt++) = ((r0 << 9) | (r1 >> 7)) & 0xffff;
        *(kt++) = ((r1 << 9) | (r2 >> 7)) & 0xffff;
        kf += 8;
    }
}

void IDEA_set_decrypt_key(IDEA_KEY_SCHEDULE *ek, IDEA_KEY_SCHEDULE *dk)
{
    int r;
    IDEA_INT *fp, *tp, t;

    tp = &(dk->data[0][0]);
    fp = &(ek->data[8][0]);
    for (r = 0; r < 9; r++) {
        *(tp++) = inverse(fp[0]);
        *(tp++) = ((int)(0x10000L - fp[2]) & 0xffff);
        *(tp++) = ((int)(0x10000L - fp[1]) & 0xffff);
        *(tp++) = inverse(fp[3]);
        if (r == 8)
            break;
        fp -= 6;
        *(tp++) = fp[4];
        *(tp++) = fp[5];
    }

    tp = &(dk->data[0][0]);
    t = tp[1];
    tp[1] = tp[2];
    tp[2] = t;

    t = tp[49];
    tp[49] = tp[50];
    tp[50] = t;
}

/* taken directly from the 'paper' I'll have a look at it later */
static IDEA_INT inverse(unsigned int xin)
{
    long n1, n2, q, r, b1, b2, t;

    if (xin == 0)
        b2 = 0;
    else {
        n1 = 0x10001;
        n2 = xin;
        b2 = 1;
        b1 = 0;

        do {
            r = (n1 % n2);
            q = (n1 - r) / n2;
            if (r == 0) {
                if (b2 < 0)
                    b2 = 0x10001 + b2;
            } else {
                n1 = n2;
                n2 = r;
                t = b2;
                b2 = b1 - q * b2;
                b1 = t;
            }
        } while (r != 0);
    }
    return (IDEA_INT)b2;
}

// https://github.com/openssl/openssl/blob/master/crypto/idea/i_cbc.c
void IDEA_encrypt(unsigned long *d, IDEA_KEY_SCHEDULE *key, int nr)
{
    IDEA_INT *p;
    unsigned long x1, x2, x3, x4, t0, t1, ul;

    x2 = d[0];
    x1 = (x2 >> 16);
    x4 = d[1];
    x3 = (x4 >> 16);

    p = &(key->data[0][0]);


    for(int i=0; i<nr; ++i){
        E_IDEA(i);
    }

    x1 &= 0xffff;
    idea_mul(x1, x1, *p, ul);
    p++;

    t0 = x3 + *(p++);
    t1 = x2 + *(p++);

    x4 &= 0xffff;
    idea_mul(x4, x4, *p, ul);

    d[0] = (t0 & 0xffff) | ((x1 & 0xffff) << 16);
    d[1] = (x4 & 0xffff) | ((t1 & 0xffff) << 16);
}


// https://github.com/openssl/openssl/blob/master/crypto/idea/i_ecb.c
void IDEA_ecb_encrypt(const unsigned char *in, unsigned char *out,
                      IDEA_KEY_SCHEDULE *ks, int nr)
{
    unsigned long l0, l1, d[2];

    IDEA_n2l(in, l0);
    d[0] = l0;
    IDEA_n2l(in, l1);
    d[1] = l1;
    IDEA_encrypt(d, ks, nr);
    l0 = d[0];
    IDEA_l2n(l0, out);
    l1 = d[1];
    IDEA_l2n(l1, out);
    l0 = l1 = d[0] = d[1] = 0;
}

}}