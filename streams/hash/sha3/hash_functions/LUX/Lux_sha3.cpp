#include <stdio.h>
#include <string.h>
#include "Lux_sha3.h"

/* Table look-up */
#define LUX_TL(table,input,shift) table[(input >> shift) & 0xff]

/* Update core element for LUX-224 and LUX-256 */
#define LUX_UPVAR32(nv,z1,ov,z2,in,l0,l1,l2,l3,add)									\
	nv##in##z1=  LUX_TL(Lux_Te0,ov##l0##z2,24) ^ LUX_TL(Lux_Te1,ov##l1##z2,16) ^ LUX_TL(Lux_Te2,ov##l2##z2,8)  ^ LUX_TL(Lux_Te3,ov##l3##z2,0)##add; 

/* Update core element for LUX-384 and LUX-512 */
#define LUX_UPVAR64(nv,z1,ov,z2,in,l0,l1,l2,l3,l4,l5,l6,l7,add)									\
	nv##in##z1= LUX_TL(Lux_T640,ov##l0##z2,56) ^ LUX_TL(Lux_T641,ov##l1##z2,48) ^ LUX_TL(Lux_T642,ov##l2##z2,40) ^ LUX_TL(Lux_T643,ov##l3##z2,32) ^ \
				LUX_TL(Lux_T644,ov##l4##z2,24) ^ LUX_TL(Lux_T645,ov##l5##z2,16) ^ LUX_TL(Lux_T646,ov##l6##z2, 8) ^ LUX_TL(Lux_T647,ov##l7##z2,0)##add; 

/* One round for optimized LUX-224 and LUX-256.  */
#define LUX_ONEROUND(offSet,t,z1,s,z2,bf, p,data)					\
	do {														\
		s##0##z2^=*(unsigned int *)(data+offSet);						\
		bf[(p+13)%16]^=*(unsigned int *)(data+offSet);					\
		LUX_UPVAR32(t,z1,s,z2,0,0,1,3,4,^0x2ad01c64)				\
		LUX_UPVAR32(t,z1,s,z2,1,1,2,4,5,)							\
		LUX_UPVAR32(t,z1,s,z2,2,2,3,5,6,)							\
		LUX_UPVAR32(t,z1,s,z2,3,3,4,6,7,)							\
		LUX_UPVAR32(t,z1,s,z2,4,4,5,7,0,)							\
		LUX_UPVAR32(t,z1,s,z2,5,5,6,0,1,)							\
		LUX_UPVAR32(t,z1,s,z2,6,6,7,1,2,)							\
		LUX_UPVAR32(t,z1,s,z2,7,7,0,2,3,)							\
		bf[p%16]^=t##0##z1;			bf[(p+1)%16]^=t##1##z1;		\
		bf[(p+2)%16]^=t##2##z1;		bf[(p+3)%16]^=t##3##z1;		\
		bf[(p+4)%16]^=t##4##z1;		bf[(p+5)%16]^=t##5##z1;		\
		bf[(p+6)%16]^=t##6##z1;		bf[(p+7)%16]^=t##7##z1;		\
		t##7##z1^=bf[(p+11)%16];								\
	} while (0)
		
/* One round for optimized LUX-384 and LUX-512.  */
#define LUX_ONEROUND64(offSet,t,z1,s,z2,bf,p,data)				\
	do {													\
		s##0##z2^=*(unsigned long long *)(data+offSet);					\
		bf[(p+13)%16]^=*(unsigned long long *)(data+offSet);				\
		LUX_UPVAR64(t,z1,s,z2,0,0,1,2,3,4,5,6,7,^0x2ad01c64);	\
		LUX_UPVAR64(t,z1,s,z2,1,1,2,3,4,5,6,7,0,);				\
		LUX_UPVAR64(t,z1,s,z2,2,2,3,4,5,6,7,0,1,);				\
		LUX_UPVAR64(t,z1,s,z2,3,3,4,5,6,7,0,1,2,);				\
		LUX_UPVAR64(t,z1,s,z2,4,4,5,6,7,0,1,2,3,);				\
		LUX_UPVAR64(t,z1,s,z2,5,5,6,7,0,1,2,3,4,);				\
		LUX_UPVAR64(t,z1,s,z2,6,6,7,0,1,2,3,4,5,);				\
		LUX_UPVAR64(t,z1,s,z2,7,7,0,1,2,3,4,5,6,);				\
		bf[p%16]^=t##0##z1;			bf[(p+1)%16]^=t##1##z1;	\
		bf[(p+2)%16]^=t##2##z1;		bf[(p+3)%16]^=t##3##z1;	\
		bf[(p+4)%16]^=t##4##z1;		bf[(p+5)%16]^=t##5##z1;	\
		bf[(p+6)%16]^=t##6##z1;		bf[(p+7)%16]^=t##7##z1;	\
		t##7##z1^=bf[(p+11)%16];							\
	} while (0)

/* One round of LUX-256 */
void Lux::compressSmallChunk(const BitSequence *data){	
	int i; 
	unsigned int temp;
	unsigned int t0,t1,t2,t3,t4,t5,t6,t7;	
	
	/* Addition of the message block to the buffer and the core  */
	luxState.core[0]^=*(unsigned int *)(data+0);
	luxState.buffer[0]^=*( unsigned int *)(data+0);	

    /* Core transformation G  */	
	LUX_UPVAR32(t,,luxState.core[,],0,0,1,3,4,^0x2ad01c64);
	LUX_UPVAR32(t,,luxState.core[,],1,1,2,4,5,);
	LUX_UPVAR32(t,,luxState.core[,],2,2,3,5,6,);
	LUX_UPVAR32(t,,luxState.core[,],3,3,4,6,7,);
	LUX_UPVAR32(t,,luxState.core[,],4,4,5,7,0,);
	LUX_UPVAR32(t,,luxState.core[,],5,5,6,0,1,);
	LUX_UPVAR32(t,,luxState.core[,],6,6,7,1,2,);
	LUX_UPVAR32(t,,luxState.core[,],7,7,0,2,3,);  

	/* Buffer transformation F  */
	temp=luxState.buffer[15];
	for(i=15;i>0;i--)
		luxState.buffer[i]=luxState.buffer[i-1];
	luxState.buffer[0]=temp;

	/* Feedback from the core to the buffer. */
	luxState.buffer[4]^=t0; luxState.buffer[5]^=t1; luxState.buffer[6]^=t2; luxState.buffer[7]^=t3;
	luxState.buffer[8]^=t4; luxState.buffer[9]^=t5; luxState.buffer[10]^=t6; luxState.buffer[11]^=t7;	

	/* Feedback from the buffer to the core	 */	
	t7^=luxState.buffer[15];

	luxState.core[0]=t0; luxState.core[1]=t1; luxState.core[2]=t2; luxState.core[3]=t3;
	luxState.core[4]=t4; luxState.core[5]=t5; luxState.core[6]=t6; luxState.core[7]=t7;	
}

/* One round of LUX-512 */
void Lux::compressSmallChunk64(const BitSequence *data){

	int i;
	unsigned long long temp;
	unsigned long long tt0,tt1,tt2,tt3,tt4,tt5,tt6,tt7;

	luxState.core64[0]^=*(unsigned long long *)(data+0);
	luxState.buffer64[0]^=*(unsigned long long *)(data+0);	

	LUX_UPVAR64(tt,,luxState.core64[,],0,0,1,2,3,4,5,6,7,^0x2ad01c64);
	LUX_UPVAR64(tt,,luxState.core64[,],1,1,2,3,4,5,6,7,0,);
	LUX_UPVAR64(tt,,luxState.core64[,],2,2,3,4,5,6,7,0,1,);
	LUX_UPVAR64(tt,,luxState.core64[,],3,3,4,5,6,7,0,1,2,);
	LUX_UPVAR64(tt,,luxState.core64[,],4,4,5,6,7,0,1,2,3,);
	LUX_UPVAR64(tt,,luxState.core64[,],5,5,6,7,0,1,2,3,4,);
	LUX_UPVAR64(tt,,luxState.core64[,],6,6,7,0,1,2,3,4,5,);
	LUX_UPVAR64(tt,,luxState.core64[,],7,7,0,1,2,3,4,5,6,);
	
	temp=luxState.buffer64[15];
	for(i=15;i>0;i--)
		luxState.buffer64[i]=luxState.buffer64[i-1];
	luxState.buffer64[0]=temp;

	luxState.buffer64[4]^=tt0; luxState.buffer64[5]^=tt1; luxState.buffer64[6]^=tt2; luxState.buffer64[7]^=tt3;
	luxState.buffer64[8]^=tt4; luxState.buffer64[9]^=tt5; luxState.buffer64[10]^=tt6; luxState.buffer64[11]^=tt7;	
	
	tt7^=luxState.buffer64[15];

	luxState.core64[0]=tt0;luxState.core64[1]=tt1;luxState.core64[2]=tt2;luxState.core64[3]=tt3;
	luxState.core64[4]=tt4;luxState.core64[5]=tt5;luxState.core64[6]=tt6;luxState.core64[7]=tt7;
}

/* 16 rounds are processed. This way, the rotations of the buffer can be avoided.   */
void Lux::compressBigChunk(const BitSequence *data){
	unsigned long int i;
	unsigned int ivo[16];
	unsigned int s0,s1,s2,s3,s4,s5,s6,s7;
	unsigned int t0,t1,t2,t3,t4,t5,t6,t7;

	for(i=0;i<luxNumRounds;i++){
		ivo[i]=luxState.buffer[i];		
	}

	if (luxNumRounds >= 1) LUX_ONEROUND(0 ,t,,luxState.core[,],ivo,3,data);					
	if (luxNumRounds >= 2) LUX_ONEROUND(4 ,s,,t,,ivo,2,data);		
	if (luxNumRounds >= 3) LUX_ONEROUND(8 ,t,,s,,ivo,1,data);		
	if (luxNumRounds >= 4) LUX_ONEROUND(12,s,,t,,ivo,0,data);		
	if (luxNumRounds >= 5) LUX_ONEROUND(16,t,,s,,ivo,15,data);	
	if (luxNumRounds >= 6) LUX_ONEROUND(20,s,,t,,ivo,14,data);	
	if (luxNumRounds >= 7) LUX_ONEROUND(24,t,,s,,ivo,13,data);	
	if (luxNumRounds >= 8) LUX_ONEROUND(28,s,,t,,ivo,12,data);	
	if (luxNumRounds >= 9) LUX_ONEROUND(32,t,,s,,ivo,11,data);	
	if (luxNumRounds >= 10) LUX_ONEROUND(36,s,,t,,ivo,10,data);	
	if (luxNumRounds >= 11) LUX_ONEROUND(40,t,,s,,ivo,9,data);		
	if (luxNumRounds >= 12) LUX_ONEROUND(44,s,,t,,ivo,8,data);		
	if (luxNumRounds >= 13) LUX_ONEROUND(48,t,,s,,ivo,7,data);		
	if (luxNumRounds >= 14) LUX_ONEROUND(52,s,,t,,ivo,6,data);		
	if (luxNumRounds >= 15) LUX_ONEROUND(56,t,,s,,ivo,5,data);		
	if (luxNumRounds >= 16) LUX_ONEROUND(60,luxState.core[,],t,,ivo,4,data);		

	for(i=0;i<luxNumRounds;i++){
		luxState.buffer[i]=ivo[i];
	}
}

/*	16 rounds for LUX-384 and LUX-512 are processed. 
	This way, the rotations of the buffer can be avoided. 
 */
void Lux::compressBigChunk64(const BitSequence *data){
	unsigned long int i;
	unsigned long long ivo[16];
	unsigned long long s0,s1,s2,s3,s4,s5,s6,s7;
	unsigned long long t0,t1,t2,t3,t4,t5,t6,t7;

	for(i=0;i<luxNumRounds;i++){
		ivo[i]=luxState.buffer64[i];		
	}

	if (luxNumRounds >= 1) LUX_ONEROUND64(0 ,t,,luxState.core64[,],ivo,3,data);		
	if (luxNumRounds >= 2) LUX_ONEROUND64(8 ,s,,t,,ivo,2,data);		
	if (luxNumRounds >= 3) LUX_ONEROUND64(16,t,,s,,ivo,1,data);		
	if (luxNumRounds >= 4) LUX_ONEROUND64(24,s,,t,,ivo,0,data);		
	if (luxNumRounds >= 5) LUX_ONEROUND64(32,t,,s,,ivo,15,data);	
	if (luxNumRounds >= 6) LUX_ONEROUND64(40,s,,t,,ivo,14,data);	
	if (luxNumRounds >= 7) LUX_ONEROUND64(48,t,,s,,ivo,13,data);	
	if (luxNumRounds >= 8) LUX_ONEROUND64(56,s,,t,,ivo,12,data);	
	if (luxNumRounds >= 9) LUX_ONEROUND64(64,t,,s,,ivo,11,data);	
	if (luxNumRounds >= 10) LUX_ONEROUND64(72,s,,t,,ivo,10,data);	
	if (luxNumRounds >= 11) LUX_ONEROUND64(80,t,,s,,ivo,9,data);		
	if (luxNumRounds >= 12) LUX_ONEROUND64(88,s,,t,,ivo,8,data);		
	if (luxNumRounds >= 13) LUX_ONEROUND64(96,t,,s,,ivo,7,data);		
	if (luxNumRounds >= 14) LUX_ONEROUND64(104,s,,t,,ivo,6,data);		
	if (luxNumRounds >= 15) LUX_ONEROUND64(112,t,,s,,ivo,5,data);		
	if (luxNumRounds >= 16) LUX_ONEROUND64(120,luxState.core64[,],t,,ivo,4,data);		

	for(i=0;i<luxNumRounds;i++){
		luxState.buffer64[i]=ivo[i];
	}
}

int Lux::Init(int hashbitlen){
	int i;

	luxState.hashbitlen=hashbitlen;
	if(!((hashbitlen==224)||(hashbitlen==256)||(hashbitlen==384)||(hashbitlen==512)))
		return BAD_HASHBITLEN;
		
	for(i=0;i<8;i++){
		luxState.core[i]=0;
		luxState.core64[i]=0;
	}
	for(i=0;i<16;i++){
		luxState.buffer[i]=0;
		luxState.buffer64[i]=0;
	}
	for(i=0;i<8;i++)
		luxState.remSeq[i]=0;

	return SUCCESS;
}

int Lux::Update(const BitSequence *data, DataLength databitlen){

	int i;
	unsigned long long doneLength=0;
	
	luxState.databitlen=databitlen;
	if(luxState.hashbitlen<=256){		
		/*	First compress the big chunks of 64 bytes each 
			in 16 rounds. This way the rotations of the 
			buffer are costless.
		 */
		while(doneLength+64<=databitlen/8){							
			Lux::compressBigChunk(data+doneLength);
			doneLength+=64;			
		}		

		/*	The rest of the input is compressed.  */
		while(doneLength+4<=databitlen/8){		
			Lux::compressSmallChunk(data+doneLength);			
			doneLength+=4;			
		}		
	}
	else{	
		/*	First compress the big chunks of 128 bytes each
			in 16 rounds. This way the rotations of the 
			buffer are costless.
		 */
		
		while(doneLength+128<=databitlen/8){		
			Lux::compressBigChunk64(data+doneLength);			
			doneLength+=128;		
		}

		/* The rest of the input is compressed.  */
		while(doneLength+8<=databitlen/8){		
			Lux::compressSmallChunk64(data+doneLength);		
			doneLength+=8;
		}

	}
	
	/* The remaining input is stored for further processing in Final().	 */
	luxState.lenRemSeq=databitlen-8*doneLength;	
	for(i=0;i<luxState.lenRemSeq/8+(luxState.lenRemSeq%8>0);i++){
			luxState.remSeq[i]=data[doneLength+i];		
	}

	return SUCCESS;
}

int Lux::Final(BitSequence *hashval){
	int i;
	BitSequence empty[128];	

	/*	If the block is not full then add 1 and a number of 0s.	*/
	if(luxState.lenRemSeq>0){		
		luxState.remSeq[luxState.lenRemSeq>>3] &=(0xff)<<(8-(luxState.lenRemSeq & 7));
		luxState.remSeq[luxState.lenRemSeq>>3] ^=(0x01)<<(7-((luxState.lenRemSeq) & 7));	
	}
	/*	Else create new block with 1 and 31 zeros. */
	else{
		luxState.remSeq[0]=0x80;
	}

	/* Hash the padded block. */
	if(luxState.hashbitlen<=256){		
		Lux::compressSmallChunk(luxState.remSeq);		
	}
	else{		
		Lux::compressSmallChunk64(luxState.remSeq);		
	}
	
	/*	Additional block(s) that contain the input 
		length is(are) created and compressed. For LUX-224 and LUX-256, 
		the message blocks are only 32 bits, therefore the message length 
		represantion takes two block. 
	*/
	for(i=0;i<8;i++)
		empty[i]   = ( (luxState.databitlen)>>(56-8*i) ) & 0xff;					
	if(luxState.hashbitlen<=256){
		Lux::compressSmallChunk(empty);
		Lux::compressSmallChunk(empty+4);
	}	
	else{		
		Lux::compressSmallChunk64(empty);
	}

	/*	16 blank rounds are iterated. 
		The input for the blank rounds are blocks with 0s.
		Output is value of the core element C_3.
	 */
	memset(empty, 0, 128);
	if(luxState.hashbitlen<=256){
		for(i=0;i<16;i++){
			Lux::compressSmallChunk(empty);		
		}
		for(i=0;i<luxState.hashbitlen/32;i++){
			Lux::compressSmallChunk(empty);				
				*(unsigned int *)(hashval + 4*i)=*(unsigned int *)(luxState.core + 3);		
		}
	}
	else{
		for(i=0;i<16;i++){
			Lux::compressSmallChunk64(empty);			
		}
		for(i=0;i<luxState.hashbitlen/64;i++){
			Lux::compressSmallChunk64(empty);
			*(unsigned long long *)(hashval + 8*i)=*(unsigned long long *)(luxState.core64 + 3);
		}
	}

	return SUCCESS;
}

int Lux::Hash(int hashbitlen, const BitSequence *data, DataLength databitlen, BitSequence *hashval){
	//hashState Temp;

	if(Lux::Init(hashbitlen)==SUCCESS){	
		Lux::Update(data,databitlen );	
		Lux::Final(hashval);
		return  SUCCESS;
	}
	else return FAIL;
}

Lux::Lux(const int numRounds) {
	if ((numRounds == -1) || (numRounds > 16)) {
		luxNumRounds = LUX_DEFAULT_ROUNDS;
	} else {
		luxNumRounds = numRounds;
	}
}