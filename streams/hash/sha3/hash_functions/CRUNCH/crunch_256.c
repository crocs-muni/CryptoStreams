/*
This algorithm doesn't shift the data between each round of the Feistel Scheme.
*/
#include <stdio.h>
#include <stdlib.h>

#include "crunch_type.h"
#include "crunch_256.h"

#ifdef M64
#define LONGSIZE 64
#else
#define LONGSIZE 32
#endif

#define CRUNCH256_NE (CRUNCH256_BLOCKSIZE/LONGSIZE)	// Number of long in input
#define CRUNCH256_NS (CRUNCH256_HASHLEN/LONGSIZE)	// Number of long in outpout
#define CRUNCH256_NPM ((LONGSIZE*CRUNCH256_K)/CRUNCH256_BLOCKSIZE)	//Longs (32 or 64 bits) are divided in CRUNCH256_NPM parts

#define CRUNCH256_NA 256			// = 2^CRUNCH256_NBE number of elements in start ensemble of random function
#define CRUNCH256_NBE (CRUNCH256_BLOCKSIZE/CRUNCH256_K)		//  number of bits in elements in start ensemble of random function

//macro used to send final results in same order in 32 and 64  bits version (due to integers representation on X86)
#define crunch256_idx(a)  ( (LONGSIZE/8) * ((a)/(LONGSIZE/8)) + ((LONGSIZE/8)-1) - (a)%(LONGSIZE/8))

//#define CRUNCH256_SHIFT	((CRUNCH256_NBROUND%128)*8)/LONGSIZE

#define CRUNCH256_MAXDATAPERUPDATE 1024ULL*1024ULL*1024ULL

/* With a 32 bit compilation crunch_nbalea[i] is exactly K_{i-28}&Mask
where K_i are the decimals of 8*abs(sin(i+29))
and Mask value is 0xffffffff most of the time
Other value of Mask are 0x00ffffff, 0xff00ffff, 0xffff00ff, 0xffffff00
see the file gen_alea.c for details
In this way we can xor all the bytes, even the byte which give the value of i in g(i)
*/
extern unsigned long int crunch_nbalea[];

//Left data after a call of Update function
unsigned long int Crunch256_LeftData[(CRUNCH256_BLOCKSIZE-CRUNCH256_HASHLEN)/LONGSIZE];
unsigned int Crunch256_SizeLeft; //size of left data in bytes
unsigned long int Crunch256_PrevHash[CRUNCH256_HASHLEN/LONGSIZE];
CrunchDataLength Crunch256TotalInputSizeInBit; //Total size of message in bits


unsigned long int *padding_256(int *nb_final_block)
{
	int new_nb_bit,i,no_block;
	unsigned long int *mes;
	char *mes_char,*DL_char;
	int temp;
	new_nb_bit=Crunch256_SizeLeft*8+1+2*LONGSIZE;
	i=new_nb_bit % (CRUNCH256_BLOCKSIZE-CRUNCH256_HASHLEN);
	new_nb_bit+=(CRUNCH256_BLOCKSIZE-CRUNCH256_HASHLEN)-i;
	*nb_final_block=new_nb_bit/LONGSIZE;

	mes=(unsigned long  int *)calloc((*nb_final_block),sizeof(unsigned long int));
	mes_char=(char*)mes;
	DL_char=(char*)Crunch256_LeftData;
	/*copy left data in beginning of the block*/
	for(i=0;i<Crunch256_SizeLeft;i++)
	{
 		temp =crunch256_idx(i) ;
		mes_char[temp]=DL_char[temp];
	}
	no_block=((Crunch256_SizeLeft)*8+1)/LONGSIZE;
	mes[no_block]|=1UL<<( (LONGSIZE-1)-( ((Crunch256_SizeLeft)*8 )%LONGSIZE));
	/*complete block with 0 and stock message size in the last 64bits*/
	#if LONGSIZE==32
	for(i=no_block+1;i<*nb_final_block-2;i++)
	{
		mes[i]=0;
	}
	mes[*nb_final_block-2]=(unsigned long int)(Crunch256TotalInputSizeInBit>>32);
	mes[*nb_final_block-1]=(unsigned long int)(Crunch256TotalInputSizeInBit);
	#endif
	#if LONGSIZE==64
	for(i=no_block+1;i<*nb_final_block-1;i++)
	{
		mes[i]=0;
	}
	mes[*nb_final_block-1]=Crunch256TotalInputSizeInBit; 
	#endif
	return(mes);
}

void Merkle_256(unsigned long int H[],unsigned long int mes[],int nb_merkle, const int rounds) {
	int n,k;
	int j_round,j,a,aa,aa2,p,block;
    	int alpha;
	unsigned long int message[CRUNCH256_NE],res[CRUNCH256_NE];
	unsigned long i,i2;
	for(k=0;k<(CRUNCH256_NS);k++)
	{
		message[k]=H[k];
	}

	for(n=0;n<nb_merkle;n++)
	{
		for(k=0;k<CRUNCH256_NS;k++)
		{
			res[k]=message[k];
		}
		for(k=(CRUNCH256_NS) ;k<CRUNCH256_NE;k++)
		{
			message[k]=mes[k-(CRUNCH256_NS)];
			res[k]=message[k];
		}
		mes=mes+(CRUNCH256_NE-CRUNCH256_NS);


		/*a is the index for the internal function. First value point to the K0 S-box */
		a=CRUNCH256_NE-(128/LONGSIZE);
 	   	p=0;block=0;
	   	alpha=1;
 	   	for(j_round=0;j_round<rounds;j_round++)
 	   	{
        		/* Computation of i and i2 to get g(i) and g'(i2)*/
			i2=(res[block]>>((CRUNCH256_NPM-1-p)*CRUNCH256_NBE))&(CRUNCH256_NA-1);
			i2=(i2*alpha)&0xff;
			i=(message[block]>>((CRUNCH256_NPM-1-p)*CRUNCH256_NBE))&(CRUNCH256_NA-1);
			i=(i*alpha)&0xff;
			aa2=a+CRUNCH256_NE*CRUNCH256_NA*16+i2*CRUNCH256_NE;
			aa=a+i*CRUNCH256_NE;
        		for(j=0;j<CRUNCH256_NE;j++)
			{
	   			message[j]^=crunch_nbalea[aa];
				aa++;
            			res[j]^=crunch_nbalea[aa2];
	    			aa2++;
        		}
        		if ((j_round & 15) == 15)
			{
				alpha=alpha+2;
				if ( (j_round&127) == 127)
				{
					a=CRUNCH256_NE-(128/LONGSIZE);
				}
				else
				{
					a-=15*CRUNCH256_NA*CRUNCH256_NE+128/LONGSIZE;
				}
			}
			else
			{
				a+=CRUNCH256_NA*CRUNCH256_NE;
			}
        		p++;
        		if (p==CRUNCH256_NPM)
			{
        	    		block++;
		    		if (block==CRUNCH256_NE) block=0;
        	    		p=0;
        		}
		}
		if ((((rounds%128)*8)/LONGSIZE) == 0) {
			for(j=0;j<CRUNCH256_NS;j++) message[j]^=res[j];
		} else {
			for(j=(((rounds%128)*8)/LONGSIZE);j<(((rounds%128)*8)/LONGSIZE)+CRUNCH256_NS;j++) res[j%CRUNCH256_NE]^=message[j%CRUNCH256_NE];
			for(j=0;j<CRUNCH256_NS;j++) message[j]=res[(j+(((rounds%128)*8)/LONGSIZE))%CRUNCH256_NE];
		}
	}
	for(i=0;i<CRUNCH256_NS;i++) H[i]=message[i];
}

//concatenate the left data of the previous call of update with the new data 
unsigned long int *concatenation_update_256(const CrunchBitSequence *data,int databitlen,int *nb_block)
{
	unsigned long int *mes;
	int nb_bits_restants;
	int nb_block_in_new_mes;
	int nb_bits_in_new_mes;
	int i;
	int bits_traite_dans_data;
	int no_block,pos_block;
	int temp;
	char *mes_char,*DL_char;
	nb_bits_in_new_mes=( (databitlen+8*Crunch256_SizeLeft)/(CRUNCH256_BLOCKSIZE-CRUNCH256_HASHLEN) )*(CRUNCH256_BLOCKSIZE-CRUNCH256_HASHLEN);
	nb_bits_restants=(databitlen+8*Crunch256_SizeLeft)%(CRUNCH256_BLOCKSIZE-CRUNCH256_HASHLEN);
	nb_block_in_new_mes=nb_bits_in_new_mes/(LONGSIZE);
	if(nb_block_in_new_mes>0) 
	{
		/*there is enough of new data to complete a block*/
		mes = (unsigned long int *)calloc( nb_block_in_new_mes,(LONGSIZE/8) );
		if(mes==NULL) return NULL;
		//copy left data to the beginning of the new "message"
		mes_char=(char*)mes;
		DL_char=(char*)Crunch256_LeftData;
		for(i=0;i<Crunch256_SizeLeft;i++)
		{
			temp =(i/(LONGSIZE/8))*(LONGSIZE/8) + ( (LONGSIZE/8-1)-(i%(LONGSIZE/8))) ;
			mes_char[temp]=DL_char[temp];
		}
		/*complete message with new datas*/
		no_block=(i*8)/(LONGSIZE);
		pos_block=(LONGSIZE-8)-( ((i)*8 )%LONGSIZE);
		for(i=0;8*i<nb_bits_in_new_mes-8*Crunch256_SizeLeft;i++) 
		{
			mes[no_block]|=(unsigned long int)data[i] << pos_block;
			pos_block-=8;
			if (pos_block<0) 
			{
				pos_block=LONGSIZE-8;
				no_block++;
			}
		}
		bits_traite_dans_data=i;
		no_block=0;pos_block=LONGSIZE-8;
		Crunch256_SizeLeft=(nb_bits_restants)/8;
		Crunch256_LeftData[0]=0;
	}
	else
	{
		//no new block with new data
		i=0;
		no_block=((Crunch256_SizeLeft)*8+1)/LONGSIZE;
		pos_block= (LONGSIZE-8)-( ((Crunch256_SizeLeft)*8 )%LONGSIZE);
		Crunch256_SizeLeft=Crunch256_SizeLeft+databitlen/8;
	}
	/*save left data of new data for the next call of update/final*/
	for( ;8*i<databitlen;i++) 
	{
		Crunch256_LeftData[no_block]|=(unsigned long int)data[i] << pos_block;
		pos_block-=8;
		if (pos_block<0) 
		{
			pos_block=LONGSIZE-8;
			no_block++;
			Crunch256_LeftData[no_block]=0UL;
		}
	}
	*nb_block=nb_block_in_new_mes;
	return mes;	
}

CrunchHashReturn Crunch_Update_256(const CrunchBitSequence * data,CrunchDataLength databitlen, const int rounds)
{
	unsigned long int *mes;int nb_final_block[1];
	int i;
	if(databitlen>=CRUNCH256_MAXDATAPERUPDATE)
	{
		printf("too many data for an update\n");
		return CRUNCH_FAIL;
	}
	mes= concatenation_update_256(data,databitlen,nb_final_block);
	if(*nb_final_block>0)
	{
		if(mes == NULL)
		{
			printf("fail: can't allocate enough memory\n");
			return CRUNCH_FAIL;
		}
	}
	if(*nb_final_block>0)
	{
		Merkle_256(Crunch256_PrevHash,mes,*nb_final_block/(CRUNCH256_NE-CRUNCH256_NS), rounds);
		free(mes);
	}
	Crunch256TotalInputSizeInBit+=databitlen;
	return CRUNCH_SUCCESS;
}



CrunchHashReturn Crunch_Init_256()
{
	int k;
	for(k=0;k<(CRUNCH256_HASHLEN/LONGSIZE);k++) 
	{
		Crunch256_PrevHash[k]=crunch_nbalea[k];
	}
	for(k=0;k<(CRUNCH256_BLOCKSIZE-CRUNCH256_HASHLEN)/LONGSIZE;k++)
	{
		Crunch256_LeftData[k]=0;
	}
	Crunch256_SizeLeft=0;
	Crunch256TotalInputSizeInBit=0;
	return CRUNCH_SUCCESS;
}


CrunchHashReturn Crunch_Final_256(CrunchBitSequence *hash, const int rounds)
{
	unsigned long int *mes;
	int i;
	int nb_final_block[1];
	char *tocopy;
	mes=padding_256(nb_final_block);
	tocopy=(char*)mes;
	Merkle_256(Crunch256_PrevHash,mes,*nb_final_block/(CRUNCH256_NE-CRUNCH256_NS), rounds);
	
	tocopy=(char*)Crunch256_PrevHash;
	for(i=0;i<CRUNCH256_HASHLEN/8;i++)
	{
		hash[i]=tocopy[crunch256_idx(i)];
	}

	free(mes);
	return CRUNCH_SUCCESS;
}

CrunchHashReturn Crunch_Hash_256(const CrunchBitSequence *data,CrunchDataLength databitlen,CrunchBitSequence *hashval, const int rounds) 
{
	crunchHashState state;
	CrunchHashReturn HR;
	int i;
	state.hashbitlen=256;
	HR=Crunch_Init_256();
	if(HR!=CRUNCH_SUCCESS) return HR;
	i=0;
	while(databitlen>CRUNCH256_MAXDATAPERUPDATE)
	{
		HR=Crunch_Update_256(&(data[i*CRUNCH256_MAXDATAPERUPDATE/8]),CRUNCH256_MAXDATAPERUPDATE, rounds);
		if(HR!=CRUNCH_SUCCESS) return HR;
		databitlen-=(CRUNCH256_MAXDATAPERUPDATE);
		i++;
	}
	HR=Crunch_Update_256(&(data[i*CRUNCH256_MAXDATAPERUPDATE/8]),databitlen, rounds);
	HR=Crunch_Final_256(hashval, rounds);
	return HR;
}



 
