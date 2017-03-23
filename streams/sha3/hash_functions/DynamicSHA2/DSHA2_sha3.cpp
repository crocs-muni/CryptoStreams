#include "DSHA2_sha3.h"

const unsigned long dsha2_i224[8] =
{
    0xc1059ed8ul, 0x367cd507ul, 0x3070dd17ul, 0xf70e5939ul,
    0xffc00b31ul, 0x68581511ul, 0x64f98fa7ul, 0xbefa4fa4ul
};
const unsigned long dsha2_i256[8] =
{
    0x6a09e667ul, 0xbb67ae85ul, 0x3c6ef372ul, 0xa54ff53aul,
    0x510e527ful, 0x9b05688cul, 0x1f83d9abul, 0x5be0cd19ul
};
const unsigned long  dsha2_i384[16] =
{
    0xcbbb9d5d,  0xc1059ed8,  0x629a292a,  0x367cd507,
    0x9159015a,  0x3070dd17,  0x152fecd8,  0xf70e5939,
    0x67332667,  0xffc00b31,  0x8eb44a87,  0x68581511,
    0xdb0c2e0d,  0x64f98fa7,  0x47b5481d,  0xbefa4fa4
};
const unsigned long  dsha2_i512[16] =
{
    0x6a09e667,  0xf3bcc908,  0xbb67ae85,  0x84caa73b,
    0x3c6ef372,  0xfe94f82b,  0xa54ff53a,  0x5f1d36f1,
    0x510e527f,  0xade682d1,  0x9b05688c,  0x2b3e6c1f,
    0x1f83d9ab,  0xfb41bd6b,  0x5be0cd19,  0x137e2179
};

int DSHA2::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval)
{
//hashState state;
//HashReturn HReturn;
int HReturn;

HReturn=DSHA2::Init(hashbitlen); 
if (HReturn != SUCCESS) {	return HReturn;}

HReturn=DSHA2::Update(data, databitlen); 
if (HReturn != SUCCESS) { 	return HReturn;}

HReturn=DSHA2::Final(hashval); 
if (HReturn != SUCCESS) { return HReturn;}

return HReturn;
}

int DSHA2::Init(int hashbitlen)
{
	int i;

dsha2State.hashbitlen=hashbitlen ; 
dsha2State.databitlen[0]=0;
dsha2State.databitlen[1]=0;
for (i=0;i<129;i++) (dsha2State.block)[i]= 0x0 ;

if ((hashbitlen==224) || (hashbitlen==256) || (hashbitlen==384) || (hashbitlen==512))
{
    if ((hashbitlen==224) || (hashbitlen==256))   dsha2State.blocksize=512;
    if ((hashbitlen==384) || (hashbitlen==512))   dsha2State.blocksize=1024;
	if (hashbitlen==224)  for(i=0;i<(dsha2State.blocksize/64);i++) (dsha2State.hashval)[i]=dsha2_i224[i] ;
    if (hashbitlen==256)  for(i=0;i<(dsha2State.blocksize/64);i++) (dsha2State.hashval)[i]=dsha2_i256[i] ;
    if (hashbitlen==384)  for(i=0;i<(dsha2State.blocksize/64);i++) (dsha2State.hashval)[i]=dsha2_i384[i] ;
    if (hashbitlen==512)  for(i=0;i<(dsha2State.blocksize/64);i++) (dsha2State.hashval)[i]=dsha2_i512[i] ;
}
else
{
	return BAD_HASHLEN;
}
return SUCCESS;
}

int DSHA2::Update(const BitSequence *data, DataLength databitlen)
{
//HashReturn HReturn;
int HReturn;
BitSequence  tempc;
unsigned int oldbitlen;
int  i,sbyte, mbits;

oldbitlen=dsha2State.databitlen[1] % dsha2State.blocksize;
dsha2State.databitlen[1]+=databitlen;
if (dsha2State.databitlen[1]<databitlen) dsha2State.databitlen[0]+=1;
sbyte=oldbitlen >> 3;
mbits=oldbitlen & 0x7;
(dsha2State.block)[dsha2State.blocksize / 8] = 0x0;

for (i=0;i<(databitlen+7)/8;i++)
{
	tempc=data[i];
	(dsha2State.block)[sbyte]=(dsha2State.block)[sbyte] | (tempc >> mbits);
	(dsha2State.block)[sbyte+1]=tempc << (8-mbits);
	sbyte+=1;	
	if (sbyte==( dsha2State.blocksize/8))
	{
		if (dsha2State.blocksize==512) HReturn=DSHA2::sha32_compile();
		if (dsha2State.blocksize==1024) HReturn=DSHA2::sha64_compile(); 
		if (HReturn != SUCCESS) { 	return HReturn;}		
		sbyte=0;
		(dsha2State.block)[0]=(dsha2State.block)[dsha2State.blocksize / 8] ;
	}
}
return SUCCESS;
}

int DSHA2::Final(BitSequence *hashval)
{
//HashReturn HReturn;
int HReturn;
BitSequence   hv[64],temp ;
DataLength dl;
int t,t1, i,maxl;

t1=((dsha2State.databitlen[1]) % (dsha2State.blocksize))/8;
t=7-(dsha2State.databitlen[1] % 8);
temp=((dsha2State.block)[t1] >> t) | 0x1;
(dsha2State.block)[t1]=temp << t; 

maxl=65;
if (dsha2State.blocksize==1024) maxl=129;
if ((dsha2State.databitlen[1] % (dsha2State.blocksize))>(dsha2State.blocksize-maxl))
{             
	if (dsha2State.blocksize==512) HReturn=DSHA2::sha32_compile();
	if (dsha2State.blocksize==1024) HReturn=DSHA2::sha64_compile();	
    if (HReturn != SUCCESS) { return HReturn;}

    //  second block
    for ( i=0; i<128; i++ )  
        (dsha2State.block)[i]=0x0;
}

t=dsha2State.blocksize/8;
dl=dsha2State.databitlen[1];
for (i=1;i<9;i++){
	(dsha2State.block)[t-i]=dl & 0xff;	
	dl=(dl >> 8);
}
if (dsha2State.blocksize==1024) {
	dl=dsha2State.databitlen[0];
	for (i=9;i<17;i++){
		(dsha2State.block)[t-i]=dl & 0xff;	
		dl=(dl >> 8);
	}
}

if (dsha2State.blocksize==512) HReturn=DSHA2::sha32_compile();
if (dsha2State.blocksize==1024) HReturn=DSHA2::sha64_compile();

for(i=0;i<16;i++) {
		hv[4*i]=((dsha2State.hashval)[i] >> 24) & 0xff ;
		hv[4*i+1]=((dsha2State.hashval)[i] >> 16) & 0xff ;
		hv[4*i+2]=((dsha2State.hashval)[i] >> 8) & 0xff ;
		hv[4*i+3]=(dsha2State.hashval)[i] & 0xff ;			
}
t=dsha2State.blocksize/16;
for(i=0;i<t;i++) hashval[i]=hv[i];
return HReturn;
} 

int DSHA2::sha32_compile()
{  
unsigned long hash_32[8], ch[8], temp, w[16];
int i;

for (i=0;i<16;i++)
{
	w[i]=(dsha2State.block)[4*i];
	w[i]=w[i]<<8 | ((dsha2State.block)[4*i+1]);
	w[i]=w[i]<<8 | ((dsha2State.block)[4*i+2]);
	w[i]=w[i]<<8 | ((dsha2State.block)[4*i+3]);	
}

for (i=0;i<8;i++) {
	ch[i]=dsha2State.hashval[i];
	hash_32[i]=dsha2State.hashval[i];
}

temp=((((((ch[0]^ch[1])+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
ch[7]=DSHA2_ROTR32(temp,(w[0] & 31))+w[1];
ch[5]=DSHA2_ROTR32(ch[5],((w[0]>>5) & 31));		
ch[4]=ch[4]+w[3];		
ch[3]=DSHA2_ROTR32(ch[3],((w[0]>>10) & 31));
switch (w[0]>>30) 
{ case 0: ch[2]=w[2]+(ch[0] ^ ch[1] ^ ch[2]);
		break;
case 1: ch[2]=w[2]+((ch[0] & ch[1]) ^ ch[2] );
		break;
case 2: ch[2]=w[2]+((~(ch[0] | ch[2])) | (ch[0] & (ch[1] ^ ch[2])));
		break;
case 3: ch[2]=w[2]+((~(ch[0] | (ch[1] ^ ch[2]))) | (ch[0] & ~(ch[2])));
		break;
}				


temp=((((((ch[7]^ch[0])+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
ch[6]=DSHA2_ROTR32(temp,((w[0]>>15) & 31))+w[4];
ch[5]=ch[5]+w[7];
ch[4]=DSHA2_ROTR32(ch[4],((w[0]>>20) & 31));	
ch[3]=ch[3]+w[6];		
ch[2]=DSHA2_ROTR32(ch[2],((w[0]>>25) & 31));	
ch[1]=w[5]+(ch[7] ^ ch[0] ^ ch[1]);
ch[0]=ch[0] + w[0];


temp=((((((ch[6]^ch[7])+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
ch[5]=DSHA2_ROTR32(temp,(w[8] & 31))+w[9];
ch[3]=DSHA2_ROTR32(ch[3],((w[8]>>5) & 31));		
ch[2]=ch[2]+w[11];		
ch[1]=DSHA2_ROTR32(ch[1],((w[8]>>10) & 31));
switch (w[8]>>30) 
{ case 0: ch[0]=w[10]+(ch[6] ^ ch[7] ^ ch[0]);
		break;
case 1: ch[0]=w[10]+((ch[6] & ch[7]) ^ ch[0] );
		break;
case 2: ch[0]=w[10]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])));
		break;
case 3: ch[0]=w[10]+((~(ch[6] | (ch[7] ^ ch[0]))) | (ch[6] & ~(ch[0])));
		break;
}				

temp=((((((ch[5]^ch[6])+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
ch[4]=DSHA2_ROTR32(temp,((w[8]>>15) & 31))+w[12];
ch[3]=ch[3]+w[15];
ch[2]=DSHA2_ROTR32(ch[2],((w[8]>>20) & 31));	
ch[1]=ch[1]+w[14];		
ch[0]=DSHA2_ROTR32(ch[0],((w[8]>>25) & 31));	
ch[7]=w[13]+(ch[5] ^ ch[6] ^ ch[7]);
ch[6]=ch[6] + w[8];	


if (dsha2NumRounds >= 2) {
temp=(((((ch[ 4]+ch[ 5])^ch[ 6])+ch[ 7])^ch[ 0])+ch[ 1])^ch[ 2];
temp=((temp>>17) ^ temp) & 0x1ffff;
temp=((temp>>10) ^ temp) & 0x3ff;
temp=((temp>>5) ^ temp) & 0x1f;
ch[ 3]=DSHA2_ROTR32(ch[ 3],temp);
}

if (dsha2NumRounds >= 3) {
temp=(((((ch[ 3]+ch[ 4])^ch[ 5])+ch[ 6])^ch[ 7])+ch[ 0])^ch[ 1];
temp=((temp>>17) ^ temp) & 0x1ffff;
temp=((temp>>10) ^ temp) & 0x3ff;
temp=((temp>>5) ^ temp) & 0x1f;
ch[ 2]=DSHA2_ROTR32(ch[ 2],temp);
}

if (dsha2NumRounds >= 4) {
temp=(((((ch[ 2]+ch[ 3])^ch[ 4])+ch[ 5])^ch[ 6])+ch[ 7])^ch[ 0];
temp=((temp>>17) ^ temp) & 0x1ffff;
temp=((temp>>10) ^ temp) & 0x3ff;
temp=((temp>>5) ^ temp) & 0x1f;
ch[ 1]=DSHA2_ROTR32(ch[ 1],temp);
}

if (dsha2NumRounds >= 5) {
temp=(((((ch[ 1]+ch[ 2])^ch[ 3])+ch[ 4])^ch[ 5])+ch[ 6])^ch[ 7];
temp=((temp>>17) ^ temp) & 0x1ffff;
temp=((temp>>10) ^ temp) & 0x3ff;
temp=((temp>>5) ^ temp) & 0x1f;
ch[ 0]=DSHA2_ROTR32(ch[ 0],temp);
}

if (dsha2NumRounds >= 6) {
temp=(((((ch[ 0]+ch[ 1])^ch[ 2])+ch[ 3])^ch[ 4])+ch[ 5])^ch[ 6];
temp=((temp>>17) ^ temp) & 0x1ffff;
temp=((temp>>10) ^ temp) & 0x3ff;
temp=((temp>>5) ^ temp) & 0x1f;
ch[ 7]=DSHA2_ROTR32(ch[ 7],temp);
}

if (dsha2NumRounds >= 7) {
temp=(((((ch[ 7]+ch[ 0])^ch[ 1])+ch[ 2])^ch[ 3])+ch[ 4])^ch[ 5];
temp=((temp>>17) ^ temp) & 0x1ffff;
temp=((temp>>10) ^ temp) & 0x3ff;
temp=((temp>>5) ^ temp) & 0x1f;
ch[ 6]=DSHA2_ROTR32(ch[ 6],temp);
}

if (dsha2NumRounds >= 8) {
temp=(((((ch[ 6]+ch[ 7])^ch[ 0])+ch[ 1])^ch[ 2])+ch[ 3])^ch[ 4];
temp=((temp>>17) ^ temp) & 0x1ffff;
temp=((temp>>10) ^ temp) & 0x3ff;
temp=((temp>>5) ^ temp) & 0x1f;
ch[ 5]=DSHA2_ROTR32(ch[ 5],temp);
}

if (dsha2NumRounds >= 9) {
temp=(((((ch[ 5]+ch[ 6])^ch[ 7])+ch[ 0])^ch[ 1])+ch[ 2])^ch[ 3];
temp=((temp>>17) ^ temp) & 0x1ffff;
temp=((temp>>10) ^ temp) & 0x3ff;
temp=((temp>>5) ^ temp) & 0x1f;
ch[ 4]=DSHA2_ROTR32(ch[ 4],temp);
}

if (dsha2NumRounds >= 10) {
temp=(((((ch[ 4]+ch[ 5])^ch[ 6])+ch[ 7])^ch[ 0])+ch[ 1])^ch[ 2];
temp=((temp>>17) ^ temp) & 0x1ffff;
temp=((temp>>10) ^ temp) & 0x3ff;
temp=((temp>>5) ^ temp) & 0x1f;
ch[ 3]=DSHA2_ROTR32(ch[ 3],temp);
}


if (dsha2NumRounds >= 11) {
temp=((((((ch[3]^ch[4])+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
ch[2]=DSHA2_ROTR32(temp,(w[1] & 31))+w[2];
ch[0]=DSHA2_ROTR32(ch[0],((w[1]>>5) & 31));		
ch[7]=ch[7]+w[4];		
ch[6]=DSHA2_ROTR32(ch[6],((w[1]>>10) & 31));
switch (w[1]>>30) 
{ case 0: ch[5]=w[3]+(ch[3] ^ ch[4] ^ ch[5]);
		break;
case 1: ch[5]=w[3]+((ch[3] & ch[4]) ^ ch[5] );
		break;
case 2: ch[5]=w[3]+((~(ch[3] | ch[5])) | (ch[3] & (ch[4] ^ ch[5])));
		break;
case 3: ch[5]=w[3]+((~(ch[3] | (ch[4] ^ ch[5]))) | (ch[3] & ~(ch[5])));
		break;
}				

temp=((((((ch[2]^ch[3])+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
ch[1]=DSHA2_ROTR32(temp,((w[1]>>15) & 31))+w[5];
ch[0]=ch[0]+w[0];
ch[7]=DSHA2_ROTR32(ch[7],((w[1]>>20) & 31));	
ch[6]=ch[6]+w[7];		
ch[5]=DSHA2_ROTR32(ch[5],((w[1]>>25) & 31));	
ch[4]=w[6]+((ch[2] & ch[3]) ^ ch[4] );
ch[3]=ch[3] + w[1];	


temp=((((((ch[1]^ch[2])+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
ch[0]=DSHA2_ROTR32(temp,(w[9] & 31))+w[10];
ch[6]=DSHA2_ROTR32(ch[6],((w[9]>>5) & 31));		
ch[5]=ch[5]+w[12];		
ch[4]=DSHA2_ROTR32(ch[4],((w[9]>>10) & 31));
switch (w[9]>>30) 
{ case 0: ch[3]=w[11]+(ch[1] ^ ch[2] ^ ch[3]);
		break;
case 1: ch[3]=w[11]+((ch[1] & ch[2]) ^ ch[3] );
		break;
case 2: ch[3]=w[11]+((~(ch[1] | ch[3])) | (ch[1] & (ch[2] ^ ch[3])));
		break;
case 3: ch[3]=w[11]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & ~(ch[3])));
		break;
}				

temp=((((((ch[0]^ch[1])+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
ch[7]=DSHA2_ROTR32(temp,((w[9]>>15) & 31))+w[13];
ch[6]=ch[6]+w[8];
ch[5]=DSHA2_ROTR32(ch[5],((w[9]>>20) & 31));	
ch[4]=ch[4]+w[15];		
ch[3]=DSHA2_ROTR32(ch[3],((w[9]>>25) & 31));	
ch[2]=w[14]+((ch[0] & ch[1]) ^ ch[2] );
ch[1]=ch[1] + w[9];	
}


if (dsha2NumRounds >= 12) {
temp=((((((ch[7]^ch[0])+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
ch[6]=DSHA2_ROTR32(temp,(w[2] & 31))+w[3];
ch[4]=DSHA2_ROTR32(ch[4],((w[2]>>5) & 31));		
ch[3]=ch[3]+w[5];		
ch[2]=DSHA2_ROTR32(ch[2],((w[2]>>10) & 31));
switch (w[2]>>30) 
{ case 0: ch[1]=w[4]+(ch[7] ^ ch[0] ^ ch[1]);
		break;
case 1: ch[1]=w[4]+((ch[7] & ch[0]) ^ ch[1] );
		break;
case 2: ch[1]=w[4]+((~(ch[7] | ch[1])) | (ch[7] & (ch[0] ^ ch[1])));
		break;
case 3: ch[1]=w[4]+((~(ch[7] | (ch[0] ^ ch[1]))) | (ch[7] & ~(ch[1])));
		break;
}				

temp=((((((ch[6]^ch[7])+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
ch[5]=DSHA2_ROTR32(temp,((w[2]>>15) & 31))+w[6];
ch[4]=ch[4]+w[1];
ch[3]=DSHA2_ROTR32(ch[3],((w[2]>>20) & 31));	
ch[2]=ch[2]+w[0];		
ch[1]=DSHA2_ROTR32(ch[1],((w[2]>>25) & 31));	
ch[0]=w[7]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])));
ch[7]=ch[7] + w[2];	


temp=((((((ch[5]^ch[6])+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
ch[4]=DSHA2_ROTR32(temp,(w[10] & 31))+w[11];
ch[2]=DSHA2_ROTR32(ch[2],((w[10]>>5) & 31));		
ch[1]=ch[1]+w[13];		
ch[0]=DSHA2_ROTR32(ch[0],((w[10]>>10) & 31));
switch (w[10]>>30) 
{ case 0: ch[7]=w[12]+(ch[5] ^ ch[6] ^ ch[7]);
		break;
case 1: ch[7]=w[12]+((ch[5] & ch[6]) ^ ch[7] );
		break;
case 2: ch[7]=w[12]+((~(ch[5] | ch[7])) | (ch[5] & (ch[6] ^ ch[7])));
		break;
case 3: ch[7]=w[12]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & ~(ch[7])));
		break;
}				

temp=((((((ch[4]^ch[5])+ch[6])^ch[7])+ch[0])^ch[1])+ch[2])^ch[3];
ch[3]=DSHA2_ROTR32(temp,((w[10]>>15) & 31))+w[14];
ch[2]=ch[2]+w[9];
ch[1]=DSHA2_ROTR32(ch[1],((w[10]>>20) & 31));	
ch[0]=ch[0]+w[8];		
ch[7]=DSHA2_ROTR32(ch[7],((w[10]>>25) & 31));	
ch[6]=w[15]+((~(ch[4] | ch[6])) | (ch[4] & (ch[5] ^ ch[6])));
ch[5]=ch[5] + w[10];
}


if (dsha2NumRounds >= 13) {
temp=((((((ch[3]^ch[4])+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
ch[2]=DSHA2_ROTR32(temp,(w[3] & 31))+w[4];
ch[0]=DSHA2_ROTR32(ch[0],((w[3]>>5) & 31));		
ch[7]=ch[7]+w[6];		
ch[6]=DSHA2_ROTR32(ch[6],((w[3]>>10) & 31));
switch (w[3]>>30) 
{ case 0: ch[5]=w[5]+(ch[3] ^ ch[4] ^ ch[5]);
		break;
case 1: ch[5]=w[5]+((ch[3] & ch[4]) ^ ch[5] );
		break;
case 2: ch[5]=w[5]+((~(ch[3] | ch[5])) | (ch[3] & (ch[4] ^ ch[5])));
		break;
case 3: ch[5]=w[5]+((~(ch[3] | (ch[4] ^ ch[5]))) | (ch[3] & ~(ch[5])));
		break;
}				

temp=((((((ch[2]^ch[3])+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
ch[1]=DSHA2_ROTR32(temp,((w[3]>>15) & 31))+w[7];
ch[0]=ch[0]+w[2];
ch[7]=DSHA2_ROTR32(ch[7],((w[3]>>20) & 31));	
ch[6]=ch[6]+w[1];		
ch[5]=DSHA2_ROTR32(ch[5],((w[3]>>25) & 31));	
ch[4]=w[0]+((~(ch[2] | (ch[3] ^ ch[4]))) | (ch[2] & ~(ch[4])));
ch[3]=ch[3] + w[3];	


temp=((((((ch[1]^ch[2])+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
ch[0]=DSHA2_ROTR32(temp,(w[11] & 31))+w[12];
ch[6]=DSHA2_ROTR32(ch[6],((w[11]>>5) & 31));		
ch[5]=ch[5]+w[14];		
ch[4]=DSHA2_ROTR32(ch[4],((w[11]>>10) & 31));
switch (w[11]>>30) 
{ case 0: ch[3]=w[13]+(ch[1] ^ ch[2] ^ ch[3]);
		break;
case 1: ch[3]=w[13]+((ch[1] & ch[2]) ^ ch[3] );
		break;
case 2: ch[3]=w[13]+((~(ch[1] | ch[3])) | (ch[1] & (ch[2] ^ ch[3])));
		break;
case 3: ch[3]=w[13]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & ~(ch[3])));
		break;
}				

temp=((((((ch[0]^ch[1])+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
ch[7]=DSHA2_ROTR32(temp,((w[11]>>15) & 31))+w[15];
ch[6]=ch[6]+w[10];
ch[5]=DSHA2_ROTR32(ch[5],((w[11]>>20) & 31));	
ch[4]=ch[4]+w[9];		
ch[3]=DSHA2_ROTR32(ch[3],((w[11]>>25) & 31));	
ch[2]=w[8]+((~(ch[0] | (ch[1] ^ ch[2]))) | (ch[0] & ~(ch[2])));
ch[1]=ch[1] + w[11];	
}


if (dsha2NumRounds >= 14) {
temp=((((((ch[7]^ch[0])+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
ch[6]=DSHA2_ROTR32(temp,(w[4] & 31))+w[5];
ch[4]=DSHA2_ROTR32(ch[4],((w[4]>>5) & 31));		
ch[3]=ch[3]+w[7];		
ch[2]=DSHA2_ROTR32(ch[2],((w[4]>>10) & 31));
switch (w[4]>>30) 
{ case 0: ch[1]=w[6]+(ch[7] ^ ch[0] ^ ch[1]);
		break;
case 1: ch[1]=w[6]+((ch[7] & ch[0]) ^ ch[1] );
		break;
case 2: ch[1]=w[6]+((~(ch[7] | ch[1])) | (ch[7] & (ch[0] ^ ch[1])));
		break;
case 3: ch[1]=w[6]+((~(ch[7] | (ch[0] ^ ch[1]))) | (ch[7] & ~(ch[1])));
		break;
}				

temp=((((((ch[6]^ch[7])+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
ch[5]=DSHA2_ROTR32(temp,((w[4]>>15) & 31))+w[0];
ch[4]=ch[4]+w[3];
ch[3]=DSHA2_ROTR32(ch[3],((w[4]>>20) & 31));	
ch[2]=ch[2]+w[2];		
ch[1]=DSHA2_ROTR32(ch[1],((w[4]>>25) & 31));	
ch[0]=w[1]+(ch[6] ^ ch[7] ^ ch[0]);
ch[7]=ch[7] + w[4];	


temp=((((((ch[5]^ch[6])+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
ch[4]=DSHA2_ROTR32(temp,(w[12] & 31))+w[13];
ch[2]=DSHA2_ROTR32(ch[2],((w[12]>>5) & 31));		
ch[1]=ch[1]+w[15];		
ch[0]=DSHA2_ROTR32(ch[0],((w[12]>>10) & 31));
switch (w[12]>>30) 
{ case 0: ch[7]=w[14]+(ch[5] ^ ch[6] ^ ch[7]);
		break;
case 1: ch[7]=w[14]+((ch[5] & ch[6]) ^ ch[7] );
		break;
case 2: ch[7]=w[14]+((~(ch[5] | ch[7])) | (ch[5] & (ch[6] ^ ch[7])));
		break;
case 3: ch[7]=w[14]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & ~(ch[7])));
		break;
}				

temp=((((((ch[4]^ch[5])+ch[6])^ch[7])+ch[0])^ch[1])+ch[2])^ch[3];
ch[3]=DSHA2_ROTR32(temp,((w[12]>>15) & 31))+w[8];
ch[2]=ch[2]+w[11];
ch[1]=DSHA2_ROTR32(ch[1],((w[12]>>20) & 31));	
ch[0]=ch[0]+w[10];		
ch[7]=DSHA2_ROTR32(ch[7],((w[12]>>25) & 31));	
ch[6]=w[9]+(ch[4] ^ ch[5] ^ ch[6]);
ch[5]=ch[5] + w[12];
}


if (dsha2NumRounds >= 15) {
temp=((((((ch[3]^ch[4])+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
ch[2]=DSHA2_ROTR32(temp,(w[5] & 31))+w[6];
ch[0]=DSHA2_ROTR32(ch[0],((w[5]>>5) & 31));		
ch[7]=ch[7]+w[0];		
ch[6]=DSHA2_ROTR32(ch[6],((w[5]>>10) & 31));
switch (w[5]>>30) 
{ case 0: ch[5]=w[7]+(ch[3] ^ ch[4] ^ ch[5]);
		break;
case 1: ch[5]=w[7]+((ch[3] & ch[4]) ^ ch[5] );
		break;
case 2: ch[5]=w[7]+((~(ch[3] | ch[5])) | (ch[3] & (ch[4] ^ ch[5])));
		break;
case 3: ch[5]=w[7]+((~(ch[3] | (ch[4] ^ ch[5]))) | (ch[3] & ~(ch[5])));
		break;
}				

temp=((((((ch[2]^ch[3])+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
ch[1]=DSHA2_ROTR32(temp,((w[5]>>15) & 31))+w[1];
ch[0]=ch[0]+w[4];
ch[7]=DSHA2_ROTR32(ch[7],((w[5]>>20) & 31));	
ch[6]=ch[6]+w[3];		
ch[5]=DSHA2_ROTR32(ch[5],((w[5]>>25) & 31));	
ch[4]=w[2]+((ch[2] & ch[3]) ^ ch[4] );
ch[3]=ch[3] + w[5];	


temp=((((((ch[1]^ch[2])+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
ch[0]=DSHA2_ROTR32(temp,(w[13] & 31))+w[14];
ch[6]=DSHA2_ROTR32(ch[6],((w[13]>>5) & 31));		
ch[5]=ch[5]+w[8];		
ch[4]=DSHA2_ROTR32(ch[4],((w[13]>>10) & 31));
switch (w[13]>>30) 
{ case 0: ch[3]=w[15]+(ch[1] ^ ch[2] ^ ch[3]);
		break;
case 1: ch[3]=w[15]+((ch[1] & ch[2]) ^ ch[3] );
		break;
case 2: ch[3]=w[15]+((~(ch[1] | ch[3])) | (ch[1] & (ch[2] ^ ch[3])));
		break;
case 3: ch[3]=w[15]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & ~(ch[3])));
		break;
}				

temp=((((((ch[0]^ch[1])+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
ch[7]=DSHA2_ROTR32(temp,((w[13]>>15) & 31))+w[9];
ch[6]=ch[6]+w[12];
ch[5]=DSHA2_ROTR32(ch[5],((w[13]>>20) & 31));	
ch[4]=ch[4]+w[11];		
ch[3]=DSHA2_ROTR32(ch[3],((w[13]>>25) & 31));	
ch[2]=w[10]+((ch[0] & ch[1]) ^ ch[2] );
ch[1]=ch[1] + w[13];
}


if (dsha2NumRounds >= 16) {
temp=((((((ch[7]^ch[0])+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
ch[6]=DSHA2_ROTR32(temp,(w[6] & 31))+w[7];
ch[4]=DSHA2_ROTR32(ch[4],((w[6]>>5) & 31));		
ch[3]=ch[3]+w[1];		
ch[2]=DSHA2_ROTR32(ch[2],((w[6]>>10) & 31));
switch (w[6]>>30) 
{ case 0: ch[1]=w[0]+(ch[7] ^ ch[0] ^ ch[1]);
		break;
case 1: ch[1]=w[0]+((ch[7] & ch[0]) ^ ch[1] );
		break;
case 2: ch[1]=w[0]+((~(ch[7] | ch[1])) | (ch[7] & (ch[0] ^ ch[1])));
		break;
case 3: ch[1]=w[0]+((~(ch[7] | (ch[0] ^ ch[1]))) | (ch[7] & ~(ch[1])));
		break;
}				

temp=((((((ch[6]^ch[7])+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
ch[5]=DSHA2_ROTR32(temp,((w[6]>>15) & 31))+w[2];
ch[4]=ch[4]+w[5];
ch[3]=DSHA2_ROTR32(ch[3],((w[6]>>20) & 31));	
ch[2]=ch[2]+w[4];		
ch[1]=DSHA2_ROTR32(ch[1],((w[6]>>25) & 31));	
ch[0]=w[3]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])));
ch[7]=ch[7] + w[6];	


temp=((((((ch[5]^ch[6])+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
ch[4]=DSHA2_ROTR32(temp,(w[14] & 31))+w[15];
ch[2]=DSHA2_ROTR32(ch[2],((w[14]>>5) & 31));		
ch[1]=ch[1]+w[9];		
ch[0]=DSHA2_ROTR32(ch[0],((w[14]>>10) & 31));
switch (w[14]>>30) 
{ case 0: ch[7]=w[8]+(ch[5] ^ ch[6] ^ ch[7]);
		break;
case 1: ch[7]=w[8]+((ch[5] & ch[6]) ^ ch[7] );
		break;
case 2: ch[7]=w[8]+((~(ch[5] | ch[7])) | (ch[5] & (ch[6] ^ ch[7])));
		break;
case 3: ch[7]=w[8]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & ~(ch[7])));
		break;
}				

temp=((((((ch[4]^ch[5])+ch[6])^ch[7])+ch[0])^ch[1])+ch[2])^ch[3];
ch[3]=DSHA2_ROTR32(temp,((w[14]>>15) & 31))+w[10];
ch[2]=ch[2]+w[13];
ch[1]=DSHA2_ROTR32(ch[1],((w[14]>>20) & 31));	
ch[0]=ch[0]+w[12];		
ch[7]=DSHA2_ROTR32(ch[7],((w[14]>>25) & 31));	
ch[6]=w[11]+((~(ch[4] | ch[6])) | (ch[4] & (ch[5] ^ ch[6])));
ch[5]=ch[5] + w[14];
}


if (dsha2NumRounds >= 17) {
temp=((((((ch[3]^ch[4])+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
ch[2]=DSHA2_ROTR32(temp,(w[7] & 31))+w[0];
ch[0]=DSHA2_ROTR32(ch[0],((w[7]>>5) & 31));		
ch[7]=ch[7]+w[2];		
ch[6]=DSHA2_ROTR32(ch[6],((w[7]>>10) & 31));
switch (w[7]>>30) 
{ case 0: ch[5]=w[1]+(ch[3] ^ ch[4] ^ ch[5]);
		break;
case 1: ch[5]=w[1]+((ch[3] & ch[4]) ^ ch[5] );
		break;
case 2: ch[5]=w[1]+((~(ch[3] | ch[5])) | (ch[3] & (ch[4] ^ ch[5])));
		break;
case 3: ch[5]=w[1]+((~(ch[3] | (ch[4] ^ ch[5]))) | (ch[3] & ~(ch[5])));
		break;
}				

temp=((((((ch[2]^ch[3])+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
ch[1]=DSHA2_ROTR32(temp,((w[7]>>15) & 31))+w[3];
ch[0]=ch[0]+w[6];
ch[7]=DSHA2_ROTR32(ch[7],((w[7]>>20) & 31));	
ch[6]=ch[6]+w[5];		
ch[5]=DSHA2_ROTR32(ch[5],((w[7]>>25) & 31));	
ch[4]=w[4]+((~(ch[2] | (ch[3] ^ ch[4]))) | (ch[2] & ~(ch[4])));
ch[3]=ch[3] + w[7];	


temp=((((((ch[1]^ch[2])+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
ch[0]=DSHA2_ROTR32(temp,(w[15] & 31))+w[8];
ch[6]=DSHA2_ROTR32(ch[6],((w[15]>>5) & 31));		
ch[5]=ch[5]+w[10];		
ch[4]=DSHA2_ROTR32(ch[4],((w[15]>>10) & 31));
switch (w[15]>>30) 
{ case 0: ch[3]=w[9]+(ch[1] ^ ch[2] ^ ch[3]);
		break;
case 1: ch[3]=w[9]+((ch[1] & ch[2]) ^ ch[3] );
		break;
case 2: ch[3]=w[9]+((~(ch[1] | ch[3])) | (ch[1] & (ch[2] ^ ch[3])));
		break;
case 3: ch[3]=w[9]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & ~(ch[3])));
		break;
}				

temp=((((((ch[0]^ch[1])+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
ch[7]=DSHA2_ROTR32(temp,((w[15]>>15) & 31))+w[11];
ch[6]=ch[6]+w[14];
ch[5]=DSHA2_ROTR32(ch[5],((w[15]>>20) & 31));	
ch[4]=ch[4]+w[13];		
ch[3]=DSHA2_ROTR32(ch[3],((w[15]>>25) & 31));	
ch[2]=w[12]+((~(ch[0] | (ch[1] ^ ch[2]))) | (ch[0] & ~(ch[2])));
ch[1]=ch[1] + w[15];
}




temp=ch[7];
for (i=7; i>0; i--) { 
	ch[i]=ch[i-1];
}
ch[0]=temp;

for ( i=0;i<8;i++){
    hash_32[i] +=ch[i];
	dsha2State.hashval[i]=hash_32[i];
}
return SUCCESS;
}

int DSHA2::sha64_compile()
{  
unsigned long long hash_64[8], ch[8], temp, w[16];
int i;


for (i=0;i<16;i++)
{
	w[i]=(dsha2State.block)[8*i];
	w[i]=w[i]<<8 | (dsha2State.block)[8*i+1];
	w[i]=w[i]<<8 | (dsha2State.block)[8*i+2];
	w[i]=w[i]<<8 | (dsha2State.block)[8*i+3];
	w[i]=w[i]<<8 | (dsha2State.block)[8*i+4];
	w[i]=w[i]<<8 | (dsha2State.block)[8*i+5];
	w[i]=w[i]<<8 | (dsha2State.block)[8*i+6];
	w[i]=w[i]<<8 | (dsha2State.block)[8*i+7];
}

//Initialize eight working variables 
for (i=0;i<8;i++)
{	
	temp=(dsha2State.hashval)[i*2];
	ch[i]=(temp<< 32)+(dsha2State.hashval)[i*2+1];
	hash_64[i]=ch[i];
}

//iterative part
temp=((((((ch[0]^ch[1])+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
temp=DSHA2_ROTR64(temp,(w[0] & 0x3f));
ch[5]=DSHA2_ROTR64(ch[5],((w[0]>>6) & 0x3f));
ch[4]=DSHA2_ROTR64(ch[4],((w[0]>>12) & 0x3f))+w[3];	
ch[3]=DSHA2_ROTR64(ch[3],((w[0]>>18) & 0x3f));	
switch (w[0]>>62) 
{ case 0: ch[2]=w[2]+(ch[0] ^ ch[1] ^ ch[2]);
		break;
case 1: ch[2]=w[2]+((ch[0] & ch[1]) ^ ch[2] );
		break;
case 2: ch[2]=w[2]+((~(ch[0] | ch[2])) | (ch[0] & (ch[1] ^ ch[2])));
		break;
case 3: ch[2]=w[2]+((~(ch[0] | (ch[1] ^ ch[2]))) | (ch[0] & ~(ch[2])));
		break;
}
ch[1]=DSHA2_ROTR64(ch[1],((w[0]>>24) & 0x3f));
ch[7]=temp+w[1];

temp=((((((ch[7]^ch[0])+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
temp=DSHA2_ROTR64(temp,((w[0]>>30) & 0x3f));
ch[5]=ch[5]+w[7];
ch[4]=DSHA2_ROTR64(ch[4],((w[0]>>36) & 0x3f));
ch[3]=DSHA2_ROTR64(ch[3],((w[0]>>42) & 0x3f))+w[6];
ch[2]=DSHA2_ROTR64(ch[2],((w[0]>>48) & 0x3f));
switch ((w[0]>>60) & 0x3) 
{ case 0: ch[1]=w[5]+(ch[7] ^ ch[0] ^ ch[1]);
		break;
case 1: ch[1]=w[5]+((ch[7] & ch[0]) ^ ch[1] );
		break;
case 2: ch[1]=w[5]+((~(ch[7] | ch[1])) | (ch[7] & (ch[0] ^ ch[1])));
		break;
case 3: ch[1]=w[5]+((~(ch[7] | (ch[0] ^ ch[1]))) | (ch[7] & (~(ch[1]))));
		break;
}
ch[0]=ch[0]+w[0];
ch[7]=DSHA2_ROTR64(ch[7],((w[0]>>54) & 0x3f));
ch[6]=temp+w[4];


temp=((((((ch[6]^ch[7])+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
temp=DSHA2_ROTR64(temp,(w[8] & 0x3f));
ch[3]=DSHA2_ROTR64(ch[3],((w[8]>>6) & 0x3f));
ch[2]=DSHA2_ROTR64(ch[2],((w[8]>>12) & 0x3f))+w[11];	
ch[1]=DSHA2_ROTR64(ch[1],((w[8]>>18) & 0x3f));	
switch (w[8]>>62) 
{ case 0: ch[0]=w[10]+(ch[6] ^ ch[7] ^ ch[0]);
		break;
case 1: ch[0]=w[10]+((ch[6] & ch[7]) ^ ch[0] );
		break;
case 2: ch[0]=w[10]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])));
		break;
case 3: ch[0]=w[10]+((~(ch[6] | (ch[7] ^ ch[0]))) | (ch[6] & ~(ch[0])));
		break;
}
ch[7]=DSHA2_ROTR64(ch[7],((w[8]>>24) & 0x3f));
ch[5]=temp+w[9];

temp=((((((ch[5]^ch[6])+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
temp=DSHA2_ROTR64(temp,((w[8]>>30) & 0x3f));
ch[3]=ch[3]+w[15];
ch[2]=DSHA2_ROTR64(ch[2],((w[8]>>36) & 0x3f));
ch[1]=DSHA2_ROTR64(ch[1],((w[8]>>42) & 0x3f))+w[14];
ch[0]=DSHA2_ROTR64(ch[0],((w[8]>>48) & 0x3f));
switch ((w[8]>>60) & 0x3) 
{ case 0: ch[7]=w[13]+(ch[5] ^ ch[6] ^ ch[7]);
		break;
case 1: ch[7]=w[13]+((ch[5] & ch[6]) ^ ch[7] );
		break;
case 2: ch[7]=w[13]+((~(ch[5] | ch[7])) | (ch[5] & (ch[6] ^ ch[7])));
		break;
case 3: ch[7]=w[13]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & (~(ch[7]))));
		break;
}
ch[6]=ch[6]+w[8];
ch[5]=DSHA2_ROTR64(ch[5],((w[8]>>54) & 0x3f));
ch[4]=temp+w[12];

if (dsha2NumRounds >= 2) {
temp=(((((ch[ 4]+ch[ 5])^ch[ 6])+ch[ 7])^ch[ 0])+ch[ 1])^ch[ 2];
temp=((temp>>36) ^ temp) & 0xfffffffff;
temp=((temp>>18) ^ temp) & 0x3ffff;
temp=((temp>>12) ^ temp) & 0xfff;
temp=((temp>>6) ^ temp) & 0x3f;
ch[ 3]=DSHA2_ROTR64(ch[ 3],temp);
}

if (dsha2NumRounds >= 3) {
temp=(((((ch[ 3]+ch[ 4])^ch[ 5])+ch[ 6])^ch[ 7])+ch[ 0])^ch[ 1];
temp=((temp>>36) ^ temp) & 0xfffffffff;
temp=((temp>>18) ^ temp) & 0x3ffff;
temp=((temp>>12) ^ temp) & 0xfff;
temp=((temp>>6) ^ temp) & 0x3f;
ch[ 2]=DSHA2_ROTR64(ch[ 2],temp);
}

if (dsha2NumRounds >= 4) {
temp=(((((ch[ 2]+ch[ 3])^ch[ 4])+ch[ 5])^ch[ 6])+ch[ 7])^ch[ 0];
temp=((temp>>36) ^ temp) & 0xfffffffff;
temp=((temp>>18) ^ temp) & 0x3ffff;
temp=((temp>>12) ^ temp) & 0xfff;
temp=((temp>>6) ^ temp) & 0x3f;
ch[ 1]=DSHA2_ROTR64(ch[ 1],temp);
}

if (dsha2NumRounds >= 5) {
temp=(((((ch[ 1]+ch[ 2])^ch[ 3])+ch[ 4])^ch[ 5])+ch[ 6])^ch[ 7];
temp=((temp>>36) ^ temp) & 0xfffffffff;
temp=((temp>>18) ^ temp) & 0x3ffff;
temp=((temp>>12) ^ temp) & 0xfff;
temp=((temp>>6) ^ temp) & 0x3f;
ch[ 0]=DSHA2_ROTR64(ch[ 0],temp);
}

if (dsha2NumRounds >= 6) {
temp=(((((ch[ 0]+ch[ 1])^ch[ 2])+ch[ 3])^ch[ 4])+ch[ 5])^ch[ 6];
temp=((temp>>36) ^ temp) & 0xfffffffff;
temp=((temp>>18) ^ temp) & 0x3ffff;
temp=((temp>>12) ^ temp) & 0xfff;
temp=((temp>>6) ^ temp) & 0x3f;
ch[ 7]=DSHA2_ROTR64(ch[ 7],temp);
}

if (dsha2NumRounds >= 7) {
temp=(((((ch[ 7]+ch[ 0])^ch[ 1])+ch[ 2])^ch[ 3])+ch[ 4])^ch[ 5];
temp=((temp>>36) ^ temp) & 0xfffffffff;
temp=((temp>>18) ^ temp) & 0x3ffff;
temp=((temp>>12) ^ temp) & 0xfff;
temp=((temp>>6) ^ temp) & 0x3f;
ch[ 6]=DSHA2_ROTR64(ch[ 6],temp);
}

if (dsha2NumRounds >= 8) {
temp=(((((ch[ 6]+ch[ 7])^ch[ 0])+ch[ 1])^ch[ 2])+ch[ 3])^ch[ 4];
temp=((temp>>36) ^ temp) & 0xfffffffff;
temp=((temp>>18) ^ temp) & 0x3ffff;
temp=((temp>>12) ^ temp) & 0xfff;
temp=((temp>>6) ^ temp) & 0x3f;
ch[ 5]=DSHA2_ROTR64(ch[ 5],temp);
}

if (dsha2NumRounds >= 9) {
temp=(((((ch[ 5]+ch[ 6])^ch[ 7])+ch[ 0])^ch[ 1])+ch[ 2])^ch[ 3];
temp=((temp>>36) ^ temp) & 0xfffffffff;
temp=((temp>>18) ^ temp) & 0x3ffff;
temp=((temp>>12) ^ temp) & 0xfff;
temp=((temp>>6) ^ temp) & 0x3f;
ch[ 4]=DSHA2_ROTR64(ch[ 4],temp);
}

if (dsha2NumRounds >= 10) {
temp=(((((ch[ 4]+ch[ 5])^ch[ 6])+ch[ 7])^ch[ 0])+ch[ 1])^ch[ 2];
temp=((temp>>36) ^ temp) & 0xfffffffff;
temp=((temp>>18) ^ temp) & 0x3ffff;
temp=((temp>>12) ^ temp) & 0xfff;
temp=((temp>>6) ^ temp) & 0x3f;
ch[ 3]=DSHA2_ROTR64(ch[ 3],temp);
}

if (dsha2NumRounds >= 11) {
//the more round
temp=((((((ch[3]^ch[4])+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
temp=DSHA2_ROTR64(temp,(w[1] & 0x3f));
ch[0]=DSHA2_ROTR64(ch[0],((w[1]>>6) & 0x3f));
ch[7]=DSHA2_ROTR64(ch[7],((w[1]>>12) & 0x3f))+w[4];	
ch[6]=DSHA2_ROTR64(ch[6],((w[1]>>18) & 0x3f));	
switch (w[1]>>62) 
{ case 0: ch[5]=w[3]+(ch[3] ^ ch[4] ^ ch[5]);
		break;
case 1: ch[5]=w[3]+((ch[3] & ch[4]) ^ ch[5] );
		break;
case 2: ch[5]=w[3]+((~(ch[3] | ch[5])) | (ch[3] & (ch[4] ^ ch[5])));
		break;
case 3: ch[5]=w[3]+((~(ch[3] | (ch[4] ^ ch[5]))) | (ch[3] & ~(ch[5])));
		break;
}
ch[4]=DSHA2_ROTR64(ch[4],((w[1]>>24) & 0x3f));
ch[2]=temp+w[2];

temp=((((((ch[2]^ch[3])+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
temp=DSHA2_ROTR64(temp,((w[1]>>30) & 0x3f));
ch[0]=ch[0]+w[0];
ch[7]=DSHA2_ROTR64(ch[7],((w[1]>>36) & 0x3f));
ch[6]=DSHA2_ROTR64(ch[6],((w[1]>>42) & 0x3f))+w[7];
ch[5]=DSHA2_ROTR64(ch[5],((w[1]>>48) & 0x3f));
switch ((w[1]>>60) & 0x3) 
{ case 0: ch[4]=w[6]+(ch[2] ^ ch[3] ^ ch[4]);
		break;
case 1: ch[4]=w[6]+((ch[2] & ch[3]) ^ ch[4] );
		break;
case 2: ch[4]=w[6]+((~(ch[2] | ch[4])) | (ch[2] & (ch[3] ^ ch[4])));
		break;
case 3: ch[4]=w[6]+((~(ch[2] | (ch[3] ^ ch[4]))) | (ch[2] & (~(ch[4]))));
		break;
}
ch[3]=ch[3]+w[1];
ch[2]=DSHA2_ROTR64(ch[2],((w[1]>>54) & 0x3f));
ch[1]=temp+w[5];


temp=((((((ch[1]^ch[2])+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
temp=DSHA2_ROTR64(temp,(w[9] & 0x3f));
ch[6]=DSHA2_ROTR64(ch[6],((w[9]>>6) & 0x3f));
ch[5]=DSHA2_ROTR64(ch[5],((w[9]>>12) & 0x3f))+w[12];	
ch[4]=DSHA2_ROTR64(ch[4],((w[9]>>18) & 0x3f));	
switch (w[9]>>62) 
{ case 0: ch[3]=w[11]+(ch[1] ^ ch[2] ^ ch[3]);
		break;
case 1: ch[3]=w[11]+((ch[1] & ch[2]) ^ ch[3] );
		break;
case 2: ch[3]=w[11]+((~(ch[1] | ch[3])) | (ch[1] & (ch[2] ^ ch[3])));
		break;
case 3: ch[3]=w[11]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & ~(ch[3])));
		break;
}
ch[2]=DSHA2_ROTR64(ch[2],((w[9]>>24) & 0x3f));
ch[0]=temp+w[10];

temp=((((((ch[0]^ch[1])+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
temp=DSHA2_ROTR64(temp,((w[9]>>30) & 0x3f));
ch[6]=ch[6]+w[8];
ch[5]=DSHA2_ROTR64(ch[5],((w[9]>>36) & 0x3f));
ch[4]=DSHA2_ROTR64(ch[4],((w[9]>>42) & 0x3f))+w[15];
ch[3]=DSHA2_ROTR64(ch[3],((w[9]>>48) & 0x3f));
switch ((w[9]>>60) & 0x3) 
{ case 0: ch[2]=w[14]+(ch[0] ^ ch[1] ^ ch[2]);
		break;
case 1: ch[2]=w[14]+((ch[0] & ch[1]) ^ ch[2] );
		break;
case 2: ch[2]=w[14]+((~(ch[0] | ch[2])) | (ch[0] & (ch[1] ^ ch[2])));
		break;
case 3: ch[2]=w[14]+((~(ch[0] | (ch[1] ^ ch[2]))) | (ch[0] & (~(ch[2]))));
		break;
}
ch[1]=ch[1]+w[9];
ch[0]=DSHA2_ROTR64(ch[0],((w[9]>>54) & 0x3f));
ch[7]=temp+w[13];
}


if (dsha2NumRounds >= 12) {
temp=((((((ch[7]^ch[0])+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
temp=DSHA2_ROTR64(temp,(w[2] & 0x3f));
ch[4]=DSHA2_ROTR64(ch[4],((w[2]>>6) & 0x3f));
ch[3]=DSHA2_ROTR64(ch[3],((w[2]>>12) & 0x3f))+w[5];	
ch[2]=DSHA2_ROTR64(ch[2],((w[2]>>18) & 0x3f));	
switch (w[2]>>62) 
{ case 0: ch[1]=w[4]+(ch[7] ^ ch[0] ^ ch[1]);
		break;
case 1: ch[1]=w[4]+((ch[7] & ch[0]) ^ ch[1] );
		break;
case 2: ch[1]=w[4]+((~(ch[7] | ch[1])) | (ch[7] & (ch[0] ^ ch[1])));
		break;
case 3: ch[1]=w[4]+((~(ch[7] | (ch[0] ^ ch[1]))) | (ch[7] & ~(ch[1])));
		break;
}
ch[0]=DSHA2_ROTR64(ch[0],((w[2]>>24) & 0x3f));
ch[6]=temp+w[3];

temp=((((((ch[6]^ch[7])+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
temp=DSHA2_ROTR64(temp,((w[2]>>30) & 0x3f));
ch[4]=ch[4]+w[1];
ch[3]=DSHA2_ROTR64(ch[3],((w[2]>>36) & 0x3f));
ch[2]=DSHA2_ROTR64(ch[2],((w[2]>>42) & 0x3f))+w[0];
ch[1]=DSHA2_ROTR64(ch[1],((w[2]>>48) & 0x3f));
switch ((w[2]>>60) & 0x3) 
{ case 0: ch[0]=w[7]+(ch[6] ^ ch[7] ^ ch[0]);
		break;
case 1: ch[0]=w[7]+((ch[6] & ch[7]) ^ ch[0] );
		break;
case 2: ch[0]=w[7]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])));
		break;
case 3: ch[0]=w[7]+((~(ch[6] | (ch[7] ^ ch[0]))) | (ch[6] & (~(ch[0]))));
		break;
}
ch[7]=ch[7]+w[2];
ch[6]=DSHA2_ROTR64(ch[6],((w[2]>>54) & 0x3f));
ch[5]=temp+w[6];


temp=((((((ch[5]^ch[6])+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
temp=DSHA2_ROTR64(temp,(w[10] & 0x3f));
ch[2]=DSHA2_ROTR64(ch[2],((w[10]>>6) & 0x3f));
ch[1]=DSHA2_ROTR64(ch[1],((w[10]>>12) & 0x3f))+w[13];	
ch[0]=DSHA2_ROTR64(ch[0],((w[10]>>18) & 0x3f));	
switch (w[10]>>62) 
{ case 0: ch[7]=w[12]+(ch[5] ^ ch[6] ^ ch[7]);
		break;
case 1: ch[7]=w[12]+((ch[5] & ch[6]) ^ ch[7] );
		break;
case 2: ch[7]=w[12]+((~(ch[5] | ch[7])) | (ch[5] & (ch[6] ^ ch[7])));
		break;
case 3: ch[7]=w[12]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & ~(ch[7])));
		break;
}
ch[6]=DSHA2_ROTR64(ch[6],((w[10]>>24) & 0x3f));
ch[4]=temp+w[11];

temp=((((((ch[4]^ch[5])+ch[6])^ch[7])+ch[0])^ch[1])+ch[2])^ch[3];
temp=DSHA2_ROTR64(temp,((w[10]>>30) & 0x3f));
ch[2]=ch[2]+w[9];
ch[1]=DSHA2_ROTR64(ch[1],((w[10]>>36) & 0x3f));
ch[0]=DSHA2_ROTR64(ch[0],((w[10]>>42) & 0x3f))+w[8];
ch[7]=DSHA2_ROTR64(ch[7],((w[10]>>48) & 0x3f));
switch ((w[10]>>60) & 0x3) 
{ case 0: ch[6]=w[15]+(ch[4] ^ ch[5] ^ ch[6]);
		break;
case 1: ch[6]=w[15]+((ch[4] & ch[5]) ^ ch[6] );
		break;
case 2: ch[6]=w[15]+((~(ch[4] | ch[6])) | (ch[4] & (ch[5] ^ ch[6])));
		break;
case 3: ch[6]=w[15]+((~(ch[4] | (ch[5] ^ ch[6]))) | (ch[4] & (~(ch[6]))));
		break;
}
ch[5]=ch[5]+w[10];
ch[4]=DSHA2_ROTR64(ch[4],((w[10]>>54) & 0x3f));
ch[3]=temp+w[14];
}


if (dsha2NumRounds >= 13) {
temp=((((((ch[3]^ch[4])+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
temp=DSHA2_ROTR64(temp,(w[3] & 0x3f));
ch[0]=DSHA2_ROTR64(ch[0],((w[3]>>6) & 0x3f));
ch[7]=DSHA2_ROTR64(ch[7],((w[3]>>12) & 0x3f))+w[6];	
ch[6]=DSHA2_ROTR64(ch[6],((w[3]>>18) & 0x3f));	
switch (w[3]>>62) 
{ case 0: ch[5]=w[5]+(ch[3] ^ ch[4] ^ ch[5]);
		break;
case 1: ch[5]=w[5]+((ch[3] & ch[4]) ^ ch[5] );
		break;
case 2: ch[5]=w[5]+((~(ch[3] | ch[5])) | (ch[3] & (ch[4] ^ ch[5])));
		break;
case 3: ch[5]=w[5]+((~(ch[3] | (ch[4] ^ ch[5]))) | (ch[3] & ~(ch[5])));
		break;
}
ch[4]=DSHA2_ROTR64(ch[4],((w[3]>>24) & 0x3f));
ch[2]=temp+w[4];

temp=((((((ch[2]^ch[3])+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
temp=DSHA2_ROTR64(temp,((w[3]>>30) & 0x3f));
ch[0]=ch[0]+w[2];
ch[7]=DSHA2_ROTR64(ch[7],((w[3]>>36) & 0x3f));
ch[6]=DSHA2_ROTR64(ch[6],((w[3]>>42) & 0x3f))+w[1];
ch[5]=DSHA2_ROTR64(ch[5],((w[3]>>48) & 0x3f));
switch ((w[3]>>60) & 0x3) 
{ case 0: ch[4]=w[0]+(ch[2] ^ ch[3] ^ ch[4]);
		break;
case 1: ch[4]=w[0]+((ch[2] & ch[3]) ^ ch[4] );
		break;
case 2: ch[4]=w[0]+((~(ch[2] | ch[4])) | (ch[2] & (ch[3] ^ ch[4])));
		break;
case 3: ch[4]=w[0]+((~(ch[2] | (ch[3] ^ ch[4]))) | (ch[2] & (~(ch[4]))));
		break;
}
ch[3]=ch[3]+w[3];
ch[2]=DSHA2_ROTR64(ch[2],((w[3]>>54) & 0x3f));
ch[1]=temp+w[7];


temp=((((((ch[1]^ch[2])+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
temp=DSHA2_ROTR64(temp,(w[11] & 0x3f));
ch[6]=DSHA2_ROTR64(ch[6],((w[11]>>6) & 0x3f));
ch[5]=DSHA2_ROTR64(ch[5],((w[11]>>12) & 0x3f))+w[14];	
ch[4]=DSHA2_ROTR64(ch[4],((w[11]>>18) & 0x3f));	
switch (w[11]>>62) 
{ case 0: ch[3]=w[13]+(ch[1] ^ ch[2] ^ ch[3]);
		break;
case 1: ch[3]=w[13]+((ch[1] & ch[2]) ^ ch[3] );
		break;
case 2: ch[3]=w[13]+((~(ch[1] | ch[3])) | (ch[1] & (ch[2] ^ ch[3])));
		break;
case 3: ch[3]=w[13]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & ~(ch[3])));
		break;
}
ch[2]=DSHA2_ROTR64(ch[2],((w[11]>>24) & 0x3f));
ch[0]=temp+w[12];

temp=((((((ch[0]^ch[1])+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
temp=DSHA2_ROTR64(temp,((w[11]>>30) & 0x3f));
ch[6]=ch[6]+w[10];
ch[5]=DSHA2_ROTR64(ch[5],((w[11]>>36) & 0x3f));
ch[4]=DSHA2_ROTR64(ch[4],((w[11]>>42) & 0x3f))+w[9];
ch[3]=DSHA2_ROTR64(ch[3],((w[11]>>48) & 0x3f));
switch ((w[11]>>60) & 0x3) 
{ case 0: ch[2]=w[8]+(ch[0] ^ ch[1] ^ ch[2]);
		break;
case 1: ch[2]=w[8]+((ch[0] & ch[1]) ^ ch[2] );
		break;
case 2: ch[2]=w[8]+((~(ch[0] | ch[2])) | (ch[0] & (ch[1] ^ ch[2])));
		break;
case 3: ch[2]=w[8]+((~(ch[0] | (ch[1] ^ ch[2]))) | (ch[0] & (~(ch[2]))));
		break;
}
ch[1]=ch[1]+w[11];
ch[0]=DSHA2_ROTR64(ch[0],((w[11]>>54) & 0x3f));
ch[7]=temp+w[15];
}


if (dsha2NumRounds >= 14) {
temp=((((((ch[7]^ch[0])+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
temp=DSHA2_ROTR64(temp,(w[4] & 0x3f));
ch[4]=DSHA2_ROTR64(ch[4],((w[4]>>6) & 0x3f));
ch[3]=DSHA2_ROTR64(ch[3],((w[4]>>12) & 0x3f))+w[7];	
ch[2]=DSHA2_ROTR64(ch[2],((w[4]>>18) & 0x3f));	
switch (w[4]>>62) 
{ case 0: ch[1]=w[6]+(ch[7] ^ ch[0] ^ ch[1]);
		break;
case 1: ch[1]=w[6]+((ch[7] & ch[0]) ^ ch[1] );
		break;
case 2: ch[1]=w[6]+((~(ch[7] | ch[1])) | (ch[7] & (ch[0] ^ ch[1])));
		break;
case 3: ch[1]=w[6]+((~(ch[7] | (ch[0] ^ ch[1]))) | (ch[7] & ~(ch[1])));
		break;
}
ch[0]=DSHA2_ROTR64(ch[0],((w[4]>>24) & 0x3f));
ch[6]=temp+w[5];

temp=((((((ch[6]^ch[7])+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
temp=DSHA2_ROTR64(temp,((w[4]>>30) & 0x3f));
ch[4]=ch[4]+w[3];
ch[3]=DSHA2_ROTR64(ch[3],((w[4]>>36) & 0x3f));
ch[2]=DSHA2_ROTR64(ch[2],((w[4]>>42) & 0x3f))+w[2];
ch[1]=DSHA2_ROTR64(ch[1],((w[4]>>48) & 0x3f));
switch ((w[4]>>60) & 0x3) 
{ case 0: ch[0]=w[1]+(ch[6] ^ ch[7] ^ ch[0]);
		break;
case 1: ch[0]=w[1]+((ch[6] & ch[7]) ^ ch[0] );
		break;
case 2: ch[0]=w[1]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])));
		break;
case 3: ch[0]=w[1]+((~(ch[6] | (ch[7] ^ ch[0]))) | (ch[6] & (~(ch[0]))));
		break;
}
ch[7]=ch[7]+w[4];
ch[6]=DSHA2_ROTR64(ch[6],((w[4]>>54) & 0x3f));
ch[5]=temp+w[0];


temp=((((((ch[5]^ch[6])+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
temp=DSHA2_ROTR64(temp,(w[12] & 0x3f));
ch[2]=DSHA2_ROTR64(ch[2],((w[12]>>6) & 0x3f));
ch[1]=DSHA2_ROTR64(ch[1],((w[12]>>12) & 0x3f))+w[15];	
ch[0]=DSHA2_ROTR64(ch[0],((w[12]>>18) & 0x3f));	
switch (w[12]>>62) 
{ case 0: ch[7]=w[14]+(ch[5] ^ ch[6] ^ ch[7]);
		break;
case 1: ch[7]=w[14]+((ch[5] & ch[6]) ^ ch[7] );
		break;
case 2: ch[7]=w[14]+((~(ch[5] | ch[7])) | (ch[5] & (ch[6] ^ ch[7])));
		break;
case 3: ch[7]=w[14]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & ~(ch[7])));
		break;
}
ch[6]=DSHA2_ROTR64(ch[6],((w[12]>>24) & 0x3f));
ch[4]=temp+w[13];

temp=((((((ch[4]^ch[5])+ch[6])^ch[7])+ch[0])^ch[1])+ch[2])^ch[3];
temp=DSHA2_ROTR64(temp,((w[12]>>30) & 0x3f));
ch[2]=ch[2]+w[11];
ch[1]=DSHA2_ROTR64(ch[1],((w[12]>>36) & 0x3f));
ch[0]=DSHA2_ROTR64(ch[0],((w[12]>>42) & 0x3f))+w[10];
ch[7]=DSHA2_ROTR64(ch[7],((w[12]>>48) & 0x3f));
switch ((w[12]>>60) & 0x3) 
{ case 0: ch[6]=w[9]+(ch[4] ^ ch[5] ^ ch[6]);
		break;
case 1: ch[6]=w[9]+((ch[4] & ch[5]) ^ ch[6] );
		break;
case 2: ch[6]=w[9]+((~(ch[4] | ch[6])) | (ch[4] & (ch[5] ^ ch[6])));
		break;
case 3: ch[6]=w[9]+((~(ch[4] | (ch[5] ^ ch[6]))) | (ch[4] & (~(ch[6]))));
		break;
}
ch[5]=ch[5]+w[12];
ch[4]=DSHA2_ROTR64(ch[4],((w[12]>>54) & 0x3f));
ch[3]=temp+w[8];
}


if (dsha2NumRounds >= 15) {
temp=((((((ch[3]^ch[4])+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
temp=DSHA2_ROTR64(temp,(w[5] & 0x3f));
ch[0]=DSHA2_ROTR64(ch[0],((w[5]>>6) & 0x3f));
ch[7]=DSHA2_ROTR64(ch[7],((w[5]>>12) & 0x3f))+w[0];	
ch[6]=DSHA2_ROTR64(ch[6],((w[5]>>18) & 0x3f));	
switch (w[5]>>62) 
{ case 0: ch[5]=w[7]+(ch[3] ^ ch[4] ^ ch[5]);
		break;
case 1: ch[5]=w[7]+((ch[3] & ch[4]) ^ ch[5] );
		break;
case 2: ch[5]=w[7]+((~(ch[3] | ch[5])) | (ch[3] & (ch[4] ^ ch[5])));
		break;
case 3: ch[5]=w[7]+((~(ch[3] | (ch[4] ^ ch[5]))) | (ch[3] & ~(ch[5])));
		break;
}
ch[4]=DSHA2_ROTR64(ch[4],((w[5]>>24) & 0x3f));
ch[2]=temp+w[6];

temp=((((((ch[2]^ch[3])+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
temp=DSHA2_ROTR64(temp,((w[5]>>30) & 0x3f));
ch[0]=ch[0]+w[4];
ch[7]=DSHA2_ROTR64(ch[7],((w[5]>>36) & 0x3f));
ch[6]=DSHA2_ROTR64(ch[6],((w[5]>>42) & 0x3f))+w[3];
ch[5]=DSHA2_ROTR64(ch[5],((w[5]>>48) & 0x3f));
switch ((w[5]>>60) & 0x3) 
{ case 0: ch[4]=w[2]+(ch[2] ^ ch[3] ^ ch[4]);
		break;
case 1: ch[4]=w[2]+((ch[2] & ch[3]) ^ ch[4] );
		break;
case 2: ch[4]=w[2]+((~(ch[2] | ch[4])) | (ch[2] & (ch[3] ^ ch[4])));
		break;
case 3: ch[4]=w[2]+((~(ch[2] | (ch[3] ^ ch[4]))) | (ch[2] & (~(ch[4]))));
		break;
}
ch[3]=ch[3]+w[5];
ch[2]=DSHA2_ROTR64(ch[2],((w[5]>>54) & 0x3f));
ch[1]=temp+w[1];


temp=((((((ch[1]^ch[2])+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
temp=DSHA2_ROTR64(temp,(w[13] & 0x3f));
ch[6]=DSHA2_ROTR64(ch[6],((w[13]>>6) & 0x3f));
ch[5]=DSHA2_ROTR64(ch[5],((w[13]>>12) & 0x3f))+w[8];	
ch[4]=DSHA2_ROTR64(ch[4],((w[13]>>18) & 0x3f));	
switch (w[13]>>62) 
{ case 0: ch[3]=w[15]+(ch[1] ^ ch[2] ^ ch[3]);
		break;
case 1: ch[3]=w[15]+((ch[1] & ch[2]) ^ ch[3] );
		break;
case 2: ch[3]=w[15]+((~(ch[1] | ch[3])) | (ch[1] & (ch[2] ^ ch[3])));
		break;
case 3: ch[3]=w[15]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & ~(ch[3])));
		break;
}
ch[2]=DSHA2_ROTR64(ch[2],((w[13]>>24) & 0x3f));
ch[0]=temp+w[14];

temp=((((((ch[0]^ch[1])+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
temp=DSHA2_ROTR64(temp,((w[13]>>30) & 0x3f));
ch[6]=ch[6]+w[12];
ch[5]=DSHA2_ROTR64(ch[5],((w[13]>>36) & 0x3f));
ch[4]=DSHA2_ROTR64(ch[4],((w[13]>>42) & 0x3f))+w[11];
ch[3]=DSHA2_ROTR64(ch[3],((w[13]>>48) & 0x3f));
switch ((w[13]>>60) & 0x3) 
{ case 0: ch[2]=w[10]+(ch[0] ^ ch[1] ^ ch[2]);
		break;
case 1: ch[2]=w[10]+((ch[0] & ch[1]) ^ ch[2] );
		break;
case 2: ch[2]=w[10]+((~(ch[0] | ch[2])) | (ch[0] & (ch[1] ^ ch[2])));
		break;
case 3: ch[2]=w[10]+((~(ch[0] | (ch[1] ^ ch[2]))) | (ch[0] & (~(ch[2]))));
		break;
}
ch[1]=ch[1]+w[13];
ch[0]=DSHA2_ROTR64(ch[0],((w[13]>>54) & 0x3f));
ch[7]=temp+w[9];
}


if (dsha2NumRounds >= 16) {
temp=((((((ch[7]^ch[0])+ch[1])^ch[2])+ch[3])^ch[4])+ch[5])^ch[6];
temp=DSHA2_ROTR64(temp,(w[6] & 0x3f));
ch[4]=DSHA2_ROTR64(ch[4],((w[6]>>6) & 0x3f));
ch[3]=DSHA2_ROTR64(ch[3],((w[6]>>12) & 0x3f))+w[1];	
ch[2]=DSHA2_ROTR64(ch[2],((w[6]>>18) & 0x3f));	
switch (w[6]>>62) 
{ case 0: ch[1]=w[0]+(ch[7] ^ ch[0] ^ ch[1]);
		break;
case 1: ch[1]=w[0]+((ch[7] & ch[0]) ^ ch[1] );
		break;
case 2: ch[1]=w[0]+((~(ch[7] | ch[1])) | (ch[7] & (ch[0] ^ ch[1])));
		break;
case 3: ch[1]=w[0]+((~(ch[7] | (ch[0] ^ ch[1]))) | (ch[7] & ~(ch[1])));
		break;
}
ch[0]=DSHA2_ROTR64(ch[0],((w[6]>>24) & 0x3f));
ch[6]=temp+w[7];

temp=((((((ch[6]^ch[7])+ch[0])^ch[1])+ch[2])^ch[3])+ch[4])^ch[5];
temp=DSHA2_ROTR64(temp,((w[6]>>30) & 0x3f));
ch[4]=ch[4]+w[5];
ch[3]=DSHA2_ROTR64(ch[3],((w[6]>>36) & 0x3f));
ch[2]=DSHA2_ROTR64(ch[2],((w[6]>>42) & 0x3f))+w[4];
ch[1]=DSHA2_ROTR64(ch[1],((w[6]>>48) & 0x3f));
switch ((w[6]>>60) & 0x3) 
{ case 0: ch[0]=w[3]+(ch[6] ^ ch[7] ^ ch[0]);
		break;
case 1: ch[0]=w[3]+((ch[6] & ch[7]) ^ ch[0] );
		break;
case 2: ch[0]=w[3]+((~(ch[6] | ch[0])) | (ch[6] & (ch[7] ^ ch[0])));
		break;
case 3: ch[0]=w[3]+((~(ch[6] | (ch[7] ^ ch[0]))) | (ch[6] & (~(ch[0]))));
		break;
}
ch[7]=ch[7]+w[6];
ch[6]=DSHA2_ROTR64(ch[6],((w[6]>>54) & 0x3f));
ch[5]=temp+w[2];


temp=((((((ch[5]^ch[6])+ch[7])^ch[0])+ch[1])^ch[2])+ch[3])^ch[4];
temp=DSHA2_ROTR64(temp,(w[14] & 0x3f));
ch[2]=DSHA2_ROTR64(ch[2],((w[14]>>6) & 0x3f));
ch[1]=DSHA2_ROTR64(ch[1],((w[14]>>12) & 0x3f))+w[9];	
ch[0]=DSHA2_ROTR64(ch[0],((w[14]>>18) & 0x3f));	
switch (w[14]>>62) 
{ case 0: ch[7]=w[8]+(ch[5] ^ ch[6] ^ ch[7]);
		break;
case 1: ch[7]=w[8]+((ch[5] & ch[6]) ^ ch[7] );
		break;
case 2: ch[7]=w[8]+((~(ch[5] | ch[7])) | (ch[5] & (ch[6] ^ ch[7])));
		break;
case 3: ch[7]=w[8]+((~(ch[5] | (ch[6] ^ ch[7]))) | (ch[5] & ~(ch[7])));
		break;
}
ch[6]=DSHA2_ROTR64(ch[6],((w[14]>>24) & 0x3f));
ch[4]=temp+w[15];

temp=((((((ch[4]^ch[5])+ch[6])^ch[7])+ch[0])^ch[1])+ch[2])^ch[3];
temp=DSHA2_ROTR64(temp,((w[14]>>30) & 0x3f));
ch[2]=ch[2]+w[13];
ch[1]=DSHA2_ROTR64(ch[1],((w[14]>>36) & 0x3f));
ch[0]=DSHA2_ROTR64(ch[0],((w[14]>>42) & 0x3f))+w[12];
ch[7]=DSHA2_ROTR64(ch[7],((w[14]>>48) & 0x3f));
switch ((w[14]>>60) & 0x3) 
{ case 0: ch[6]=w[11]+(ch[4] ^ ch[5] ^ ch[6]);
		break;
case 1: ch[6]=w[11]+((ch[4] & ch[5]) ^ ch[6] );
		break;
case 2: ch[6]=w[11]+((~(ch[4] | ch[6])) | (ch[4] & (ch[5] ^ ch[6])));
		break;
case 3: ch[6]=w[11]+((~(ch[4] | (ch[5] ^ ch[6]))) | (ch[4] & (~(ch[6]))));
		break;
}
ch[5]=ch[5]+w[14];
ch[4]=DSHA2_ROTR64(ch[4],((w[14]>>54) & 0x3f));
ch[3]=temp+w[10];
}


if (dsha2NumRounds >= 17) {
temp=((((((ch[3]^ch[4])+ch[5])^ch[6])+ch[7])^ch[0])+ch[1])^ch[2];
temp=DSHA2_ROTR64(temp,(w[7] & 0x3f));
ch[0]=DSHA2_ROTR64(ch[0],((w[7]>>6) & 0x3f));
ch[7]=DSHA2_ROTR64(ch[7],((w[7]>>12) & 0x3f))+w[2];	
ch[6]=DSHA2_ROTR64(ch[6],((w[7]>>18) & 0x3f));	
switch (w[7]>>62) 
{ case 0: ch[5]=w[1]+(ch[3] ^ ch[4] ^ ch[5]);
		break;
case 1: ch[5]=w[1]+((ch[3] & ch[4]) ^ ch[5] );
		break;
case 2: ch[5]=w[1]+((~(ch[3] | ch[5])) | (ch[3] & (ch[4] ^ ch[5])));
		break;
case 3: ch[5]=w[1]+((~(ch[3] | (ch[4] ^ ch[5]))) | (ch[3] & ~(ch[5])));
		break;
}
ch[4]=DSHA2_ROTR64(ch[4],((w[7]>>24) & 0x3f));
ch[2]=temp+w[0];

temp=((((((ch[2]^ch[3])+ch[4])^ch[5])+ch[6])^ch[7])+ch[0])^ch[1];
temp=DSHA2_ROTR64(temp,((w[7]>>30) & 0x3f));
ch[0]=ch[0]+w[6];
ch[7]=DSHA2_ROTR64(ch[7],((w[7]>>36) & 0x3f));
ch[6]=DSHA2_ROTR64(ch[6],((w[7]>>42) & 0x3f))+w[5];
ch[5]=DSHA2_ROTR64(ch[5],((w[7]>>48) & 0x3f));
switch ((w[7]>>60) & 0x3) 
{ case 0: ch[4]=w[4]+(ch[2] ^ ch[3] ^ ch[4]);
		break;
case 1: ch[4]=w[4]+((ch[2] & ch[3]) ^ ch[4] );
		break;
case 2: ch[4]=w[4]+((~(ch[2] | ch[4])) | (ch[2] & (ch[3] ^ ch[4])));
		break;
case 3: ch[4]=w[4]+((~(ch[2] | (ch[3] ^ ch[4]))) | (ch[2] & (~(ch[4]))));
		break;
}
ch[3]=ch[3]+w[7];
ch[2]=DSHA2_ROTR64(ch[2],((w[7]>>54) & 0x3f));
ch[1]=temp+w[3];


temp=((((((ch[1]^ch[2])+ch[3])^ch[4])+ch[5])^ch[6])+ch[7])^ch[0];
temp=DSHA2_ROTR64(temp,(w[15] & 0x3f));
ch[6]=DSHA2_ROTR64(ch[6],((w[15]>>6) & 0x3f));
ch[5]=DSHA2_ROTR64(ch[5],((w[15]>>12) & 0x3f))+w[10];	
ch[4]=DSHA2_ROTR64(ch[4],((w[15]>>18) & 0x3f));	
switch (w[15]>>62) 
{ case 0: ch[3]=w[9]+(ch[1] ^ ch[2] ^ ch[3]);
		break;
case 1: ch[3]=w[9]+((ch[1] & ch[2]) ^ ch[3] );
		break;
case 2: ch[3]=w[9]+((~(ch[1] | ch[3])) | (ch[1] & (ch[2] ^ ch[3])));
		break;
case 3: ch[3]=w[9]+((~(ch[1] | (ch[2] ^ ch[3]))) | (ch[1] & ~(ch[3])));
		break;
}
ch[2]=DSHA2_ROTR64(ch[2],((w[15]>>24) & 0x3f));
ch[0]=temp+w[8];

temp=((((((ch[0]^ch[1])+ch[2])^ch[3])+ch[4])^ch[5])+ch[6])^ch[7];
temp=DSHA2_ROTR64(temp,((w[15]>>30) & 0x3f));
ch[6]=ch[6]+w[14];
ch[5]=DSHA2_ROTR64(ch[5],((w[15]>>36) & 0x3f));
ch[4]=DSHA2_ROTR64(ch[4],((w[15]>>42) & 0x3f))+w[13];
ch[3]=DSHA2_ROTR64(ch[3],((w[15]>>48) & 0x3f));
switch ((w[15]>>60) & 0x3) 
{ case 0: ch[2]=w[12]+(ch[0] ^ ch[1] ^ ch[2]);
		break;
case 1: ch[2]=w[12]+((ch[0] & ch[1]) ^ ch[2] );
		break;
case 2: ch[2]=w[12]+((~(ch[0] | ch[2])) | (ch[0] & (ch[1] ^ ch[2])));
		break;
case 3: ch[2]=w[12]+((~(ch[0] | (ch[1] ^ ch[2]))) | (ch[0] & (~(ch[2]))));
		break;
}
ch[1]=ch[1]+w[15];
ch[0]=DSHA2_ROTR64(ch[0],((w[15]>>54) & 0x3f));
ch[7]=temp+w[11];
}

temp=ch[7];
for (i=7; i>0; i--) { 
	ch[i]=ch[i-1];
}
ch[0]=temp;
// turn back

// Compute intermediate hash value 
for (i=0;i<8;i++){
    hash_64[i] +=ch[i];	
	(dsha2State.hashval)[2*i]=hash_64[i] >> 32 ;
	(dsha2State.hashval)[2*i+1]=hash_64[i] & 0xffffffff;
}
return SUCCESS;
}

DSHA2::DSHA2(const int numRounds) {
	if (numRounds == -1) {
		dsha2NumRounds = DSHA2_DEFAULT_ROUNDS;
	} else {
		dsha2NumRounds = numRounds;
	}
}