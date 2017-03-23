#include "DSHA_sha3.h"

const unsigned long dsha_i224[8] =
{
    0xc1059ed8ul, 0x367cd507ul, 0x3070dd17ul, 0xf70e5939ul,
    0xffc00b31ul, 0x68581511ul, 0x64f98fa7ul, 0xbefa4fa4ul
};
const unsigned long dsha_i256[8] =
{
    0x6a09e667ul, 0xbb67ae85ul, 0x3c6ef372ul, 0xa54ff53aul,
    0x510e527ful, 0x9b05688cul, 0x1f83d9abul, 0x5be0cd19ul
};
const unsigned long  dsha_i384[16] =
{
    0xcbbb9d5d,  0xc1059ed8,  0x629a292a,  0x367cd507,
    0x9159015a,  0x3070dd17,  0x152fecd8,  0xf70e5939,
    0x67332667,  0xffc00b31,  0x8eb44a87,  0x68581511,
    0xdb0c2e0d,  0x64f98fa7,  0x47b5481d,  0xbefa4fa4
};
const unsigned long  dsha_i512[16] =
{
    0x6a09e667,  0xf3bcc908,  0xbb67ae85,  0x84caa73b,
    0x3c6ef372,  0xfe94f82b,  0xa54ff53a,  0x5f1d36f1,
    0x510e527f,  0xade682d1,  0x9b05688c,  0x2b3e6c1f,
    0x1f83d9ab,  0xfb41bd6b,  0x5be0cd19,  0x137e2179
};

int DSHA::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
//hashState state;
//HashReturn HReturn;
int HReturn;

HReturn=DSHA::Init(hashbitlen); 
if (HReturn != SUCCESS) {	return HReturn;}

HReturn=DSHA::Update(data, databitlen); 
if (HReturn != SUCCESS) { 	return HReturn;}

HReturn=DSHA::Final(hashval); 
if (HReturn != SUCCESS) { return HReturn;}

return HReturn;
}

int DSHA::Init(int hashbitlen)
{
	int i;

dshaState.hashbitlen=hashbitlen ; 
dshaState.databitlen[0]=0;
dshaState.databitlen[1]=0;
for (i=0;i<129;i++) (dshaState.block)[i]= 0x0 ;

if ((hashbitlen==224) || (hashbitlen==256) || (hashbitlen==384) || (hashbitlen==512))
{
    if ((hashbitlen==224) || (hashbitlen==256))   dshaState.blocksize=512;
    if ((hashbitlen==384) || (hashbitlen==512))   dshaState.blocksize=1024;
	if (hashbitlen==224)  for(i=0;i<(dshaState.blocksize/64);i++) (dshaState.hashval)[i]=dsha_i224[i] ;
    if (hashbitlen==256)  for(i=0;i<(dshaState.blocksize/64);i++) (dshaState.hashval)[i]=dsha_i256[i] ;
    if (hashbitlen==384)  for(i=0;i<(dshaState.blocksize/64);i++) (dshaState.hashval)[i]=dsha_i384[i] ;
    if (hashbitlen==512)  for(i=0;i<(dshaState.blocksize/64);i++) (dshaState.hashval)[i]=dsha_i512[i] ;
}
else
{
	return BAD_HASHLEN;
}
return SUCCESS;
}

int DSHA::Update(const BitSequence *data, DataLength databitlen)
{
//HashReturn HReturn;
int HReturn;
BitSequence  tempc;
unsigned int oldbitlen;
int  i,sbyte, mbits;

oldbitlen=dshaState.databitlen[1] % dshaState.blocksize;
dshaState.databitlen[1]+=databitlen;
if (dshaState.databitlen[1]<databitlen) dshaState.databitlen[0]+=1;
sbyte=oldbitlen >> 3;
mbits=oldbitlen & 0x7;
(dshaState.block)[dshaState.blocksize / 8] = 0x0;

for (i=0;i<(databitlen+7)/8;i++)
{
	tempc=data[i];
	(dshaState.block)[sbyte]=(dshaState.block)[sbyte] | (tempc >> mbits);
	(dshaState.block)[sbyte+1]=tempc << (8-mbits);
	sbyte+=1;	
	if (sbyte==( dshaState.blocksize/8))
	{
		if (dshaState.blocksize==512) HReturn=DSHA::sha32_compile();
		if (dshaState.blocksize==1024) HReturn=DSHA::sha64_compile(); 
		if (HReturn != SUCCESS) { 	return HReturn;}		
		sbyte=0;
		(dshaState.block)[0]=(dshaState.block)[dshaState.blocksize / 8] ;
	}
}
return SUCCESS;
}

int DSHA::Final(BitSequence *hashval)
{
//HashReturn HReturn;
int HReturn;
BitSequence   hv[64],temp ;
DataLength dl;
int t,t1, i,maxl;

t1=((dshaState.databitlen[1]) % (dshaState.blocksize))/8;
t=7-(dshaState.databitlen[1] % 8);
temp=((dshaState.block)[t1] >> t) | 0x1;
(dshaState.block)[t1]=temp << t; 

maxl=65;
if (dshaState.blocksize==1024) maxl=129;
if ((dshaState.databitlen[1] % (dshaState.blocksize))>(dshaState.blocksize-maxl))
{             
	if (dshaState.blocksize==512) HReturn=DSHA::sha32_compile();
	if (dshaState.blocksize==1024) HReturn=DSHA::sha64_compile();	
    if (HReturn != SUCCESS) { return HReturn;}

    //  second block
    for ( i=0; i<128; i++ )  
        (dshaState.block)[i]=0x0;
}

t=dshaState.blocksize/8;
dl=dshaState.databitlen[1];
for (i=1;i<9;i++){
	(dshaState.block)[t-i]=dl & 0xff;	
	dl=(dl >> 8);
}
if (dshaState.blocksize==1024) {
	dl=dshaState.databitlen[0];
	for (i=9;i<17;i++){
		(dshaState.block)[t-i]=dl & 0xff;	
		dl=(dl >> 8);
	}
}

if (dshaState.blocksize==512) HReturn=DSHA::sha32_compile();
if (dshaState.blocksize==1024) HReturn=DSHA::sha64_compile();

for(i=0;i<16;i++) {
		hv[4*i]=((dshaState.hashval)[i] >> 24) & 0xff ;
		hv[4*i+1]=((dshaState.hashval)[i] >> 16) & 0xff ;
		hv[4*i+2]=((dshaState.hashval)[i] >> 8) & 0xff ;
		hv[4*i+3]=(dshaState.hashval)[i] & 0xff ;			
}
t=dshaState.blocksize/16;
for(i=0;i<t;i++) hashval[i]=hv[i];
return HReturn;
} 

int DSHA::sha32_compile()
{  
unsigned long hash_32[8], ch[8], temp, w[16],tt[3],ttc;
int i;

for (i=0;i<16;i++)
{
	w[i]=(dshaState.block)[4*i];
	w[i]=w[i]<<8 | ((dshaState.block)[4*i+1]);
	w[i]=w[i]<<8 | ((dshaState.block)[4*i+2]);
	w[i]=w[i]<<8 | ((dshaState.block)[4*i+3]);	
}

for (i=0;i<8;i++) {
	ch[i]=dshaState.hashval[i];
	hash_32[i]=dshaState.hashval[i];
}
tt[0]=0x5a827999;
tt[1]=0x6ed9eba1;
tt[2]=0x8f1bbcdc;

for ( i=0; i<3; i++ )  
{
	//NASTAVENIE RUND + DALSIE DOLE:
	if (dshaNumRounds >= 1) {
	ttc=tt[i];
	temp=(((((ch[0]+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[7]=DSHA_ROTR32( ch[7] ,temp );		
    ch[2]=w[0]+(ch[0] ^ ch[1] ^ ch[2])+ttc;
//0-th round
	}
	if (dshaNumRounds >= 2) {
	temp=(((((ch[7]+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[6]=DSHA_ROTR32( ch[6] ,temp );		
    ch[1]=w[1]+((ch[7] & ch[0]) ^ ch[1] )+ttc;  
//1-th round
	}
	if (dshaNumRounds >= 3) {
	temp=(((((ch[6]+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[5]=DSHA_ROTR32( ch[5] ,temp );		
    ch[0]=w[2]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])))+ttc;
//2-th round
	}
	if (dshaNumRounds >= 4) {
	temp=(((((ch[5]+ch[6])^ch[7])+ch[0])^ch[1])+ch[2])^ch[3];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[4]=DSHA_ROTR32( ch[4] ,temp );    
    ch[7]=w[3]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & (~(ch[7]))))+ttc;
//3-th round
	}
	if (dshaNumRounds >= 5) {
 	temp=(((((ch[4]+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[3]=DSHA_ROTR32( ch[3] ,temp );	
    ch[6]=w[4]+(ch[4] ^ ch[5] ^ ch[6])+ttc;
//4-th round
	}
	if (dshaNumRounds >= 6) {
	temp=(((((ch[3]+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[2]=DSHA_ROTR32( ch[2] ,temp );		
    ch[5]=w[5]+((ch[3] & ch[4]) ^ ch[5])+ttc; 
//5-th round
	}
	if (dshaNumRounds >= 7) {
	temp=(((((ch[2]+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[1]=DSHA_ROTR32( ch[1] ,temp );		
    ch[4]=w[6]+((~(ch[2] | ch[4])) | (ch[2] & (ch[3] ^ ch[4])))+ttc;
//6-th round
	}
	if (dshaNumRounds >= 8) {
	temp=(((((ch[1]+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[0]=DSHA_ROTR32( ch[0] ,temp );    
    ch[3]=w[7]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & (~(ch[3]))))+ttc;
//7-th round
	}
	if (dshaNumRounds >= 9) {
 	temp=(((((ch[0]+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[7]=DSHA_ROTR32( ch[7] ,temp );	
    ch[2]=w[8]+(ch[0] ^ ch[1] ^ ch[2])+ttc;
//8-th round
	}
	if (dshaNumRounds >= 10) {
	temp=(((((ch[7]+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[6]=DSHA_ROTR32( ch[6] ,temp );		
    ch[1]=w[9]+((ch[7] & ch[0]) ^ ch[1] )+ttc;  
//9-th round
	}
	if (dshaNumRounds >= 11) {
	temp=(((((ch[6]+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[5]=DSHA_ROTR32( ch[5] ,temp );		
    ch[0]=w[10]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])))+ttc;
//10-th round
	}
	if (dshaNumRounds >= 12) {
	temp=(((((ch[5]+ch[6])^ch[7])+ch[0])^ch[1])+ch[2])^ch[3];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[4]=DSHA_ROTR32( ch[4] ,temp );    
    ch[7]=w[11]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & (~(ch[7]))))+ttc;
//11-th round
	}
	if (dshaNumRounds >= 13) {
 	temp=(((((ch[4]+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[3]=DSHA_ROTR32( ch[3] ,temp );	
    ch[6]=w[12]+(ch[4] ^ ch[5] ^ ch[6])+ttc;
//12-th round
	}
	if (dshaNumRounds >= 14) {
	temp=(((((ch[3]+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[2]=DSHA_ROTR32( ch[2] ,temp );		
    ch[5]=w[13]+((ch[3] & ch[4]) ^ ch[5] )+ttc;  
//13-th round
	}
	if (dshaNumRounds >= 15) {
	temp=(((((ch[2]+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[1]=DSHA_ROTR32( ch[1] ,temp );		
    ch[4]=w[14]+((~(ch[2] | ch[4])) | (ch[2] & (ch[3] ^ ch[4])))+ttc;
//14-th round
	}
	if (dshaNumRounds >= 16) {
	temp=(((((ch[1]+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
    temp=((temp>>17) ^ temp) & 0x1ffff;
	temp=((temp>>10) ^ temp) & 0x3ff;
	temp=((temp>>5) ^ temp) & 0x1f;
	ch[0]=DSHA_ROTR32( ch[0] ,temp );    
    ch[3]=w[15]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & (~(ch[3]))))+ttc;
//15-th round
	}
}
// Compute intermediate hash value 
for ( i=0;i<8;i++){
    hash_32[i] +=ch[i];
	dshaState.hashval[i]=hash_32[i];
}
return SUCCESS;
}

int DSHA::sha64_compile()
{  
unsigned long long hash_64[8], ch[8], temp, w[16],tt[3],ttc;
int i;

for (i=0;i<16;i++)
{
	w[i]=(dshaState.block)[8*i];
	w[i]=w[i]<<8 | (dshaState.block)[8*i+1];
	w[i]=w[i]<<8 | (dshaState.block)[8*i+2];
	w[i]=w[i]<<8 | (dshaState.block)[8*i+3];
	w[i]=w[i]<<8 | (dshaState.block)[8*i+4];
	w[i]=w[i]<<8 | (dshaState.block)[8*i+5];
	w[i]=w[i]<<8 | (dshaState.block)[8*i+6];
	w[i]=w[i]<<8 | (dshaState.block)[8*i+7];
}

//Initialize eight working variables 
for (i=0;i<8;i++)
{	
	temp=(dshaState.hashval)[i*2];
	ch[i]=(temp<< 32)+(dshaState.hashval)[i*2+1];
	hash_64[i]=ch[i];
}

tt[0]=0x5a82799950a28be6;
tt[1]=0x6ed9eba15c4dd124;
tt[2]=0x8f1bbcdc6d703ef3;

for ( i=0; i<3; i++ )   {
	//NASTAVENIE RUND:
	if (dshaNumRounds >= 1) {
	ttc=tt[i];
    temp=(((((ch[0]+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[7]=DSHA_ROTR64( ch[7] ,temp );	
    ch[2]=w[0]+(ch[0] ^ ch[1] ^ ch[2])+ttc;
//0-th round
	}
	if (dshaNumRounds >= 2) {
	temp=(((((ch[7]+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[6]=DSHA_ROTR64( ch[6] ,temp );		
    ch[1]=w[1]+((ch[7] & ch[0]) ^ ch[1] )+ttc; 	

//1-th round
	}
	if (dshaNumRounds >= 3) {
	temp=(((((ch[6]+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[5]=DSHA_ROTR64( ch[5] ,temp );		
    ch[0]=w[2]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])))+ttc;

//2-th round
	}
	if (dshaNumRounds >= 4) {
	temp=(((((ch[5]+ch[6])^ch[7])+ch[0])^ch[1])+ch[2])^ch[3];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[4]=DSHA_ROTR64( ch[4] ,temp );    
    ch[7]=w[3]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & (~(ch[7]))))+ttc;

//3-th round
	}
	if (dshaNumRounds >= 5) {
 	temp=(((((ch[4]+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[3]=DSHA_ROTR64( ch[3] ,temp );	
    ch[6]=w[4]+(ch[4] ^ ch[5] ^ ch[6])+ttc;

//4-th round
	}
	if (dshaNumRounds >= 6) {
	temp=(((((ch[3]+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[2]=DSHA_ROTR64( ch[2] ,temp );		
    ch[5]=w[5]+((ch[3] & ch[4]) ^ ch[5])+ttc;  

//5-th round
	}
	if (dshaNumRounds >= 7) {
	temp=(((((ch[2]+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[1]=DSHA_ROTR64( ch[1] ,temp );		
    ch[4]=w[6]+((~(ch[2] | ch[4])) | (ch[2] & (ch[3] ^ ch[4])))+ttc;

//6-th round
	}
	if (dshaNumRounds >= 8) {
	temp=(((((ch[1]+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[0]=DSHA_ROTR64( ch[0] ,temp );    
    ch[3]=w[7]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & (~(ch[3]))))+ttc;

//7-th round
	}
	if (dshaNumRounds >= 9) {
 	temp=(((((ch[0]+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[7]=DSHA_ROTR64( ch[7] ,temp );	
    ch[2]=w[8]+(ch[0] ^ ch[1] ^ ch[2])+ttc;

//8-th round
	}
	if (dshaNumRounds >= 10) {
	temp=(((((ch[7]+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[6]=DSHA_ROTR64( ch[6] ,temp );		
    ch[1]=w[9]+((ch[7] & ch[0]) ^ ch[1] )+ttc; 

//9-th round
	}
	if (dshaNumRounds >= 11) {
	temp=(((((ch[6]+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[5]=DSHA_ROTR64( ch[5] ,temp );		
    ch[0]=w[10]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])))+ttc;

//10-th round
	}
	if (dshaNumRounds >= 12) {
	temp=(((((ch[5]+ch[6])^ch[7])+ch[0])^ch[1])+ch[2])^ch[3];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[4]=DSHA_ROTR64( ch[4] ,temp );    
    ch[7]=w[11]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & (~(ch[7]))))+ttc;

//11-th round
	}
	if (dshaNumRounds >= 13) {
 	temp=(((((ch[4]+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[3]=DSHA_ROTR64( ch[3] ,temp );	
    ch[6]=w[12]+(ch[4] ^ ch[5] ^ ch[6])+ttc;

//12-th round
	}
	if (dshaNumRounds >= 14) {
	temp=(((((ch[3]+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[2]=DSHA_ROTR64( ch[2] ,temp );		
    ch[5]=w[13]+((ch[3] & ch[4]) ^ ch[5] )+ttc;  

//13-th round
	}
	if (dshaNumRounds >= 15) {
	temp=(((((ch[2]+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[1]=DSHA_ROTR64( ch[1] ,temp );		
    ch[4]=w[14]+((~(ch[2] | ch[4])) | (ch[2] & (ch[3] ^ ch[4])))+ttc;

//14-th round
	}
	if (dshaNumRounds >= 16) {
	temp=(((((ch[1]+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
    temp=((temp>>36) ^ temp) & 0xfffffffff;
	temp=((temp>>18) ^ temp) & 0x3ffff;
	temp=((temp>>12) ^ temp) & 0xfff;
	temp=((temp>>6) ^ temp) & 0x3f;
	ch[0]=DSHA_ROTR64( ch[0] ,temp );    
    ch[3]=w[15]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & (~(ch[3]))))+ttc;

//15-th round
	}
}
// Compute intermediate hash value 
for (i=0;i<8;i++){
    hash_64[i] +=ch[i];	
	(dshaState.hashval)[2*i]=hash_64[i] >> 32 ;
	(dshaState.hashval)[2*i+1]=hash_64[i] & 0xffffffff;
}
return SUCCESS;
}

DSHA::DSHA(const int numRounds) {
	if (numRounds == -1) {
		dshaNumRounds = DSHA_DEFAULT_ROUNDS;
	} else {
		dshaNumRounds = numRounds;
	}
}